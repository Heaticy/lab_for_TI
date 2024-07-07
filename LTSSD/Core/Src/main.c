/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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
#include "fft.h"
#include <stdlib.h>
#include "AD9958.h"
#include "debug.h"
#include <math.h>
#include "LT.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define ADC_DATA_LENGTH 1024
uint32_t g_adc1_dma_data1[ADC_DATA_LENGTH];//定义adc1采集数据存放数组
volatile uint8_t g_adc1_dma_complete_flag = 0;	//adc1数据dma采集完成标志，在dma采集完成
uint32_t g_adc2_dma_data1[ADC_DATA_LENGTH];//定义adc1采集数据存放数组
volatile uint8_t g_adc2_dma_complete_flag = 0;	//adc1数据dma采集完成标志，在dma采集完成
float dataR[1024] = {0};
float dataI[1024] = {0};
float datafR[1024] = {0};
float datafI[1024] = {0};
int offset = 0;
int offset_ = 0;
int phase = 0;
int GP = 0;
volatile uint8_t adjust_flag = 1;
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
void ADC_fft_collect(void)
{
    uint32_t i = 0;
    adjust_flag = 0;
    HAL_Delay(20);
		memset(g_adc1_dma_data1,0,ADC_DATA_LENGTH * 4);//清除数据
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
			  printf("%f\n",dataR[i]);
    }
		memset(dataI, 0, 1024 * sizeof(float));
    kfft(dataR, dataI, 1024, 10, datafR, datafI);
		for (i = 0; i < 1024; i++)
    {
				dataR[i] = dataR[i]/1024;
				printf("%f\n",dataR[i]);
    }
		float max_value1 = -100;
		float max_value2 = -100;
		uint32_t max_index1 = 0;
		uint32_t max_index2 = 0;

		for (i = 0; i < 512; i++) {
				if (dataR[i] > max_value1) {
						max_value2 = max_value1;
						max_index2 = max_index1;
						max_value1 = dataR[i];
						max_index1 = i;
				} else if (dataR[i] > max_value2) {
						max_value2 = dataR[i];
						max_index2 = i;
				}
		}
		if (max_index1 > max_index2){
			int temp = max_index2;
			max_index2 = max_index1;
			max_index1 = temp;
		}
		if (max_value2 >0.05f){

			if ( (dataR[3*max_index1] > 0.003f) && dataR[5*max_index1] > 0.003f){
							//两个三角波
					if ( (dataR[3*max_index2] > 0.003f) && dataR[5*max_index2] > 0.003f){
						  LTset(3);
							Init_AD9958();
							AD9958_SetFrequency2Channel(20 * 2500 * max_index1,2500* 20 * max_index2);   // AD9958两个通道同时输出，单位：Hz
							AD9958_SetPhase2Channel(0, phase);     // 通道0、通道1相位调节输出，单位：度
							AD9958_SetAmp2Channel(1023, 1023); // 通道0、通道1幅值调节输出，，单位：DAC分辨率
						  
					}
								//三角波 + 正弦
					else{
						  LTset(2);
							Init_AD9958();
							AD9958_SetFrequency2Channel(20 * 2500 * max_index1,2500* 20 * max_index2);   // AD9958两个通道同时输出，单位：Hz
							AD9958_SetPhase2Channel(0, phase);     // 通道0、通道1相位调节输出，单位：度
							AD9958_SetAmp2Channel(1023, 1023); // 通道0、通道1幅值调节输出，，单位：DAC分辨率
					}
				}
			else {
				      LTset(1);
							//正弦+ 三角
							if ( (dataR[3*max_index2] > 0.003f) && dataR[5*max_index2] > 0.003f){
							Init_AD9958();
							AD9958_SetFrequency2Channel(20 * 2500 * max_index1,2500* 20 * max_index2);   // AD9958两个通道同时输出，单位：Hz
							AD9958_SetPhase2Channel(0, phase);     // 通道0、通道1相位调节输出，单位：度
							AD9958_SetAmp2Channel(1023, 1023); // 通道0、通道1幅值调节输出，，单位：DAC分辨率
					}
							//正弦加正弦
					else{
						  LTset(0);
							Init_AD9958();
							AD9958_SetFrequency2Channel(20 * 2500 * max_index1,2500* 20 * max_index2);   // AD9958两个通道同时输出，单位：Hz
							AD9958_SetPhase2Channel(0, phase);     // 通道0、通道1相位调节输出，单位：度
							AD9958_SetAmp2Channel(1023, 1023); // 通道0、通道1幅值调节输出，，单位：DAC分辨率
					}
			}
		}
