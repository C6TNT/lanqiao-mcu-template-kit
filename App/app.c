/* 默认业务层示例：独立按键演示 + 温度/ADC/超声波/频率/RTC + 高级按键事件演示 */
#include "app.h"

typedef enum
{
    PAGE_DATA = 0,
    PAGE_PARAM,
    PAGE_TIME,
    PAGE_FREQ,
    PAGE_RECORD
} app_page_t;

typedef enum
{
    FIELD_TEMP_LIMIT = 0,
    FIELD_DIST_LIMIT,
    FIELD_ADC_LIMIT
} app_field_t;

typedef struct
{
    float temp;
    u8 adc;
    u16 dist;
    u16 freq;
    rtc_time_t rtc;
} app_data_t;

typedef struct
{
    int temp_limit_x10;
    u16 dist_limit;
    u8 adc_limit;
} app_param_t;

static app_page_t g_page = PAGE_DATA;
static app_field_t g_field = FIELD_TEMP_LIMIT;
static app_data_t g_data = {0};
static app_param_t g_param = {300, 30, 200};
static bit g_param_mode = 0;
static bit g_alarm = 0;
static bit g_alarm_flash = 0;
static u16 g_dist_filtered = 999;
static u16 g_freq_filtered = 0;
static u8 g_last_adv_type = 0;
static u8 g_last_adv_key = 0;

static void App_UartSendU16(u16 value)
{
    char buf[6];
    u8 i = 0;

    if(value == 0u)
    {
        UART_SendByte('0');
        return;
    }

    while(value > 0u)
    {
        buf[i++] = (char)('0' + (value % 10u));
        value /= 10u;
    }

    while(i > 0u)
    {
        UART_SendByte(buf[--i]);
    }
}

static void App_UartSendTemp10(int temp10)
{
    if(temp10 < 0)
    {
        UART_SendByte('-');
        temp10 = -temp10;
    }

    App_UartSendU16((u16)(temp10 / 10));
    UART_SendByte('.');
    UART_SendByte((u8)('0' + (temp10 % 10)));
}

static void App_UartSendSnapshot(void)
{
    UART_SendString("T=");
    App_UartSendTemp10((int)(g_data.temp * 10.0f));
    UART_SendString(",ADC=");
    App_UartSendU16(g_data.adc);
    UART_SendString(",DIST=");
    App_UartSendU16(g_data.dist);
    UART_SendString(",FREQ=");
    App_UartSendU16(g_data.freq);
    UART_SendString("\r\n");
}

static void App_UartSendParam(void)
{
    UART_SendString("TL=");
    App_UartSendTemp10(g_param.temp_limit_x10);
    UART_SendString(",DL=");
    App_UartSendU16(g_param.dist_limit);
    UART_SendString(",AL=");
    App_UartSendU16(g_param.adc_limit);
    UART_SendString("\r\n");
}

static void App_HandleUart(void)
{
    u8 rx;

    if(!UART_GetByte(&rx))
    {
        return;
    }

    switch(rx)
    {
    case '?':
    case 's':
    case 'S':
        App_UartSendSnapshot();
        break;

    case 'p':
    case 'P':
        App_UartSendParam();
        break;

    case 'h':
    case 'H':
        UART_SendString("?:snapshot p:param h:help\r\n");
        break;

    default:
        break;
    }
}

static void App_SaveParam(void)
{
    AT24C02_WriteByte(0x00, (u8)(g_param.temp_limit_x10 / 10));
    AT24C02_WriteByte(0x01, (u8)(g_param.temp_limit_x10 % 10));
    AT24C02_WriteByte(0x02, (u8)g_param.dist_limit);
    AT24C02_WriteByte(0x03, g_param.adc_limit);
}

static void App_LoadParam(void)
{
    u8 a;
    u8 b;
    u8 c;
    u8 d;

    a = AT24C02_ReadByte(0x00);
    b = AT24C02_ReadByte(0x01);
    c = AT24C02_ReadByte(0x02);
    d = AT24C02_ReadByte(0x03);

    if((a <= 9u) && (b <= 9u))
    {
        g_param.temp_limit_x10 = a * 10 + b;
        if(g_param.temp_limit_x10 < 50)
        {
            g_param.temp_limit_x10 = 300;
        }
    }

    if(c != 0xFFu)
    {
        g_param.dist_limit = c;
    }

    if(d != 0xFFu)
    {
        g_param.adc_limit = d;
    }
}

