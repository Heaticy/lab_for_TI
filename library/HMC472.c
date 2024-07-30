//-----------------------------------------------------------------
// 程序描述:
//     HMC472驱动程序
// 作    者: Heaticy
// 当前版本: V1.0
// 历史版本:
//  - V1.0:  HMC472驱动程序
// 调试工具: 硬木课堂STM32H750核心板、HMC472模块
// 说明:引脚 PC0->V6,PC1->V5,PC4->V4,PC5->V3,PC6->V2,PC7->V1

// 头文件包含
#include "HMC472.h"
#include "stm32h7xx_hal.h"
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void HMC472_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: HMC472初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void HMC472_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOC_CLK_ENABLE(); // 使能GPIOC时钟

    GPIO_Initure.Pin =  GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; // 设为输出模式
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;    
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);   
	GPIOC->BSRR = GPIO_PIN_0 | GPIO_PIN_1 |GPIO_PIN_4 | GPIO_PIN_5| GPIO_PIN_6 | GPIO_PIN_7 ;// 初始化为高电平
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void HMC472_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: HMC472设置
// 入口参数: 一个十进制的数值num，0-63
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void HMC472set(unsigned int num) 
{
	num = (num & 0x00000003) | (num << 2);
    GPIOC->BSRR = (~num & 0x000000F3) | ((num & 0x000000F3) << 16);
}