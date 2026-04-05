/* 第九届省赛模板版示例：LED 模式 + 速度档位 + 参数保存 */
#include "..\App\app.h"

typedef enum
{
    PAGE_LED = 0,
    PAGE_SPEED,
    PAGE_PARAM
} app_page_t;

typedef struct
{
    u8 mode;
    u16 interval_ms[4];
    u8 pwm_enable;
} app_param_t;

static app_page_t g_page = PAGE_LED;
static app_param_t g_param = {1, {400, 600, 800, 1000}, 1};
static u8 g_edit_mode = 0;
static u8 g_current_led = 0;
static u8 g_speed_level = 2;
static u8 g_adc_raw = 0;
static u16 g_elapsed_ms = 0;
static u16 g_flash_ms = 0;
static u8 g_led_on = 1;

static void App_SaveParam(void)
{
    AT24C02_WriteByte(0x00, g_param.mode);
    AT24C02_WriteByte(0x01, (u8)(g_param.interval_ms[0] / 10));
    AT24C02_WriteByte(0x02, (u8)(g_param.interval_ms[1] / 10));
    AT24C02_WriteByte(0x03, (u8)(g_param.interval_ms[2] / 10));
    AT24C02_WriteByte(0x04, (u8)(g_param.interval_ms[3] / 10));
}

static void App_LoadParam(void)
{
    u8 i;
    u8 mode;
    u8 v;

    mode = AT24C02_ReadByte(0x00);
    if((mode >= 1) && (mode <= 4))
    {
        g_param.mode = mode;
    }

    for(i = 0; i < 4; i++)
    {
        v = AT24C02_ReadByte((u8)(0x01 + i));
        if((v != 0xFF) && (v >= 10) && (v <= 200))
        {
            g_param.interval_ms[i] = (u16)v * 10u;
        }
    }
}

static void App_UpdateSpeedLevel(void)
{
    if(g_adc_raw < 64)
    {
        g_speed_level = 1;
    }
    else if(g_adc_raw < 128)
    {
        g_speed_level = 2;
    }
    else if(g_adc_raw < 192)
    {
        g_speed_level = 3;
    }
    else
    {
        g_speed_level = 4;
    }
}

static void App_NextLedPattern(void)
{
    switch(g_param.mode)
    {
    case 1:
        LED_AllOff();
        LED_Set(g_current_led, 1);
        g_current_led++;
        if(g_current_led >= 8)
        {
            g_current_led = 0;
        }
        break;

    case 2:
        LED_AllOff();
        LED_Set((u8)(7 - g_current_led), 1);
        g_current_led++;
        if(g_current_led >= 8)
        {
            g_current_led = 0;
        }
        break;

    case 3:
        LED_AllOff();
        LED_Set(g_current_led, 1);
        LED_Set((u8)(7 - g_current_led), 1);
        g_current_led++;
        if(g_current_led >= 4)
        {
            g_current_led = 0;
        }
        break;

    default:
        if(g_current_led & 0x01)
        {
            LED_WriteByte(0xAA);
        }
        else
        {
            LED_WriteByte(0x55);
        }
        g_current_led++;
        break;
    }
}

static void App_ShowLedPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);
    SEG_SetDigit(1, 0);
    SEG_SetDigit(2, 9);
    SEG_SetCode(3, SEG_DASH);
    SEG_SetDigit(4, g_param.mode);
    SEG_SetDigit(6, g_current_led);
    SEG_SetDigit(7, g_speed_level);
}

static void App_ShowSpeedPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_U);
    SEG_SetDigit(2, g_speed_level);
    SEG_SetCode(3, SEG_DASH);
    SEG_SetNumber(g_adc_raw, 4, 4, 1);
}

static void App_ShowParamPage(void)
{
    u16 value;

    value = g_param.interval_ms[g_param.mode - 1];
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);
    SEG_SetDigit(1, g_param.mode);
    SEG_SetCode(2, SEG_DASH);
    if(g_edit_mode)
    {
        SEG_SetCode(3, SEG_A);
    }
    SEG_SetNumber(value, 4, 4, 1);
}

static void App_UpdateDisplay(void)
{
    switch(g_page)
    {
    case PAGE_LED:
        App_ShowLedPage();
        break;
    case PAGE_SPEED:
        App_ShowSpeedPage();
        break;
    default:
        App_ShowParamPage();
        break;
    }
}

static void App_HandleKey(u8 key)
{
    if(!g_edit_mode)
    {
        switch(key)
        {
        case 1:
            g_page++;
            if(g_page > PAGE_PARAM)
            {
                g_page = PAGE_LED;
            }
            break;
        case 2:
            g_param.mode++;
            if(g_param.mode > 4)
            {
                g_param.mode = 1;
            }
            break;
        case 3:
            g_param.pwm_enable = !g_param.pwm_enable;
            break;
        case 5:
            g_page = PAGE_PARAM;
            g_edit_mode = 1;
            break;
        default:
            break;
        }
    }
    else
    {
        switch(key)
        {
        case 5:
            App_SaveParam();
            g_edit_mode = 0;
            break;
        case 6:
            if(g_param.interval_ms[g_param.mode - 1] < 2000)
            {
                g_param.interval_ms[g_param.mode - 1] += 100;
            }
            break;
        case 7:
            if(g_param.interval_ms[g_param.mode - 1] > 100)
            {
                g_param.interval_ms[g_param.mode - 1] -= 100;
            }
            break;
        case 8:
            g_edit_mode = 0;
            break;
        default:
            break;
        }
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
    if(g_flag_1ms)
    {
        g_flag_1ms = 0;
        g_elapsed_ms++;
        g_flash_ms++;

        if(g_flash_ms >= 10)
        {
            g_flash_ms = 0;
            g_led_on = !g_led_on;
        }

        if(g_elapsed_ms >= (u16)(g_param.interval_ms[g_param.mode - 1] / g_speed_level))
        {
            g_elapsed_ms = 0;
            App_NextLedPattern();
        }

        if(g_param.pwm_enable && !g_led_on)
        {
            LED_AllOff();
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
        g_adc_raw = PCF8591_ReadADC(0x03);
        App_UpdateSpeedLevel();
    }

    App_UpdateDisplay();
}
