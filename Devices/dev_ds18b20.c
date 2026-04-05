#include "dev_ds18b20.h"
#include "..\BSP\bsp_tick.h"

/*
 * 函数名：DS18B20_StartConvert
 * 功能：启动一次温度转换
 * 参数：无
 * 返回值：无
 */
static void DS18B20_StartConvert(void)
{
    ONEWIRE_Reset();
    ONEWIRE_WriteByte(0xCC);
    ONEWIRE_WriteByte(0x44);
}

/*
 * 函数名：DS18B20_ReadTemp
 * 功能：读取当前温度
 * 参数：无
 * 返回值：温度值，单位摄氏度
 * 备注：首轮调用会等待第一次转换完成；之后读取上一次转换结果并立即启动下一轮转换
 */
float DS18B20_ReadTemp(void)
{
    static bit started = 0;
    static float last_temp = 0.0f;
    u32 wait_start;
    int raw;
    u8 lsb;
    u8 msb;

    if(!started)
    {
        DS18B20_StartConvert();
        wait_start = g_ms_ticks;
        while((g_ms_ticks - wait_start) < 800)
        {
        }
        started = 1;
    }

    ONEWIRE_Reset();
    ONEWIRE_WriteByte(0xCC);
    ONEWIRE_WriteByte(0xBE);
    lsb = ONEWIRE_ReadByte();
    msb = ONEWIRE_ReadByte();

    raw = ((int)((u16)msb << 8)) | lsb;
    last_temp = raw / 16.0f;

    DS18B20_StartConvert();

    return last_temp;
}
