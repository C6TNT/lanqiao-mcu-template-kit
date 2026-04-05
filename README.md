# lanqiao-mcu-template-kit

蓝桥杯单片机竞赛模板工程与训练资料包。

这套仓库面向蓝桥杯单片机省赛训练场景整理，目标是让零基础同学也能尽快完成：

- 跑通 `Keil` 工程
- 理解模板分层
- 学会按题面改业务层
- 通过刷历届真题提升到省一和国赛备赛水平

当前维护版本：

- [V1.0](./VERSION)

## Project Status

当前仓库定位为：

- 稳定可用的竞赛模板工程
- 持续迭代的训练产品
- 适合社团交接、新生入门、真题训练和冲刺复盘

其中：

- 主模板工程优先保证稳定、可编译、可迁移
- 文档层优先帮助零基础快速上手
- `Examples` 主要用于学习、复盘和模板迁移

当前版本：

- [VERSION](./VERSION)
- [CHANGELOG](./CHANGELOG.md)

## Repository Structure

```text
lanqiao-mcu-template-kit/
├─ App/         业务层模板
├─ BSP/         板级驱动
├─ Core/        核心头文件
├─ Devices/     设备封装层
├─ Drivers/     底层时序驱动
├─ Docs/        使用说明与训练资料
├─ Examples/    历届省赛示例业务层
├─ main.c
├─ LanQiaoTemplate.uvproj
└─ LanQiaoTemplate.uvopt
```

## What This Repository Includes

### 1. Stable template project

主工程采用分层结构，已经围绕蓝桥杯常见场景做了多轮整理和兼容处理，适合作为统一起手模板。

已覆盖的常见模块包括：

- 数码管动态显示
- LED / 蜂鸣器 / 继电器控制
- 独立按键 / 矩阵键盘
- 串口
- `DS18B20`
- `DS1302`
- `PCF8591`
- 超声波
- 频率测量

### 2. Learning documents

仓库内保留了完整的训练资料，方便从零开始循序渐进地学：

- [使用说明](./Docs/使用说明.md)
- [新手最短阅读入口](./Docs/新手最短阅读入口.md)
- [模板源码导读](./Docs/模板源码导读.md)
- [赛题要求与代码对应表](./Docs/赛题要求与代码对应表.md)
- [Keil与烧录入门](./Docs/Keil与烧录入门.md)
- [Keil与烧录速查版](./Docs/Keil与烧录速查版.md)
- [零基础省一学习路线](./Docs/零基础省一学习路线.md)
- [7天冲刺版学习路线](./Docs/7天冲刺版学习路线.md)
- [比赛当天做题流程清单](./Docs/比赛当天做题流程清单.md)

### 3. Provincial examples

`Examples` 中放的是历届省赛的模板版示例代码与配套说明文档，便于理解：

- 如何从基础模板出发
- 如何根据题面加页面、变量、参数和输出逻辑
- 如何把历届题目逐渐迁移到统一模板上

入口在：

- [Examples/README.md](./Examples/README.md)

## Recommended Reading Order

如果你是第一次接触这套工程，推荐按下面顺序看：

1. [Docs/使用说明.md](./Docs/使用说明.md)
2. [Docs/新手最短阅读入口.md](./Docs/新手最短阅读入口.md)
3. [Docs/模板源码导读.md](./Docs/模板源码导读.md)
4. [Docs/赛题要求与代码对应表.md](./Docs/赛题要求与代码对应表.md)
5. [Docs/Keil与烧录入门.md](./Docs/Keil与烧录入门.md)
6. [App/app.c](./App/app.c)
7. [Examples/README.md](./Examples/README.md)
8. 对应届数的 `.md` 说明
9. 对应届数的 `.c` 示例

## Important Note About Examples

`Examples` 里的示例代码不要直接理解成“全部已经完整验收的标准答案”。

更准确地说，它们是：

- 题目复盘版
- 模板迁移版
- 学习演示版

推荐使用方式：

1. 先阅读对应的 `.md`
2. 再看对应的 `.c`
3. 自己替换到 `App/app.c`
4. 重新编译
5. 上板验证
6. 对照题面继续修正

尤其是以下届数，更建议在学习过程中同步验证：

- 第九届
- 第十届
- 第十一届

实际上，目前所有示例代码都建议边学习边编译、边上板、边核对题面。如果发现问题，可以继续反馈并迭代修改。

## Target Users

这套仓库主要适合：

- 刚接触 `Keil / STC-ISP / 51 单片机` 的同学
- 想通过模板快速入门蓝桥杯单片机的同学
- 想用“背模板 + 刷真题”方式冲省一的同学
- 想在社团里做交接和复用的同学

## Development Environment

- MCU: `IAP15F2K61S2`
- IDE: `Keil uVision`
- ISP tool: `STC-ISP`
- Board: 蓝桥杯单片机竞赛实训平台

## Design Principles

这套工程在设计时更强调下面几件事：

- 先稳定，再追求花哨
- 先能改题，再追求理解更深原理
- 先把模块拆清楚，再做业务层组合
- 让零基础同学尽快具备刷题能力

## Feedback

如果你在使用过程中遇到以下问题：

- 编译不过
- 数码管显示异常
- 按键逻辑异常
- 传感器数据不稳定
- 示例代码与题面不一致

欢迎继续反馈。这个仓库本身就是按“边使用、边验证、边修正”的思路持续迭代的。

反馈前建议先看：

- [FEEDBACK.md](./FEEDBACK.md)
