/* 按键模块：兼容原有按下事件，并补充单击、双击、长按事件 */
#include "bsp_key.h"
#include "bsp_uart.h"

static u8 g_key_event = KEY_NONE;
static u8 g_key_short_event = KEY_NONE;
static u8 g_key_long_event = KEY_NONE;
static u8 g_key_double_event = KEY_NONE;
static u8 g_pending_short_key = KEY_NONE;
static u8 g_pending_short_ticks = 0;

u8 Key_ReadIndependent(void)
{
    u8 key_temp;

    UART_BeginKeyScan();
    P44 = 1;
    P42 = 1;
    P35 = 1;
    P34 = 1;
    P3 |= 0x0F;
    key_temp = P3 & 0x0F;
    UART_EndKeyScan();

    switch(key_temp)
    {
    case 0x0E:
        return 1;
    case 0x0D:
        return 2;
    case 0x0B:
        return 3;
    case 0x07:
        return 4;
    default:
        break;
    }

    return KEY_NONE;
}

u8 Key_ReadMatrix(void)
{
    u8 key = KEY_NONE;

    P44 = 0; P42 = 1; P35 = 1; P34 = 1;
    P3 |= 0x0F;
    if(P33 == 0) key = 5;
    if(P32 == 0) key = 9;
    if(P31 == 0) key = 13;
    if(P30 == 0) key = 17;

    P44 = 1; P42 = 0; P35 = 1; P34 = 1;
    P3 |= 0x0F;
    if(P33 == 0) key = 6;
    if(P32 == 0) key = 10;
    if(P31 == 0) key = 14;
    if(P30 == 0) key = 18;

    P44 = 1; P42 = 1; P35 = 0; P34 = 1;
    P3 |= 0x0F;
    if(P33 == 0) key = 7;
    if(P32 == 0) key = 11;
    if(P31 == 0) key = 15;
    if(P30 == 0) key = 19;

    P44 = 1; P42 = 1; P35 = 1; P34 = 0;
    P3 |= 0x0F;
    if(P33 == 0) key = 8;
    if(P32 == 0) key = 12;
    if(P31 == 0) key = 16;
    if(P30 == 0) key = 20;

    P44 = 1;
    P42 = 1;
    P35 = 1;
    P34 = 1;
    P3 |= 0x0F;

    return key;
}

static u8 Key_ReadNow(void)
{
    u8 now;

    now = Key_ReadIndependent();
    if(now == KEY_NONE)
    {
        now = Key_ReadMatrix();
    }

    return now;
}

static void Key_OnPressed(u8 key)
{
    if((g_pending_short_key != KEY_NONE) && (g_pending_short_key != key) && (g_pending_short_ticks > 0))
    {
        g_key_short_event = g_pending_short_key;
        g_pending_short_key = KEY_NONE;
        g_pending_short_ticks = 0;
    }

    g_key_event = key;
}

static void Key_OnReleased(u8 key, u8 long_sent)
{
    if(long_sent)
    {
        g_pending_short_key = KEY_NONE;
        g_pending_short_ticks = 0;
        return;
    }

    if((g_pending_short_key == key) && (g_pending_short_ticks > 0) && (g_pending_short_ticks <= KEY_DOUBLE_TICKS))
    {
        g_key_double_event = key;
        g_pending_short_key = KEY_NONE;
        g_pending_short_ticks = 0;
    }
    else
    {
        g_pending_short_key = key;
        g_pending_short_ticks = 1;
    }
}

static void Key_UpdatePendingShort(void)
{
    if((g_pending_short_key == KEY_NONE) || (g_pending_short_ticks == 0))
    {
        return;
    }

    g_pending_short_ticks++;
    if(g_pending_short_ticks > KEY_DOUBLE_TICKS)
    {
        g_key_short_event = g_pending_short_key;
        g_pending_short_key = KEY_NONE;
        g_pending_short_ticks = 0;
    }
}

void Key_Scan10ms(void)
{
    static u8 stable_key = KEY_NONE;
    static u8 last_raw = KEY_NONE;
    static u8 debounce_count = 0;
    static u8 press_ticks = 0;
    static u8 long_sent = 0;
    u8 raw;

    raw = Key_ReadNow();

    if(raw == last_raw)
    {
        if(debounce_count < 2)
        {
            debounce_count++;
        }
    }
    else
    {
        debounce_count = 0;
        last_raw = raw;
    }

    if((debounce_count >= 1) && (stable_key != raw))
    {
        if((stable_key != KEY_NONE) && (raw == KEY_NONE))
        {
            Key_OnReleased(stable_key, long_sent);
            press_ticks = 0;
            long_sent = 0;
        }
        else if((stable_key == KEY_NONE) && (raw != KEY_NONE))
        {
            Key_OnPressed(raw);
            press_ticks = 0;
            long_sent = 0;
        }
        else if((stable_key != KEY_NONE) && (raw != KEY_NONE))
        {
            Key_OnReleased(stable_key, long_sent);
            Key_OnPressed(raw);
            press_ticks = 0;
            long_sent = 0;
        }

        stable_key = raw;
    }

    if(stable_key != KEY_NONE)
    {
        if(press_ticks < 255)
        {
            press_ticks++;
        }

        if((!long_sent) && (press_ticks >= KEY_LONG_TICKS))
        {
            g_key_long_event = stable_key;
            long_sent = 1;
        }
    }
    else
    {
        Key_UpdatePendingShort();
    }
}

u8 Key_GetEvent(void)
{
    u8 key;

    key = g_key_event;
    g_key_event = KEY_NONE;
    return key;
}

u8 Key_GetShortEvent(void)
{
    u8 key;

    key = g_key_short_event;
    g_key_short_event = KEY_NONE;
    return key;
}

u8 Key_GetLongEvent(void)
{
    u8 key;

    key = g_key_long_event;
    g_key_long_event = KEY_NONE;
    return key;
}

u8 Key_GetDoubleEvent(void)
{
    u8 key;

    key = g_key_double_event;
    g_key_double_event = KEY_NONE;
    return key;
}
