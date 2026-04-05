# Keil 与烧录入门

这份文档给刚开始接触蓝桥杯单片机的同学用，目标只有一个：

- 会打开工程
- 会加 `.c` / `.h` 文件
- 会编译
- 会找到 `hex`
- 会用 `STC-ISP` 烧录

## 一、先认识常见文件

在一个 Keil 工程目录里，常见文件有这些：

- `.uvproj`
作用：Keil 工程文件，双击它通常就能打开工程

- `.uvopt`
作用：Keil 的个人配置文件，不是核心，没它一般也能打开

- `.c`
作用：写函数实现，也就是主要代码

- `.h`
作用：写函数声明、宏定义、结构体定义

- `.hex`
作用：编译后生成的烧录文件，最后烧进单片机的就是它

## 二、怎么打开工程

最简单的方法：

1. 进入工程文件夹
2. 双击 `.uvproj`

如果双击没反应：

1. 先打开 `Keil uVision`
2. 点击 `Project`
3. 点击 `Open Project...`
4. 选中对应的 `.uvproj`

例如这里可以打开：

- [LanQiaoTemplate.uvproj](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/LanQiaoTemplate.uvproj)
- [FrequencyTemplate.uvproj](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机频率题专用模板/FrequencyTemplate.uvproj)

## 三、怎么看工程窗口

Keil 左边一般会有工程树。

常见内容：

- `Source Group 1`
这里通常放 `.c` 文件

- `Header Files`
这里通常放 `.h` 文件

- 有些工程会自己建组，比如 `App`、`BSP`、`Devices`

重点记住一句话：

文件在文件夹里存在，不代表 Keil 已经把它加入工程。

## 四、怎么在 Keil 里新建文件和分组

### 1. 新建分组

1. 右键工程名或某个分组
2. 选择 `Add Group...`
3. 输入名字，例如 `Examples`

### 2. 新建 `.c`

1. `File -> New`
2. 写代码
3. `File -> Save As`
4. 保存成 `xxx.c`

### 3. 新建 `.h`

方法一样，只是保存成 `xxx.h`

## 五、怎么把文件加入工程

### 1. 加 `.c`

1. 右键左侧某个分组
2. 选择 `Add Existing Files to Group...`
3. 选中对应 `.c`
4. 点击确定

### 2. 加 `.h`

`.h` 也可以加进去，主要是为了管理方便。

注意：

- `.c` 不加进工程，就不会参与编译
- `.h` 不加一般也能编译，但建议加上方便看

## 六、非常重要的一条

不是目录里所有 `.c` / `.h` 都要加入 `Keil` 工程。

例如当前模板里有这种文件：

- [BSP/bsp_seg_chinese_note.h](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/BSP/bsp_seg_chinese_note.h)
- [BSP/bsp_seg_chinese_note.c](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/BSP/bsp_seg_chinese_note.c)

它们只是中文讲解副本，只用于阅读，不参与编译。

不要把这种带 `chinese_note` 的文件加入工程。

真正参与编译的是：

- [BSP/bsp_seg.h](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/BSP/bsp_seg.h)
- [BSP/bsp_seg.c](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/BSP/bsp_seg.c)

## 七、怎么写 `#include`

如果 `.c` 和 `.h` 在同一层目录，通常这样写：

```c
#include "app.h"
```

如果有分层目录，也可能写成：

```c
#include "..\\App\\app.h"
```

报 `cannot open source input file` 时，优先检查：

- 路径对不对
- 文件名对不对

## 八、怎么编译工程

最推荐直接用：

- `Project -> Rebuild all target files`

## 九、怎么判断编译成功

看下方输出窗口。

成功一般会看到：

```text
0 Error(s), 0 Warning(s)
```

如果有报错，先看最前面的第一条。

## 十、怎么生成 hex

如果编译成功但没有 `.hex`：

1. `Project -> Options for Target`
2. 打开 `Output`
3. 勾选 `Create HEX File`
4. 重新编译

## 十一、怎么用 STC-ISP 烧录

1. 打开 `STC-ISP`
2. 选芯片型号
3. 选串口号
4. 打开 `.hex`
5. 点击下载
6. 给开发板重新上电

很多 STC 板子都是：

1. 先点下载
2. 再上电

## 十二、最常见错误

### 1. 函数找不到

一般是：

- `.c` 没加进工程
- 少了 `#include`

### 2. 头文件找不到

一般是：

- 路径写错
- 文件名写错

### 3. 没有 hex

一般是：

- 没勾选 `Create HEX File`

### 4. 烧录失败

一般是：

- 串口号错
- 芯片型号错
- 没重新上电

## 十三、推荐流程

1. 打开 `.uvproj`
2. 确认当前改的是正确的 `.c`
3. 确认新加的 `.c` 已经加入工程
4. `Rebuild`
5. 看是不是 `0 Error`
6. 确认生成 `.hex`
7. 打开 `STC-ISP`
8. 选芯片、串口、`hex`
9. 点击下载
10. 板子重新上电