static void App_ShowDataPage(void)
{
    int temp10;

    temp10 = (int)(g_data.temp * 10.0f);

    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_C);

    if(g_ms_ticks < 800u)
    {
        SEG_SetCode(1, SEG_DASH);
        SEG_SetCode(2, SEG_DASH);
        SEG_SetCode(3, SEG_DASH);
        SEG_SetCode(4, SEG_DASH);
    }
    else
    {
        SEG_SetDigit(1, (u8)(temp10 / 100));
        SEG_SetDigit(2, (u8)((temp10 / 10) % 10));
        SEG_SetCode(3, SEG_DP);
        SEG_SetDigit(4, (u8)(temp10 % 10));
    }

    SEG_SetDigit(5, g_data.adc / 100);
    SEG_SetDigit(6, (g_data.adc / 10) % 10);
    SEG_SetDigit(7, g_data.adc % 10);
}

static void App_ShowParamPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_P);

    switch(g_field)
    {
    case FIELD_TEMP_LIMIT:
        SEG_SetCode(1, SEG_C);
        SEG_SetDigit(2, (u8)(g_param.temp_limit_x10 / 100));
        SEG_SetDigit(3, (u8)((g_param.temp_limit_x10 / 10) % 10));
        SEG_SetCode(4, SEG_DP);
        SEG_SetDigit(5, (u8)(g_param.temp_limit_x10 % 10));
        break;

    case FIELD_DIST_LIMIT:
        SEG_SetCode(1, SEG_U);
        SEG_SetDigit(3, (u8)((g_param.dist_limit / 100) % 10));
        SEG_SetDigit(4, (u8)((g_param.dist_limit / 10) % 10));
        SEG_SetDigit(5, (u8)(g_param.dist_limit % 10));
        break;

    default:
        SEG_SetCode(1, SEG_A);
        SEG_SetDigit(3, g_param.adc_limit / 100);
        SEG_SetDigit(4, (g_param.adc_limit / 10) % 10);
        SEG_SetDigit(5, g_param.adc_limit % 10);
        break;
    }

    if(g_param_mode)
    {
        SEG_SetCode(7, SEG_A);
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

static void App_ShowFreqPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_F);
    SEG_SetNumber(g_data.freq, 1, 7, 1);
}

static void App_ShowRecordPage(void)
{
    SEG_ClearBuffer();
    SEG_SetCode(0, SEG_U);

    if(g_dist_filtered >= 999u)
    {
        SEG_SetDigit(1, 9);
        SEG_SetDigit(2, 9);
        SEG_SetDigit(3, 9);
    }
    else
    {
        SEG_SetDigit(1, (u8)((g_dist_filtered / 100) % 10));
        SEG_SetDigit(2, (u8)((g_dist_filtered / 10) % 10));
        SEG_SetDigit(3, (u8)(g_dist_filtered % 10));
    }

    SEG_SetCode(4, SEG_DASH);
    if(g_last_adv_type == 0u)
    {
        SEG_SetCode(5, g_alarm ? SEG_A : SEG_BLANK);
        SEG_SetCode(6, g_param_mode ? SEG_P : SEG_BLANK);
        SEG_SetCode(7, SEG_R);
    }
    else
    {
        SEG_SetDigit(5, g_last_adv_type);
        SEG_SetDigit(6, (u8)((g_last_adv_key / 10) % 10));
        SEG_SetDigit(7, (u8)(g_last_adv_key % 10));
    }
}

static void App_UpdateDisplay(void)
{
    switch(g_page)
    {
    case PAGE_DATA:
        App_ShowDataPage();
        break;

    case PAGE_PARAM:
        App_ShowParamPage();
        break;

    case PAGE_TIME:
        App_ShowTimePage();
        break;

    case PAGE_FREQ:
        App_ShowFreqPage();
        break;

    default:
        App_ShowRecordPage();
        break;
    }
}

static void App_FieldNext(void)
{
    g_field++;
    if(g_field > FIELD_ADC_LIMIT)
    {
        g_field = FIELD_TEMP_LIMIT;
    }
}

static void App_FieldAdd(void)
{
    switch(g_field)
    {
    case FIELD_TEMP_LIMIT:
        if(g_param.temp_limit_x10 < 990)
        {
            g_param.temp_limit_x10 += 5;
        }
        break;

    case FIELD_DIST_LIMIT:
        if(g_param.dist_limit < 250u)
        {
            g_param.dist_limit += 1u;
        }
        break;

    default:
        if(g_param.adc_limit < 250u)
        {
            g_param.adc_limit += 5u;
        }
        break;
    }
}

