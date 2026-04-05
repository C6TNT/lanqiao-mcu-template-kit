/* LED 控制接口 */
#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "bsp_board.h"

/*
 * 函数名：LED_WriteByte
 * 功能：一次性写入整组 LED 状态
 * 参数：value - LED 状态字节
 * 返回值：无
 * 备注：低电平表示点亮
 */
void LED_WriteByte(u8 value);

/*
 * 函数名：LED_Set
 * 功能：设置单个 LED 的开关状态
 * 参数：index - LED 编号
 * 参数：on - 是否点亮
 * 返回值：无
 * 备注：适合业务层单点控制
 */
void LED_Set(u8 index, bit on);

/*
 * 函数名：LED_AllOff
 * 功能：关闭所有 LED
 * 参数：无
 * 返回值：无
 * 备注：初始化和清状态常用
 */
void LED_AllOff(void);

#endif
