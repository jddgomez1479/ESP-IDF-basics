#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/freeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define led1 8
#define led2 18
#define led3 17
#define led4 16
#define led5 15

#define STACK_SIZE 1024*20

esp_err_t create_tasks(void);
void Nucleo1(void * pvParameters);


void app_main(void)
{
    create_tasks();
    while(1)
    {
        printf("main principal \n");
        vTaskDelay(10000/portTICK_PERIOD_MS);
    }
}

void Nucleo1(void * pvParameters)
{
    while(1)
    {
        printf("tarea asignada al procesador 2 \n");
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}


esp_err_t create_tasks(void)
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xhandle = NULL;
    xTaskCreatePinnedToCore(Nucleo1,"Nucleo1", STACK_SIZE, &ucParameterToPass, 1,&xhandle, 0);
    return ESP_OK;
}

