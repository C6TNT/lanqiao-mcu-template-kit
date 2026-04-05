/* 这里保留比赛最常用的 9600 串口发送与查询接口 */
#include "bsp_uart.h"

static void UART_SelectPin(void)
{
#if UART1_PIN_SELECT == UART1_PIN_P30P31
    P_SW1 &= ~0xC0;
#elif UART1_PIN_SELECT == UART1_PIN_P36P37
    P_SW1 = (P_SW1 & ~0xC0) | 0x40;
#elif UART1_PIN_SELECT == UART1_PIN_P16P17
    P_SW1 = (P_SW1 & ~0xC0) | 0x80;
#else
    P_SW1 = (P_SW1 & ~0xC0) | 0xC0;
#endif
}

void UART_Init9600(void)
{
    UART_SelectPin();

    SCON = 0x50;
    AUXR |= 0x01;
    AUXR |= 0x04;
    AUXR &= ~0x10;
    T2L = 0xC7;
    T2H = 0xFE;
    AUXR |= 0x10;
    RI = 0;
    TI = 0;
}

void UART_SendByte(u8 dat)
{
    SBUF = dat;
    while(TI == 0)
    {
    }
    TI = 0;
}

void UART_SendString(char *str)
{
    while(*str != '\0')
    {
        UART_SendByte(*str++);
    }
}

u8 UART_GetByte(u8 *dat)
{
    if(RI == 0)
    {
        return 0;
    }

    RI = 0;
    *dat = SBUF;
    return 1;
}

void UART_BeginKeyScan(void)
{
}

void UART_EndKeyScan(void)
{
}
