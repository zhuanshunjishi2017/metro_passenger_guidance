#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <stdint.h>

#define SCREEN_W 1024
#define SCREEN_H 600  // 屏幕分辨率
#define GEO_X_MIN 0
#define GEO_X_MAX 45
#define GEO_Y_MIN -35
#define GEO_Y_MAX 0  // GeoGebra 坐标范围
#define PAD_X 12
#define PAD_Y 40  // 屏幕边距
#define GEO_TO_SCREEN_X(x) ((lv_coord_t)((((x) - GEO_X_MIN) * 1.0f / (GEO_X_MAX - GEO_X_MIN)) * (SCREEN_W - 2 * PAD_X) + PAD_X))
#define GEO_TO_SCREEN_Y(y) ((lv_coord_t)((((y) - GEO_Y_MIN) * 1.0f / (GEO_Y_MAX - GEO_Y_MIN)) * (SCREEN_H - 2 * PAD_Y) + PAD_Y))  // 坐标映射公式
#define STATION_RADIUS 8 //半径
#define LINE_WIDTH 4  

typedef struct {
    const char* id;
    const char* name;
    const char* ENname;
    lv_coord_t geo_x;
    lv_coord_t geo_y;
    lv_color_t color;
} Station;  // 站点结构体
typedef struct {
    const Station* stations;
    uint16_t count;
    lv_color_t line_color;
} MetroLine;  // 线路结构体

static void init_line_dsc(lv_draw_line_dsc_t* dsc, lv_color_t color, lv_coord_t w) {
    lv_draw_line_dsc_init(dsc);
    dsc->color = color;
    dsc->width = w;
    dsc->round_start = true;
    dsc->round_end = true;  //圆角
    dsc->opa = LV_OPA_COVER;  //不透明度
}  // 初始化线条

static void draw_line(lv_obj_t* canvas, const MetroLine* line) {
    lv_draw_line_dsc_t dsc;
    init_line_dsc(&dsc, line->line_color, LINE_WIDTH);

    lv_point_t pts[64];
    if (line->count > 64) return;

    for (int i = 0; i < line->count; i++) {
        pts[i].x = GEO_TO_SCREEN_X(line->stations[i].geo_x);
        pts[i].y = GEO_TO_SCREEN_Y(line->stations[i].geo_y);
    }
    lv_canvas_draw_line(canvas, pts, line->count, &dsc);
}

static void draw_station(lv_obj_t* canvas, const Station* s) {
    lv_coord_t x = GEO_TO_SCREEN_X(s->geo_x);
    lv_coord_t y = GEO_TO_SCREEN_Y(s->geo_y);

    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = s->color;
    rect_dsc.bg_opa = LV_OPA_COVER;
    rect_dsc.radius = STATION_RADIUS;

    lv_canvas_draw_rect(canvas,x - STATION_RADIUS,y - STATION_RADIUS,STATION_RADIUS * 2,STATION_RADIUS * 2,&rect_dsc);  //站点形状

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_color_white();
    label_dsc.font = LV_FONT_MONTSERRAT_12;

    lv_coord_t tw = lv_txt_get_width(s->name, strlen(s->name), label_dsc.font,0,0);
    lv_canvas_draw_label(canvas,x - tw / 2, y + STATION_RADIUS + 4, LV_TEXT_ALIGN_CENTER, s->name); //站点名
}

void create_metro_map(lv_obj_t* canvas) {
    lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);

    // 画所有线路
    for (int l = 0; l < LINE_COUNT; l++) {
        draw_line(canvas, &metro_lines[l]);
    }

    // 画所有站点
    for (int l = 0; l < LINE_COUNT; l++) {
        const MetroLine* line = &metro_lines[l];
        for (int i = 0; i < line->count; i++) {
            draw_station(c, &line->stations[i]);
        }
    }
}