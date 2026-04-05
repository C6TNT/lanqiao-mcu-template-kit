/* 第十二届省赛模板版示例：DS18B20 + DAC 控制 */
#include "..\App\app.h"

typedef enum
{
    PAGE_C = 0,
    PAGE_P,
    PAGE_A
} app_page_t;

static app_page_t g_page = PAGE_C;
static float g_temp = 0.0f;
static char g_limit = 25;
static char g_edit_limit = 25;
static u16 g_dac_mv = 0;
static u8 g_dac_dat = 0;
static u8 g_mode = 1;

static void App_UpdateDacByRule(void)
{
    if(g_mode == 1)
    {
        if(g_temp < g_limit)
        {
            g_dac_dat = 0;
            g_dac_mv = 0;
        }
        else
        {
            g_dac_dat = 255;
            g_dac_mv = 500;
        }
    }
    else
    {
        if(g_temp <= 20.0f)
        {
            g_dac_dat = 51;
            g_dac_mv = 100;
        }
        else if(g_temp >= 40.0f)
        {
            g_dac_dat = 204;
            g_dac_mv = 400;
        }
        else
        {
            g_dac_mv = (u16)(g_temp * 15.0f - 200.0f);
            g_dac_dat = (u8)(g_dac_mv * 255u / 500u);
        }
    }

    PCF8591_WriteDAC(g_dac_dat);
}

static void App_UpdateLed(void)
{
    LED_AllOff();
    LED_Set(0, g_mode == 1);
    LED_Set(1, g_mode == 2);
    LED_Set(2, g_page == PAGE_C);
    LED_Set(3, g_page == PAGE_P);
    LED_Set(4, g_page == PAGE_A);
}

static void App_ShowCPage(void)
{
    int temp10;

    temp10 = (int)(g_temp * 10);
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_C);
    SEG_SetDigit(4, (u8)(temp10 / 100));
    SEG_SetDigit(5, (u8)((temp10 / 10) % 10));
    SEG_SetCode(6, SEG_DP);
    SEG_SetDigit(7, (u8)(temp10 % 10));
}

static void App_ShowPPage(void)
{
    char value;

    value = g_edit_limit;
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);
    SEG_SetDigit(6, (u8)(value / 10));
    SEG_SetDigit(7, (u8)(value % 10));
}

static void App_ShowAPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_A);
    SEG_SetDigit(4, (u8)(g_dac_mv / 1000));
    SEG_SetDigit(5, (u8)((g_dac_mv / 100) % 10));
    SEG_SetCode(6, SEG_DP);
    SEG_SetDigit(7, (u8)((g_dac_mv / 10) % 10));
}

static void App_UpdateDisplay(void)
{
    switch(g_page)
    {
    case PAGE_C:
        App_ShowCPage();
        break;
    case PAGE_P:
        App_ShowPPage();
        break;
    default:
        App_ShowAPage();
        break;
    }
}

static void App_HandleKey(u8 key)
{
    switch(key)
    {
    case 4:
        if(g_page == PAGE_C)
        {
            g_page = PAGE_P;
            g_edit_limit = g_limit;
        }
        else if(g_page == PAGE_P)
        {
            g_page = PAGE_A;
            g_limit = g_edit_limit;
        }
        else
        {
            g_page = PAGE_C;
        }
        break;

    case 5:
        if(g_mode == 1)
        {
            g_mode = 2;
        }
        else
        {
            g_mode = 1;
        }
        break;

    case 8:
        if(g_page == PAGE_P)
        {
            g_edit_limit--;
            if(g_edit_limit < 0)
            {
                g_edit_limit = 99;
            }
        }
        break;

    case 9:
        if(g_page == PAGE_P)
        {
            g_edit_limit++;
            if(g_edit_limit > 99)
            {
                g_edit_limit = 0;
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
    App_UpdateDacByRule();
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
        g_temp = DS18B20_ReadTemp();
    }

    if(g_flag_500ms)
    {
        g_flag_500ms = 0;
        App_UpdateDacByRule();
        App_UpdateLed();
    }

    App_UpdateDisplay();
}
