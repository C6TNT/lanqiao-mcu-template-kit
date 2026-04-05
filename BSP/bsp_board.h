/* 板级初始化与 74HC138 通道选择 */
#ifndef __BSP_BOARD_H__
#define __BSP_BOARD_H__

#include "..\Core\common.h"

#define HC138_LED   0x80
#define HC138_BEEP  0xA0
#define HC138_DIGIT 0xC0
#define HC138_SEG   0xE0

/*
 * 函数名：Board_SelectHC138
 * 功能：选择 74HC138 当前通道
 * 参数：channel - 通道宏
 * 返回值：无
 * 备注：访问板上外设前通常先切换这个通道
 */
void Board_SelectHC138(u8 channel);

/*
 * 函数名：Board_Init
 * 功能：初始化开发板默认状态
 * 参数：无
 * 返回值：无
 * 备注：建议系统启动后优先调用
 */
void Board_Init(void);

#endif
