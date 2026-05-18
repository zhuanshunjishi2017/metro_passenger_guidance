#ifndef METRO_LINE
#define METRO_LINE

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <stdint.h>
#include "timestruct.h"
//#include "route_planner.h"

#define SCREEN_W 1024
#define SCREEN_H 600  // 屏幕分辨率

#define GEO_X_MIN 0
#define GEO_X_MAX 38
#define GEO_Y_MIN 0
#define GEO_Y_MAX 33 // GeoGebra 坐标范围
#define PAD_X 12
#define PAD_Y 40  // 屏幕边距
#define STATION_RADIUS 5 //半径
#define TRANSFER_STATION_RADIUS 7
#define LINE_WIDTH 5  
#define OUTLINE_WIDTH 2
#define LABEL_OFFSET 4
#define TEXT_MAX 100

#define NORMAL_STATE 0
#define HIGHLIGHT_STATE 1

#define TOUCH_RANGE 15

#define LINE_COUNT   4
#define MAX_TRANSFER 3


#define COLOR_LINE1    0x0066A4  // 1号线 蓝色
#define COLOR_LINE2    0xE99CC0  // 2号线 粉色
#define COLOR_LINE3    0xD7B665  // 3号线 橙色
#define COLOR_LINE4    0xBAD200  // 4号线 绿色


#define DEPARTURE 0   //始发站
#define TERMINUS  1    //终点站

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1

typedef struct {
    int8_t       only_id;
    int8_t       id;
    lv_coord_t   geo_x;
    lv_coord_t   geo_y;
    int8_t       transfer_line[2];        //是否为换乘站,数字表示换乘几号线，标记为负数说明已经出现过
    int8_t       horizon_offset;   //标签水平偏移量，右为正
    int8_t       vertical_offset;  //标签垂直偏移量，下为正
    int8_t       is_draw_point;      //是否为端点
    int8_t       line_belonged; //所属的线路
    char*        name;
    char*        name_pinyin;

} Station;// 站点结构体


typedef struct{
    TimeStruct first_train_time;
    TimeStruct last_train_time;
    uint16_t depart_period;
} Timetable; //列车时间表


typedef struct {
    const Station* stations;
    uint16_t count;
    uint32_t line_color;
    uint8_t line_number;
    const uint16_t * station_period;
    Timetable * timetable;
} MetroLine; // 线路结构体



extern const lv_font_t heiti_14;

const MetroLine * get_metro_line(int8_t line_number);

const Station * get_station(const Station * sta);

const Station * get_transfer_station(const Station * src, const Station ** first_dist);

const Station * get_first_station(int8_t line_number, int8_t direction);

lv_color_t get_line_color(int8_t line_number);



//声明一些有关地图绘制的函数

lv_coord_t geo_to_screen(lv_coord_t pos, lv_coord_t origin);
static void draw_metro_line(lv_obj_t* canvas, const MetroLine* line, int8_t state);
static void draw_station(lv_obj_t* canvas, const Station* s, lv_color_t color, int8_t state);

void create_metro_map(void);
void draw_line_container(lv_obj_t * canvas);
void draw_transparent_rect(lv_obj_t * canvas, lv_color_t color);

void start_end_pin_move(void);


#endif
