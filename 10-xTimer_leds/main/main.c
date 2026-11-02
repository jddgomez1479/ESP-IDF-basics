#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/freeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

#define led1 8
#define led2 18
#define led3 17
#define led4 16
#define led5 15

#define TimerTicks 1000
uint8_t led_level = 0;
int timerId=1;
TimerHandle_t xTimers;

void vTimerCallback (TimerHandle_t pxTimer)
{
	led_level = !led_level;
	gpio_set_level(led1, led_level);
    printf("led level: %u\n", led_level);
}

esp_err_t set_timer(void)
{
    printf("Timer: configuración inicial \n");
    xTimers = xTimerCreate("Timer", (pdMS_TO_TICKS(TimerTicks)), pdTRUE, (void *)timerId, vTimerCallback);
    if (xTimers == NULL) printf("Timer: no creado! \n");
    else if (xTimerStart(xTimers, 0) != pdPASS) printf("Timer: xTimers \n");
    return ESP_OK;
}

void app_main(void)
{
	//gpio_reset_pin(led1);
	gpio_set_direction(led1, GPIO_MODE_OUTPUT);
	gpio_set_direction(led2, GPIO_MODE_OUTPUT);
	gpio_set_direction(led3, GPIO_MODE_OUTPUT);
	gpio_set_direction(led4, GPIO_MODE_OUTPUT);
	gpio_set_direction(led5, GPIO_MODE_OUTPUT);

    set_timer();

    while(1)
    {
    	gpio_set_level(led2, 0);
    	gpio_set_level(led3, 0);
    	gpio_set_level(led4, 0);
    	gpio_set_level(led5, 0);
    	vTaskDelay(1000/portTICK_PERIOD_MS);
    	gpio_set_level(led2, 1);
    	gpio_set_level(led3, 1);
    	gpio_set_level(led4, 1);
    	gpio_set_level(led5, 1);
    	vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}



