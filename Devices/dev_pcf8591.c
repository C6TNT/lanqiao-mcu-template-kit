/* PCF8591 设备封装 */
#include "dev_pcf8591.h"

/*
 * 函数名：PCF8591_ReadADC
 * 功能：读取指定通道的 ADC 值
 * 参数：channel - 通道控制字
 * 返回值：8 位 ADC 结果
 * 备注：
 * 0x01 通常用于光敏电阻
 * 0x03 通常用于滑动电位器
 * 0x05 通常用于外部 ADC 输入口
 */
u8 PCF8591_ReadADC(u8 channel)
{
    u8 dat;

    IIC_Start();
    IIC_SendByte(0x90);
    IIC_WaitAck();
    IIC_SendByte(channel);
    IIC_WaitAck();
    IIC_Stop();

    IIC_Start();
    IIC_SendByte(0x91);
    IIC_WaitAck();
    dat = IIC_ReadByte();
    IIC_SendAck(1);
    IIC_Stop();

    return dat;
}

/*
 * 函数名：PCF8591_WriteDAC
 * 功能：向 DAC 输出一个 8 位值
 * 参数：dat - DAC 输出数据
 * 返回值：无
 * 备注：适合电压控制、亮度控制等题型
 */
void PCF8591_WriteDAC(u8 dat)
{
    IIC_Start();
    IIC_SendByte(0x90);
    IIC_WaitAck();
    IIC_SendByte(0x40);
    IIC_WaitAck();
    IIC_SendByte(dat);
    IIC_WaitAck();
    IIC_Stop();
}
