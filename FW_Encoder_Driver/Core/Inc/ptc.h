/*
 * ptc.c
 *
 * 	Created on: 22 mar 2022
 * 		Author: Lisa Santarossa
 */

#ifndef __PTC_H
#define __PTC_H

#include "adc.h"
#include <inttypes.h>

#define GPIO_MAX_VOLTAGE 3.3
#define ADC_BIT_RESOLUTION 2048.0
#define SAMPLES 30
#define PTC_CHANNEL (ADC_CHANNEL_11)


#define PTC_20_DEG_VOLTAGE_VALUE 2.332945
#define PTC_40_DEG_VOLTAGE_VALUE 2.476266
#define PTC_60_DEG_VOLTAGE_VALUE 2.615992
#define PTC_MIN_DEG_VOLTAGE_VALUE 1.879981409
#define PTC_MAX_DEG_VOLTAGE_VALUE 3.046762718

typedef enum{
	COLD = 0U,
	NORMAL,
	HOT,
	DANGER,
	PTC_ERROR
} temperature_level;


double get_ptc_volt(ADC_HandleTypeDef* adc, uint32_t timeout);
temperature_level  get_ptc_temp_zone(double ptc_volt);

#endif /* __PTC_H */
