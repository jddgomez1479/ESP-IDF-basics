#include <stdio.h>
#include <freertos/freeRTOS.h>
#include <freertos/Task.h>
#include <String.h>


int numero=256;
float flotante=5.3265;
bool booleana=0;
char cadena[10]="1234567\0";

void app_main(void)
{
	while(1)
	{

		numero++; // incrementos +1
		flotante=flotante+0.01;

		printf("numero: %i flotante: %f booleano: %i cadena: %s \n", numero, flotante, booleana, cadena);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}
