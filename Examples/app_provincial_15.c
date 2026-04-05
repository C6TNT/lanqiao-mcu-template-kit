/* 第十五届省赛模板版示例：NE555 频率、P1/P2 参数、RTC、HF/HA 回显、DAC 联动 */
#include "..\App\app.h"

typedef enum
{
    PAGE_FREQ = 0,
    PAGE_PARAM,
    PAGE_TIME,
    PAGE_ECHO
} app_page_t;

typedef enum
{
    PARAM_LIMIT = 0,
    PARAM_CAL
} app_param_page_t;

typedef enum
{
    ECHO_FREQ = 0,
    ECHO_TIME
} app_echo_page_t;

typedef struct
{
    u16 freq_raw;
    u16 freq_limit;
    int freq_corr;
    int freq_value;
    int freq_max;
    rtc_time_t rtc;
    rtc_time_t max_time;
    u8 dac_out;
} app_data_t;

static app_page_t g_page = PAGE_FREQ;
static app_param_page_t g_param_page = PARAM_LIMIT;
static app_echo_page_t g_echo_page = ECHO_FREQ;
static app_data_t g_data = {0};
static u8 g_led_flash_200ms = 0;
static u8 g_led_div_100ms = 0;

static int App_GetFreqValue(void)
{
    return (int)g_data.freq_raw + g_data.freq_corr;
}

static void App_UpdateFreqValue(void)
{
    g_data.freq_value = App_GetFreqValue();
}

static void App_UpdateMaxRecord(void)
{
    if(g_data.freq_value >= 0)
    {
        if(g_data.freq_value > g_data.freq_max)
        {
            g_data.freq_max = g_data.freq_value;
            g_data.max_time = g_data.rtc;
        }
    }
}

static void App_UpdateDac(void)
{
    float v;
    int freq;

    freq = g_data.freq_value;

    if(freq < 0)
    {
        v = 0.0f;
    }
    else if(freq <= 500)
    {
        v = 1.0f;
    }
    else if(freq >= (int)g_data.freq_limit)
    {
        v = 5.0f;
    }
    else
    {
        v = 1.0f + (4.0f * (float)(freq - 500)) / (float)((int)g_data.freq_limit - 500);
    }

    if(v < 0.0f)
    {
        v = 0.0f;
    }
    if(v > 5.0f)
    {
        v = 5.0f;
    }

    g_data.dac_out = (u8)(v * 51.0f);
    PCF8591_WriteDAC(g_data.dac_out);
}

static void App_UpdateLed(void)
{
    LED_AllOff();

    if(g_page == PAGE_FREQ)
    {
        LED_Set(0, g_led_flash_200ms);
    }

    if(g_data.freq_value < 0)
    {
        LED_Set(1, 1);
    }
    else if(g_data.freq_value > (int)g_data.freq_limit)
    {
        LED_Set(1, g_led_flash_200ms);
    }
}

static void App_ShowFreqPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_F);

    if(g_data.freq_value < 0)
    {
        SEG_SetCode(6, SEG_L);
        SEG_SetCode(7, SEG_L);
    }
    else
    {
        SEG_SetNumber((u32)g_data.freq_value, 3, 5, 1);
    }
}

static void App_ShowParamLimitPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);
    SEG_SetDigit(1, 1);
    SEG_SetNumber((u32)g_data.freq_limit, 4, 4, 1);
}

static void App_ShowParamCalPage(void)
{
    u16 value;

    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);
    SEG_SetDigit(1, 2);

    if(g_data.freq_corr < 0)
    {
        value = (u16)(-g_data.freq_corr);
        SEG_SetCode(4, SEG_DASH);
        SEG_SetNumber((u32)value, 5, 3, 1);
    }
    else
    {
        value = (u16)g_data.freq_corr;
        SEG_SetNumber((u32)value, 4, 4, 1);
    }
}

static void App_ShowParamPage(void)
{
    if(g_param_page == PARAM_LIMIT)
    {
        App_ShowParamLimitPage();
    }
    else
    {
        App_ShowParamCalPage();
    }
}

static void App_ShowTimePage(void)
{
    SEG_ClearBuffer();
    SEG_SetDigit(0, g_data.rtc.hour / 10);
    SEG_SetDigit(1, g_data.rtc.hour % 10);
    SEG_SetCode(2, SEG_DASH);
    SEG_SetDigit(3, g_data.rtc.min / 10);
    SEG_SetDigit(4, g_data.rtc.min % 10);
    SEG_SetCode(5, SEG_DASH);
    SEG_SetDigit(6, g_data.rtc.sec / 10);
    SEG_SetDigit(7, g_data.rtc.sec % 10);
}

