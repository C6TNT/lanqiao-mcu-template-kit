/* 对 LED 做一层状态封装，业务层直接调用 */
#include "bsp_led.h"

static u8 g_led_value = 0xFF;

/*
 * 函数名：LED_WriteByte
 * 功能：一次性写入 8 个 LED 的状态
 * 参数：value - LED 状态字节，低电平表示点亮
 * 返回值：无
 * 备注：适合整组更新 LED 显示
 */
void LED_WriteByte(u8 value)
{
    g_led_value = value;
    Board_SelectHC138(HC138_LED);
    P0 = g_led_value;
    P2 &= 0x1F;
}

/*
 * 函数名：LED_Set
 * 功能：设置某一个 LED 的开关状态
 * 参数：index - LED 编号，范围 0~7
 * 参数：on - 1 表示点亮，0 表示熄灭
 * 返回值：无
 * 备注：内部会自动维护整组 LED 状态
 */
void LED_Set(u8 index, bit on)
{
    if(index >= 8)
    {
        return;
    }

    if(on)
    {
        g_led_value &= ~(1 << index);
    }
    else
    {
        g_led_value |= (1 << index);
    }

    LED_WriteByte(g_led_value);
}

/*
 * 函数名：LED_AllOff
 * 功能：关闭所有 LED
 * 参数：无
 * 返回值：无
 * 备注：常用于初始化和恢复默认状态
 */
void LED_AllOff(void)
{
    LED_WriteByte(0xFF);
}
