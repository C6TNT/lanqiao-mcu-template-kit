/* 封装开发板最底层的锁存器切换 */
#include "bsp_board.h"

/*
 * 函数名：Board_SelectHC138
 * 功能：选择 74HC138 当前控制的外设通道
 * 参数：channel - 通道号宏，例如 LED、数码管、蜂鸣器通道
 * 返回值：无
 * 备注：蓝桥杯开发板大量外设都通过这个通道切换访问
 */
void Board_SelectHC138(u8 channel)
{
    P2 = (P2 & 0x1F) | channel;
}

/*
 * 函数名：Board_Init
 * 功能：初始化开发板默认输出状态
 * 参数：无
 * 返回值：无
 * 备注：上电时先关闭蜂鸣器、继电器、LED 和数码管显示
 */
void Board_Init(void)
{
    Board_SelectHC138(HC138_BEEP);
    P0 = 0x00;
    P2 &= 0x1F;

    Board_SelectHC138(HC138_LED);
    P0 = 0xFF;
    P2 &= 0x1F;

    Board_SelectHC138(HC138_DIGIT);
    P0 = 0xFF;
    P2 &= 0x1F;

    Board_SelectHC138(HC138_SEG);
    P0 = 0xFF;
    P2 &= 0x1F;
}
