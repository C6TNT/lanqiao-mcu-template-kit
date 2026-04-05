/* 温度采集设备层接口 */
#ifndef __DEV_DS18B20_H__
#define __DEV_DS18B20_H__

#include "..\Drivers\drv_onewire.h"

/*
 * 函数名：DS18B20_ReadTemp
 * 功能：读取温度值
 * 参数：无
 * 返回值：温度值
 * 备注：单位为摄氏度
 */
float DS18B20_ReadTemp(void);

#endif
