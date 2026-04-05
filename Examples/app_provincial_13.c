/* 第十三届省赛模板版示例：温度、时间、参数、继电器双模式控制 */
#include "..\App\app.h"

typedef enum
{
    PAGE_U1 = 0,
    PAGE_U2,
    PAGE_U3
} app_page_t;

typedef enum
{
    MODE_TEMP = 0,
    MODE_TIME
} app_mode_t;

static app_page_t g_page = PAGE_U1;
static app_mode_t g_mode = MODE_TEMP;

static rtc_time_t g_time = {17, 59, 50};
static float g_temp = 0.0f;
static u8 g_temp_limit = 23;
static u8 g_edit_temp_limit = 23;

static u8 g_relay_on = 0;
static u8 g_hour_pulse_ms = 0;
static u16 g_relay_hold_ms = 0;
static u8 g_flash_100ms = 0;
static u8 g_last_hour_trigger_sec = 0xFF;

static u16 App_GetTemp10(float temp)
{
    int temp10;

    temp10 = (int)(temp * 10.0f);
    if(temp10 < 0)
    {
        temp10 = 0;
    }
    if(temp10 > 999)
    {
        temp10 = 999;
    }
    return (u16)temp10;
}

static u8 App_IsS17PressedNow(void)
{
    P35 = 1;
    P34 = 0;
    P3 |= 0x0F;

    if(P32 == 0)
    {
        P35 = 1;
        P34 = 1;
        P3 |= 0x0F;
        return 1;
    }

    P35 = 1;
    P34 = 1;
    P3 |= 0x0F;
    return 0;
}

static void App_SetRelay(u8 on)
{
    g_relay_on = on;
    Relay_Set(on);
}

static void App_UpdateRelayByTemp(void)
{
    if(g_temp > g_temp_limit)
    {
        App_SetRelay(1);
    }
    else
    {
        App_SetRelay(0);
    }
}

static void App_UpdateRelayByTime(void)
{
    if((g_time.min == 0u) && (g_time.sec == 0u) && (g_last_hour_trigger_sec != 0u))
    {
        App_SetRelay(1);
        g_relay_hold_ms = 5000u;
        g_hour_pulse_ms = 5000u;
    }

    g_last_hour_trigger_sec = g_time.sec;
}

static void App_UpdateLed(void)
{
    LED_AllOff();

    if(g_hour_pulse_ms > 0u)
    {
        LED_Set(0, 1);
    }

    if(g_mode == MODE_TEMP)
    {
        LED_Set(1, 1);
    }

    if(g_relay_on)
    {
        LED_Set(2, g_flash_100ms);
    }
}

static void App_ShowTempPage(void)
{
    u16 temp10;

    temp10 = App_GetTemp10(g_temp);

    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_U);
    SEG_SetDigit(1, 1);
    SEG_SetDigit(5, (u8)(temp10 / 100));
    SEG_SetDigitDp(6, (u8)((temp10 / 10) % 10));
    SEG_SetDigit(7, temp10 % 10);
}

static void App_ShowTimePage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_U);
    SEG_SetDigit(1, 2);

    if(App_IsS17PressedNow())
    {
        SEG_SetDigit(3, g_time.min / 10);
        SEG_SetDigit(4, g_time.min % 10);
        SEG_SetCode(5, SEG_DASH);
        SEG_SetDigit(6, g_time.sec / 10);
        SEG_SetDigit(7, g_time.sec % 10);
    }
    else
    {
        SEG_SetDigit(3, g_time.hour / 10);
        SEG_SetDigit(4, g_time.hour % 10);
        SEG_SetCode(5, SEG_DASH);
        SEG_SetDigit(6, g_time.min / 10);
        SEG_SetDigit(7, g_time.min % 10);
    }
}

static void App_ShowParamPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_U);
    SEG_SetDigit(1, 3);
    SEG_SetDigit(6, g_edit_temp_limit / 10);
    SEG_SetDigit(7, g_edit_temp_limit % 10);
}

static void App_UpdateDisplay(void)
{
    switch(g_page)
    {
    case PAGE_U1:
        App_ShowTempPage();
        break;
    case PAGE_U2:
        App_ShowTimePage();
        break;
    default:
        App_ShowParamPage();
        break;
    }
}

static void App_HandleKey(u8 key)
{
    switch(key)
    {
    case 3:
        if(g_page == PAGE_U1)
        {
            g_page = PAGE_U2;
        }
        else if(g_page == PAGE_U2)
        {
            g_page = PAGE_U3;
            g_edit_temp_limit = g_temp_limit;
        }
        else
        {
            g_page = PAGE_U1;
            g_temp_limit = g_edit_temp_limit;
        }
        break;

    case 7:
        if(g_mode == MODE_TEMP)
        {
            g_mode = MODE_TIME;
        }
        else
        {
            g_mode = MODE_TEMP;
        }
        g_relay_hold_ms = 0;
        App_SetRelay(0);
        break;

    case 4:
        if(g_page == PAGE_U3)
        {
            if(g_edit_temp_limit < 99u)
            {
                g_edit_temp_limit++;
            }
        }
        break;

    case 8:
        if(g_page == PAGE_U3)
        {
            if(g_edit_temp_limit > 10u)
            {
                g_edit_temp_limit--;
            }
        }
        break;

    default:
        break;
    }
}

void App_Init(void)
{
    Board_Init();
    LED_AllOff();
    BeepRelay_AllOff();
    SEG_ClearBuffer();
    Tick_Init();
    RTC_SetTime(&g_time);
    g_temp = DS18B20_ReadTemp();
    App_UpdateRelayByTemp();
    App_UpdateLed();
}

void App_Loop(void)
{
    if(g_flag_1ms)
    {
        g_flag_1ms = 0;

        if(g_relay_hold_ms > 0u)
        {
            g_relay_hold_ms--;
            if(g_relay_hold_ms == 0u)
            {
                App_SetRelay(0);
            }
        }

        if(g_hour_pulse_ms > 0u)
        {
            g_hour_pulse_ms--;
        }
    }

    if(g_flag_10ms)
    {
        g_flag_10ms = 0;
        Key_Scan10ms();
        App_HandleKey(Key_GetEvent());
    }

    if(g_flag_100ms)
    {
        g_flag_100ms = 0;
        g_flash_100ms = !g_flash_100ms;
        App_UpdateLed();
    }

    if(g_flag_500ms)
    {
        g_flag_500ms = 0;
        g_temp = DS18B20_ReadTemp();
        if(g_mode == MODE_TEMP)
        {
            App_UpdateRelayByTemp();
        }
        App_UpdateLed();
    }

    if(g_flag_1s)
    {
        g_flag_1s = 0;
        RTC_ReadTime(&g_time);
        if(g_mode == MODE_TIME)
        {
            App_UpdateRelayByTime();
        }
        App_UpdateLed();
    }

    App_UpdateDisplay();
}
