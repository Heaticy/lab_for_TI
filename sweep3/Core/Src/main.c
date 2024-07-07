/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "debug.h"
#include "fft.h"
#include <stdlib.h>
#include "AD9959.h"
#include "HMC472.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define ADC_DATA_LENGTH 1024
uint32_t g_adc1_dma_data1[ADC_DATA_LENGTH];//定义adc1采集数据存放数组
volatile uint8_t g_adc1_dma_complete_flag = 0;	//adc1数据dma采集完成标志，在dma采集完成
float dataR[1024] = {0};
float dataI[1024] = {0};
float datafR[1024] = {0};
float datafI[1024] = {0};
int f_=11;
float CH1 = 0;
float CH2 = 0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{	
	if(hadc->Instance==ADC1)
	{
		g_adc1_dma_complete_flag = 1;//采集完成标志
//		HAL_ADCEx_MultiModeStop_DMA(&hadc1);
	}
}

void ADC_start(uint32_t freq)
{
  int f;
	if (freq >= 500000){
    f = 12;
	}else if(freq >= 200000 ){
		f = 11;
	}else if(freq >= 100000 ){
		f = 10;
	}else if(freq >= 50000 ){
		f = 9;
//	}else if(freq >= 20000 ){
//		f = 8;
	}else if(freq >= 10000 ){
		f = 7;
	}else if(freq >= 5000 ){
		f = 6;
//	}else if(freq >= 2000){
//		f = 5;
	}else if(freq >= 1000){
		f = 4;
	}else if(freq >= 500){
		f = 3;
//	}else if(freq >= 200){
//		f = 2;
	}else if(freq >= 100){
		f = 1;
	}else{
		f = 0;
	}
	if(f != f_){
		RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0}; //RCC时钟初始结构体
		ADC_Disable(&hadc1);
		ADC_Disable(&hadc2);
		f_ = f;
		HAL_Delay(5);

	//  if(f < 9){
	//			HAL_Delay(20);
	//	}
	//	if(f < 8){
	//			HAL_Delay(20);
	//	}
	//	if(f < 7){
	//			HAL_Delay(20);
	//	}
		switch(f)
		{
			case 0://200ms
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 164;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 128;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV128;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV128;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 1://100ms
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 164;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 128;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV64;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV64;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 2://50ms
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 164;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 128;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV32;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV32;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 3://20ms
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 320;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 100;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV32;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV32;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 4://10ms
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 320;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 100;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 5://5ms
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 320;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 100;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV8;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 6://2ms
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 150;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 75;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 7://1ms
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 75;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 8://500us
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 300;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 50;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 9://200us
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV4;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 10://100us
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV2;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 11://50us
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 12://20us
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 360;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			case 13://10us
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
			break;
			default:
				PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
				PeriphClkInitStruct.PLL3.PLL3M = 25;	//PLL输入25M分频,Max 63
				PeriphClkInitStruct.PLL3.PLL3N = 200;	//PLL分频后倍频,Max 512
				PeriphClkInitStruct.PLL3.PLL3R = 5;	//PLL分频,Max 128
				PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
				HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
			
				hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;//adc内部分频,Max 256,只能倍数
				HAL_ADC_Init(&hadc1);
				HAL_ADC_Init(&hadc2);
				break;
			}	
	}	
}
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

//  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */
  HAL_NVIC_SetPriority(SysTick_IRQn,1,0);
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART4_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	Init_AD9958();
    uint8_t testMsg[] = "test\n";
    uint8_t response[5];
    // 发送测试消息
    int statu = 1;
    // 等待设备的响应
    while (statu)
    {
        HAL_UART_Transmit(&huart4, testMsg, sizeof(testMsg), 1000);
        HAL_StatusTypeDef status = HAL_UART_Receive(&huart4, response, sizeof(response), 1000);
        statu = memcmp(testMsg, response, sizeof(response));
    }

    __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
		
		HMC472_Init();
		HMC472set(63);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 200;
  PeriphClkInitStruct.PLL3.PLL3P = 10;
  PeriphClkInitStruct.PLL3.PLL3Q = 10;
  PeriphClkInitStruct.PLL3.PLL3R = 4;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void ADC_fft_collect(uint32_t freq)
{
    uint32_t i = 0;
	  Init_AD9958();
		AD9958_Set_Freq(freq);            // AD9958两个通道同时输出，单位：Hz
		AD9958_SetPhase2Channel(0, 90);     // 通道0、通道1相位调节输出，单位：度
    AD9958_SetAmp2Channel(1023, 1023); // 通道0、通道1幅值调节输出，，单位：DAC分辨率
    HAL_Delay(20);
    ADC_start(freq);
		memset(g_adc1_dma_data1,0,ADC_DATA_LENGTH * 4);//清除数据
		while((((g_adc1_dma_data1[500])>>16)&0x0000ffff)== 0){
			HAL_ADCEx_MultiModeStart_DMA(&hadc1,g_adc1_dma_data1,ADC_DATA_LENGTH);//ADC的dma开始采集
			 while (g_adc1_dma_complete_flag == 0)
			{
						HAL_Delay(20);		//延时
			}
			 g_adc1_dma_complete_flag = 0;
		}
    memset(dataI, 0, 1024 * sizeof(float));

    for (i = 0; i < 1024; i++)
    {
        dataR[i] = ((float)(g_adc1_dma_data1[i] & 0x0000ffff) - 32768) * 3.3f / 65536;
    }
    kfft(dataR, dataI, 1024, 10, datafR, datafI);
//				dataR[0]=0;
    for (i = 0; i < 1024; i++)
    {
        printf("%f\r\n", dataR[i] / 1024);
    }
    memset(dataI, 0, 1024 * sizeof(float));
    for (i = 0; i < 1024; i++)
    {
        dataR[i] = ((float)(((g_adc1_dma_data1[i])>>16)&0x0000ffff) - 32768) * 3.3f / 65536;
    }
    kfft(dataR, dataI, 1024, 10, datafR, datafI);
//		dataR[0]=0;
    for (i = 0; i < 1024; i++)
    {
        printf("%f\r\n", dataR[i] / 1024);
    }
}

