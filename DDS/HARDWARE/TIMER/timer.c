////-----------------------------------------------------------------
//// 程序描述:
//// 		 定时器中断驱动程序
//// 作    者: 凌智电子
//// 开始日期: 2020-11-11
//// 完成日期: 2020-11-11
//// 修改日期: 
//// 当前版本: V1.0
//// 历史版本:
////  - V1.0: (2020-11-11)定时器中断初始化，定时器中断服务函数
//// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
//// 说    明: 
////    
////-----------------------------------------------------------------

////-----------------------------------------------------------------
//// 头文件包含
////-----------------------------------------------------------------
//#include "timer.h"
//#include "ad9226.h"
////-----------------------------------------------------------------
//TIM_HandleTypeDef TIM3_Handler;      // 定时器3句柄 
//extern uint16_t AD9226_Data[Draw_Number];
//extern uint8_t  Time_Flag;
//uint16_t k=0;
////-----------------------------------------------------------------
//// vvoid TIM3_Init(u16 arr,u16 psc)
////-----------------------------------------------------------------
////
//// 函数功能: 定时器3中断初始化
//// 入口参数: u16 arr：自动重装值
////					 u16 psc：时钟预分频数
//// 返回参数: 无
//// 注意事项: 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
////					 Ft=定时器工作频率,单位:Mhz
////					 这里使用的是定时器3!(定时器3挂在APB1上)
////
////-----------------------------------------------------------------
//void TIM3_Init(u16 arr,u16 psc)
//{  
//	__HAL_RCC_TIM3_CLK_ENABLE();            // 使能TIM3时钟
//	
//  TIM3_Handler.Instance=TIM3;                          		// 通用定时器3
//  TIM3_Handler.Init.Prescaler=psc;                     		// 分频系数
//  TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    		// 向上计数器
//  TIM3_Handler.Init.Period=arr;                        		// 自动装载值
//  TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;	// 时钟分频因子
//  HAL_TIM_Base_Init(&TIM3_Handler);
//    
//	HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    // 设置中断优先级，抢占优先级1，子优先级3
//	HAL_NVIC_EnableIRQ(TIM3_IRQn);          // 开启ITM3中断   
//	
//  HAL_TIM_Base_Start_IT(&TIM3_Handler); // 使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
//}

////-----------------------------------------------------------------
//// void TIM3_IRQHandler(void)
////-----------------------------------------------------------------
////
//// 函数功能: /定时器3中断服务函数
//// 入口参数: 无
//// 返回参数: 无
//// 注意事项: 无
////
////-----------------------------------------------------------------
//void TIM3_IRQHandler(void)
//{
//	__HAL_TIM_CLEAR_IT(&TIM3_Handler,TIM_IT_UPDATE);

//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
//		AD9226_Data[k] = GPIOC->IDR;
////		AD9226_Data[k] = (uint16_t)(((double)(AD9226_Data[k]&0x0FFF)/4095*10000)/50);
//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
//	k++;
//	if(k==Sampling_Number)
//	{
//		Time_Flag = 1;
//		k = 0;
//		HAL_TIM_Base_Stop_IT(&TIM3_Handler);
//		HAL_NVIC_DisableIRQ(TIM3_IRQn);
//	}
//}

////-----------------------------------------------------------------
//// End Of File
////-----------------------------------------------------------------