static void App_FieldSub(void)
{
    switch(g_field)
    {
    case FIELD_TEMP_LIMIT:
        if(g_param.temp_limit_x10 > 50)
        {
            g_param.temp_limit_x10 -= 5;
        }
        break;

    case FIELD_DIST_LIMIT:
        if(g_param.dist_limit > 5u)
        {
            g_param.dist_limit -= 1u;
        }
        break;

    default:
        if(g_param.adc_limit > 10u)
        {
            g_param.adc_limit -= 5u;
        }
        break;
    }
}

static void App_HandleKey(u8 key)
{
    if(!g_param_mode)
    {
        switch(key)
        {
        case 1:
            g_page++;
            if(g_page > PAGE_RECORD)
            {
                g_page = PAGE_DATA;
            }
            break;

        case 2:
            if(g_page == PAGE_PARAM)
            {
                g_param_mode = 1;
            }
            break;

        default:
            break;
        }
    }
    else
    {
        switch(key)
        {
        case 1:
            App_FieldNext();
            break;

        case 2:
            App_SaveParam();
            g_param_mode = 0;
            break;

        case 3:
            App_FieldAdd();
            break;

        case 4:
            App_FieldSub();
            break;

        default:
            break;
        }
    }
}

static void App_UpdateAlarm(void)
{
    g_alarm = 0;

    if(((int)(g_data.temp * 10.0f) > g_param.temp_limit_x10) ||
       (g_data.dist < g_param.dist_limit) ||
       (g_data.adc > g_param.adc_limit))
    {
        g_alarm = 1;
    }
}

void App_Init(void)
{
    Board_Init();
    LED_AllOff();
    BeepRelay_AllOff();
    SEG_ClearBuffer();
    UART_Init9600();
    Tick_Init();
    Ultrasonic_Init();
    FREQ_InitCounter0();
    FREQ_PollTask();

    g_data.rtc.hour = 12;
    g_data.rtc.min = 0;
    g_data.rtc.sec = 0;
    RTC_SetTime(&g_data.rtc);
    App_LoadParam();
    App_UpdateDisplay();
    UART_SendString("UART ready. Send '?' for snapshot.\r\n");
}

void App_Loop(void)
{
    u8 key;
    u8 short_key;
    u8 long_key;
    u8 double_key;

    if(g_flag_10ms)
    {
        g_flag_10ms = 0;
        FREQ_PollTask();
        Key_Scan10ms();

        key = Key_GetEvent();
        short_key = Key_GetShortEvent();
        long_key = Key_GetLongEvent();
        double_key = Key_GetDoubleEvent();

        if(short_key != KEY_NONE)
        {
            g_last_adv_type = 1;
            g_last_adv_key = short_key;
        }

        if(double_key != KEY_NONE)
        {
            g_last_adv_type = 2;
            g_last_adv_key = double_key;
        }

        if(long_key != KEY_NONE)
        {
            g_last_adv_type = 3;
            g_last_adv_key = long_key;
        }

        if(key != KEY_NONE)
        {
            App_HandleKey(key);
            App_UpdateDisplay();
        }
    }

    App_HandleUart();

    if(g_flag_100ms)
    {
        u16 dist_now;

        g_flag_100ms = 0;
        g_data.adc = PCF8591_ReadADC(0x01);
        g_data.temp = DS18B20_ReadTemp();

        dist_now = Ultrasonic_ReadCm();
        if(dist_now >= 999u)
        {
            g_dist_filtered = 999u;
        }
        else if(g_dist_filtered >= 999u)
        {
            g_dist_filtered = dist_now;
        }
        else
        {
            g_dist_filtered = (u16)((g_dist_filtered * 2u + dist_now) / 3u);
        }

        g_data.dist = g_dist_filtered;
        App_UpdateAlarm();
        App_UpdateDisplay();
    }

    if(g_flag_500ms)
    {
        g_flag_500ms = 0;
        g_alarm_flash = !g_alarm_flash;

        FREQ_PollTask();
        g_data.freq = FREQ_GetHz();
        if(g_freq_filtered == 0u)
        {
            g_freq_filtered = g_data.freq;
        }
        else
        {
            g_freq_filtered = (u16)((g_freq_filtered * 2u + g_data.freq) / 3u);
        }
        g_data.freq = g_freq_filtered;

        if(g_alarm && g_alarm_flash)
        {
            LED_Set(0, 1);
            Beep_Set(1);
        }
        else
        {
            LED_Set(0, 0);
            Beep_Set(0);
        }

        App_UpdateDisplay();
    }

    if(g_flag_1s)
    {
        g_flag_1s = 0;
        RTC_ReadTime(&g_data.rtc);
        App_UpdateDisplay();
    }
}
