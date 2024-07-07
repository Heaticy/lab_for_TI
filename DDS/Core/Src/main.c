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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "AD9959.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t AD9226_Data[1034]={0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	Init_AD9958();
	AD9958_SetPhase2Channel(0,0);
	AD9958_SetFrequency2Channel(700000,500000);
	AD9958_SetAmp2Channel(1023,1023);
	
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
  MX_TIM1_Init(95);
  /* USER CODE BEGIN 2 */
//	HAL_HRTIM_WaveformCounterStart_IT(&hhrtim, HRTIM_TIMERID_MASTER);
//	HAL_HRTIM_SimpleBaseStart(&hhrtim, HRTIM_TIMERID_MASTER);
   AD9958_SetPhase2Channel(30,0);
		IO_Update();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	GPIOC->BSRR = GPIO_PIN_0<<16;
//	AD9226_Data = GPIOB->IDR;
//	GPIOC->BSRR = GPIO_PIN_0;	
//		GPIOC->ODR &= ~GPIO_PIN_0;
//	AD9226_Data = GPIOB->IDR;
//	GPIOC->ODR |= GPIO_PIN_0;	
    /* USER CODE END WHILE */

//	 AD9958_SetPhase2Channel(0,0);
//		IO_Update();
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

/* USER CODE BEGIN 4 */
////-----------------------------------------------------------------
//// void CPU_CACHE_Enable(void)
////-----------------------------------------------------------------
//// 
//// 函数功能: 启用CPU缓存
//// 入口参数: 无
//// 返 回 值: 无
//// 注意事项: 无
////
////-----------------------------------------------------------------
//void CPU_CACHE_Enable(void)
//{
//  SCB_EnableICache();	// 使能指令缓存
//  SCB_EnableDCache();	// 使能数据缓存
//	SCB->CACR|=1<<2;		// 设置D-Cache为Write Through，保证数据的一致性
//}

////-----------------------------------------------------------------
//// void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B)
////-----------------------------------------------------------------
//// 
//// 函数功能: 配置MPU
//// 入口参数: u32 Number：配置保护区域
////					 u32 BASEADDR：设置基地址
////					 u32 SIZE： 设置保护区域的大小
////					 u8 AP：设置访问权限
////					 u8 DE：是否允许指令访问
////					 u8 S：是否共享
////					 u8 C：是否缓存
////					 u8 B：是否缓冲
//// 返 回 值: 无
//// 注意事项: 无
////
////-----------------------------------------------------------------
//void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B)
//{
//	MPU_Region_InitTypeDef MPU_Initure;
//	
//	HAL_MPU_Disable(); 											// 配置 MPU 之前先关闭 MPU,配置完成以后在使能 MPU
//	MPU_Initure.Enable=MPU_REGION_ENABLE; 	// 使能该保护区域
//	MPU_Initure.Number=Number; 							// 设置保护区域
//	MPU_Initure.BaseAddress=BASEADDR; 			// 设置基址
//	MPU_Initure.Size=SIZE; 									// 设置保护区域大小
//	MPU_Initure.SubRegionDisable=0X00; 			// 禁止子区域
//	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;// 设置类型扩展域为 level0
//	MPU_Initure.AccessPermission=AP; 				// 设置访问权限,
//	MPU_Initure.DisableExec=DE; 						// 是否允许指令访问(允许读取指令)
//	MPU_Initure.IsShareable=S; 							// 是否共享
//	MPU_Initure.IsCacheable=C; 							// 是否缓存
//	MPU_Initure.IsBufferable=B; 						// 是否缓冲
//	HAL_MPU_ConfigRegion(&MPU_Initure); 		// 配置 MPU
//	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT); // 开启 MPU
//}

////-----------------------------------------------------------------
//// void MPU_Memory_Protection(void)
////-----------------------------------------------------------------
//// 
//// 函数功能: 设置需要的保护区域，AXI SRAM和FMC
//// 入口参数: 无
//// 返 回 值: 无
//// 注意事项: 无
////
////-----------------------------------------------------------------
//void MPU_Memory_Protection(void)
//{
//	// 配置AXI SRAM，共512KB字节，全访问，允许指令访问，禁止共享，允许缓存，允许缓冲
//	MPU_Set_Protection(
//		MPU_REGION_NUMBER0,
//		0x24000000,						
//		MPU_REGION_SIZE_512KB,
//		MPU_REGION_FULL_ACCESS,
//		MPU_INSTRUCTION_ACCESS_ENABLE,
//		MPU_ACCESS_NOT_SHAREABLE,
//		MPU_ACCESS_CACHEABLE,
//		MPU_ACCESS_BUFFERABLE
//	);
//	
//	// 配置FMC，共64MB字节，全访问，允许指令访问，禁止共享，禁止缓存，允许缓冲
//	MPU_Set_Protection(
//		MPU_REGION_NUMBER1,
//		0x60000000,				
//		MPU_REGION_SIZE_64MB,
//		MPU_REGION_FULL_ACCESS,
//		MPU_INSTRUCTION_ACCESS_ENABLE,
//		MPU_ACCESS_NOT_SHAREABLE,
//		MPU_ACCESS_NOT_CACHEABLE,
//		MPU_ACCESS_BUFFERABLE
//	);
//}

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
