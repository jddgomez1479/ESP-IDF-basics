#include <stdio.h>
#include <freertos/freeRTOS.h>
#include <freertos/Task.h>
#include <driver/gpio.h>
#include <driver/ledc.h>




esp_err_t set_pwm(void)
{
	ledc_timer_config_t timer_config = {0};
	timer_config.speed_mode = LEDC_LOW_SPEED_MODE;
	timer_config.duty_resolution = LEDC_TIMER_10_BIT;
	timer_config.timer_num = LEDC_TIMER_0;
	timer_config.freq_hz = 78000;

	ledc_timer_config(&timer_config);

	ledc_channel_config_t channel_config = {0};
	channel_config.gpio_num = 18;
	channel_config.speed_mode = LEDC_LOW_SPEED_MODE;
	channel_config.channel = LEDC_CHANNEL_0;
	channel_config.intr_type = LEDC_INTR_DISABLE;
	channel_config.timer_sel = LEDC_TIMER_0;
	channel_config.duty = 0;

	ledc_channel_config(&channel_config);
	return ESP_OK;
}


void app_main(void) {
	set_pwm();
	while (1) {
		printf("PWM 1023 \n");
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 1023);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		printf("PWM 950 \n");
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 950);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		printf("PWM 900 \n");
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 900);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		printf("PWM 850 \n");
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 850);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		printf("PWM 800 \n");
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 800);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		printf("PWM 750 \n");
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 750);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		printf("PWM 700 \n");
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 700);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		vTaskDelay(100 / portTICK_PERIOD_MS);

		printf("PWM 500 \n");
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 500);
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
