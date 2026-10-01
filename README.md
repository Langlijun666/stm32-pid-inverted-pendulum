# STM32 PID 倒立摆

一个用于学习和实践 PID 控制、嵌入式控制以及倒立摆系统的 STM32 项目。

本项目当前已经导入 STM32 Template 基础工程，后续将逐步实现电机控制、编码器反馈、IMU 姿态测量、PID 控制和倒立摆稳定控制。

## 项目目标

- 学习 PID 控制算法
- 学习 STM32 外设和实时控制
- 学习 PWM 电机控制
- 学习编码器数据读取
- 学习 IMU 数据处理
- 学习传感器数据滤波
- 学习电机闭环控制
- 学习倒立摆系统建模
- 学习角度环、速度环等多环控制
- 最终实现倒立摆稳定直立

## 硬件

当前项目尚未确定最终硬件型号，以下内容将在硬件确认后补充：

| 模块 | 当前状态 |
| --- | --- |
| STM32 | 待确认 |
| IMU | 待确认 |
| 编码器 | 待确认 |
| 电机 | 待确认 |
| 电机驱动 | 待确认 |
| 电源 | 待确认 |

当前工程配置指向 STM32F103C8、Cortex-M3，具体开发板和外围硬件仍以实际使用情况为准。

## 软件环境

根据当前工程文件和构建记录：

- STM32F10x 标准外设库
- EIDE 4.1
- VS Code
- EIDE VS Code 扩展
- Keil uVision 工程
- ARM Compiler 5.06 Update 6（AC5）
- ST-Link
- Git
- GitHub

工程中还保留了 GCC 工具链配置，但当前 EIDE 目标使用的是 AC5 工具链。

## 项目结构

当前仓库的主要结构如下：

```text
.
├── README.md
├── LICENSE
├── .gitignore
└── template/
    ├── .cmsis/                  CMSIS 支持文件
    ├── .eide/                   EIDE 工程配置
    ├── .pack/                   STM32F1xx_DFP 2.4.1 软件包
    ├── .vscode/                 VS Code 任务配置
    ├── build/                   构建输出，不提交
    ├── DebugConfig/             调试配置
    ├── Hardware/                硬件模块，当前为空
    ├── Library/                 STM32F10x 标准外设库
    ├── Listings/                Keil 列表输出，不提交
    ├── Objects/                 Keil 编译输出，不提交
    ├── Start/                   启动文件、CMSIS、芯片文件
    ├── System/                  Delay.c / Delay.h
    ├── User/                    main.c、中断模板
    ├── Project.code-workspace   VS Code 工作区
    ├── Project.uvprojx          Keil 工程
    ├── Project.uvoptx           Keil 工程选项
    └── RTE_Components.h         Keil RTE 配置
```

说明：

- `template/User/main.c` 目前是基础空主循环，PID 和倒立摆功能尚未开始。
- `template/Hardware/` 当前为空，但 EIDE 配置中仍保留了 LED、Key、OLED、Serial 等待补充模块的引用。
- `template/.cmsis` 和 `template/.pack` 是当前 EIDE/Keil 工程依赖的一部分。
- `build`、`Objects`、`Listings` 等目录属于本地编译产物，不纳入版本管理。

## 当前进度

- [x] STM32 基础工程初始化
- [ ] 电机驱动
- [ ] PWM
- [ ] 编码器读取
- [ ] IMU 数据读取
- [ ] 角度计算
- [ ] PID 算法
- [ ] 电机闭环控制
- [ ] 倒立摆直立控制
- [ ] 参数调试
- [ ] 系统稳定性优化

## PID 学习路线

```text
基础硬件
    -> PWM
    -> 电机
    -> 编码器
    -> IMU
    -> 数据滤波
    -> PID
    -> 单环控制
    -> 双环控制
    -> 倒立摆稳定
```

建议先完成基础 GPIO、延时、串口调试和 PWM，再逐步增加编码器、IMU 与 PID 控制。

## Git 提交规范

本项目使用简洁的提交前缀：

- `feat:` 新增功能
- `fix:` 修复问题
- `refactor:` 重构代码
- `docs:` 修改文档
- `chore:` 工程配置修改
- `test:` 测试相关

例如：

```text
feat: add PWM motor control
fix: correct encoder direction
docs: update PID learning notes
chore: initialize STM32 PID inverted pendulum project
```

## 注意事项

这是一个持续学习和实验中的项目，硬件配置、控制参数、目录内容和实现方式都可能随着开发进度变化。

项目原创代码默认采用 MIT 许可证。STM32 标准外设库、CMSIS、Keil 设备包以及其他第三方文件仍遵守其各自原始许可证，不能删除或替换其版权声明。

调试电机和倒立摆时请注意供电、机械结构和人身安全。PID 参数应从保守值开始逐步调整。