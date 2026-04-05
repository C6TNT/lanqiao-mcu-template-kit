/* 蜂鸣器和继电器控制接口 */
#ifndef __BSP_BEEP_RELAY_H__
#define __BSP_BEEP_RELAY_H__

#include "bsp_board.h"

/*
 * 函数名：Beep_Set
 * 功能：控制蜂鸣器开关
 * 参数：on - 是否打开
 * 返回值：无
 * 备注：提示音和报警常用
 */
void Beep_Set(bit on);

/*
 * 函数名：Relay_Set
 * 功能：控制继电器开关
 * 参数：on - 是否打开
 * 返回值：无
 * 备注：执行器类题型常用
 */
void Relay_Set(bit on);

/*
 * 函数名：BeepRelay_AllOff
 * 功能：同时关闭蜂鸣器和继电器
 * 参数：无
 * 返回值：无
 * 备注：上电初始化常用
 */
void BeepRelay_AllOff(void);

#endif
