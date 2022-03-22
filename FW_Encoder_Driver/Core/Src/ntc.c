/*
 * ntc.c
 *
 *  Created on: 22 mar 2022
 *      Author: Tommaso Canova
 */

#include "ntc.h"

double get_ntc_volt(ADC_HandleTypeDef* adc, uint32_t timeout){

	adc_value = 0.0;

	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, timeout);

	for (uint8_t i = 0; i < SAMPLES; i++){
		adc_value += HAL_ADC_GetValue(adc);
	}

	ntc_volt = (adc_value / SAMPLES) * GPIO_MAX_VOLTAGE / ADC_BIT_RESOLUTION;

	return ntc_volt;
}




