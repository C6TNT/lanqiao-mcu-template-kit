/* DS1302 底层三线时序驱动 */
#ifndef __DRV_DS1302_H__
#define __DRV_DS1302_H__

#include "..\Core\common.h"

sbit DS1302_SCK = P1^7;
sbit DS1302_IO  = P2^3;
sbit DS1302_RST = P1^3;

/*
 * 函数名：DS1302_WriteByteRaw
 * 功能：发送一个原始字节到 DS1302
 * 参数：dat - 待发送数据
 * 返回值：无
 * 备注：底层时序接口
 */
void DS1302_WriteByteRaw(u8 dat);

/*
 * 函数名：DS1302_WriteReg
 * 功能：向寄存器写一个字节
 * 参数：addr - 寄存器地址
 * 参数：dat - 写入数据
 * 返回值：无
 * 备注：设备层基于这个接口继续封装
 */
void DS1302_WriteReg(u8 addr, u8 dat);

/*
 * 函数名：DS1302_ReadReg
 * 功能：从寄存器读一个字节
 * 参数：addr - 寄存器地址
 * 返回值：读取到的数据
 * 备注：设备层基于这个接口继续封装
 */
u8 DS1302_ReadReg(u8 addr);

#endif
