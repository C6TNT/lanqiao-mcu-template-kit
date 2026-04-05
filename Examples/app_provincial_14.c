/* 第十四届省赛模板版示例：光照触发采样 + 温湿度回显 + RTC + NE555 频率换湿度 */
#include "..\App\app.h"

typedef enum
{
    PAGE_TIME = 0,
    PAGE_REVIEW,
    PAGE_PARAM,
    PAGE_SAMPLE
} app_page_t;

typedef enum
{
    REVIEW_TEMP = 0,
    REVIEW_HUMI,
    REVIEW_TIME
} app_review_t;

static app_page_t g_page = PAGE_TIME;
static app_page_t g_page_before_sample = PAGE_TIME;
static app_review_t g_review_page = REVIEW_TEMP;

static rtc_time_t g_time = {8, 0, 0};
static u8 g_temp_limit = 30;

static float g_temp = 0.0f;
static u16 g_freq_hz = 0;
static u8 g_light_raw = 0;
static u8 g_is_dark = 0;
static u8 g_last_dark = 0;

static u8 g_sample_active = 0;
static u8 g_sample_valid = 0;
static u16 g_sample_elapsed_ms = 0;
static u8 g_sample_temp_now = 0;
static u8 g_sample_humi_now = 0;
static u8 g_sample_humi_valid = 0;
static u16 g_sample_count = 0;
static u16 g_sample_temp_sum10 = 0;
static u16 g_sample_humi_sum10 = 0;
static u8 g_sample_temp_max = 0;
static u8 g_sample_humi_max = 0;

static u8 g_trigger_count = 0;
static u16 g_last_temp10 = 0;
static u16 g_last_humi10 = 0;
static u8 g_has_last_valid = 0;
static u8 g_record_temp_max = 0;
static u8 g_record_humi_max = 0;
static u16 g_record_temp_avg10 = 0;
static u16 g_record_humi_avg10 = 0;
static u8 g_record_hour = 0;
static u8 g_record_min = 0;

static u8 g_invalid_data_latch = 0;
static u8 g_rise_both_latch = 0;
static u8 g_flash_100ms = 0;
static u16 g_key9_hold_ms = 0;

static u16 App_FreqToHumi10(u16 freq_hz)
{
    return (u16)((((u32)freq_hz * 444u) + 500u) / 1000u + 12u);
}

static u8 App_IsFreqValid(u16 freq_hz)
{
    if((freq_hz < 200u) || (freq_hz > 2000u))
    {
        return 0;
    }
    return 1;
}

static u8 App_GetTempInt(float temp)
{
    int temp_int;

    temp_int = (int)temp;
    if(temp_int < 0)
    {
        temp_int = 0;
    }
    if(temp_int > 99)
    {
        temp_int = 99;
    }
    return (u8)temp_int;
}

static void App_ClearRecord(void)
{
    g_trigger_count = 0;
    g_record_temp_max = 0;
    g_record_humi_max = 0;
    g_record_temp_avg10 = 0;
    g_record_humi_avg10 = 0;
    g_record_hour = 0;
    g_record_min = 0;
    g_has_last_valid = 0;
    g_last_temp10 = 0;
    g_last_humi10 = 0;
    g_rise_both_latch = 0;
}

static void App_StartSample(void)
{
    g_page_before_sample = g_page;
    g_page = PAGE_SAMPLE;
    g_sample_active = 1;
    g_sample_valid = 1;
    g_sample_elapsed_ms = 0;
    g_sample_count = 0;
    g_sample_temp_sum10 = 0;
    g_sample_humi_sum10 = 0;
    g_sample_temp_max = 0;
    g_sample_humi_max = 0;
    g_sample_temp_now = 0;
    g_sample_humi_now = 0;
    g_sample_humi_valid = 0;
    g_record_hour = g_time.hour;
    g_record_min = g_time.min;
}

static void App_FinishSample(void)
{
    u16 temp_avg10;
    u16 humi_avg10;

    if((g_sample_count == 0u) || (!g_sample_valid))
    {
        g_invalid_data_latch = 1;
    }
    else
    {
        temp_avg10 = g_sample_temp_sum10 / g_sample_count;
        humi_avg10 = g_sample_humi_sum10 / g_sample_count;

        g_trigger_count++;
        g_record_temp_max = g_sample_temp_max;
        g_record_humi_max = g_sample_humi_max;
        g_record_temp_avg10 = temp_avg10;
        g_record_humi_avg10 = humi_avg10;

        if(g_has_last_valid && (g_trigger_count >= 2u))
        {
            if((temp_avg10 > g_last_temp10) && (humi_avg10 > g_last_humi10))
            {
                g_rise_both_latch = 1;
            }
            else
            {
                g_rise_both_latch = 0;
            }
        }
        else
        {
            g_rise_both_latch = 0;
        }

        g_last_temp10 = temp_avg10;
        g_last_humi10 = humi_avg10;
        g_has_last_valid = 1;
        g_invalid_data_latch = 0;
    }

    g_sample_active = 0;
    g_page = g_page_before_sample;
}

