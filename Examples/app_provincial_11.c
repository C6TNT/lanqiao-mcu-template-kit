/* 第十一届省赛模板版示例：电压阈值 + 次数统计 */
#include "..\App\app.h"

typedef enum
{
    PAGE_U = 0,
    PAGE_P,
    PAGE_N
} app_page_t;

static app_page_t g_page = PAGE_U;
static int g_vp = 300;
static int g_edit_vp = 300;
static u16 g_u_mv = 0;
static u8 g_adc_raw = 0;
static u8 g_cross_count = 0;
static u8 g_edit_mode = 0;
static u8 g_over_alarm = 0;
static u8 g_last_above = 0;

static void App_SaveParam(void)
{
    AT24C02_WriteByte(0x00, (u8)(g_vp / 10));
}

static void App_LoadParam(void)
{
    u8 eep_value;

    eep_value = AT24C02_ReadByte(0x00);
    if((eep_value != 0xFF) && (eep_value <= 50))
    {
        g_vp = (int)eep_value * 10;
        g_edit_vp = g_vp;
    }
}

static void App_UpdateCrossCount(void)
{
    u8 now_above;

    now_above = (g_u_mv > g_vp);
    if(g_last_above && !now_above)
    {
        g_cross_count++;
        g_over_alarm = 1;
    }

    g_last_above = now_above;
}

static void App_UpdateLed(void)
{
    LED_AllOff();
    LED_Set(0, g_over_alarm);
    LED_Set(1, (g_cross_count & 0x01) != 0);
    LED_Set(2, g_page == PAGE_U);
    LED_Set(3, g_page == PAGE_P);
    LED_Set(4, g_page == PAGE_N);
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

static void App_ShowPPage(void)
{
    int value;

    value = g_edit_mode ? g_edit_vp : g_vp;
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);
    if(g_edit_mode)
    {
        SEG_SetCode(3, SEG_A);
    }
    SEG_SetDigit(4, (u8)(value / 1000));
    SEG_SetDigit(5, (u8)((value / 100) % 10));
    SEG_SetCode(6, SEG_DP);
    SEG_SetDigit(7, (u8)((value / 10) % 10));
}

static void App_ShowNPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_N);
    SEG_SetDigit(6, g_cross_count / 10);
    SEG_SetDigit(7, g_cross_count % 10);
}

static void App_UpdateDisplay(void)
{
    switch(g_page)
    {
    case PAGE_U:
        App_ShowUPage();
        break;
    case PAGE_P:
        App_ShowPPage();
        break;
    default:
        App_ShowNPage();
        break;
    }
}

static void App_HandleKey(u8 key)
{
    switch(key)
    {
    case 3:
        if(g_page == PAGE_U)
        {
            g_page = PAGE_P;
            g_edit_vp = g_vp;
        }
        else if(g_page == PAGE_P)
        {
            g_vp = g_edit_vp;
            App_SaveParam();
            g_page = PAGE_N;
            g_edit_mode = 0;
        }
        else
        {
            g_page = PAGE_U;
        }
        break;

    case 4:
        if(g_page == PAGE_P)
        {
            g_edit_mode = 1;
            g_edit_vp += 50;
            if(g_edit_vp > 500)
            {
                g_edit_vp = 0;
            }
        }
        break;

    case 8:
        if(g_page == PAGE_P)
        {
            g_edit_mode = 1;
            g_edit_vp -= 50;
            if(g_edit_vp < 0)
            {
                g_edit_vp = 500;
            }
        }
        break;

    case 7:
        if(g_page == PAGE_N)
        {
            g_cross_count = 0;
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
    App_LoadParam();
}

void App_Loop(void)
{
    if(g_flag_10ms)
    {
        g_flag_10ms = 0;
        Key_Scan10ms();
        App_HandleKey(Key_GetEvent());
    }

    if(g_flag_100ms)
    {
        g_flag_100ms = 0;
        g_adc_raw = PCF8591_ReadADC(0x03);
        g_u_mv = (u16)g_adc_raw * 500u / 255u;
        App_UpdateCrossCount();
        App_UpdateLed();
    }

    if(g_flag_500ms)
    {
        g_flag_500ms = 0;
        if(g_over_alarm)
        {
            LED_Set(0, !g_last_above);
            g_over_alarm = 0;
        }
    }

    App_UpdateDisplay();
}
