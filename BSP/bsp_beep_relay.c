/* 蜂鸣器和继电器共用同一组锁存器 */
#include "bsp_beep_relay.h"

/*
 * 函数名：Beep_Set
 * 功能：控制蜂鸣器开关
 * 参数：on - 1 表示打开，0 表示关闭
 * 返回值：无
 * 备注：适合做报警、提示音
 */
void Beep_Set(bit on)
{
    Board_SelectHC138(HC138_BEEP);
    P06 = on ? 1 : 0;
    P2 &= 0x1F;
}

/*
 * 函数名：Relay_Set
 * 功能：控制继电器开关
 * 参数：on - 1 表示打开，0 表示关闭
 * 返回值：无
 * 备注：部分赛题把继电器当作执行器使用
 */
void Relay_Set(bit on)
{
    Board_SelectHC138(HC138_BEEP);
    P04 = on ? 1 : 0;
    P2 &= 0x1F;
}

/*
 * 函数名：BeepRelay_AllOff
 * 功能：同时关闭蜂鸣器和继电器
 * 参数：无
 * 返回值：无
 * 备注：防止上电误动作
 */
void BeepRelay_AllOff(void)
{
    Board_SelectHC138(HC138_BEEP);
    P06 = 0;
    P04 = 0;
    P2 &= 0x1F;
}