static u8 App_IsKey9PressedNow(void)
{
    P44 = 0;
    P42 = 1;
    P35 = 1;
    P34 = 1;
    P3 |= 0x0F;

    if(P32 == 0)
    {
        P44 = 1;
        P42 = 1;
        P35 = 1;
        P34 = 1;
        P3 |= 0x0F;
        return 1;
    }

    P44 = 1;
    P42 = 1;
    P35 = 1;
    P34 = 1;
    P3 |= 0x0F;
    return 0;
}

static void App_UpdateLed(void)
{
    u8 over_limit;

    over_limit = 0;
    if(g_sample_active && (g_sample_temp_now > g_temp_limit))
    {
        over_limit = 1;
    }

    LED_AllOff();
    LED_Set(0, g_page == PAGE_TIME);
    LED_Set(1, g_page == PAGE_REVIEW);
    LED_Set(2, g_page == PAGE_SAMPLE);
    LED_Set(3, over_limit && g_flash_100ms);
    LED_Set(4, g_invalid_data_latch);
    LED_Set(5, g_rise_both_latch);
}

static void App_ShowTimePage(void)
{
    SEG_ClearBuffer();
    SEG_SetDigit(0, g_time.hour / 10);
    SEG_SetDigit(1, g_time.hour % 10);
    SEG_SetCode(2, SEG_DASH);
    SEG_SetDigit(3, g_time.min / 10);
    SEG_SetDigit(4, g_time.min % 10);
    SEG_SetCode(5, SEG_DASH);
    SEG_SetDigit(6, g_time.sec / 10);
    SEG_SetDigit(7, g_time.sec % 10);
}

static void App_ShowReviewTemp(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_C);

    if(g_trigger_count == 0u)
    {
        return;
    }

    SEG_SetDigit(2, g_record_temp_max / 10);
    SEG_SetDigit(3, g_record_temp_max % 10);
    SEG_SetCode(4, SEG_DASH);
    SEG_SetDigit(5, (u8)(g_record_temp_avg10 / 100));
    SEG_SetDigit(6, (u8)((g_record_temp_avg10 / 10) % 10));
    SEG_SetDigit(7, (u8)(g_record_temp_avg10 % 10));
}

static void App_ShowReviewHumi(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_H);

    if(g_trigger_count == 0u)
    {
        return;
    }

    SEG_SetDigit(2, g_record_humi_max / 10);
    SEG_SetDigit(3, g_record_humi_max % 10);
    SEG_SetCode(4, SEG_DASH);
    SEG_SetDigit(5, (u8)(g_record_humi_avg10 / 100));
    SEG_SetDigit(6, (u8)((g_record_humi_avg10 / 10) % 10));
    SEG_SetDigit(7, (u8)(g_record_humi_avg10 % 10));
}

static void App_ShowReviewTime(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_F);
    SEG_SetDigit(1, g_trigger_count / 10);
    SEG_SetDigit(2, g_trigger_count % 10);

    if(g_trigger_count == 0u)
    {
        return;
    }

    SEG_SetDigit(3, g_record_hour / 10);
    SEG_SetDigit(4, g_record_hour % 10);
    SEG_SetCode(5, SEG_DASH);
    SEG_SetDigit(6, g_record_min / 10);
    SEG_SetDigit(7, g_record_min % 10);
}

static void App_ShowReviewPage(void)
{
    switch(g_review_page)
    {
    case REVIEW_TEMP:
        App_ShowReviewTemp();
        break;
    case REVIEW_HUMI:
        App_ShowReviewHumi();
        break;
    default:
        App_ShowReviewTime();
        break;
    }
}

static void App_ShowParamPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);
    SEG_SetDigit(6, g_temp_limit / 10);
    SEG_SetDigit(7, g_temp_limit % 10);
}

