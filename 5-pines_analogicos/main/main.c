#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/Task.h>
#include <esp_adc/adc_oneshot.h>

adc_oneshot_unit_handle_t adc1_handle;

static void oneshot_init(void)
{
	adc_oneshot_unit_init_cfg_t adc_oneshot_init = { .unit_id = ADC_UNIT_1, .ulp_mode = 0};
	adc_oneshot_new_unit(&adc_oneshot_init, &adc1_handle);
	adc_oneshot_chan_cfg_t config = {.atten = ADC_ATTEN_DB_12, .bitwidth = ADC_BITWIDTH_12};
	adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_8, &config);
	printf("ADC configurado!!");
}


void app_main(void)
{
	oneshot_init();
	while(1)
	{
		int adc;
		adc_oneshot_read(adc1_handle, ADC_CHANNEL_8, &adc);
		printf("ADC: %i \n", adc);
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}
