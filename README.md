# 武汉地铁乘客导视系统

此项目为华中科技大学人工智能与自动化学院嵌入式 C 语言课程设计项目。

基于 GD32H759IM 微控制器的嵌入式地铁乘客导视系统，提供武汉地铁线路图的交互式浏览、站点信息查询、最短路径规划、收藏与到站提醒等功能。

## 功能特性

- **交互式地铁地图** — 支持拖拽平移与缩放，可按线路筛选显示，点击站点查看详细信息
- **站点搜索** — 支持中文站名与拼音首字母搜索，自动补全，保留搜索历史
- **站点详情** — 显示首末班车时间、最近两班列车的实时倒计时与到站预测，支持查看全日发车时刻表
- **路径规划** — 基于 0-1 BFS 算法的最少站数路径规划，兼顾最少换乘；结果在地图上高亮显示，列出每段乘车信息（线路、方向、站数、耗时）
- **收藏功能** — 支持收藏站点与常用路线，数据持久化到 SD 卡
- **到站提醒** — 可提前设置到站提醒，到时弹窗提示

## 硬件平台


| 组件       | 型号                                             |
| -------- | ---------------------------------------------- |
| MCU      | GigaDevice GD32H759IM (ARM Cortex-M7, 400 MHz) |
| 显示屏      | 正点原子 7 寸 RGBLCD, 1024×600, RGB565              |
| 触摸       | Goodix GT9xx 电容触摸                              |
| 存储       | SD 卡 (SDIO 4-bit)                              |
| 外部 SDRAM | 32 MB (用于帧缓冲与画布缓冲区)                            |


## 软件架构

```
┌─────────────────────────────────────────────────────┐
│                    应用层 (main/)                     │
│  main.c → gui.c → displaycanvas/maps  (地图)        │
│                 → gui_display1        (线路规划)      │
│                 → gui_display2        (到站提醒)      │
│                 → route_planner       (寻路引擎)      │
│                 → station_info        (站点详情)      │
│                 → favorites / remind  (收藏/提醒)    │
├─────────────────────────────────────────────────────┤
│  LVGL v8.2.0 (GUI 框架)                             │
├─────────────────────────────────────────────────────┤
│  BSP 驱动层 (Drivers/)                               │
│  sys.c · LCD_tli · exmc_sdram · sdcard · touch_gtxx │
├─────────────────────────────────────────────────────┤
│  GD32H7xx 标准外设库 · CMSIS · FatFs R0.14          │
└─────────────────────────────────────────────────────┘
```

- **单线程事件循环**：主循环每 40 ms 调用 `lv_timer_handler()`，无 RTOS 内核
- **内存管理**：复用 FreeRTOS heap\_4 分配器，基于外部 SDRAM 管理大块画布内存
- **持久化**：收藏站点、收藏路线、搜索历史均以文本文件形式存储在 SD 卡上

## 路径规划算法

采用 **0-1 BFS**（双端队列广搜）：

- 状态定义为 `(站点, 线路)`
- 同线路移动一站，代价为 1
- 换乘（同站不同线路），代价为 0
- 使用双端队列保证节点按非递减代价出队，保证首次找到目标即为最优解
- 优化目标：最少乘车站数，换乘次数作为次要优化目标

## 项目结构

```
metro_passenger_guidance/
├── main/
│   ├── include/          # 头文件（数据结构、API 声明）
│   │   ├── metro_line.h     # Station, MetroLine, Timetable 核心数据结构
│   │   ├── route_planner.h  # Route, RouteStep, 路径规划 API
│   │   ├── canvas.h         # 地图画布全局状态
│   │   ├── gui.h            # GUI 布局宏与工具函数
│   │   ├── favorites.h      # 收藏 API
│   │   ├── remind.h         # 提醒数据结构与 API
│   │   └── timestruct.h     # TimeStruct 时间运算 API
│   └── source/           # 源文件
│       ├── main.c           # 入口，主循环
│       ├── gui.c            # 公共界面（标签栏、搜索框、键盘）
│       ├── displaycanvas.c  # 地图界面（画布、弹窗、缩放平移）
│       ├── maps.c           # 地图绘制（线路、站点、路线高亮）
│       ├── metro_line_info.c # 武汉地铁数据（10 条线，约 280 站）
│       ├── route_planner.c  # 0-1 BFS 寻路引擎
│       ├── station_info.c   # 站点详情界面（倒计时、时刻表）
│       ├── gui_display1.c   # 线路规划界面
│       ├── gui_display2.c   # 到站提醒列表界面
│       ├── favorites.c      # 站点收藏（SD 卡持久化）
│       ├── favorite_route.c # 路线收藏（SD 卡持久化）
│       ├── reminder.c       # 提醒逻辑
│       ├── top_search_record.c # 搜索历史
│       ├── msgbox.c         # 通用弹窗/Toast
│       ├── time_struct.c    # 时间运算
│       ├── rtc_time.c       # RTC 时钟读写
│       └── custom_font/     # 嵌入式中文点阵字体
├── Drivers/              # BSP 驱动（LCD、SDRAM、SD卡、触摸）
├── GD32H7xx/             # CMSIS 设备层
├── GD32H7xx_standard_peripheral/  # GD32 标准外设库
├── FATFS/                # FatFs R0.14 文件系统
├── lvgl/                 # LVGL v8.2.0 图形库 + 移植层
├── imagesource/          # 图标素材与字体字符集
└── metro_passenger_guidance.uvprojx  # Keil MDK 工程文件
```

## 构建与运行

### 环境要求

- **IDE**: Keil MDK-ARM v5
- **编译器**: ARMCC V5.06
- **硬件**: GD32H759IM 开发板 + ATK 7 寸 RGBLCD 模块（含 SD 卡、GT9xx 触摸）

### 编译与烧录

1. 用 Keil µVision 打开 `metro_passenger_guidance.uvprojx`
2. 确保 SD 卡已插入，根目录包含图标 `.bin` 文件（`imagesource/` 目录下有素材）
3. 编译并下载到目标板

### 首次运行

首次运行时 SD 卡上不存在持久化文件（`favorites.txt`、`favorite_routes.txt`、`search_record.txt`），程序会自动处理，无需手动创建。

## 数据说明

武汉地铁线路数据（站点名称、坐标、站间行驶时间、首末班时刻）均硬编码在 `main/source/metro_line_info.c` 中，包含以下线路：

- 1 号线、2 号线、3 号线、4 号线、5 号线
- 6 号线、7 号线、8 号线、11 号线、19 号线

