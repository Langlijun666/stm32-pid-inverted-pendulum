# STM32 PID 倒立摆

一个用于学习和实践 PID 控制、嵌入式控制以及倒立摆系统的 STM32 项目。

目前已完成基础硬件驱动、编码器反馈、PID 模块以及电机双环闭环控制。当前双环结构为位置外环和速度内环，用于电机运动控制实验；倒立摆所需的 IMU 角度反馈和直立控制仍待实现。

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

工程当前配置为 STM32F103C8、Cortex-M3。已使用的主要外设和接口如下：

| 模块 | 当前状态 | 主要接口 |
| --- | --- | --- |
| STM32 | STM32F103C8 | Cortex-M3 |
| 编码器 | 已接入 | TIM3、PA6、PA7 |
| 直流电机 | 已接入 | TIM2 CH1 PWM、PA0 |
| 电机方向控制 | 已接入 | PB12、PB13 |
| 电位器 | 已接入 | ADC2、PA2 至 PA5 |
| 按键 | 已接入 | PA11、PA12、PB10、PB11 |
| OLED | 已接入 | I2C 软件模拟 |
| 串口 | 已接入 | USART1、PA9、PA10 |
| IMU | 待确认 | 待补充 |
| 电源 | 待确认 | 待补充 |

具体开发板、编码器型号、电机、驱动板和供电方案仍以实际硬件为准。

## 软件环境

根据当前工程文件和构建配置：

- STM32F10x 标准外设库
- EIDE 4.1
- VS Code
- EIDE VS Code 扩展
- Keil uVision 工程
- ARM Compiler 5.06 Update 6（AC5）
- ST-Link
- Git
- GitHub

工程中还保留了 GCC 工具链配置，但当前 EIDE 目标使用 AC5。

## 项目结构

```text
.
├── README.md
├── LICENSE
├── .gitignore
└── template/
    ├── .cmsis/                  CMSIS 支持文件
    ├── .eide/                   EIDE 工程配置
    ├── .pack/                   STM32F1xx_DFP 2.4.1 软件包
    ├── .vscode/                 VS Code 配置
    ├── build/                   EIDE 构建输出，不提交
    ├── DebugConfig/             调试配置，不提交
    ├── Hardware/
    │   ├── Encoder.c/.h         编码器接口
    │   ├── Key.c/.h             非阻塞按键
    │   ├── LED.c/.h             LED 控制
    │   ├── Motor.c/.h           电机方向和 PWM 控制
    │   ├── OLED.c/.h            OLED 显示
    │   ├── OLED_Data.c/.h       字模和图像数据
    │   ├── PWM.c/.h             TIM2 PWM 输出
    │   ├── RP.c/.h              电位器 ADC 采样
    │   └── Serial.c/.h          USART1 串口收发
    ├── Library/                 STM32F10x 标准外设库
    ├── Listings/                Keil 列表输出，不提交
    ├── Objects/                 Keil 编译输出，不提交
    ├── Start/                   启动文件、CMSIS、芯片文件
    ├── System/
    │   ├── Delay.c/.h           延时函数
    │   └── Timer.c/.h           TIM1 周期中断
    ├── User/
    │   ├── main.c               主程序和双环控制调度
    │   ├── PID.c/.h             位置式 PID 模块
    │   └── stm32f10x_it.c/.h    中断相关文件
    ├── Project.code-workspace   VS Code 工作区
    ├── Project.uvprojx          Keil 工程
    ├── Project.uvoptx           Keil 工程选项
    └── RTE_Components.h         Keil RTE 配置
```

## 当前实现

- TIM1 提供周期中断和按键扫描时基。
- TIM3 工作在编码器接口模式，读取电机速度并累计位置。
- TIM2 CH1 输出电机 PWM，GPIO 控制电机方向。
- `PID_Update()` 实现位置式 PID，包含输出上下限。
- `Outer` 为位置外环，根据目标位置计算速度目标。
- `Inner` 为速度内环，根据编码器速度计算电机 PWM。
- OLED 和 USART1 用于显示、记录目标值、实际值和控制输出。
- 电位器用于设置位置目标，并预留 PID 参数调节接口。

## 当前进度

- [x] STM32 基础工程初始化
- [x] GPIO、延时和周期中断
- [x] LED、按键、OLED、串口驱动
- [x] PWM 电机驱动
- [x] 编码器读取
- [x] PID 算法
- [x] 电机位置、速度双环控制
- [ ] IMU 数据读取
- [ ] 姿态角度计算
- [ ] 传感器数据滤波
- [ ] 倒立摆角度环
- [ ] 倒立摆直立控制
- [ ] 抗积分饱和与保护逻辑
- [ ] 参数调试
- [ ] 系统稳定性优化

## PID 学习路线

```text
基础硬件
    -> PWM
    -> 电机
    -> 编码器
    -> PID
    -> 单环控制
    -> 双环控制
    -> IMU
    -> 数据滤波
    -> 角度环
    -> 倒立摆稳定
```

当前已完成电机位置、速度双环控制。下一步需要接入 IMU，并将控制目标从电机位置切换为摆杆角度。

## 当前限制

- 当前双环控制用于电机运动实验，尚未实现倒立摆直立。
- PID 参数仍为初始实验值，需要结合实际机械结构和电源调整。
- `location` 使用 16 位有符号整数累计，长时间运行需要增加溢出保护。
- PID 输出已经限幅，但积分项仍需增加抗饱和处理。
- 电机启停、异常恢复和参数越界保护仍待完善。
- 编码器方向和位置单位需要根据实际安装方向、减速比和编码器线数标定。

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
feat: add cascaded motor position and speed PID control
fix: correct encoder direction
docs: update project status for PID motor control
chore: add PID module to EIDE project
```

## 注意事项

这是一个持续学习和实验中的项目，硬件配置、控制参数、目录内容和实现方式都可能随着开发进度变化。

项目原创代码默认采用 MIT 许可证。STM32 标准外设库、CMSIS、Keil 设备包以及其他第三方文件仍遵守其各自原始许可证，不能删除或替换其版权声明。

调试电机和倒立摆时请注意供电、机械结构和人身安全。PID 参数应从保守值开始逐步调整，并确保急停和电机限幅可靠。
