#ifndef GUI
#define GUI

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include <stdio.h>

#define COLOR_DARK_BLUE     0x194888
#define COLOR_MID_BLUE      0x3F6EAD
#define COLOR_LIGHT_BLUE    0xD2DBE7
#define COLOR_BG_BLUE       0xEFF5FD
#define COLOR_LIGHT_GRAY    0xF5F5F5
#define COLOR_MID_GRAY      0x9D9D9D

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


void btn1_cb(lv_event_t *e);
void btn2_cb(lv_event_t *e);
void btn3_cb(lv_event_t *e);
void kb_show_cb(lv_event_t *e);
void kb_hide_cb(lv_event_t *e);
void keyBoard_event_cb(lv_event_t *e);
extern lv_obj_t* display0;
extern lv_obj_t* display1;
extern lv_obj_t* display2;
static int flag;


#endif
