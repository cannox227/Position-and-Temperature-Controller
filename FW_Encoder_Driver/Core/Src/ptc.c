/*
 * ptc.c
 *
 * Created on: 22 mar 2022
 * Author: Lisa Santarossa
 */
#include <ptc.h>
#include <string.h>
#include <stdio.h>
#include "usart.h"

double ptc_volt, adc_value;
char ptc_buff[300];

double get_ptc_volt(ADC_HandleTypeDef* adc, uint32_t timeout){

	adc_value = 0.0;

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = PTC_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
	if(HAL_ADC_ConfigChannel(adc, &sConfig) != HAL_OK){
		Error_Handler();
	}

	HAL_ADC_Start(adc);
	HAL_ADC_PollForConversion(adc, timeout);

	for (uint8_t i = 0; i < SAMPLES; i++){
			adc_value += HAL_ADC_GetValue(adc);
	}

	ptc_volt = (adc_value / SAMPLES) * GPIO_MAX_VOLTAGE / ADC_BIT_RESOLUTION;

	HAL_ADC_Stop(adc);
	sprintf(ptc_buff, "PTC %f [V] \r\n", ptc_volt);
	HAL_UART_Transmit(&huart2, (uint8_t*)ptc_buff, strlen(ptc_buff), 100);
	return ptc_volt;
}

temperature_level  get_ptc_temp_zone(double ptc_volt){
	temperature_level temp_zone = PTC_ERROR;
	if(ptc_volt <= PTC_20_DEG_VOLTAGE_VALUE && ptc_volt >= PTC_MIN_DEG_VOLTAGE_VALUE){
		temp_zone = COLD;
		sprintf(ptc_buff, "COLD  \r\n");
	} else if(ptc_volt >= PTC_20_DEG_VOLTAGE_VALUE && ptc_volt <= PTC_40_DEG_VOLTAGE_VALUE){
		temp_zone = NORMAL;
		sprintf(ptc_buff, "NORMAL  \r\n");
	} else if(ptc_volt >= PTC_40_DEG_VOLTAGE_VALUE && ptc_volt <= PTC_60_DEG_VOLTAGE_VALUE){
		temp_zone = HOT;
		sprintf(ptc_buff, "HOT  \r\n");
	} else if(ptc_volt >= PTC_60_DEG_VOLTAGE_VALUE && ptc_volt <= PTC_MAX_DEG_VOLTAGE_VALUE){
		temp_zone = DANGER;
		sprintf(ptc_buff, "DANGER  \r\n");
	} else {
		sprintf(ptc_buff, "ERROR  \r\n");
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)ptc_buff, strlen(ptc_buff), 100);
	return temp_zone;
}
