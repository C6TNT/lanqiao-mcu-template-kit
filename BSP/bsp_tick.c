/* 用 Timer1 产生 1ms 节拍，同时顺手扫描数码管 */
#include "bsp_tick.h"
#include "bsp_seg.h"

volatile bit g_flag_1ms = 0;
volatile bit g_flag_10ms = 0;
volatile bit g_flag_100ms = 0;
volatile bit g_flag_500ms = 0;
volatile bit g_flag_1s = 0;
volatile u32 g_ms_ticks = 0;

/*
 * 函数名：Tick_Init
 * 功能：初始化 1ms 系统节拍定时器
 * 参数：无
 * 返回值：无
 * 备注：当前使用 Timer1 1T 模式，同时服务数码管扫描
 */
void Tick_Init(void)
{
    AUXR |= 0x40;
    TMOD &= 0x0F;
    TMOD |= 0x10;
    TL1 = 0x20;
    TH1 = 0xD1;
    TF1 = 0;
    ET1 = 1;
    TR1 = 1;
    EA = 1;
}

/*
 * 函数名：Timer1_ISR
 * 功能：1ms 定时中断服务函数
 * 参数：无
 * 返回值：无
 * 备注：负责扫描数码管，并置位 10ms/100ms/500ms/1s 标志
 */
void Timer1_ISR(void) interrupt 3
{
    static u16 cnt10 = 0;
    static u16 cnt100 = 0;
    static u16 cnt500 = 0;
    static u16 cnt1000 = 0;

    TL1 = 0x20;
    TH1 = 0xD1;

    g_ms_ticks++;
    g_flag_1ms = 1;

    SEG_Scan();

    if(++cnt10 >= 10)
    {
        cnt10 = 0;
        g_flag_10ms = 1;
    }
    if(++cnt100 >= 100)
    {
        cnt100 = 0;
        g_flag_100ms = 1;
    }
    if(++cnt500 >= 500)
    {
        cnt500 = 0;
        g_flag_500ms = 1;
    }
    if(++cnt1000 >= 1000)
    {
        cnt1000 = 0;
        g_flag_1s = 1;
    }
}
