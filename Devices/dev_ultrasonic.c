#include "dev_ultrasonic.h"

/*
 * 函数名：Delay12us
 * 功能：产生约 12us 的短延时
 * 参数：无
 * 返回值：无
 */
static void Delay12us(void)
{
    u8 i;

    _nop_();
    _nop_();
    i = 33;
    while(--i);
}

/*
 * 函数名：Ultrasonic_Init
 * 功能：初始化超声波测距使用的 PCA
 * 参数：无
 * 返回值：无
 * 备注：PCA 采用 Fosc/12 计数，1 个计数约为 1us
 */
void Ultrasonic_Init(void)
{
    CMOD = 0x00;
    CCON = 0x00;
    CL = 0x00;
    CH = 0x00;
    CF = 0;
    CR = 0;
    ULT_TX = 0;
}

/*
 * 函数名：Ultrasonic_ReadCm
 * 功能：读取一次距离值
 * 参数：无
 * 返回值：距离值，单位厘米，超时返回 999
 * 备注：驱动层只做单次测量，稳定处理放到应用层跨周期平滑
 */
u16 Ultrasonic_ReadCm(void)
{
    u8 i;
    u16 time;

    for(i = 0; i < 9; i++)
    {
        ULT_TX = 1;
        Delay12us();
        ULT_TX = 0;
        Delay12us();
    }

    CH = 0x00;
    CL = 0x00;
    CF = 0;
    CR = 1;
    while((ULT_RX == 1) && (CF == 0))
    {
    }
    CR = 0;

    if(CF == 1)
    {
        CF = 0;
        CH = 0x00;
        CL = 0x00;
        return 999;
    }

    time = ((u16)CH << 8) | CL;
    CH = 0x00;
    CL = 0x00;

    return (u16)((time + 29u) / 58u);
}
