/* 第十届省赛模板版示例：ADC + DAC + 频率测量 */
#include "..\App\app.h"

typedef enum
{
    PAGE_U = 0,
    PAGE_F,
    PAGE_HIDE
} app_page_t;

static app_page_t g_page = PAGE_U;
static app_page_t g_last_page = PAGE_U;
static u16 g_u_mv = 0;
static u16 g_freq_hz = 0;
static u8 g_adc_raw = 0;
static u8 g_dac_mode = 1;
static u8 g_seg_enable = 1;
static u8 g_led_enable = 1;

static void App_UpdateDac(void)
{
    if(g_dac_mode == 1)
    {
        PCF8591_WriteDAC(102);
    }
    else
    {
        PCF8591_WriteDAC(g_adc_raw);
    }
}

static void App_UpdateLed(void)
{
    LED_AllOff();

    if(!g_led_enable)
    {
        return;
    }

    LED_Set(0, g_page == PAGE_U);
    LED_Set(1, g_page == PAGE_F);
    LED_Set(2, (g_u_mv < 150) || ((g_u_mv >= 250) && (g_u_mv < 350)));
    LED_Set(3, (g_u_mv >= 150) && ((g_u_mv < 250) || (g_u_mv >= 350)));
    LED_Set(4, (g_freq_hz < 1000) || ((g_freq_hz >= 5000) && (g_freq_hz < 10000)));
    LED_Set(5, (g_freq_hz >= 1000) && ((g_freq_hz < 5000) || (g_freq_hz >= 10000)));
    LED_Set(6, g_dac_mode == 1);
    LED_Set(7, g_dac_mode == 2);
}

static void App_ShowUPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_U);
    SEG_SetDigit(4, (u8)(g_u_mv / 1000));
    SEG_SetDigit(5, (u8)((g_u_mv / 100) % 10));
    SEG_SetCode(6, SEG_DP);
    SEG_SetDigit(7, (u8)((g_u_mv / 10) % 10));
}

static void App_ShowFPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_F);
    SEG_SetNumber(g_freq_hz, 1, 7, 1);
}

static void App_UpdateDisplay(void)
{
    if(!g_seg_enable)
    {
        SEG_ClearBuffer();
        return;
    }

    switch(g_page)
    {
    case PAGE_U:
        App_ShowUPage();
        break;
    case PAGE_F:
        App_ShowFPage();
        break;
    default:
        SEG_ClearBuffer();
        break;
    }
}

static void App_HandleKey(u8 key)
{
    switch(key)
    {
    case 1:
        if(g_page == PAGE_U)
        {
            g_page = PAGE_F;
        }
        else
        {
            g_page = PAGE_U;
        }
        break;

    case 2:
        if(g_dac_mode == 1)
        {
            g_dac_mode = 2;
        }
        else
        {
            g_dac_mode = 1;
        }
        break;

    case 3:
        g_led_enable = !g_led_enable;
        if(!g_led_enable)
        {
            LED_AllOff();
        }
        break;

    case 4:
        g_seg_enable = !g_seg_enable;
        if(g_seg_enable)
        {
            g_page = g_last_page;
        }
        else
        {
            g_last_page = g_page;
            g_page = PAGE_HIDE;
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
    App_UpdateDac();
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
        g_adc_raw = PCF8591_ReadADC(0x03);
        g_u_mv = (u16)g_adc_raw * 500u / 255u;
        App_UpdateDac();
        App_UpdateLed();
    }

    if(g_flag_500ms)
    {
        g_flag_500ms = 0;
        g_freq_hz = FREQ_GetHz();
        App_UpdateLed();
    }

    App_UpdateDisplay();
}
