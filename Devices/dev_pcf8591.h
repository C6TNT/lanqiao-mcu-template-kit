/* ADC / DAC 设备层接口 */
#ifndef __DEV_PCF8591_H__
#define __DEV_PCF8591_H__

#include "..\Drivers\drv_iic.h"

/*
 * PCF8591 常用 ADC 通道参数说明：
 * 0x01 -> AIN0，开发板上常接光敏电阻
 * 0x03 -> AIN1，开发板上常接滑动电位器
 * 0x05 -> AIN2，开发板上常接外部 ADC 输入接口
 * 0x07 -> AIN3，开发板上常作为备用模拟输入
 *
 * 常见用法：
 * 采样光敏电阻：PCF8591_ReadADC(0x01)
 * 采样电位器：  PCF8591_ReadADC(0x03)
 * 采样真实 ADC 外部引脚：PCF8591_ReadADC(0x05)
 */

/*
 * 函数名：PCF8591_ReadADC
 * 功能：读取 ADC 值
 * 参数：channel - 通道控制字
 * 返回值：ADC 结果
 * 备注：比赛中最常用的是 0x01、0x03、0x05
 */
u8 PCF8591_ReadADC(u8 channel);

/*
 * 函数名：PCF8591_WriteDAC
 * 功能：写入 DAC 输出值
 * 参数：dat - 输出值
 * 返回值：无
 * 备注：用于模拟量输出
 */
void PCF8591_WriteDAC(u8 dat);

#endif
