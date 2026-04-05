/* AT24C02 设备封装：业务层直接按地址读写 */
#include "dev_at24c02.h"

/*
 * 函数名：AT24C02_Delay
 * 功能：等待 EEPROM 内部写周期完成
 * 参数：无
 * 返回值：无
 * 备注：这是内部静态函数，写操作后必须调用
 */
static void AT24C02_Delay(void)
{
    u16 i = 5000; /* 粗略延时计数，用来等待 EEPROM 内部写周期 */
    while(i--);
}

/*
 * 函数名：AT24C02_WriteByte
 * 功能：向 EEPROM 指定地址写入一个字节
 * 参数：addr - 存储地址
 * 参数：dat - 要写入的数据
 * 返回值：无
 * 备注：适合保存阈值、模式等参数
 */
void AT24C02_WriteByte(u8 addr, u8 dat)
{
    IIC_Start();
    IIC_SendByte(0xA0);
    IIC_WaitAck();
    IIC_SendByte(addr);
    IIC_WaitAck();
    IIC_SendByte(dat);
    IIC_WaitAck();
    IIC_Stop();
    AT24C02_Delay();
}

/*
 * 函数名：AT24C02_ReadByte
 * 功能：从 EEPROM 指定地址读取一个字节
 * 参数：addr - 存储地址
 * 返回值：读取到的数据
 * 备注：适合读取保存的比赛参数
 */
u8 AT24C02_ReadByte(u8 addr)
{
    u8 dat; /* 保存最终读取到的数据 */

    IIC_Start();
    IIC_SendByte(0xA0);
    IIC_WaitAck();
    IIC_SendByte(addr);
    IIC_WaitAck();

    IIC_Start();
    IIC_SendByte(0xA1);
    IIC_WaitAck();
    dat = IIC_ReadByte();
    IIC_SendAck(1);
    IIC_Stop();

    return dat;
}
