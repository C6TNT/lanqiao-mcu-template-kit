/* 数码管模块中文注释版头文件
   说明：
   1. 这个文件只用于阅读和学习
   2. 不要把它加入 Keil 工程
   3. 真正参与编译的是 bsp_seg.h */

#ifndef __BSP_SEG_CHINESE_NOTE_H__
#define __BSP_SEG_CHINESE_NOTE_H__

#include "bsp_board.h"

/* 特殊段码索引定义 */
#define SEG_BLANK 10  /* 空白 */
#define SEG_DASH  11  /* 横杠 */
#define SEG_U     12  /* 字母 U */
#define SEG_H     13  /* 字母 H */
#define SEG_N     14  /* 字母 N */
#define SEG_P     15  /* 字母 P */
#define SEG_C     16  /* 字母 C */
#define SEG_A     17  /* 字母 A */
#define SEG_O     18  /* 字母 O */
#define SEG_R     19  /* 字母 R */
#define SEG_DP    20  /* 小数点 */

/* 清空数码管显示缓冲区，调用后 8 位都显示为空白 */
void SEG_ClearBuffer(void);

/* 向指定位置写入一个“段码索引”
   pos：第几位数码管，范围 0~7
   seg_code：段码表中的索引值 */
void SEG_SetCode(u8 pos, u8 seg_code);

/* 向指定位置写一个数字
   value：只能是 0~9
   如果超出范围，会自动显示为空白 */
void SEG_SetDigit(u8 pos, u8 value);

/* 显示一个整数
   value：要显示的数
   start：从哪一位开始放
   len：总共占几位
   leading_blank：是否隐藏前导 0，1 表示隐藏 */
void SEG_SetNumber(u32 value, u8 start, u8 len, u8 leading_blank);

/* 把一组段码索引按顺序写进缓冲区 */
void SEG_SetString(const u8 *codes, u8 len);

/* 动态扫描函数
   一般放在定时中断里高频调用 */
void SEG_Scan(void);

#endif
