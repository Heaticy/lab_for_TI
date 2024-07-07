//-----------------------------------------------------------------
// 程序描述:
//     AD9226驱动程序
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0:  AD9226驱动
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2、单路AD9226模块
// 说    明: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "ad9226.h"
//-----------------------------------------------------------------
#include "hrtim.h"
#include "usart.h"

extern uint32_t f;
//-----------------------------------------------------------------
// void AD9226_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: AD9226初始化
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void AD9226_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();			// 使能GPIOC时钟
	__HAL_RCC_GPIOD_CLK_ENABLE();			// 使能GPIOD时钟
	
	GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
										 GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
										 GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10| GPIO_PIN_11;
  GPIO_Initure.Mode = GPIO_MODE_INPUT;
  GPIO_Initure.Pull = GPIO_PULLUP;
  GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_Initure);
	
	GPIO_Initure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
										 GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
										 GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10| GPIO_PIN_11;
  GPIO_Initure.Mode = GPIO_MODE_INPUT;
  GPIO_Initure.Pull = GPIO_PULLUP;
  GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_Initure);
}


void AD9226_start(uint32_t freq){
	AD9226_Init();
	if (freq > 2000000){
    f = 240;
	}else if(freq > 200000 ){
		f = 240;
	}else if(freq > 100000 ){
		f = 480;
	}else if(freq > 50000 ){
		f = 960;
	}else if(freq > 25000 ){
		f = 1920;
	}else if(freq > 10000 ){
		f = 4800;
	}else if(freq > 5000 ){
		f = 9600;
	}else if(freq > 2500){
		f = 24000;
	}else{
		f = 48000;
	}
	MX_HRTIM_Init();
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
