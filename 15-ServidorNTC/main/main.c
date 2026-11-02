#include <stdio.h>
#include <stdlib.h>

//SNTP
#include <time.h>
#include <sys/time.h>
#include "esp_attr.h"
#include "esp_sleep.h"
#include "esp_sntp.h"

//wifi
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/i2c.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define EXAMPLE_ESP_WIFI_SSID      "NAME"
#define EXAMPLE_ESP_WIFI_PASS      "123456"
#define I2C_SLAVE_ADDR 0x68
#define RX_BUFFER_LEN 8
#define TX_BUFFER_LEN 8
#define TIMEOUT_MS 1000
#define DELAY_MS 1000

static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1
static const char *TAG = "NTC";

char Current_Date_Time[100];
int8_t anio, mes, dia, hora, minuto;
uint8_t Write_data[TX_BUFFER_LEN];
uint8_t Read_data[RX_BUFFER_LEN];
uint8_t command = 0x00;


static esp_err_t set_i2c()
{
    i2c_config_t i2c_config = {};

    i2c_config.mode = I2C_MODE_MASTER;
    i2c_config.sda_io_num = 21;
    i2c_config.scl_io_num = 22;
    i2c_config.sda_pullup_en = true;
    i2c_config.scl_pullup_en = true;
    i2c_config.master.clk_speed = 400000;
    i2c_config.clk_flags = 0;

    ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_LEVEL1));

    return ESP_OK;
}


void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
	{
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
	{
       // if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
		//{
            esp_wifi_connect();
           // s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        //} else {
        //    xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        //s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}


void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));

	ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                    WIFI_EVENT_STA_DISCONNECTED,
                    &event_handler,
                    NULL,
                    NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

    /* The event will not be processed after unregister */
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    vEventGroupDelete(s_wifi_event_group);
}





void NTC_Setup(void)
{
    time_t now = 0;
        struct tm timeinfo = { 0 };
        int retry = 0;
        const int retry_count = 10;

	    time(&now);
	    localtime_r(&now, &timeinfo);
	    // Is time set? If not, tm_year will be (1970 - 1900).

        //inicializando servidor NTC
        ESP_LOGI(TAG, "Inicializando SNTP");
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_setservername(0, "pool.ntp.org");
        sntp_set_time_sync_notification_cb(time_sync_notification_cb);
    #ifdef CONFIG_SNTP_TIME_SYNC_METHOD_SMOOTH
        sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
    #endif
        sntp_init();

        while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
            ESP_LOGI(TAG, "Estableciendo fecha y hora del servidor.. (%d/%d)", retry, retry_count);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }

        setenv("TZ", "UTC+05:00", 1);
	    tzset();
        time(&now);
        localtime_r(&now, &timeinfo);  
        
         
        Write_data[2] = timeinfo.tm_min;
        Write_data[3] = timeinfo.tm_hour;
        Write_data[5] = timeinfo.tm_mday;
        Write_data[6] = timeinfo.tm_mon + 1;
        Write_data[7] = timeinfo.tm_year - 100;
        
        
        //printf("fecha: %i, %i, %i, %i, %i, \n", anio, mes, dia, hora, minuto);
        //printf("buffer: %hhx \n", anio);
}


void NTC_Sync(void)
{

}


void app_main(void)     {

	//Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init_sta();
    NTC_Setup();

    ESP_ERROR_CHECK(set_i2c());
    i2c_master_write_to_device(I2C_NUM_0, I2C_SLAVE_ADDR, &Write_data, sizeof(Write_data), 1000); 

    while(1)  
    {

		i2c_master_write_read_device(I2C_NUM_0, I2C_SLAVE_ADDR, &command, sizeof(command), Read_data, sizeof(Read_data), 1000);       
        ESP_LOG_BUFFER_HEX(TAG, Read_data, sizeof(Read_data));
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS));
    }


}