/**********************************************************
                       康威电子

功能：stm32f103rct6控制，75MHz时钟， AD9834正弦波点频输出，范围0-30M(可编辑0-100M)，
            方波：50k-5M，三角波：0-10M，扫频默认正弦波 。
            显示：12864cog
接口：控制接口请参照AD9834.h  按键接口请参照key.h
时间：2015/11/10
版本：1.0
作者：康威电子

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/
#include "stm32h7xx_hal.h"
#include "AD9834.h"
/****************************************************************

函数名称: AD9834_Write_16Bits

功    能: 向AD9834写入16为数据

参    数: data --  要写入的16位数据

返回值  : 无

*****************************************************************/

void AD9834_Write_16Bits(unsigned int data)
{
    unsigned char i = 0;

    AD9834_SCLK_SET;
    AD9834_FSYNC_CLR;

    for (i = 0; i < 16; i++)
    {
        if (data & 0x8000)
            AD9834_SDATA_SET;
        else
            AD9834_SDATA_CLR;

        AD9834_SCLK_CLR;
        data <<= 1;
        AD9834_SCLK_SET;
    }
    AD9834_SDATA_SET;
    AD9834_FSYNC_SET;
}

/***********************************************************************************

函数名称：AD9834_Select_Wave

功    能：软件为控制，

    --------------------------------------------------

    IOUT正弦波 ，SIGNBITOUT方波 ，写FREQREG0 ，写PHASE0

    ad9834_write_16bit(0x2028)   一次性写FREQREG0

    ad9834_write_16bit(0x0038)   单独改写FREQREG0的LSB

    ad9834_write_16bit(0x1038)   单独改写FREQREG0的MSB

    --------------------------------------------------

    IOUT三角波 ，写PHASE0

    ad9834_write_16bit(0x2002)   一次性写FREQREG0

    ad9834_write_16bit(0x0002)   单独改写FREQREG0的LSB

    ad9834_write_16bit(0x1008)   单独改写FREQREG0的MSB

参    数：initdata -- 要输入的命令

返回值  ：无

************************************************************************************/
void AD9834_Select_Wave(unsigned int initdata)

{

    AD9834_FSYNC_SET;

    AD9834_SCLK_SET;

    AD9834_RESET_SET;

    AD9834_RESET_SET;

    AD9834_RESET_CLR;

    AD9834_Write_16Bits(initdata);
}

/****************************************************************

函数名称: Init_AD9834

功    能: 初始化AD9834控制引脚 参    数: 无

返回值  : 无

*****************************************************************/
// B6 FS  B5 PS    B7 FSY   B8 SCK   B9 SDA B10 RST

void AD9834_Init()

{

    GPIO_InitTypeDef GPIO_InitStructure;

    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitStructure.Pin = AD9834_FSYNC | AD9834_SCLK | AD9834_SDATA | AD9834_RESET | AD9834_FS | AD9834_PS;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    HAL_GPIO_Init(AD9834_Control_Port, &GPIO_InitStructure);
    HAL_GPIO_WritePin(AD9834_Control_Port, AD9834_FS, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(AD9834_Control_Port, AD9834_PS, GPIO_PIN_RESET);

    AD9834_Write_16Bits(0x2100);
    AD9834_Write_16Bits(0x2038);
    AD9834_Write_16Bits(0XC000);
    AD9834_Write_16Bits(0x2100);
}

/****************************************************************

函数名称: AD9834_Set_Freq

功    能: 设置频率值

参    数: freq_number -- 要写入的平率寄存器(FREQ_0或FREQ_1)

          freq -- 频率值 (Freq_value(value)=Freq_data(data)*FCLK/2^28)

返回值  : 无

*****************************************************************/
void AD9834_Set_Freq(unsigned char freq_number, unsigned long freq)
{
    unsigned long FREQREG = (unsigned long)(268435456.0 / AD9834_SYSTEM_COLCK * freq)+8;
    unsigned int FREQREG_LSB_14BIT = (unsigned int)FREQREG;
    unsigned int FREQREG_MSB_14BIT = (unsigned int)(FREQREG >> 14);

    if (freq_number == FREQ_0)
    {
        FREQREG_LSB_14BIT &= ~(1U << 15);
        FREQREG_LSB_14BIT |= 1 << 14;
        FREQREG_MSB_14BIT &= ~(1U << 15);
        FREQREG_MSB_14BIT |= 1 << 14;
    }
    else
    {
        FREQREG_LSB_14BIT &= ~(1 << 14);
        FREQREG_LSB_14BIT |= 1U << 15;
        FREQREG_MSB_14BIT &= ~(1 << 14);
        FREQREG_MSB_14BIT |= 1U << 15;
    }
    AD9834_Write_16Bits(FREQREG_LSB_14BIT);
    AD9834_Write_16Bits(FREQREG_MSB_14BIT);
}
//============================================================//

/*   注：使用时先调用初始化函数初始化IO配置，然后调用AD9834_Select_Wave()选择

波形，最后选择频率值即可输出。 */
