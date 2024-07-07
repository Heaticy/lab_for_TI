//-----------------------------------------------------------------
// ��������:
//     AD9226��������
// ��    ��: ���ǵ���
// ��ʼ����: 2020-11-11
// �������: 2020-11-11
// �޸�����: 
// ��ǰ�汾: V1.0
// ��ʷ�汾:
//  - V1.0:  AD9226����
// ���Թ���: ����STM32H750���İ塢LZE_ST_LINK2����·AD9226ģ��
// ˵    ��: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
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
// ��������: AD9226��ʼ��
// ��ڲ���: �� 
// �� �� ֵ: ��
// ע������: ��
//
//-----------------------------------------------------------------
void AD9226_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOC_CLK_ENABLE();			// ʹ��GPIOCʱ��
	__HAL_RCC_GPIOD_CLK_ENABLE();			// ʹ��GPIODʱ��
	
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
