#ifndef METRO_LINE
#define METRO_LINE

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <stdint.h>

#define SCREEN_W 1024
#define SCREEN_H 600  // 屏幕分辨率

#define GEO_X_MIN 0
#define GEO_X_MAX 45
#define GEO_Y_MIN 0
#define GEO_Y_MAX 35  // GeoGebra 坐标范围
#define PAD_X 12
#define PAD_Y 40  // 屏幕边距
#define STATION_RADIUS 5 //半径
#define TRANSFER_STATION_RADIUS 7
#define LINE_WIDTH 5  
#define OUTLINE_WIDTH 2
#define LABEL_OFFSET 4
#define TEXT_MAX 100


#define COLOR_LINE1    0x0066A4  // 1号线 蓝色
#define COLOR_LINE2    0xE99CC0  // 2号线 粉色
#define COLOR_LINE3    0xD7B665  // 3号线 橙色
#define COLOR_LINE4    0xBAD200  // 4号线 绿色

typedef struct {
    const int8_t id;
    const char* name;
    const char* name_pinyin;
    lv_coord_t geo_x;
    lv_coord_t geo_y;
    int8_t is_transfer;        //是否为换乘站
    int8_t horizon_offset;   //标签水平偏移量，右为正
    int8_t vertical_offset;  //标签垂直偏移量，下为正
    int8_t is_draw_point;      //是否为端点
} Station;                   // 站点结构体 两个偏移皆为0则不绘制（换乘车站）


typedef struct {
    const Station* stations;
    uint16_t count;
    uint32_t line_color;
    uint8_t draw_point_count;
} MetroLine; // 线路结构体



extern const lv_font_t heiti_14;

static lv_coord_t geo_to_screen(lv_coord_t pos, lv_coord_t origin);
static void draw_metro_line(lv_obj_t* canvas, const MetroLine* line);
static void draw_station(lv_obj_t* canvas, const Station* s, lv_color_t color);
void create_metro_map(void);
void draw_line_container(lv_obj_t * canvas);

#endif
