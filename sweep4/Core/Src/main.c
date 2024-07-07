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
#include "lptim.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "debug.h"
#include "fft.h"
#include <stdlib.h>
#include "AD9959.h"
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
 int voltageArray[30]={
8640,8515,8273,8029,7785,
7553,7231,6863,6530,6266,
5995,5785,5550,5335,5143,
4934,4746,4618,4460,4300,
4150,4025,3888,3810,3705,
3605,3517,3430,3345,3255,
};
int search( int a[],int left,int right,int x)
{  //二分查找 
	while(left<right)
	{
		int mid=(left+right)/2;
		if((right-left)<=2)
		{
			break;
		}
		if(a[mid]==x)return mid;
		else if(a[mid]>x)  left =mid+1;
		else if(a[mid]<x)  right=mid;
	}
	if((a[left]-x)<0)
	{
		return left+1;
	}
	if((x-a[right])<0)
	{
		return right+1;
	}
	if((a[left] - x)<(x-a[right]))
	{
		return left+1;
	}else
	{
		return right+1;
	}
//	return -1;  //找不到返回-1 
}
void BUZZER_SetFrequency(uint16_t freq)
{
		//根据查表得到的频率值反推ARR寄存器的值，1MHz为TIM4的计数频率
		uint32_t Period = 1000000 / freq;   
		//比较寄存器设为自动重载寄存器的1/2，这样占空比就是50%
		uint16_t Pulse  = Period  / 2;
    //写入TIM4的ARR寄存器
    TIM4->ARR  = Period;
    //写入TIM4的CCR1寄存器
    TIM4->CCR1 = Pulse;
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{	
	if(hadc->Instance==ADC1)
	{
		g_adc1_dma_complete_flag = 1;//采集完成标志
		HAL_ADC_Stop_DMA(&hadc1);
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_LPTIM_Counter_Start_IT(&hlptim2,600);  //启动LPTIM1计数和其中断
}
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	HAL_LPTIM_Counter_Stop_IT(hlptim);  //停止LPTIM1计数和其中断
	if (hlptim == &hlptim2){
	 BUZZER_SetFrequency(1000);
	__HAL_LPTIM_AUTORELOAD_SET(&hlptim1,3905);
	BUZZER_SetFrequency(262);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
	HAL_Delay(200);
	HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
	ADC_fft_collect(500000);
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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_LPTIM1_Init();
  MX_LPTIM2_Init();
  MX_TIM4_Init();

  /* USER CODE BEGIN 2 */
	Init_AD9958();
	//开始输出PWM信号
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); 
	//使能LPTIM1中断
	HAL_LPTIM_Counter_Start_IT(&hlptim1,3905);
	 printf("main.t2.txt=\"1 Ohm\"\xff\xff\xff");
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_CSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.CSIState = RCC_CSI_ON;
  RCC_OscInitStruct.CSICalibrationValue = RCC_CSICALIBRATION_DEFAULT;
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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_CSI;
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
		AD9958_SetPhase2Channel(0, 0);     // 通道0、通道1相位调节输出，单位：度
    AD9958_SetAmp2Channel(1023, 1023); // 通道0、通道1幅值调节输出，，单位：DAC分辨率
    HAL_Delay(200);
		while((g_adc1_dma_data1[0]&0x0000ffff) == 0){
			HAL_ADC_Start_DMA(&hadc1,g_adc1_dma_data1,ADC_DATA_LENGTH);//ADC的dma开始采集
			 while (g_adc1_dma_complete_flag == 0)
			{
			}
			 g_adc1_dma_complete_flag = 0;
		}
    for (i = 0; i < 1024; i++)
    {
        dataR[i] = ((float)(g_adc1_dma_data1[i] & 0x0000ffff) - 32768) * 3.3f / 65536;
    }
		memset(dataI, 0, 1024 * sizeof(float));
    kfft(dataR, dataI, 1024, 10, datafR, datafI);
	  printf("main.t2.txt=\"%.3f mV\"\xff\xff\xff",dataR[0]/1024);
//		paperPages = search(voltageArray,0,29,adc_voltage_Z);
		memset(g_adc1_dma_data1,0,ADC_DATA_LENGTH);//清除数据
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