void ADC_init(){
		Init_AD9958();
    uint32_t i = 0;
	  HAL_Delay(20);
    ADC_start(10000);
	  memset(g_adc1_dma_data1,0 ,ADC_DATA_LENGTH * 4);//清除数据
		while((((g_adc1_dma_data1[500])>>16)&0x0000ffff)== 0){
		HAL_ADCEx_MultiModeStart_DMA(&hadc1,g_adc1_dma_data1,ADC_DATA_LENGTH);//ADC的dma开始采集
		 while (g_adc1_dma_complete_flag == 0)
		{
					HAL_Delay(2);		//延时
		}
		 g_adc1_dma_complete_flag = 0;
		}
    memset(dataI, 0, 1024 * sizeof(float));

    for (i = 0; i < 1024; i++)
    {
        dataR[i] = ((float)(g_adc1_dma_data1[i] & 0x0000ffff) - 32768) * 3.3f / 65536;
    }
    kfft(dataR, dataI, 1024, 10, datafR, datafI);
    CH1 = dataR[0];
//    for (i = 0; i < 1024; i++)
//    {
//        printf("%f\r\n", dataR[i] / 1024);
//    }
    memset(dataI, 0, 1024 * sizeof(float));
    for (i = 0; i < 1024; i++)
    {
        dataR[i] = ((float)(((g_adc1_dma_data1[i])>>16)&0x0000ffff) - 32768) * 3.3f / 65536;
    }
    kfft(dataR, dataI, 1024, 10, datafR, datafI);
		CH2 = dataR[0];
}
void ADC_fft_sweep(uint32_t freq)
{   
		uint32_t i = 0;
		Init_AD9958();
		AD9958_Set_Freq(freq);            // AD9958两个通道同时输出，单位：Hz
		AD9958_SetPhase2Channel(0, 90);     // 通道0、通道1相位调节输出，单位：度
    AD9958_SetAmp2Channel(1023, 1023); // 通道0、通道1幅值调节输出，，单位：DAC分辨率
		HAL_Delay(20);
    ADC_start(10000);
				memset(g_adc1_dma_data1,0,ADC_DATA_LENGTH * 4);//清除数据
		while((((g_adc1_dma_data1[500])>>16)&0x0000ffff)== 0){
			HAL_ADCEx_MultiModeStart_DMA(&hadc1,g_adc1_dma_data1,ADC_DATA_LENGTH);//ADC的dma开始采集
			 while (g_adc1_dma_complete_flag == 0)
			{
						HAL_Delay(2);		//延时
			}
			 g_adc1_dma_complete_flag = 0;
		}
		    memset(dataI, 0, 1024 * sizeof(float));

    for (i = 0; i < 1024; i++)
    {
        dataR[i] = ((float)(g_adc1_dma_data1[i] & 0x0000ffff) - 32768) * 3.3f / 65536;
    }
    kfft(dataR, dataI, 1024, 10, datafR, datafI);
    float I = dataR[0] - CH1;
//    for (i = 0; i < 1024; i++)
//    {
//        printf("%f\r\n", dataR[i] / 1024);
//    }
    memset(dataI, 0, 1024 * sizeof(float));
    for (i = 0; i < 1024; i++)
    {
        dataR[i] = ((float)(((g_adc1_dma_data1[i])>>16)&0x0000ffff) - 32768) * 3.3f / 65536;
    }
    kfft(dataR, dataI, 1024, 10, datafR, datafI);
		float Q = dataR[0] -CH2;
		float V = sqrtf(2*(I*I+Q*Q));
		float PHI = atan2(-Q,I)/3.14159f*180;
		 printf("%f\r\n", V/2650);
		 printf("%f\r\n", PHI);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
