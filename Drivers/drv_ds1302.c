/* DS1302 寄存器读写底层实现 */
#include "drv_ds1302.h"

/*
 * 函数名：DS1302_WriteByteRaw
 * 功能：向 DS1302 总线发送一个原始字节
 * 参数：dat - 待发送字节
 * 返回值：无
 * 备注：供寄存器读写函数内部调用
 */
void DS1302_WriteByteRaw(u8 dat)
{
    u8 i; /* 位发送循环变量 */
    for(i = 0; i < 8; i++)
    {
        DS1302_SCK = 0;
        DS1302_IO = dat & 0x01;
        dat >>= 1;
        DS1302_SCK = 1;
    }
}

/*
 * 函数名：DS1302_WriteReg
 * 功能：向 DS1302 指定寄存器写入一个字节
 * 参数：addr - 寄存器地址
 * 参数：dat - 写入数据
 * 返回值：无
 * 备注：地址应使用写寄存器地址
 */
void DS1302_WriteReg(u8 addr, u8 dat)
{
    DS1302_RST = 0;
    _nop_();
    DS1302_SCK = 0;
    _nop_();
    DS1302_RST = 1;
    _nop_();
    DS1302_WriteByteRaw(addr);
    DS1302_WriteByteRaw(dat);
    DS1302_RST = 0;
}

/*
 * 函数名：DS1302_ReadReg
 * 功能：从 DS1302 指定寄存器读取一个字节
 * 参数：addr - 寄存器地址
 * 返回值：读取到的数据
 * 备注：地址应使用读寄存器地址
 */
u8 DS1302_ReadReg(u8 addr)
{
    u8 i;       /* 位读取循环变量 */
    u8 dat = 0; /* 最终读出的寄存器值 */

    DS1302_RST = 0;
    _nop_();
    DS1302_SCK = 0;
    _nop_();
    DS1302_RST = 1;
    _nop_();
    DS1302_WriteByteRaw(addr);

    for(i = 0; i < 8; i++)
    {
        DS1302_SCK = 0;
        dat >>= 1;
        if(DS1302_IO)
        {
            dat |= 0x80;
        }
        DS1302_SCK = 1;
    }

    DS1302_RST = 0;
    _nop_();
    DS1302_SCK = 0;
    _nop_();
    DS1302_SCK = 1;
    _nop_();
    DS1302_IO = 0;
    _nop_();
    DS1302_IO = 1;
    return dat;
}
