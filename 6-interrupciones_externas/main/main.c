#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/freeRTOS.h>
#include <freertos/Task.h>
#include <freertos/timers.h>
#include <esp_timer.h>

#define encoder 1


int estado=0;
float rev=0, p=0.025, velocidad=0, tiempo=0;
int tiempoActual, tiempoInicial;
int g=0;

void interrupt(void *args)
{
	g++;
}


esp_err_t init_irs(void)
{
	gpio_config_t gpio_config_pin;
	gpio_config_pin.pin_bit_mask = (1ULL<<encoder);
	gpio_config_pin.mode = GPIO_MODE_INPUT;
	gpio_config_pin.pull_up_en = GPIO_PULLUP_DISABLE;
	gpio_config_pin.pull_down_en = GPIO_PULLDOWN_DISABLE;
	gpio_config_pin.intr_type = GPIO_INTR_POSEDGE;

	gpio_config(&gpio_config_pin);
	gpio_install_isr_service(0);
	gpio_isr_handler_add(encoder, interrupt, NULL);
	return ESP_OK;
}

void app_main(void)
{
	init_irs();

	while(1)
	{
		tiempoInicial = esp_timer_get_time()/1000;
		vTaskDelay(1000/portTICK_PERIOD_MS);
		tiempoActual = esp_timer_get_time()/1000;
		tiempo = tiempoActual - tiempoInicial;
		tiempo=tiempo/1000;

		rev = g/4;
		velocidad = (rev*p)/tiempo;
		printf("velocidad: %f9.6 %f %f %f \n", velocidad, tiempo, rev, p);
		g=0;
	}
}
