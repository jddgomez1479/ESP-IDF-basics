#include <stdio.h>
#include <freertos/freeRTOS.h>
#include <freertos/Task.h>
#include <driver/gpio.h>
#include <freertos/timers.h>

#define led 16
#define TimerTicks 1000
uint8_t led_level=0;
int timerId=1;
TimerHandle_t xTimers;

esp_err_t init_led(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);

void vTimerCallback (TimerHandle_t pxTimer)
{
	blink_led();
	printf("led: %u \n", led_level);
}


void app_main(void)
{
	init_led();
	set_timer();
}


esp_err_t init_led(void)
{
	gpio_reset_pin(led);
	gpio_set_direction(led, GPIO_MODE_OUTPUT);
	return ESP_OK;
}

esp_err_t blink_led(void)
{
	led_level = !led_level;
	gpio_set_level(led, led_level);
	return ESP_OK;
}

esp_err_t set_timer(void)
{
	xTimers = xTimerCreate("Timer", (pdMS_TO_TICKS(TimerTicks)), pdTRUE, (void *)timerId, vTimerCallback);
	if(xTimers == NULL) printf("Timer: no creado!! \n");
	else if (xTimerStart(xTimers, 0) != pdPASS) printf("Timer: timer creado!!");
	return ESP_OK;
}
