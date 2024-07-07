//-----------------------------------------------------------------
// 程序描述:
//     LT驱动程序
// 作    者: Heaticy
// 当前版本: V1.0
// 历史版本:
//  - V1.0:  LT驱动程序
// 调试工具: 硬木课堂STM32H750核心板、LT模块
// 说明:引脚 PB8，PB9

// 头文件包含
#include "LT.h"
#include "stm32h7xx_hal.h"
//-----------------------------------------------------------------
// void LT_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: LT初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LT_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE(); // 使能GPIOB时钟

    GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; // 设为输出模式
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_Initure);

    // 初始化为低电平
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
}

//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void LTset(int num)
//-----------------------------------------------------------------
//
// 函数功能: 继电器设置，用0，1,2,3，控制两个bit信号的输出
// 入口参数: 一个10进制的数值num，但是只有可能是0,1,2,3
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LTset(int num)
{
    // 根据输入数值设置GPIO的状态
    GPIO_PinState pin8state = (num & 2) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    GPIO_PinState pin9state = (num & 1) ? GPIO_PIN_SET : GPIO_PIN_RESET;

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, pin8state);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, pin9state);
}
