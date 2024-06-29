//-----------------------------------------------------------------
// 程序描述:
//     AD9958驱动程序头文件
// 作    者: Heaticy
// 调试工具: 硬木课堂STM32H750核心板、HMC472模块
// 说明:引脚 PA1-PA5
// PA1->CSB
// PA2->SCLK
// PA3->SDIO
// PA4->IOUP
// PA5->RST
//-----------------------------------------------------------------
#ifndef AD9958_H
#define AD9958_H
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#include "stm32h7xx_hal.h"
// AD9958寄存器地址定义
#define CSR_ADD 0x00   // CSR 通道选择寄存器
#define FR1_ADD 0x01   // FR1 功能寄存器1
#define FR2_ADD 0x02   // FR2 功能寄存器2
#define CFR_ADD 0x03   // CFR 通道功能寄存器
#define CFTW0_ADD 0x04 // CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05 // CPW0 通道相位转换字寄存器
#define ACR_ADD 0x06   // ACR 幅度控制寄存器
#define LSRR_ADD 0x07  // LSR 通道线性扫描寄存器
#define RDW_ADD 0x08   // RDW 通道线性向上扫描寄存器
#define FDW_ADD 0x09   // FDW 通道线性向下扫描寄
void Init_AD9958(void);

void delay1(u32 length);
void IntReset(void);  // AD9958复位
void IO_Update(void); // AD9958更新数据
void Intserve(void);  // IO口初始化

void WriteData_AD9958(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData, u8 temp); // AD9958数据写入函数
void AD9958_Set_Freq(u32 Freq);                                                             // AD9958频率配置

void Write_frequence(u8 Channel, u32 Freq);  // AD9958通道与频率配置
void Write_Amplitude(u8 Channel, u16 Ampli); // AD9958通道与幅度配置
void Write_Phase(u8 Channel, u16 Phase);     // AD9958通道与相位配置

void AD9958_SetFrequency2Channel(u32 Freq2, u32 Freq3); // AD9958双通道频率配置
void AD9958_SetAmp2Channel(u32 Amp2, u32 Amp3);         // AD9958双通道幅度配置
void AD9958_SetPhase2Channel(u32 Phase2, u32 Phase3);   // AD9958双通道相位配置

void mode1(uint32_t starthz, uint32_t stophz, uint32_t stephz); // 简易扫频函数
#endif