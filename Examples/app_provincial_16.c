/* 第十六届省赛模板版示例：环境、运动、参数、统计 */
#include "..\App\app.h"

typedef enum
{
    PAGE_ENV = 0,
    PAGE_MOVE,
    PAGE_PARAM,
    PAGE_STAT
} app_page_t;

typedef enum
{
    PARAM_TEMP = 0,
    PARAM_DIST
} app_param_t;

static app_page_t g_page = PAGE_ENV;
static app_param_t g_param_page = PARAM_TEMP;

static u8 g_temp = 0;
static u8 g_light_raw = 0;
static u8 g_light_level = 1;

static u8 g_temp_limit = 30;
static u8 g_dist_limit = 30;
static u8 g_edit_temp_limit = 30;
static u8 g_edit_dist_limit = 30;

static u8 g_distance = 0;
static u8 g_last_distance = 0;
static u8 g_distance_ready = 0;
static u8 g_delta_distance = 0;
static u8 g_motion_state = 1;
static u8 g_last_motion_state = 1;
static u8 g_close = 0;

static u8 g_relay_state = 0;
static u8 g_relay_counted = 0;
static u16 g_relay_count = 0;

static u8 g_output_locked = 0;
static u16 g_sample_gap_ms = 0;
static u16 g_motion_lock_ms = 0;
static u16 g_clear_hold_ms = 0;
static u8 g_flash_100ms = 0;

static void App_UpdateLightLevel(void)
{
    if(g_light_raw >= 153u)
    {
        g_light_level = 1;
    }
    else if(g_light_raw >= 102u)
    {
        g_light_level = 2;
    }
    else if(g_light_raw >= 26u)
    {
        g_light_level = 3;
    }
    else
    {
        g_light_level = 4;
    }
}

static void App_UpdateMotionState(void)
{
    if(g_delta_distance < 5u)
    {
        g_motion_state = 1;
    }
    else if(g_delta_distance < 10u)
    {
        g_motion_state = 2;
    }
    else
    {
        g_motion_state = 3;
    }
}

static void App_ApplyRelayOutput(void)
{
    u8 high_temp;

    if(g_output_locked)
    {
        return;
    }

    high_temp = (g_temp > g_temp_limit);
    if(high_temp && g_close)
    {
        g_relay_state = 1;
        Relay_Set(1);
        if(!g_relay_counted)
        {
            g_relay_count++;
            g_relay_counted = 1;
        }
    }
    else
    {
        g_relay_state = 0;
        Relay_Set(0);
        g_relay_counted = 0;
    }
}

static void App_ApplyLedOutput(void)
{
    if(g_output_locked)
    {
        return;
    }

    LED_AllOff();

    if(g_close)
    {
        if(g_light_level >= 1u)
        {
            LED_Set(0, 1);
        }
        if(g_light_level >= 2u)
        {
            LED_Set(1, 1);
        }
        if(g_light_level >= 3u)
        {
            LED_Set(2, 1);
        }
        if(g_light_level >= 4u)
        {
            LED_Set(3, 1);
        }
    }

    if(g_motion_state == 2u)
    {
        LED_Set(7, 1);
    }
    else if(g_motion_state == 3u)
    {
        LED_Set(7, g_flash_100ms);
    }
}

static void App_RefreshOutputs(void)
{
    App_ApplyRelayOutput();
    App_ApplyLedOutput();
}

static u8 App_IsKey8And9PressedNow(void)
{
    u8 key8_down;
    u8 key9_down;

    key8_down = 0;
    key9_down = 0;

    P44 = 1;
    P42 = 1;
    P35 = 1;
    P34 = 0;
    P3 |= 0x0F;
    if(P33 == 0)
    {
        key8_down = 1;
    }

    P44 = 0;
    P42 = 1;
    P35 = 1;
    P34 = 1;
    P3 |= 0x0F;
    if(P32 == 0)
    {
        key9_down = 1;
    }

    P44 = 1;
    P42 = 1;
    P35 = 1;
    P34 = 1;
    P3 |= 0x0F;

    return (u8)(key8_down && key9_down);
}

static void App_ShowEnvPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_C);
    SEG_SetDigit(1, g_temp / 10);
    SEG_SetDigit(2, g_temp % 10);
    SEG_SetCode(6, SEG_N);
    SEG_SetDigit(7, g_light_level);
}

static void App_ShowMovePage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_L);
    SEG_SetDigit(1, g_motion_state);
    SEG_SetDigit(5, g_distance / 100);
    SEG_SetDigit(6, (u8)((g_distance / 10) % 10));
    SEG_SetDigit(7, g_distance % 10);
}

static void App_ShowParamPage(void)
{
    u8 value;

    value = (g_param_page == PARAM_TEMP) ? g_edit_temp_limit : g_edit_dist_limit;

    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);
    if(g_param_page == PARAM_TEMP)
    {
        SEG_SetCode(1, SEG_C);
    }
    else
    {
        SEG_SetCode(1, SEG_L);
    }
    SEG_SetDigit(6, value / 10);
    SEG_SetDigit(7, value % 10);
}

