#include <stdio.h>
#include <freertos/freeRTOS.h>
#include <freertos/Task.h>
#include <driver/gpio.h>
#include <String.h>

#define led1 18
#define led2 17
#define led3 16

#define boton 36

int numero=1;
float flotante=2.3;
bool booleano=0;
char cadena[10]="01234567\0";

void app_main(void)
{
	gpio_set_direction(led1, GPIO_MODE_OUTPUT);
	gpio_set_direction(led2, GPIO_MODE_OUTPUT);
	gpio_set_direction(led3, GPIO_MODE_OUTPUT);

	gpio_set_direction(boton, GPIO_MODE_INPUT);

	while(1)
	{
		int estado = gpio_get_level(boton);
		printf("valor: %i %f %i %s  \n", numero, flotante, booleano, cadena);
		vTaskDelay(200/portTICK_PERIOD_MS);

		if(estado == 0)
		{
			gpio_set_level(led1, 1);
			printf("led1: 1 led2: 0 led3: 0 \n");
			vTaskDelay(1000/portTICK_PERIOD_MS);
			gpio_set_level(led1, 0);

			gpio_set_level(led2, 1);
			printf("led1: 0 led2: 1 led3: 0 \n");
			vTaskDelay(1000/portTICK_PERIOD_MS);
			gpio_set_level(led2, 0);

			gpio_set_level(led3, 1);
			printf("led1: 0 led2: 0 led3: 1 \n");
			vTaskDelay(1000/portTICK_PERIOD_MS);
			gpio_set_level(led3, 0);
		}
	}

}
