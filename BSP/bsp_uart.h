/* 串口基础发送与查询接口 */
#ifndef __BSP_UART_H__
#define __BSP_UART_H__

#include "..\Core\common.h"

/* UART1 引脚选择宏 */
#define UART1_PIN_P30P31 0
#define UART1_PIN_P36P37 1
#define UART1_PIN_P16P17 2
#define UART1_PIN_P43P44 3

/*
 * 默认把 UART1 放回 P3.0/P3.1。
 * 当前模板通过 Timer2 解决了定时器冲突，
 * 引脚仍保持和常见下载口一致，便于直接串口观察。
 */
#define UART1_PIN_SELECT UART1_PIN_P30P31

void UART_Init9600(void);
void UART_SendByte(u8 dat);
void UART_SendString(char *str);
u8 UART_GetByte(u8 *dat);
void UART_BeginKeyScan(void);
void UART_EndKeyScan(void);

#endif
