/* 独立按键和矩阵按键统一接口 */
#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

#include "..\Core\common.h"

#define KEY_NONE 0

#define KEY_LONG_TICKS    25
#define KEY_DOUBLE_TICKS  40

/*
 * 函数名：Key_ReadIndependent
 * 功能：读取独立按键原始状态
 * 参数：无
 * 返回值：按键编号
 * 备注：不带消抖
 */
u8 Key_ReadIndependent(void);

/*
 * 函数名：Key_ReadMatrix
 * 功能：读取矩阵按键原始状态
 * 参数：无
 * 返回值：按键编号
 * 备注：不带消抖
 */
u8 Key_ReadMatrix(void);

/*
 * 函数名：Key_GetEvent
 * 功能：获取一次按键事件
 * 参数：无
 * 返回值：按键编号
 * 备注：读取后事件自动清空
 */
u8 Key_GetEvent(void);

/*
 * 函数名：Key_GetShortEvent
 * 功能：获取确认后的单击事件
 * 参数：无
 * 返回值：按键编号
 * 备注：为了兼容双击判定，单击事件会在等待双击窗口结束后产生
 */
u8 Key_GetShortEvent(void);

/*
 * 函数名：Key_GetLongEvent
 * 功能：获取长按事件
 * 参数：无
 * 返回值：按键编号
 * 备注：默认长按阈值为 25 个 10ms 节拍，即约 250ms
 */
u8 Key_GetLongEvent(void);

/*
 * 函数名：Key_GetDoubleEvent
 * 功能：获取双击事件
 * 参数：无
 * 返回值：按键编号
 * 备注：默认双击窗口为 40 个 10ms 节拍，即约 400ms
 */
u8 Key_GetDoubleEvent(void);

/*
 * 函数名：Key_Scan10ms
 * 功能：执行 10ms 周期按键扫描
 * 参数：无
 * 返回值：无
 * 备注：带消抖和边沿检测
 */
void Key_Scan10ms(void);

/*
 * 使用建议：
 * 1. 只需要普通按下事件时，继续使用 Key_GetEvent()
 * 2. 需要区分单击/双击/长按时，优先使用：
 *    Key_GetShortEvent()
 *    Key_GetDoubleEvent()
 *    Key_GetLongEvent()
 * 3. 典型写法：
 *    Key_Scan10ms();
 *    key = Key_GetEvent();
 *    short_key = Key_GetShortEvent();
 *    double_key = Key_GetDoubleEvent();
 *    long_key = Key_GetLongEvent();
 */

#endif
