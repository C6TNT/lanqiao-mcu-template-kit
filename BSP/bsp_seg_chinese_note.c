/* 数码管模块中文注释版源文件
   说明：
   1. 这个文件只用于阅读和学习
   2. 不要把它加入 Keil 工程
   3. 真正参与编译的是 bsp_seg.c */

#include "bsp_seg_chinese_note.h"

/* 段码表
   前 0~9 对应数字 0~9
   后面的索引用来显示空白、横杠、字母、小数点等 */
static unsigned char code kSegCode[] =
{
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,
    0x92, 0x82, 0xF8, 0x80, 0x90,
    0xFF, 0xBF, 0xC1, 0x89, 0xC8,
    0x8C, 0xC6, 0x88, 0xC0, 0xAF,
    0x7F
};

/* 数码管显示缓冲区
   一共 8 个元素，对应 8 位数码管 */
static u8 g_seg_buf[8] =
{
    SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK,
    SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK
};

/* 清空缓冲区 */
void SEG_ClearBuffer(void)
{
    u8 i;
    for(i = 0; i < 8; i++)
    {
        g_seg_buf[i] = SEG_BLANK;
    }
}

/* 往某一位写入段码索引 */
void SEG_SetCode(u8 pos, u8 seg_code)
{
    if(pos >= 8)
    {
        return;
    }
    g_seg_buf[pos] = seg_code;
}

/* 往某一位写入数字 */
void SEG_SetDigit(u8 pos, u8 value)
{
    if(value > 9)
    {
        value = SEG_BLANK;
    }
    SEG_SetCode(pos, value);
}

/* 显示一个整数
   这个函数会先把整数拆位，再放进缓冲区 */
void SEG_SetNumber(u32 value, u8 start, u8 len, u8 leading_blank)
{
    u8 i;
    u8 temp[8];

    if(len > 8)
    {
        len = 8;
    }

    /* 从低位到高位拆分数字 */
    for(i = 0; i < len; i++)
    {
        temp[len - 1 - i] = (u8)(value % 10);
        value /= 10;
    }

    /* 如果需要隐藏前导 0，就把前面的 0 改成空白 */
    if(leading_blank)
    {
        for(i = 0; i < (u8)(len - 1); i++)
        {
            if(temp[i] == 0)
            {
                temp[i] = SEG_BLANK;
            }
            else
            {
                break;
            }
        }
    }

    /* 把结果写回显示缓冲区 */
    for(i = 0; i < len; i++)
    {
        SEG_SetCode((u8)(start + i), temp[i]);
    }
}

/* 按顺序写入一组段码 */
void SEG_SetString(const u8 *codes, u8 len)
{
    u8 i;

    if(len > 8)
    {
        len = 8;
    }

    for(i = 0; i < len; i++)
    {
        g_seg_buf[i] = codes[i];
    }
}

/* 动态扫描
   每次只点亮一位，快速循环后就会看起来像 8 位同时亮着 */
void SEG_Scan(void)
{
    static u8 index = 0;

    /* 先关段码，防止重影 */
    Board_SelectHC138(HC138_SEG);
    P0 = 0xFF;
    P2 &= 0x1F;

    /* 选择当前位 */
    Board_SelectHC138(HC138_DIGIT);
    P0 = (u8)(0x01 << index);
    P2 &= 0x1F;

    /* 输出当前位对应的段码 */
    Board_SelectHC138(HC138_SEG);
    P0 = kSegCode[g_seg_buf[index]];
    P2 &= 0x1F;

    /* 切换到下一位 */
    index++;
    if(index >= 8)
    {
        index = 0;
    }
}
