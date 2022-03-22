/*
 * ntc.h
 *
 *  Created on: 22 mar 2022
 *      Author: Tommaso Canova
 */

#ifndef INC_NTC_H_
#define INC_NTC_H_

#include "adc.h"
#include "usart.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 300

#define GPIO_MAX_VOLTAGE 3.3
#define ADC_BIT_RESOLUTION 2048.0
#define SAMPLES 30

#define NTC_20_DEG_VOLTAGE_VALUE 		2.203456
#define NTC_40_DEG_VOLTAGE_VALUE 		1.482085
#define NTC_60_DEG_VOLTAGE_VALUE 		0.933597
#define NTC_MINUS40_DEG_VOLTAGE_VALUE 	3.823179
#define NTC_125_DEG_VOLTAGE_VALUE 		0.204090


typedef enum{
	COLD = 0U,
	NORMAL,
	HOT,
	DANGER,
	NTC_ERROR
} temperature_level;


char ntc_buff [BUFF_SIZE];
double ntc_volt, adc_value;

double get_ntc_volt(ADC_HandleTypeDef* adc, uint32_t timeout);

temperature_level get_temp_zone(double ntc_volt);

#endif /* INC_NTC_H_ */
