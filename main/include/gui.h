#ifndef GUI
#define GUI

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "route_planner.h"
#include <stdio.h>

// --- 颜色定义 ---
#define COLOR_DARK_BLUE     0x194888
#define COLOR_MID_BLUE      0x3F6EAD
#define COLOR_LIGHT_BLUE    0xD2DBE7
#define COLOR_BG_BLUE       0xEFF5FD
#define COLOR_LIGHT_GRAY    0xF5F5F5
#define COLOR_MID_GRAY      0x9D9D9D
#define COLOR_BLACK         0x000000

// --- 布局宏定义 ---
#define BUTTON_RADIUS       4
#define CANVAS_X            60
#define CANVAS_Y            55
#define CANVAS_W            964
#define CANVAS_H            545
#define BUF_SIZE            LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_W,CANVAS_H)
#define BUTTON_LEN          35
#define BUTTON_X            954 - CANVAS_X
#define BUTTON_Y            476 - CANVAS_Y
#define REC_X               896 - CANVAS_X
#define REC_Y               74  - CANVAS_Y
#define REC_W               102
#define REC_H               143
#define STEP                5 //缩放的步长

#define POP_WINDOW_W        290
#define POP_WINDOW_H_TRANS  375
#define POP_WINDOW_H_NORM   246
#define LINE_BTN_W          267
#define LINE_BTN_H          121
#define BOTTOM_BTN_W        128
#define BOTTOM_BTN_H        38
#define TOP_BTN_W           70
#define TOP_BTN_H           30
#define TOP_BAR_H           46
#define BOTTOM_BTN_Y_NORM   190
#define BOTTOM_BTN_Y_TRANS  322
#define BOTTOM_BTN_X        149

#define SEARCH_LIST_LEN     10

//下面是有关车站详细信息显示界面的宏定义

#define STATION_INFO_X      493 - CANVAS_X
#define STATION_INFO_Y      55  - CANVAS_Y
#define STATION_INFO_W      531
#define STATION_INFO_H      CANVAS_H

#define TIME_LABEL_X        24
#define TIME_LABEL_Y        270
#define TIME_LABEL_W        480
#define TIME_LABEL_H        90
#define TIMETABLE_LABEL_H   230

#define STATION_DIST        45
#define STATION_Y           55
#define STATION_RADIUS_NORM 6
#define STATION_RADIUS_NOW  7
#define STA_LINE_W          6


// --- 外部变量声明 ---
extern const lv_font_t heiti_14;
extern const lv_font_t heiti_16;
extern const lv_font_t heiti_20;
extern const lv_font_t heiti_24;
extern const lv_font_t heiti_32;
extern lv_obj_t* display0;
extern lv_obj_t* display1;
extern lv_obj_t* display2;
extern lv_obj_t* canvas;
extern lv_obj_t* kb,*ta;
extern lv_obj_t* timetable1,*timetable2,*timetable3;
extern lv_obj_t* transparent;

// --- 路线结果显示相关 ---
extern RouteResult last_route_result;
extern char last_route_desc[1024];





// ==========================================
// 工具函数声明
// ==========================================
/**
 * @brief 通用 Label 创建工具
 * @return 返回创建好的 lv_obj_t* 指针
 * 只用于设置坐标、尺寸、文本及文本颜色
 */
void create_simple_label(lv_obj_t** lb, lv_obj_t* parent, int x, int y, int w, int h, 
                              const char* text, const lv_font_t* font);

/**
 * @brief 通用按钮创建工具 (带圆角和背景色)
 * @return 返回创建好的 lv_obj_t* 指针
 * 默认圆角为4
 */
void create_simple_btn(lv_obj_t** btn, lv_obj_t* parent, int x, int y, int w, int h, lv_color_t bg_color);

/**
 * @brief 键盘的弹出与隐去
 * 将kb与隐藏的transparent标签绑定
 * 实现弹出键盘同时生成transparent标签，隐去键盘同时删除transparent标签
 * 代替lv_obj_clear_flag，lv_obj_add_flag
 * 键盘的hide和event回调无需创建直接调用kb_show
 * 键盘的show需自创，调用kb_show可同时创建键盘的hide和event回调
 */
void kb_show(lv_obj_t* kb,lv_obj_t* ta,lv_color_t bg_color);
void kb_hide(lv_obj_t* kb);
void transparent_init(lv_obj_t* display,lv_color_t bg_color);  //包含键盘隐藏回调函数创建

/**
 * @brief 路线规划确定按钮回调
 * @param e 事件结构体
 */
void btn4_cb(lv_event_t *e);

#endif

