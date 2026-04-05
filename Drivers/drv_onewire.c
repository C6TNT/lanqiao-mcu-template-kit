#include "drv_onewire.h"

/*
 * 函数名：ONEWIRE_Delay
 * 功能：提供单总线时序所需延时
 * 参数：t - 延时计数
 * 返回值：无
 * 备注：按原训练工程的双层循环风格实现，时序更接近你本地已验证代码
 */
static void ONEWIRE_Delay(u16 t)
{
    u8 i;

    while(t--)
    {
        for(i = 0; i < 12; i++)
        {
            ;
        }
    }
}

/*
 * 函数名：ONEWIRE_Reset
 * 功能：执行一次单总线复位并检测器件应答
 * 参数：无
 * 返回值：1 表示未检测到器件，0 表示检测到器件
 */
bit ONEWIRE_Reset(void)
{
    bit presence;

    ONEWIRE_DQ = 1;
    ONEWIRE_Delay(12);
    ONEWIRE_DQ = 0;
    ONEWIRE_Delay(80);
    ONEWIRE_DQ = 1;
    ONEWIRE_Delay(10);
    presence = ONEWIRE_DQ;
    ONEWIRE_Delay(5);

    return presence;
}

/*
 * 函数名：ONEWIRE_WriteByte
 * 功能：向单总线写入 1 字节
 * 参数：dat - 待发送数据
 * 返回值：无
 */
void ONEWIRE_WriteByte(u8 dat)
{
    u8 i;

    for(i = 0; i < 8; i++)
    {
        ONEWIRE_DQ = 0;
        ONEWIRE_DQ = dat & 0x01;
        ONEWIRE_Delay(5);
        ONEWIRE_DQ = 1;
        dat >>= 1;
    }

    ONEWIRE_Delay(5);
}

/*
 * 函数名：ONEWIRE_ReadByte
 * 功能：从单总线读取 1 字节
 * 参数：无
 * 返回值：读到的数据
 */
u8 ONEWIRE_ReadByte(void)
{
    u8 i;
    u8 dat;

    dat = 0;
    for(i = 0; i < 8; i++)
    {
        ONEWIRE_DQ = 0;
        dat >>= 1;
        ONEWIRE_DQ = 1;
        if(ONEWIRE_DQ)
        {
            dat |= 0x80;
        }
        ONEWIRE_Delay(5);
    }

    return dat;
}
