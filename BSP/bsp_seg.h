#ifndef __BSP_SEG_H__
#define __BSP_SEG_H__

#include "bsp_board.h"

#define SEG_BLANK 10
#define SEG_DASH  11
#define SEG_U     12
#define SEG_H     13
#define SEG_N     14
#define SEG_P     15
#define SEG_C     16
#define SEG_A     17
#define SEG_O     18
#define SEG_R     19
#define SEG_DP    20
#define SEG_F     21
#define SEG_E     22
#define SEG_L     23

void SEG_ClearBuffer(void);
void SEG_SetCode(u8 pos, u8 seg_code);
void SEG_SetDigit(u8 pos, u8 value);
void SEG_SetDigitDp(u8 pos, u8 value);
void SEG_SetNumber(u32 value, u8 start, u8 len, u8 leading_blank);
void SEG_SetString(const u8 *codes, u8 len);
void SEG_Scan(void);

#endif
