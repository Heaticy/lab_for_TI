//-----------------------------------------------------------------
// ��������:
// 		 ��ʱ���ж���������ͷ�ļ�
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0: (2020-11-11)��ʱ���жϳ�ʼ������ʱ���жϷ�����
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2
// ˵    ��: 
//    
//-----------------------------------------------------------------
#ifndef _TIMER_H
#define _TIMER_H
#include "stm32h7xx_hal.h"
typedef int32_t  				s32;
typedef int16_t 				s16;
typedef int8_t  				s8;

typedef const int32_t 	sc32;  
typedef const int16_t 	sc16;  
typedef const int8_t 		sc8;  

typedef __IO int32_t  	vs32;
typedef __IO int16_t  	vs16;
typedef __IO int8_t   	vs8;

typedef __I int32_t 		vsc32;  
typedef __I int16_t 		vsc16; 
typedef __I int8_t 			vsc8;   

typedef uint32_t  			u32;
typedef uint16_t 				u16;
typedef uint8_t  				u8;

typedef const uint32_t 	uc32;  
typedef const uint16_t 	uc16;  
typedef const uint8_t 	uc8; 

typedef __IO uint32_t  	vu32;
typedef __IO uint16_t 	vu16;
typedef __IO uint8_t  	vu8;

typedef __I uint32_t 		vuc32;  
typedef __I uint16_t 		vuc16; 
typedef __I uint8_t 		vuc8;  
//-----------------------------------------------------------------
// �궨��
//-----------------------------------------------------------------
#define Draw_Number  			280    			// ���Ƶ���
#define Sampling_Number  	290    			// ��������
//-----------------------------------------------------------------
// ����
//-----------------------------------------------------------------
extern TIM_HandleTypeDef TIM3_Handler;      // ��ʱ����� 
//-----------------------------------------------------------------
// �ⲿ��������
//-----------------------------------------------------------------
extern void TIM3_Init(u16 arr,u16 psc);
#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
