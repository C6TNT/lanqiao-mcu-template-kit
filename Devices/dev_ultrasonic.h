/* 超声波测距设备层接口 */
#ifndef __DEV_ULTRASONIC_H__
#define __DEV_ULTRASONIC_H__

#include "..\Core\common.h"

sbit ULT_TX = P1^0;
sbit ULT_RX = P1^1;

/*
 * 函数名：Ultrasonic_Init
 * 功能：初始化超声波测距模块
 * 参数：无
 * 返回值：无
 * 备注：当前使用 PCA 作为计时基准，避免占用 Timer1/Timer2
 */
void Ultrasonic_Init(void);

/*
 * 函数名：Ultrasonic_ReadCm
 * 功能：读取距离值
 * 参数：无
 * 返回值：距离，单位厘米
 * 备注：超时返回 999
 */
u16 Ultrasonic_ReadCm(void);

#endif
