/* I2C 底层时序驱动 */
#ifndef __DRV_IIC_H__
#define __DRV_IIC_H__

#include "..\Core\common.h"

sbit IIC_SDA = P2^1;
sbit IIC_SCL = P2^0;

/*
 * 函数名：IIC_Start
 * 功能：发送 I2C 起始信号
 * 参数：无
 * 返回值：无
 * 备注：底层时序接口
 */
void IIC_Start(void);

/*
 * 函数名：IIC_Stop
 * 功能：发送 I2C 停止信号
 * 参数：无
 * 返回值：无
 * 备注：底层时序接口
 */
void IIC_Stop(void);

/*
 * 函数名：IIC_WaitAck
 * 功能：等待应答
 * 参数：无
 * 返回值：应答状态
 * 备注：0 表示收到应答
 */
bit IIC_WaitAck(void);

/*
 * 函数名：IIC_SendAck
 * 功能：发送应答位
 * 参数：ack - 应答状态位
 * 返回值：无
 * 备注：读数据后常用
 */
void IIC_SendAck(bit ack);

/*
 * 函数名：IIC_SendByte
 * 功能：发送一个字节
 * 参数：dat - 待发送数据
 * 返回值：无
 * 备注：I2C 字节发送接口
 */
void IIC_SendByte(u8 dat);

/*
 * 函数名：IIC_ReadByte
 * 功能：读取一个字节
 * 参数：无
 * 返回值：接收到的数据
 * 备注：I2C 字节读取接口
 */
u8 IIC_ReadByte(void);

#endif
