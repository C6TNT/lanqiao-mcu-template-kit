/* 业务层总入口：学生刷题时主要改这里 */
#ifndef __APP_H__
#define __APP_H__

#include "..\BSP\bsp_board.h"
#include "..\BSP\bsp_led.h"
#include "..\BSP\bsp_beep_relay.h"
#include "..\BSP\bsp_seg.h"
#include "..\BSP\bsp_key.h"
#include "..\BSP\bsp_uart.h"
#include "..\BSP\bsp_tick.h"
#include "..\Devices\dev_at24c02.h"
#include "..\Devices\dev_pcf8591.h"
#include "..\Devices\dev_ds18b20.h"
#include "..\Devices\dev_ds1302.h"
#include "..\Devices\dev_ultrasonic.h"
#include "..\Devices\dev_freq_counter.h"

/*
 * 函数名：App_Init
 * 功能：完成业务层初始化
 * 参数：无
 * 返回值：无
 * 备注：比赛时常在这里决定启用哪些模块
 */
void App_Init(void);

/*
 * 函数名：App_Loop
 * 功能：执行业务层主循环
 * 参数：无
 * 返回值：无
 * 备注：处理按键、页面、采样、报警、串口等任务
 */
void App_Loop(void);

#endif
