/* EEPROM 设备层接口 */
#ifndef __DEV_AT24C02_H__
#define __DEV_AT24C02_H__

#include "..\Drivers\drv_iic.h"

/*
 * 函数名：AT24C02_WriteByte
 * 功能：写一个字节到 EEPROM
 * 参数：addr - 目标地址
 * 参数：dat - 写入数据
 * 返回值：无
 * 备注：适合保存参数
 */
void AT24C02_WriteByte(u8 addr, u8 dat);

/*
 * 函数名：AT24C02_ReadByte
 * 功能：从 EEPROM 读一个字节
 * 参数：addr - 目标地址
 * 返回值：读取到的数据
 * 备注：适合载入参数
 */
u8 AT24C02_ReadByte(u8 addr);

#endif
