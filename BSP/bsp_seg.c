#include "bsp_seg.h"

static unsigned char code kSegCode[] =
{
    0xC0, 0xF9, 0xA4, 0xB0, 0x99,
    0x92, 0x82, 0xF8, 0x80, 0x90,
    0xFF, 0xBF, 0xC1, 0x89, 0xC8,
    0x8C, 0xC6, 0x88, 0xC0, 0xAF,
    0x7F, 0x8E, 0x86, 0xC7
};

static u8 g_seg_buf[8] =
{
    SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK,
    SEG_BLANK, SEG_BLANK, SEG_BLANK, SEG_BLANK
};

static u8 g_seg_dp_mask[8] =
{
    0, 0, 0, 0, 0, 0, 0, 0
};

void SEG_ClearBuffer(void)
{
    u8 i;
    for(i = 0; i < 8; i++)
    {
        g_seg_buf[i] = SEG_BLANK;
        g_seg_dp_mask[i] = 0;
    }
}

void SEG_SetCode(u8 pos, u8 seg_code)
{
    if(pos >= 8)
    {
        return;
    }
    g_seg_buf[pos] = seg_code;
    g_seg_dp_mask[pos] = 0;
}

void SEG_SetDigit(u8 pos, u8 value)
{
    if(value > 9)
    {
        value = SEG_BLANK;
    }
    SEG_SetCode(pos, value);
}

void SEG_SetDigitDp(u8 pos, u8 value)
{
    if(pos >= 8)
    {
        return;
    }
    if(value > 9)
    {
        value = SEG_BLANK;
    }
    g_seg_buf[pos] = value;
    g_seg_dp_mask[pos] = 1;
}

void SEG_SetNumber(u32 value, u8 start, u8 len, u8 leading_blank)
{
    u8 i;
    u8 temp[8];

    if(len > 8)
    {
        len = 8;
    }

    for(i = 0; i < len; i++)
    {
        temp[len - 1 - i] = (u8)(value % 10);
        value /= 10;
    }

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

    for(i = 0; i < len; i++)
    {
        SEG_SetCode((u8)(start + i), temp[i]);
    }
}

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

void SEG_Scan(void)
{
    static u8 index = 0;

    Board_SelectHC138(HC138_SEG);
    P0 = 0xFF;
    P2 &= 0x1F;

    Board_SelectHC138(HC138_DIGIT);
    P0 = (u8)(0x01 << index);
    P2 &= 0x1F;

    Board_SelectHC138(HC138_SEG);
    P0 = kSegCode[g_seg_buf[index]];
    if(g_seg_dp_mask[index])
    {
        P0 &= 0x7F;
    }
    P2 &= 0x1F;

    index++;
    if(index >= 8)
    {
        index = 0;
    }
}
