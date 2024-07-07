//-----------------------------------------------------------------
// ��������:
//     AD9958��������ͷ�ļ�
// ��    ��: Heaticy
// ���Թ���: Ӳľ����STM32H750���İ塢HMC472ģ��
// ˵��:���� PA1-PA5
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
// AD9958�Ĵ�����ַ����
#define CSR_ADD 0x00   // CSR ͨ��ѡ��Ĵ���
#define FR1_ADD 0x01   // FR1 ���ܼĴ���1
#define FR2_ADD 0x02   // FR2 ���ܼĴ���2
#define CFR_ADD 0x03   // CFR ͨ�����ܼĴ���
#define CFTW0_ADD 0x04 // CTW0 ͨ��Ƶ��ת���ּĴ���
#define CPOW0_ADD 0x05 // CPW0 ͨ����λת���ּĴ���
#define ACR_ADD 0x06   // ACR ���ȿ��ƼĴ���
#define LSRR_ADD 0x07  // LSR ͨ������ɨ��Ĵ���
#define RDW_ADD 0x08   // RDW ͨ����������ɨ��Ĵ���
#define FDW_ADD 0x09   // FDW ͨ����������ɨ���
void Init_AD9958(void);

void delay1(u32 length);
void IntReset(void);  // AD9958��λ
void IO_Update(void); // AD9958��������
void Intserve(void);  // IO�ڳ�ʼ��

void WriteData_AD9958(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData, u8 temp); // AD9958����д�뺯��
void AD9958_Set_Freq(u32 Freq);                                                             // AD9958Ƶ������

void Write_frequence(u8 Channel, u32 Freq);  // AD9958ͨ����Ƶ������
void Write_Amplitude(u8 Channel, u16 Ampli); // AD9958ͨ�����������
void Write_Phase(u8 Channel, u16 Phase);     // AD9958ͨ������λ����

void AD9958_SetFrequency2Channel(u32 Freq2, u32 Freq3); // AD9958˫ͨ��Ƶ������
void AD9958_SetAmp2Channel(u32 Amp2, u32 Amp3);         // AD9958˫ͨ����������
void AD9958_SetPhase2Channel(u32 Phase2, u32 Phase3);   // AD9958˫ͨ����λ����

void mode1(uint32_t starthz, uint32_t stophz, uint32_t stephz); // ����ɨƵ����
#endif
