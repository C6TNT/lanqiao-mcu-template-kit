/* 统一系统时基：把常见周期任务做成标志位 */
#ifndef __BSP_TICK_H__
#define __BSP_TICK_H__

#include "..\Core\common.h"

extern volatile bit g_flag_1ms;
extern volatile bit g_flag_10ms;
extern volatile bit g_flag_100ms;
extern volatile bit g_flag_500ms;
extern volatile bit g_flag_1s;
extern volatile u32 g_ms_ticks;

/*
 * 函数名：Tick_Init
 * 功能：初始化系统节拍
 * 参数：无
 * 返回值：无
 * 备注：默认使用 Timer1 提供 1ms 时基
 */
void Tick_Init(void);

#endif
