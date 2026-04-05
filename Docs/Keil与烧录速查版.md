# Keil 与烧录速查版

这份就是最短流程版，按顺序做就行。

## 1. 打开工程

1. 找到工程里的 `.uvproj`
2. 双击打开

常用工程：

- [LanQiaoTemplate.uvproj](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/LanQiaoTemplate.uvproj)
- [FrequencyTemplate.uvproj](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机频率题专用模板/FrequencyTemplate.uvproj)

## 2. 改代码

通常先改：

- [App/app.c](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/App/app.c)

## 3. 新建 `.c/.h`

1. `File -> New`
2. `File -> Save As`
3. 保存成 `xxx.c` 或 `xxx.h`

## 4. 把文件加入工程

1. 左侧工程树右键某个分组
2. `Add Existing Files to Group...`
3. 选中刚保存的文件

注意：

- `.c` 不加进工程，编译不会生效
- 不是目录里所有 `.c` 都要加进工程
- 带 `chinese_note` 的文件不要加入工程

例如：

- [BSP/bsp_seg_chinese_note.c](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/BSP/bsp_seg_chinese_note.c)
- [BSP/bsp_seg_chinese_note.h](D:/桌面/2023蓝桥杯省国赛训练过程/蓝桥杯单片机通用模板工程/BSP/bsp_seg_chinese_note.h)

这两个只是讲解副本，不参与编译。

## 5. 编译

直接点：

- `Project -> Rebuild all target files`

## 6. 判断有没有编译成功

看下方输出窗口。

成功一般是：

```text
0 Error(s), 0 Warning(s)
```

## 7. 没有 `.hex` 怎么办

1. `Project -> Options for Target`
2. 打开 `Output`
3. 勾选 `Create HEX File`
4. 重新编译

## 8. 用 STC-ISP 烧录

1. 打开 `STC-ISP`
2. 选芯片型号
3. 选串口号
4. 选择 `.hex`
5. 点下载
6. 给开发板重新上电

## 9. 最常见问题

### 编译报函数找不到

一般是：

- `.c` 没加进工程
- 少了 `#include`

### 找不到头文件

一般是：

- 路径写错
- 文件名写错

### 烧录失败

一般是：

- 芯片型号错
- 串口号错
- 没重新上电

## 10. 最短记忆版

1. 打开 `.uvproj`
2. 改 `app.c`
3. `Rebuild`
4. 看是不是 `0 Error`
5. 找 `.hex`
6. 打开 `STC-ISP`
7. 选芯片、串口、`hex`
8. 点下载
9. 板子重新上电
