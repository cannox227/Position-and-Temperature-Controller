/*
 * pt1000.c
 *
 * Created on: 22 mar 2022
 * Author: Lisa Santarossa
 */
#include "pt1000.h"
#include <string.h>
#include <stdio.h>
#include "usart.h"

double pt1000_volt, adc_value;
char pt1000_buff[300];

double get_pt1000_volt(ADC_HandleTypeDef* adc, uint32_t timeout){

	adc_value = 0.0;

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = PT1000_CHANNEL;
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

	pt1000_volt = (adc_value / SAMPLES) * GPIO_MAX_VOLTAGE / ADC_BIT_RESOLUTION;

	HAL_ADC_Stop(adc);
	sprintf(pt1000_buff, "PT1000 %f [V] \r\n", pt1000_volt);
	HAL_UART_Transmit(&huart2, (uint8_t*)pt1000_buff, strlen(pt1000_buff), 100);
	return pt1000_volt;
}

temperature_level  get_pt1000_temp_zone(double pt1000_volt){
	temperature_level temp_zone = PT1000_ERROR;
	if(pt1000_volt <= PT1000_20_DEG_VOLTAGE_VALUE){
		temp_zone = COLD;
		sprintf(pt1000_buff, "COLD  \r\n");
	} else if(pt1000_volt >= PT1000_20_DEG_VOLTAGE_VALUE && pt1000_volt <= PT1000_40_DEG_VOLTAGE_VALUE){
		temp_zone = NORMAL;
		sprintf(pt1000_buff, "NORMAL  \r\n");
	} else if(pt1000_volt >= PT1000_40_DEG_VOLTAGE_VALUE && pt1000_volt <= PT1000_60_DEG_VOLTAGE_VALUE){
		temp_zone = HOT;
		sprintf(pt1000_buff, "HOT  \r\n");
	} else if(pt1000_volt >= PT1000_60_DEG_VOLTAGE_VALUE){
		temp_zone = DANGER;
		sprintf(pt1000_buff, "DANGER  \r\n");
	} else {
		sprintf(pt1000_buff, "ERROR  \r\n");
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)pt1000_buff, strlen(pt1000_buff), 100);
	return temp_zone;
}
