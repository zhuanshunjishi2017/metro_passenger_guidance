#ifndef GUI
#define GUI

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
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
#define CANVAS_X 60
#define CANVAS_Y 55
#define CANVAS_W 964
#define CANVAS_H 545
#define BUF_SIZE    LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_W,CANVAS_H)
#define BUTTON_LEN  35
#define BUTTON_X    954 - CANVAS_X
#define BUTTON_Y    476 - CANVAS_Y
#define REC_X       896 - CANVAS_X
#define REC_Y       74  - CANVAS_Y
#define REC_W       102
#define REC_H       143
#define STEP  5 //缩放的步长

// --- 外部变量声明 ---
extern const lv_font_t heiti_14;
extern const lv_font_t heiti_16;
extern const lv_font_t heiti_20;
extern const lv_font_t heiti_24;
extern lv_obj_t* display0;
extern lv_obj_t* display1;
extern lv_obj_t* display2;
extern lv_obj_t* canvas;
extern lv_obj_t* kb;
extern lv_obj_t* timetable1,*timetable2,*timetable3;

// ==========================================
// 工具函数声明
// ==========================================
/**
 * @brief 通用 Label 创建工具
 * @return 返回创建好的 lv_obj_t* 指针
 * 只用于设置坐标、尺寸、文本及文本颜色
 */
lv_obj_t* create_simple_label(lv_obj_t* parent, int x, int y, int w, int h, 
                              const char* text, const lv_font_t* font);

/**
 * @brief 通用按钮创建工具 (带圆角和背景色)
 * @return 返回创建好的 lv_obj_t* 指针
 * 默认圆角为4
 */
lv_obj_t* create_simple_btn(lv_obj_t* parent, int x, int y, int w, int h, lv_color_t bg_color);


#endif