/* DS1302 设备封装：业务层直接按时分秒读写 */
#include "dev_ds1302.h"

#define DS1302_WP_REG   0x8E
#define DS1302_SEC_WREG 0x80
#define DS1302_MIN_WREG 0x82
#define DS1302_HOU_WREG 0x84
#define DS1302_SEC_RREG 0x81
#define DS1302_MIN_RREG 0x83
#define DS1302_HOU_RREG 0x85

/*
 * 函数名：BcdToDec
 * 功能：把 BCD 码转换成十进制
 * 参数：bcd - BCD 格式数据
 * 返回值：十进制结果
 * 备注：内部静态工具函数
 */
static u8 BcdToDec(u8 bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

/*
 * 函数名：DecToBcd
 * 功能：把十进制转换成 BCD 码
 * 参数：dec - 十进制数据
 * 返回值：BCD 结果
 * 备注：内部静态工具函数
 */
static u8 DecToBcd(u8 dec)
{
    return (dec / 10) * 16 + (dec % 10);
}

/*
 * 函数名：RTC_SetTime
 * 功能：写入时钟芯片的时分秒
 * 参数：t - 时间结构体指针
 * 返回值：无
 * 备注：会自动处理写保护
 */
void RTC_SetTime(rtc_time_t *t)
{
    DS1302_WriteReg(DS1302_WP_REG, 0x00);
    DS1302_WriteReg(DS1302_HOU_WREG, DecToBcd(t->hour));
    DS1302_WriteReg(DS1302_MIN_WREG, DecToBcd(t->min));
    DS1302_WriteReg(DS1302_SEC_WREG, DecToBcd(t->sec));
    DS1302_WriteReg(DS1302_WP_REG, 0x80);
}

/*
 * 函数名：RTC_ReadTime
 * 功能：读取时钟芯片的时分秒
 * 参数：t - 时间结构体指针
 * 返回值：无
 * 备注：读出的结果已经是十进制
 */
void RTC_ReadTime(rtc_time_t *t)
{
    t->hour = BcdToDec(DS1302_ReadReg(DS1302_HOU_RREG));
    t->min = BcdToDec(DS1302_ReadReg(DS1302_MIN_RREG));
    t->sec = BcdToDec(DS1302_ReadReg(DS1302_SEC_RREG));
}
