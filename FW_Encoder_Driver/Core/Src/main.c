/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 * WDGA 36A-06-1312-SIA-B01-CB8 - Magnetic Absolute Encoder Binary/SSI 25 BIT
 *	13 BIT SINGLETURN
 *	12 BIT MULTITURN
 TODO dal clock dello ioc calcolare in automatico la frewuenza del clock generato
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "ntc.h"
#include "ptc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define WORK
#define N_BIT 52
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void setInitialClockState( GPIO_PinState PinState);
void runClock();
void readEncoderBit(GPIO_TypeDef*, uint16_t);
void println(UART_HandleTypeDef *, char[]);
void nop(uint16_t);
void getTemperature();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef struct{
	uint64_t totalBits;
	uint64_t STbits;
	uint64_t MTbits;
} encoderStruct;

encoderStruct encoder;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM11_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	//setInitialClockState(GPIO_PIN_SET);
	//HAL_GPIO_WritePin(DEBUG_LINE_GPIO_Port, DEBUG_LINE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SSI_CLK_GPIO_Port,  SSI_CLK_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	println(&huart2, "HELLO! \r\n");
	println(&huart2, "------------------------------");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		//runClock();
		//HAL_Delay(100);
		//nop(500);

		getTemperature();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#ifdef WORK
void setInitialClockState( GPIO_PinState PinState){
	if(PinState==GPIO_PIN_RESET || PinState==GPIO_PIN_SET){
		HAL_GPIO_WritePin(SSI_CLK_GPIO_Port,  SSI_CLK_Pin, PinState);
	}else{
		printf("error");
	}
}

void runClock(){
	// Inizio trasmissione
	encoder.totalBits = 0U;
	for(int i=0; i<N_BIT; i++) {
		HAL_GPIO_WritePin(SSI_CLK_GPIO_Port, SSI_CLK_Pin, GPIO_PIN_RESET);
		nop(4);
		HAL_GPIO_WritePin(SSI_CLK_GPIO_Port, SSI_CLK_Pin, GPIO_PIN_SET);
		nop(6);
		HAL_GPIO_WritePin(DEBUG_LINE_GPIO_Port, DEBUG_LINE_Pin, GPIO_PIN_SET);
		readEncoderBit(DATA_IN_GPIO_Port, DATA_IN_Pin);
		HAL_GPIO_WritePin(DEBUG_LINE_GPIO_Port, DEBUG_LINE_Pin, GPIO_PIN_RESET);
		nop(2);
		//HAL_GPIO_WritePin(SSI_CLK_GPIO_Port, SSI_CLK_Pin, GPIO_PIN_RESET);
		//nop(1);
	}
	HAL_GPIO_WritePin(SSI_CLK_GPIO_Port, SSI_CLK_Pin, GPIO_PIN_SET);
	//encoder.STbits = (encoder.totalBits & 0x1FFF000) >> 12; //primi 13 bit di maschera Single turn
	//encoder.MTbits = (encoder.totalBits & 0xFFF); //12 bit di maschera Multi turn
	//printf(".");
	//println(&huart2, stbit);
}

void readEncoderBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
	encoder.totalBits <<= 1;
	if(HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_SET){
		encoder.totalBits |= 1;
	}
}
void println(UART_HandleTypeDef *huart, char out[]){
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 10);
	char newline[2] = "\r\n";
	HAL_UART_Transmit(huart, (uint8_t *) newline, 2, 10);
}

void nop(uint16_t N){
	for(int i=0; i<N; i++){
		__asm("nop"); //1/84Mhz=1.25x10e-8s
	}
}

void getSample(){

}
#endif

void getTemperature(){
	double ptc_volt = get_ptc_volt(&hadc1, 200);
	get_ptc_temp_zone(ptc_volt);
	HAL_Delay(1000);

	double ntc_volt = get_ntc_volt(&hadc1, 200);
	get_ntc_temp_zone(ntc_volt);
	HAL_Delay(1000);
	println(&huart2, "------------------------------");
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