static void App_ShowEchoFreqPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_H);
    SEG_SetCode(1, SEG_F);
    SEG_SetNumber((u32)g_data.freq_max, 3, 5, 1);
}

static void App_ShowEchoTimePage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_H);
    SEG_SetCode(1, SEG_A);
    SEG_SetDigit(2, g_data.max_time.hour / 10);
    SEG_SetDigit(3, g_data.max_time.hour % 10);
    SEG_SetDigit(4, g_data.max_time.min / 10);
    SEG_SetDigit(5, g_data.max_time.min % 10);
    SEG_SetDigit(6, g_data.max_time.sec / 10);
    SEG_SetDigit(7, g_data.max_time.sec % 10);
}

static void App_ShowEchoPage(void)
{
    if(g_echo_page == ECHO_FREQ)
    {
        App_ShowEchoFreqPage();
    }
    else
    {
        App_ShowEchoTimePage();
    }
}

static void App_UpdateDisplay(void)
{
    switch(g_page)
    {
    case PAGE_FREQ:
        App_ShowFreqPage();
        break;

    case PAGE_PARAM:
        App_ShowParamPage();
        break;

    case PAGE_TIME:
        App_ShowTimePage();
        break;

    default:
        App_ShowEchoPage();
        break;
    }
}

static void App_HandleKey(u8 key)
{
    switch(key)
    {
    case 4:
        if(g_page == PAGE_FREQ)
        {
            g_page = PAGE_PARAM;
            g_param_page = PARAM_LIMIT;
        }
        else if(g_page == PAGE_PARAM)
        {
            g_page = PAGE_TIME;
        }
        else if(g_page == PAGE_TIME)
        {
            g_page = PAGE_ECHO;
            g_echo_page = ECHO_FREQ;
        }
        else
        {
            g_page = PAGE_FREQ;
        }
        break;

    case 5:
        if(g_page == PAGE_PARAM)
        {
            if(g_param_page == PARAM_LIMIT)
            {
                g_param_page = PARAM_CAL;
            }
            else
            {
                g_param_page = PARAM_LIMIT;
            }
        }
        else if(g_page == PAGE_ECHO)
        {
            if(g_echo_page == ECHO_FREQ)
            {
                g_echo_page = ECHO_TIME;
            }
            else
            {
                g_echo_page = ECHO_FREQ;
            }
        }
        break;

    case 8:
        if(g_page == PAGE_PARAM)
        {
            if(g_param_page == PARAM_LIMIT)
            {
                if(g_data.freq_limit < 9000u)
                {
                    g_data.freq_limit += 1000u;
                }
            }
            else
            {
                if(g_data.freq_corr < 900)
                {
                    g_data.freq_corr += 100;
                }
            }
            App_UpdateFreqValue();
            App_UpdateDac();
            App_UpdateLed();
        }
        break;

    case 9:
        if(g_page == PAGE_PARAM)
        {
            if(g_param_page == PARAM_LIMIT)
            {
                if(g_data.freq_limit > 1000u)
                {
                    g_data.freq_limit -= 1000u;
                }
            }
            else
            {
                if(g_data.freq_corr > -900)
                {
                    g_data.freq_corr -= 100;
                }
            }
            App_UpdateFreqValue();
            App_UpdateDac();
            App_UpdateLed();
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
    FREQ_InitCounter0();
    FREQ_PollTask();

    g_data.freq_limit = 2000u;
    g_data.freq_corr = 0;
    g_data.freq_raw = 0u;
    g_data.freq_value = 0;
    g_data.freq_max = 0;
    g_data.rtc.hour = 13;
    g_data.rtc.min = 3;
    g_data.rtc.sec = 5;
    g_data.max_time = g_data.rtc;

    RTC_SetTime(&g_data.rtc);
    App_UpdateDac();
    App_UpdateLed();
}

void App_Loop(void)
{
    if(g_flag_10ms)
    {
        g_flag_10ms = 0;
        FREQ_PollTask();
        Key_Scan10ms();
        App_HandleKey(Key_GetEvent());
    }

    if(g_flag_100ms)
    {
        g_flag_100ms = 0;
        g_data.freq_raw = FREQ_GetHz();
        App_UpdateFreqValue();
        App_UpdateMaxRecord();
        App_UpdateDac();

        g_led_div_100ms++;
        if(g_led_div_100ms >= 2u)
        {
            g_led_div_100ms = 0u;
            g_led_flash_200ms = !g_led_flash_200ms;
        }
        App_UpdateLed();
    }

    if(g_flag_1s)
    {
        g_flag_1s = 0;
        RTC_ReadTime(&g_data.rtc);
    }

    App_UpdateDisplay();
}
