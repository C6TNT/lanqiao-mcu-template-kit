/* 软件模拟 I2C：供 EEPROM、PCF8591 等设备调用 */
#include "drv_iic.h"

#define IIC_DELAY 5

/*
 * 函数名：IIC_Delay
 * 功能：提供 I2C 时序所需的短延时
 * 参数：t - 延时计数值
 * 返回值：无
 * 备注：这是文件内部使用的静态函数
 */
static void IIC_Delay(u8 t)
{
    do
    {
        _nop_();
    } while(t--);
}

/*
 * 函数名：IIC_Start
 * 功能：发送 I2C 起始信号
 * 参数：无
 * 返回值：无
 * 备注：在 SCL 高电平期间让 SDA 从高变低
 */
void IIC_Start(void)
{
    IIC_SDA = 1;
    IIC_SCL = 1;
    IIC_Delay(IIC_DELAY);
    IIC_SDA = 0;
    IIC_Delay(IIC_DELAY);
    IIC_SCL = 0;
}

/*
 * 函数名：IIC_Stop
 * 功能：发送 I2C 停止信号
 * 参数：无
 * 返回值：无
 * 备注：在 SCL 高电平期间让 SDA 从低变高
 */
void IIC_Stop(void)
{
    IIC_SDA = 0;
    IIC_SCL = 1;
    IIC_Delay(IIC_DELAY);
    IIC_SDA = 1;
    IIC_Delay(IIC_DELAY);
}

/*
 * 函数名：IIC_WaitAck
 * 功能：等待从机返回应答位
 * 参数：无
 * 返回值：0 表示收到应答，1 表示未应答
 * 备注：调试 I2C 外设时这个函数非常关键
 */
bit IIC_WaitAck(void)
{
    bit ack; /* 保存从机返回的应答位 */
    IIC_SCL = 1;
    IIC_Delay(IIC_DELAY);
    ack = IIC_SDA;
    IIC_SCL = 0;
    IIC_Delay(IIC_DELAY);
    return ack;
}

/*
 * 函数名：IIC_SendAck
 * 功能：发送应答或非应答位
 * 参数：ack - 0 表示应答，1 表示非应答
 * 返回值：无
 * 备注：主机读数据结束时常用来通知从机
 */
void IIC_SendAck(bit ack)
{
    IIC_SCL = 0;
    IIC_SDA = ack;
    IIC_Delay(IIC_DELAY);
    IIC_SCL = 1;
    IIC_Delay(IIC_DELAY);
    IIC_SCL = 0;
    IIC_SDA = 1;
}

/*
 * 函数名：IIC_SendByte
 * 功能：通过 I2C 发送一个字节
 * 参数：dat - 要发送的数据
 * 返回值：无
 * 备注：按高位在前的方式依次发送
 */
void IIC_SendByte(u8 dat)
{
    u8 i; /* 位发送循环变量 */
    for(i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        IIC_SDA = (dat & 0x80) ? 1 : 0;
        IIC_Delay(IIC_DELAY);
        IIC_SCL = 1;
        IIC_Delay(IIC_DELAY);
        dat <<= 1;
    }
    IIC_SCL = 0;
}

/*
 * 函数名：IIC_ReadByte
 * 功能：通过 I2C 读取一个字节
 * 参数：无
 * 返回值：接收到的字节数据
 * 备注：读取结束后通常还要调用 IIC_SendAck
 */
u8 IIC_ReadByte(void)
{
    u8 i;       /* 位读取循环变量 */
    u8 dat = 0; /* 拼接后的接收字节 */
    for(i = 0; i < 8; i++)
    {
        IIC_SCL = 1;
        IIC_Delay(IIC_DELAY);
        dat <<= 1;
        if(IIC_SDA)
        {
            dat |= 0x01;
        }
        IIC_SCL = 0;
        IIC_Delay(IIC_DELAY);
    }
    return dat;
}
