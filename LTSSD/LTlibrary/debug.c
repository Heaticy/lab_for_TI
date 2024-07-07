#include "debug.h"
#include "stm32h7xx_hal.h"
#include "usart.h"
#include <string.h>
#include "AD9958.h"
#include <stdlib.h>
#include "main.h"
UART_HandleTypeDef *huart_debug = &huart1; ///< Debug串口

static uint8_t device_er[2] = {'E', 'R'}; // 返回错误ER
uint8_t USART_RX_BUF[USART_REC_LEN];      // 接收缓冲,最大USART_REC_LEN个字节.

void reply_er(void);

// 接收状态
// bit15，	接收完成标志
// bit14，	接收到0x0d
// bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA = 0;         // 接收状态标记
uint8_t aRxBuffer[RXBUFFERSIZE];   // HAL库使用的串口接收缓冲
uint8_t temp[USART_REC_LEN] = {0}; // 接收缓冲,最大USART_REC_LEN个字节.
int single_sweep = 0;
int double_sweep= 0;
#if defined(__GNUC__) // GCC
/*	With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
    set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#elif defined(__CC_ARM) // ARMCC
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#else
#error "Unknown compiler"
#endif
static uint8_t s_usart_tmp;

#if defined(__CC_ARM)
int fputc(int ch, FILE *f)
{
    s_usart_tmp = (uint8_t)(ch);
    HAL_UART_Transmit(huart_debug, &s_usart_tmp, 1, 1);
    //	HAL_UART_Transmit_IT(huart_debug,&s_usart_tmp,1);
    return ch;
}
#elif defined(__GNUC__)
int __io_putchar(int ch)
{
    s_usart_tmp = (uint8_t)(ch);
    HAL_UART_Transmit(huart_debug, &s_usart_tmp, 1, 1);
    //	HAL_UART_Transmit_IT(huart_debug,&s_usart_tmp,1);
    return ch;
}
#endif

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if (huart->Instance == LPUART1)
//    {
//        if ((USART_RX_STA & 0x8000) == 0) // 接收未完成
//        {
//            if (USART_RX_STA & 0x4000) // 接收到了0x0d
//            {
//                if (aRxBuffer[0] != 0x0a)
//                    USART_RX_STA = 0; // 接收错误,重新开始
//                else
//                {
//                    USART_RX_STA |= 0x8000; // 接收完成了
//                    // 执行操作
//                }
//            }
//            else // 还没收到0X0D
//            {
//                if (aRxBuffer[0] == 0x0d)
//                    USART_RX_STA |= 0x4000;
//                else
//                {
////                    if (aRxBuffer[0] == '\n') // 最后一个字符是换行符
////                    {
////                        // 清空接收缓冲区之前的检查和操作
////                        if (strcmp((const char *)USART_RX_BUF, "single_sweep_open") == 0)
////                        {
////                            single_sweep = 1;
////                        }
////                        else if (strcmp((const char *)USART_RX_BUF, "single_sweep_closed") == 0)
////                        {
////                            single_sweep = 0;
////                        }
////                        char *endPtr;
////                        long int value = strtol((const char *)USART_RX_BUF, &endPtr, 10);
////                        if (*endPtr == '\0' && value >= 0 && value <= 5000000 && single_sweep == 1)
////                        {
////                            // USART_RX_BUF 是介于 0 到 5000000 之间的数字
////													  Init_AD9958(); // AD9958初始化
////                            ADC_fft_collect(value);
////                        }
////												if (strcmp((const char *)USART_RX_BUF, "double_sweep_open") == 0)
////												{													  
////													  Init_AD9958(); // AD9958初始化
////														double_sweep = 1;
////														uint32_t freqs[20];  // 初始化频率数组，大小为20
////														double start = 3;  // 对数扫描起始点，以10为底数，1k对应3
////														double end = 6;  // 对数扫描终止点，以10为底数，800k对应5.9031
////														double delta = (end - start) / 19;  // 步长，根据扫描点数计算，注意：分母为总扫描点数减一
////														for (int i = 0; i < 20; i++) {
////																freqs[i] = (uint32_t)pow(10, start + i * delta);  // 计算每个扫描点的频率
//////																ADC_fft_sweep(freqs[i]);  // 使用扫描点频率进行扫频
////														}											
////												}
////                        else if (strcmp((const char *)USART_RX_BUF, "double_sweep_closed") == 0)
////                        {
////                            double_sweep = 0;
////                        }
////                        memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF)); // 清空接收缓冲区
////                        USART_RX_STA = 0;
////                    }
//                    else
//                    {
//                        USART_RX_BUF[USART_RX_STA & 0X3FFF] = aRxBuffer[0];
//                        USART_RX_STA++;
//                        if (USART_RX_STA > (USART_REC_LEN - 1))
//                        {
//                            USART_RX_STA = 0; // 接收数据错误,重新开始接收
//                            reply_er();
//                        }
//                    }
//                }
//            }
//        }
//    }
//}

// 串口4中断服务程序
void UART4_IRQHandler(void)
{
    uint32_t timeout = 0;
    uint32_t maxDelay = 0x1FFFF;
#if SYSTEM_SUPPORT_OS // 使用OS
    OSIntEnter();
#endif

    HAL_UART_IRQHandler(huart_debug); // 调用HAL库中断处理公用函数

    timeout = 0;
    while (HAL_UART_GetState(huart_debug) != HAL_UART_STATE_READY) // 等待就绪
    {
        timeout++; ////超时处理
        if (timeout > maxDelay)
        {
            reply_er();
            break;
        }
    }

    timeout = 0;
    while (HAL_UART_Receive_IT(huart_debug, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK) // 一次处理完成之后，重新开启中断并设置RxXferCount为1
    {
        timeout++; // 超时处理
        if (timeout > maxDelay)
        {
            reply_er();
            break;
        }
    }
#if SYSTEM_SUPPORT_OS // 使用OS
    OSIntExit();
#endif
}

void reply_er(void)
{
    HAL_UART_Transmit(huart_debug, (uint8_t *)device_er, sizeof(device_er), 10);
    while (__HAL_UART_GET_FLAG(huart_debug, UART_FLAG_TC) != SET)
        ;
    USART_RX_STA = 0;
}
