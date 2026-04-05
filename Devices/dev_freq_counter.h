/* 频率测量设备层接口 */
#ifndef __DEV_FREQ_COUNTER_H__
#define __DEV_FREQ_COUNTER_H__

#include "..\Core\common.h"

/*
 * 频率测量模块说明：
 * 1. 默认使用 Timer0 作为外部计数器
 * 2. 默认使用 T0(P3.4) 作为计数输入口
 * 3. 采用 250ms 门控时间，最终结果乘 4 得到 Hz
 * 4. 使用后台非阻塞测量，不会卡住按键扫描和主循环
 */

void FREQ_InitCounter0(void);
void FREQ_PollTask(void);
u16 FREQ_GetHz(void);

#endif
