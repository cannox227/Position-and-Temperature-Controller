/*
 * ntc.h
 *
 *  Created on: 22 mar 2022
 *      Author: Tommaso Canova
 */

#ifndef INC_NTC_H_
#define INC_NTC_H_

#include "adc.h"
#include <inttypes.h>

#define GPIO_MAX_VOLTAGE 3.3
#define ADC_BIT_RESOLUTION 2048.0
#define SAMPLES 30

double ntc_volt, adc_value;

double get_ntc_volt(ADC_HandleTypeDef* adc, uint32_t timeout);


#endif /* INC_NTC_H_ */
