/* 时钟设备层接口 */
#ifndef __DEV_DS1302_H__
#define __DEV_DS1302_H__

#include "..\Drivers\drv_ds1302.h"

typedef struct
{
    u8 hour; /* 小时，范围通常为 0~23 */
    u8 min;  /* 分钟，范围通常为 0~59 */
    u8 sec;  /* 秒，范围通常为 0~59 */
} rtc_time_t;

/*
 * 函数名：RTC_SetTime
 * 功能：设置时钟
 * 参数：t - 时间结构体指针
 * 返回值：无
 * 备注：写入时分秒
 */
void RTC_SetTime(rtc_time_t *t);

/*
 * 函数名：RTC_ReadTime
 * 功能：读取时钟
 * 参数：t - 时间结构体指针
 * 返回值：无
 * 备注：读回十进制时分秒
 */
void RTC_ReadTime(rtc_time_t *t);

#endif
