# 模板里最常见的C语法

这份文档不是普通 C 语言教材，而是专门帮助你看懂这套蓝桥杯模板工程里最常见的写法。

建议配合这两个文件一起看：
- [app.c](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/App/app.c)
- [app.h](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/App/app.h)

## 1. 变量

变量就是“用来存数据的名字”。

模板里最常见的变量例子：

```c
static u16 g_dist_filtered = 999;
static u16 g_freq_filtered = 0;
static bit g_alarm = 0;
```

可以这样理解：
- `g_dist_filtered`：存“平滑后的距离”
- `g_freq_filtered`：存“平滑后的频率”
- `g_alarm`：存“当前有没有报警”

你刷题时最常改的变量，一般就是：
- 当前页面变量
- 参数变量
- 实时采样变量
- 状态标志变量

## 2. 函数

函数就是“把一段功能单独拿出来写”。

模板里最常见的函数例子：

```c
static void App_ShowDataPage(void)
static void App_ShowParamPage(void)
static void App_HandleKey(void)
void App_Loop(void)
```

可以这样理解：
- `App_ShowDataPage`：只负责数据页显示
- `App_ShowParamPage`：只负责参数页显示
- `App_HandleKey`：只负责按键处理
- `App_Loop`：把所有小功能按节拍拼起来

刷题时不要把所有代码都塞进 `main` 或 `App_Loop`，要学会拆成几个小函数。

## 3. if / else

`if` 的作用是“做判断”。

模板里最常见的用途：
- 判断当前页面
- 判断是否报警
- 判断参数模式
- 判断传感器值是否超限

例子：

```c
if(g_ms_ticks < 800u)
{
    SEG_SetCode(1, SEG_DASH);
}
else
{
    SEG_SetDigit(1, (u8)(temp10 / 100));
}
```

意思是：
- 如果上电时间还没到 800ms，就先显示横杠
- 否则再显示真实温度

## 4. switch / case

`switch` 的作用是“一个变量有多种情况时，分别处理”。

模板里最常见的就是页面切换和参数切换。

例子：

```c
switch(g_field)
{
case FIELD_TEMP_LIMIT:
    ...
    break;

case FIELD_DIST_LIMIT:
    ...
    break;
}
```

意思是：
- 如果现在选中的是温度参数，就显示温度参数
- 如果现在选中的是距离参数，就显示距离参数

## 5. 数组

数组就是“同一类数据放一排”。

模板里常见的数组场景：
- 数码管显示缓冲区
- 字符缓存
- 按键映射表

比如串口发送数字时：

```c
char buf[6];
```

意思是：
- 准备 6 个字符的位置
- 用来暂存数字转换出来的字符

## 6. 结构体

结构体就是“把一组有关联的数据打包到一起”。

模板里两个最重要的结构体：

```c
typedef struct
{
    float temp;
    u8 adc;
    u16 dist;
    u16 freq;
    rtc_time_t rtc;
} app_data_t;
```

```c
typedef struct
{
    int temp_limit_x10;
    u16 dist_limit;
    u8 adc_limit;
} app_param_t;
```

可以这样理解：
- `app_data_t`：一包“实时数据”
- `app_param_t`：一包“参数”

所以你会看到：

```c
static app_data_t g_data = {0};
static app_param_t g_param = {300, 30, 200};
```

意思就是：
- `g_data` 用来装当前测到的数据
- `g_param` 用来装当前参数

## 7. typedef

`typedef` 的作用是“给一种类型起个更好记的名字”。

比如：

```c
typedef struct
{
    ...
} app_param_t;
```

意思是：
- 以后这整个结构体，不用每次都写一大串
- 直接叫 `app_param_t`

模板里你会频繁看到：
- `app_page_t`
- `app_field_t`
- `app_data_t`
- `app_param_t`

## 8. enum

`enum` 就是“枚举”，适合表示“有限个状态”。

例子：

```c
typedef enum
{
    PAGE_DATA = 0,
    PAGE_PARAM,
    PAGE_TIME,
    PAGE_FREQ,
    PAGE_RECORD
} app_page_t;
```

意思是：
- 页面只有这 5 种
- 以后 `g_page` 只在这 5 种页面之间切换

你可以把它理解成“给页面编号，但是编号有名字”。

## 9. static

`static` 在模板里非常常见。

最简单的理解：
- 写在全局变量前：表示这个变量只在当前 `.c` 文件里用
- 写在函数前：表示这个函数只在当前 `.c` 文件里用

比如：

```c
static void App_ShowDataPage(void)
```

意思是：
- 这个函数是 `app.c` 自己内部用的
- 不打算给别的文件直接调用

模板里大量用 `static`，是为了让模块边界更清楚。

## 10. u8 / u16 / bit

这些不是“神秘语法”，本质上就是更适合单片机的类型名。

常见理解方式：
- `u8`：无符号 8 位整数
- `u16`：无符号 16 位整数
- `bit`：只有 0 或 1 的位变量

在模板里：
- `u8` 常用来存按键值、ADC 值、小范围参数
- `u16` 常用来存距离、频率、计数值
- `bit` 常用来存开关状态，比如报警标志

## 11. 函数声明和函数定义

在模板里你会发现：
- `.h` 里写函数声明
- `.c` 里写真正实现

比如 [app.h](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/App/app.h) 里：

```c
void App_Init(void);
void App_Loop(void);
```

而在 [app.c](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/App/app.c) 里会有真正函数体。

最重要的规则是：
- `.h` 里的声明
- `.c` 里的定义
- 调用时传的参数

这三处必须一致。

## 12. 你刷题时最该先掌握哪些语法

如果你现在基础弱，优先顺序建议是：

1. 变量
2. 函数
3. `if / else`
4. `switch`
5. 结构体
6. `enum`
7. `static`
8. `u8 / u16 / bit`

把这些看懂之后，你再回去看模板代码，会轻松很多。

