#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/Task.h>
#include <driver/gpio.h>


#define led1 14
#define led2 27
#define led3 26
#define led4 25
#define led5 33

#define boton1 4
#define boton2 5
#define boton3 18
#define boton4 19
#define boton5 21


void app_main(void)
{
	gpio_set_direction(led1, GPIO_MODE_OUTPUT);
	gpio_set_direction(led2, GPIO_MODE_OUTPUT);
	gpio_set_direction(led3, GPIO_MODE_OUTPUT);
	gpio_set_direction(led4, GPIO_MODE_OUTPUT);
	gpio_set_direction(led5, GPIO_MODE_OUTPUT);

	gpio_set_direction(boton1, GPIO_MODE_INPUT);
	gpio_set_direction(boton2, GPIO_MODE_INPUT);
	gpio_set_direction(boton3, GPIO_MODE_INPUT);
	gpio_set_direction(boton4, GPIO_MODE_INPUT);
	gpio_set_direction(boton5, GPIO_MODE_INPUT);

	gpio_set_pull_mode(boton1, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(boton2, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(boton3, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(boton4, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(boton5, GPIO_PULLUP_ONLY);



	while(1)
	{
		int estado1=gpio_get_level(boton1);
		int estado2=gpio_get_level(boton2);
		int estado3=gpio_get_level(boton3);
		int estado4=gpio_get_level(boton4);
		int estado5=gpio_get_level(boton5);

		printf("Estados: %i %i %i %i %i \n", estado1, estado2, estado3, estado4, estado5);

		if(estado1==0) gpio_set_level(led1, 1);
		else gpio_set_level(led1, 0);

		if (estado2==0) gpio_set_level(led2, 1);
		else gpio_set_level(led2, 0);

		if (estado3==0) gpio_set_level(led3, 1);
		else gpio_set_level(led3, 0);

		if (estado4==0) gpio_set_level(led4, 1);
		else gpio_set_level(led4, 0);

		if (estado5==0) gpio_set_level(led5, 1);
		else gpio_set_level(led5, 0);

		vTaskDelay(10/portTICK_PERIOD_MS);
	}

}
