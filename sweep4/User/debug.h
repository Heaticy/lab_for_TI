#ifndef debug_H
#define debug_H
/**
* @file debug.h
* @brief debug??????
* @date 2018-07-04
* @version V1.2
*/
	
#include <stdio.h> //??printf
#include "stm32h7xx_hal.h" // ??HAL_StatusTypeDef

#define USART_REC_LEN  			1500  	//定义最大接收字节数 200

#define RXBUFFERSIZE   1 //缓存大小

extern UART_HandleTypeDef *huart_debug;
extern uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
extern uint16_t USART_RX_STA;       //接收状态标记	


#endif /*debug_H*/

