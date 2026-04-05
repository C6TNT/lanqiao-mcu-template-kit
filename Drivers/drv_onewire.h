/* 单总线底层驱动 */
#ifndef __DRV_ONEWIRE_H__
#define __DRV_ONEWIRE_H__

#include "..\Core\common.h"

sbit ONEWIRE_DQ = P1^4;

/*
 * 函数名：ONEWIRE_Reset
 * 功能：执行一次单总线复位
 * 参数：无
 * 返回值：设备响应状态
 * 备注：0 通常表示检测到设备
 */
bit ONEWIRE_Reset(void);

/*
 * 函数名：ONEWIRE_WriteByte
 * 功能：发送一个字节到单总线
 * 参数：dat - 待发送数据
 * 返回值：无
 * 备注：DS18B20 指令和数据都用它发送
 */
void ONEWIRE_WriteByte(u8 dat);

/*
 * 函数名：ONEWIRE_ReadByte
 * 功能：从单总线读取一个字节
 * 参数：无
 * 返回值：读取到的数据
 * 备注：常用于读取温度寄存器
 */
u8 ONEWIRE_ReadByte(void);

#endif
