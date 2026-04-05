/* 频率测量封装：给频率类赛题或综合模板使用 */
#include "dev_freq_counter.h"
#include "..\BSP\bsp_tick.h"

static bit g_freq_running = 0;
static u32 g_freq_start_tick = 0;
static u16 g_freq_hz = 0;

void FREQ_InitCounter0(void)
{
    AUXR &= 0x7F;
    TMOD &= 0xF0;
    TMOD |= 0x05;
    TH0 = 0x00;
    TL0 = 0x00;
    TF0 = 0;
    TR0 = 0;

    g_freq_running = 0;
    g_freq_start_tick = 0;
    g_freq_hz = 0;
}

void FREQ_PollTask(void)
{
    u16 count;

    if(!g_freq_running)
    {
        TH0 = 0x00;
        TL0 = 0x00;
        TF0 = 0;
        TR0 = 1;
        g_freq_start_tick = g_ms_ticks;
        g_freq_running = 1;
        return;
    }

    if((g_ms_ticks - g_freq_start_tick) < 250u)
    {
        return;
    }

    TR0 = 0;
    count = ((u16)TH0 << 8) | TL0;
    g_freq_hz = (u16)(count * 4u);
    g_freq_running = 0;
}

u16 FREQ_GetHz(void)
{
    return g_freq_hz;
}
