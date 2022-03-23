/*
 * pt1000.c
 *
 * 	Created on: 22 mar 2022
 * 		Author: Lisa Santarossa
 */

#ifndef __PT1000_H
#define __PT1000_H

#include "adc.h"
#include <inttypes.h>

#define GPIO_MAX_VOLTAGE 3.3
#define ADC_BIT_RESOLUTION 2048.0
#define SAMPLES 30
#define PT1000_CHANNEL (ADC_CHANNEL_11)


#define PT1000_20_DEG_VOLTAGE_VALUE 2.332945
#define PT1000_40_DEG_VOLTAGE_VALUE 2.476266
#define PT1000_60_DEG_VOLTAGE_VALUE 2.615992

typedef enum{
	COLD = 0U,
	NORMAL,
	HOT,
	DANGER,
	PT1000_ERROR
} temperature_level;


double get_pt1000_volt(ADC_HandleTypeDef* adc, uint32_t timeout);
temperature_level  get_pt1000_temp_zone(double pt1000_volt);

#endif /* __PT1000_H */