//	  printf("main.t2.txt=\"%.3f mV\"\xff\xff\xff",dataR[0]/1024);
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
	else if(hadc->Instance==ADC2)
	{
		g_adc2_dma_complete_flag = 1;//采集完成标志
		HAL_ADC_Stop_DMA(&hadc2);
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
		if (GPIO_Pin == GPIO_PIN_3)
		{			
			GP = 3;
		}
	if (GPIO_Pin == GPIO_PIN_12)
		{			
			GP = 12;
		}
	HAL_LPTIM_Counter_Start_IT(&hlptim2,800);  //启动LPTIM1计数和其中断
}
void HAL_LPTIM_AutoReloadMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
	HAL_LPTIM_Counter_Stop_IT(hlptim);  //停止LPTIM1计数和其中断
	if (hlptim == &hlptim2){
		if (GP == 12)
		{
		 	 BUZZER_SetFrequency(1000);
			__HAL_LPTIM_AUTORELOAD_SET(&hlptim1,3905);
			BUZZER_SetFrequency(262);
			HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
			HAL_Delay(200);
			HAL_TIM_PWM_Stop(&htim4,TIM_CHANNEL_1);
			ADC_fft_collect();
			GP = 0;
		}
		if (GP == 3)
		{
		  phase = (phase + 5) % 180;
			AD9958_SetPhase2Channel(0, phase);     // 通道0、通道1相位调节输出，单位：度
			AD9958_SetAmp2Channel(1023, 1023); // 通道0、通道1幅值调节输出，，单位：DAC分辨率
			GP = 0;
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
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	//开始输出PWM信号
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1); 
	//使能LPTIM1中断
	HAL_LPTIM_Counter_Start_IT(&hlptim1,3905);
	LT_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1){
		 if(adjust_flag){
			HAL_ADC_Start_DMA(&hadc2,g_adc2_dma_data1,ADC_DATA_LENGTH);
			while(g_adc2_dma_complete_flag == 0){}
			g_adc2_dma_complete_flag = 0;
			uint32_t i;
			for (i = 0; i < 1024; i++)
			{
					dataR[i] = ((float)(g_adc2_dma_data1[i] & 0x0000ffff)) * 3.3f / 65536;
			}
			memset(dataI, 0, 1024 * sizeof(float));
			kfft(dataR, dataI, 1024, 10, datafR, datafI);
			offset_ = dataR[0] / 1024 / 3.3f  * 600.0f - 200.0f;
			
			printf("%d\n",offset);
			memset(dataR, 0 , ADC_DATA_LENGTH * sizeof(float));//清除数据
			if (offset_ != offset){
				offset = offset_;
				Init_AD9958();
				AD9958_SetPhase2Channel(0,0);
				AD9958_SetFrequency2Channel(1000000,2000000);
				AD9958_SetAmp2Channel(1023,1023);  
			}
			HAL_Delay(500);
			
		}
 }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.PLL3.PLL3M = 25;
  PeriphClkInitStruct.PLL3.PLL3N = 256;
  PeriphClkInitStruct.PLL3.PLL3P = 10;
  PeriphClkInitStruct.PLL3.PLL3Q = 10;
  PeriphClkInitStruct.PLL3.PLL3R = 5;
  PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_0;
  PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
  PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_CSI;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