static void App_ShowStatPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_N);
    SEG_SetCode(1, SEG_C);
    SEG_SetNumber(g_relay_count, 4, 4, 1);
}

static void App_UpdateDisplay(void)
{
    switch(g_page)
    {
    case PAGE_ENV:
        App_ShowEnvPage();
        break;
    case PAGE_MOVE:
        App_ShowMovePage();
        break;
    case PAGE_PARAM:
        App_ShowParamPage();
        break;
    default:
        App_ShowStatPage();
        break;
    }
}

static void App_EnterParamPage(void)
{
    g_page = PAGE_PARAM;
    g_param_page = PARAM_TEMP;
    g_output_locked = 1;
}

static void App_ExitParamPage(void)
{
    g_output_locked = 0;
    g_temp_limit = g_edit_temp_limit;
    g_dist_limit = g_edit_dist_limit;
    App_RefreshOutputs();
}

static void App_HandleKey(u8 key)
{
    switch(key)
    {
    case 4:
        if(g_page == PAGE_ENV)
        {
            g_page = PAGE_MOVE;
        }
        else if(g_page == PAGE_MOVE)
        {
            App_EnterParamPage();
        }
        else if(g_page == PAGE_PARAM)
        {
            App_ExitParamPage();
            g_page = PAGE_STAT;
        }
        else
        {
            g_page = PAGE_ENV;
        }
        break;

    case 5:
        if(g_page == PAGE_PARAM)
        {
            if(g_param_page == PARAM_TEMP)
            {
                g_param_page = PARAM_DIST;
            }
            else
            {
                g_param_page = PARAM_TEMP;
            }
        }
        break;

    case 8:
        if(g_page == PAGE_PARAM)
        {
            if(g_param_page == PARAM_TEMP)
            {
                if(g_edit_temp_limit < 80u)
                {
                    g_edit_temp_limit++;
                }
            }
            else if(g_edit_dist_limit < 80u)
            {
                g_edit_dist_limit += 5u;
                if(g_edit_dist_limit > 80u)
                {
                    g_edit_dist_limit = 80u;
                }
            }
        }
        break;

    case 9:
        if(g_page == PAGE_PARAM)
        {
            if(g_param_page == PARAM_TEMP)
            {
                if(g_edit_temp_limit > 20u)
                {
                    g_edit_temp_limit--;
                }
            }
            else if(g_edit_dist_limit > 20u)
            {
                g_edit_dist_limit -= 5u;
                if(g_edit_dist_limit < 20u)
                {
                    g_edit_dist_limit = 20u;
                }
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
    Ultrasonic_Init();

    g_temp = (u8)DS18B20_ReadTemp();
    g_edit_temp_limit = g_temp_limit;
    g_edit_dist_limit = g_dist_limit;
    App_RefreshOutputs();
}

void App_Loop(void)
{
    u8 distance_now;

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

        g_light_raw = PCF8591_ReadADC(0x01);
        App_UpdateLightLevel();

        g_sample_gap_ms += 100u;
        if(g_sample_gap_ms >= 1000u)
        {
            g_sample_gap_ms = 0;

            distance_now = (u8)Ultrasonic_ReadCm();
            if(distance_now > 99u)
            {
                distance_now = 99u;
            }

            g_distance = distance_now;
            g_close = (u8)(g_distance < g_dist_limit);

            if(!g_distance_ready)
            {
                g_last_distance = g_distance;
                g_distance_ready = 1;
                g_motion_state = 1;
                g_last_motion_state = 1;
            }
            else if(g_motion_lock_ms == 0u)
            {
                if(g_distance >= g_last_distance)
                {
                    g_delta_distance = g_distance - g_last_distance;
                }
                else
                {
                    g_delta_distance = g_last_distance - g_distance;
                }

                App_UpdateMotionState();
                if(g_motion_state != g_last_motion_state)
                {
                    g_motion_lock_ms = 3000u;
                }
                g_last_motion_state = g_motion_state;
                g_last_distance = g_distance;
            }
            else
            {
                g_last_distance = g_distance;
            }
        }

        if(g_motion_lock_ms >= 100u)
        {
            g_motion_lock_ms -= 100u;
        }
        else
        {
            g_motion_lock_ms = 0u;
        }

        if(g_page == PAGE_STAT)
        {
            if(App_IsKey8And9PressedNow())
            {
                if(g_clear_hold_ms < 3000u)
                {
                    g_clear_hold_ms += 100u;
                }
                if(g_clear_hold_ms >= 2000u)
                {
                    g_relay_count = 0;
                }
            }
            else
            {
                g_clear_hold_ms = 0u;
            }
        }
        else
        {
            g_clear_hold_ms = 0u;
        }

        App_ApplyLedOutput();
    }

    if(g_flag_500ms)
    {
        g_flag_500ms = 0;
        g_temp = (u8)DS18B20_ReadTemp();
        App_ApplyRelayOutput();
    }

    App_UpdateDisplay();
}