static void App_ShowSamplePage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_E);
    SEG_SetDigit(3, g_sample_temp_now / 10);
    SEG_SetDigit(4, g_sample_temp_now % 10);
    SEG_SetCode(5, SEG_DASH);

    if(g_sample_humi_valid)
    {
        SEG_SetDigit(6, g_sample_humi_now / 10);
        SEG_SetDigit(7, g_sample_humi_now % 10);
    }
    else
    {
        SEG_SetCode(6, SEG_A);
        SEG_SetCode(7, SEG_A);
    }
}

static void App_UpdateDisplay(void)
{
    switch(g_page)
    {
    case PAGE_TIME:
        App_ShowTimePage();
        break;
    case PAGE_REVIEW:
        App_ShowReviewPage();
        break;
    case PAGE_PARAM:
        App_ShowParamPage();
        break;
    default:
        App_ShowSamplePage();
        break;
    }
}

static void App_HandleKey(u8 key)
{
    if(g_sample_active)
    {
        return;
    }

    switch(key)
    {
    case 4:
        if(g_page == PAGE_TIME)
        {
            g_page = PAGE_REVIEW;
            g_review_page = REVIEW_TEMP;
        }
        else if(g_page == PAGE_REVIEW)
        {
            g_page = PAGE_PARAM;
        }
        else
        {
            g_page = PAGE_TIME;
        }
        break;

    case 5:
        if(g_page == PAGE_REVIEW)
        {
            g_review_page++;
            if(g_review_page > REVIEW_TIME)
            {
                g_review_page = REVIEW_TEMP;
            }
        }
        break;

    case 8:
        if(g_page == PAGE_PARAM)
        {
            if(g_temp_limit < 99u)
            {
                g_temp_limit++;
            }
        }
        break;

    case 9:
        if(g_page == PAGE_PARAM)
        {
            if(g_temp_limit > 0u)
            {
                g_temp_limit--;
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
    FREQ_InitCounter0();
    FREQ_PollTask();
    RTC_SetTime(&g_time);
    g_temp = DS18B20_ReadTemp();
    g_sample_temp_now = App_GetTempInt(g_temp);
    App_UpdateLed();
}

void App_Loop(void)
{
    u8 now_dark;
    u16 humi10;
    u8 key9_now;

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
        g_flash_100ms = !g_flash_100ms;

        g_light_raw = PCF8591_ReadADC(0x01);
        g_temp = DS18B20_ReadTemp();
        g_freq_hz = FREQ_GetHz();

        now_dark = 0;
        if(g_light_raw < 20u)
        {
            now_dark = 1;
        }
        g_is_dark = now_dark;

        if((!g_sample_active) && g_is_dark && (!g_last_dark))
        {
            App_StartSample();
        }
        g_last_dark = g_is_dark;

        if(g_sample_active)
        {
            g_sample_elapsed_ms += 100u;
            g_sample_temp_now = App_GetTempInt(g_temp);

            if(App_IsFreqValid(g_freq_hz))
            {
                humi10 = App_FreqToHumi10(g_freq_hz);
                g_sample_humi_now = (u8)(humi10 / 10u);
                g_sample_humi_valid = 1;
                g_sample_count++;
                g_sample_temp_sum10 += (u16)(g_sample_temp_now * 10u);
                g_sample_humi_sum10 += humi10;

                if(g_sample_temp_now > g_sample_temp_max)
                {
                    g_sample_temp_max = g_sample_temp_now;
                }
                if(g_sample_humi_now > g_sample_humi_max)
                {
                    g_sample_humi_max = g_sample_humi_now;
                }
            }
            else
            {
                g_sample_humi_valid = 0;
                g_sample_valid = 0;
            }

            if(g_sample_elapsed_ms >= 3000u)
            {
                App_FinishSample();
            }
        }

        if((g_page == PAGE_REVIEW) && (g_review_page == REVIEW_TIME) && (!g_sample_active))
        {
            key9_now = App_IsKey9PressedNow();
            if(key9_now)
            {
                if(g_key9_hold_ms < 3000u)
                {
                    g_key9_hold_ms += 100u;
                }
            }
            else
            {
                if(g_key9_hold_ms >= 2000u)
                {
                    App_ClearRecord();
                }
                g_key9_hold_ms = 0;
            }
        }
        else
        {
            g_key9_hold_ms = 0;
        }

        App_UpdateLed();
    }

    if(g_flag_1s)
    {
        g_flag_1s = 0;
        RTC_ReadTime(&g_time);
    }

    App_UpdateDisplay();
}
