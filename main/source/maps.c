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

#define COLOR_LINE1    0x001F  // 1号线 蓝色 (R=0x00, G=0x7A, B=0xFF)
#define COLOR_LINE2    0xF31F  // 2号线 粉色 (R=0xFF, G=0x69, B=0xB4)
#define COLOR_LINE3    0xFD60  // 3号线 橙色 (R=0xFF, G=0x98, B=0x00)
#define COLOR_LINE4    0x05A7  // 4号线 绿色 (R=0x00, G=0xB4, B=0x2D)

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


static const Station line_1[] = {
    {"径河", 2, -16, COLOR_LINE1},
    {"三店", 3, -16, COLOR_LINE1},
    {"码头潭公园", 4, -16, COLOR_LINE1},
    {"东吴大道", 5, -16, COLOR_LINE1},
    {"五环大道", 6, -17, COLOR_LINE1},
    {"额头湾", 7, -18, COLOR_LINE1},
    {"竹叶海", 8, -19, COLOR_LINE1},
    {"舵落口", 9, -19, COLOR_LINE1},
    {"古田一路", 10, -18, COLOR_LINE1},
    {"古田二路", 11, -18, COLOR_LINE1},
    {"古田三路", 12, -18, COLOR_LINE1},
    {"古田四路", 13, -18, COLOR_LINE1},
    {"汉西一路", 14, -18, COLOR_LINE1},
    {"宗关", 15, -16, COLOR_LINE1},
    {"太平洋", 16, -19, COLOR_LINE1},
    {"硚口路", 17, -19, COLOR_LINE1},
    {"崇仁路", 18, -19, COLOR_LINE1},
    {"利济北路", 19, -19, COLOR_LINE1},
    {"友谊路", 20, -19, COLOR_LINE1},
    {"循礼门", 21, -22, COLOR_LINE1},
    {"大智路", 22, -22, COLOR_LINE1},
    {"三阳路", 23, -22, COLOR_LINE1},
    {"黄浦路", 24, -22, COLOR_LINE1},
    {"头道街", 25, -22, COLOR_LINE1},
    {"二七路", 26, -22, COLOR_LINE1},
    {"徐州新村", 27, -20, COLOR_LINE1},
    {"丹水池", 28, -19, COLOR_LINE1},
    {"新荣", 29, -18, COLOR_LINE1},
    {"堤角", 30, -17, COLOR_LINE1},
    {"滕子岗", 31, -16, COLOR_LINE1},
    {"滠口新城", 32, -15, COLOR_LINE1},
    {"汉口北", 33, -14, COLOR_LINE1},
};
static const Station line_2[] = {
    {"天河机场", 17, -17, COLOR_LINE2},
    {"航空总部", 16, -17, COLOR_LINE2},
    {"宋家岗", 15, -17, COLOR_LINE2},
    {"巨龙大道", 14, -15, COLOR_LINE2},
    {"盘龙城", 13, -15, COLOR_LINE2},
    {"宏图大道", 12, -14, COLOR_LINE2},
    {"常青城", 11, -13, COLOR_LINE2},
    {"金银潭", 10, -12, COLOR_LINE2},
    {"常青花园", 9, -11, COLOR_LINE2},
    {"长港路", 8, -10, COLOR_LINE2},
    {"汉口火车站", 7, -9, COLOR_LINE2},
    {"范湖", 6, -8, COLOR_LINE2},
    {"王家墩东", 5, -7, COLOR_LINE2},
    {"青年路", 5, -6, COLOR_LINE2},
    {"中山公园", 5, -5, COLOR_LINE2},
    {"循礼门", 5, -4, COLOR_LINE2},
    {"积玉桥", 18, -17, COLOR_LINE2},
    {"螃蟹岬", 19, -17, COLOR_LINE2},
    {"小龟山", 20, -16, COLOR_LINE2},
    {"洪山广场", 21, -14, COLOR_LINE2},
    {"中南路", 22, -13, COLOR_LINE2},
    {"宝通寺", 23, -12, COLOR_LINE2},
    {"街道口", 24, -11, COLOR_LINE2},
    {"广埠屯", 25, -10, COLOR_LINE2},
    {"虎泉", 26, -9, COLOR_LINE2},
    {"杨家湾", 27, -8, COLOR_LINE2},
    {"光谷广场", 28, -7, COLOR_LINE2},
    {"珞雄路", 29, -6, COLOR_LINE2},
    {"华中科技大学", 30, -5, COLOR_LINE2},
    {"光谷大道", 31, -5, COLOR_LINE2},
    {"佳园路", 31, -4, COLOR_LINE2},
    {"武汉东站", 31, -3, COLOR_LINE2},
};
static const Station line_3[] = {
    {"沌阳大道", 18, -19, COLOR_LINE3},
    {"东风公司", 17, -19, COLOR_LINE3},
    {"体育中心", 16, -19, COLOR_LINE3},
    {"三角湖", 15, -19, COLOR_LINE3},
    {"汉阳客运站", 14, -19, COLOR_LINE3},
    {"四新大道", 13, -19, COLOR_LINE3},
    {"陶家岭", 12, -19, COLOR_LINE3},
    {"龙阳村", 11, -19, COLOR_LINE3},
    {"王家湾", 30, -20, COLOR_LINE3},
    {"宗关", 29, -10, COLOR_LINE3},
    {"双墩", 28, -9, COLOR_LINE3},
    {"武汉商务区", 27, -8, COLOR_LINE3},
    {"云飞路", 26, -7, COLOR_LINE3},
    {"范湖", 25, -6, COLOR_LINE3},
    {"菱角湖", 24, -7, COLOR_LINE3},
    {"香港路", 23, -8, COLOR_LINE3},
    {"惠济二路", 22, -9, COLOR_LINE3},
    {"赵家条", 21, -10, COLOR_LINE3},
    {"罗家庄", 20, -11, COLOR_LINE3},
    {"二七小路", 19, -12, COLOR_LINE3},
    {"兴业路", 18, -12, COLOR_LINE3},
    {"后湖大道", 17, -13, COLOR_LINE3},
    {"市民之家", 16, -15, COLOR_LINE3},
    {"宏图大道", 15, -15, COLOR_LINE3},
};
static const Station line_4[] = {
    {"武汉火车站", 17, 2, COLOR_LINE4},
    {"杨春湖", 17, -1, COLOR_LINE4},
    {"工业四路", 18, -3, COLOR_LINE4},
    {"仁和路", 19, -4, COLOR_LINE4},
    {"园林路", 20, -5, COLOR_LINE4},
    {"罗家港", 19, -7, COLOR_LINE4},
    {"铁机路", 18, -8, COLOR_LINE4},
    {"岳家嘴", 18, -9, COLOR_LINE4},
    {"东亭", 18, -10, COLOR_LINE4},
    {"青鱼嘴", 19, -12, COLOR_LINE4},
    {"楚河汉街", 20, -14, COLOR_LINE4},
    {"洪山广场", 21, -15, COLOR_LINE4},
    {"中南路", 22, -16, COLOR_LINE4},
    {"梅苑小区", 23, -18, COLOR_LINE4},
    {"武昌火车站", 24, -19, COLOR_LINE4},
    {"首义路", 25, -20, COLOR_LINE4},
    {"复兴路", 27, -21, COLOR_LINE4},
    {"拦江路", 28, -23, COLOR_LINE4},
    {"钟家村", 29, -24, COLOR_LINE4},
    {"汉阳火车站", 30, -25, COLOR_LINE4},
    {"五里墩", 31, -26, COLOR_LINE4},
    {"七里庙", 32, -27, COLOR_LINE4},
    {"十里铺", 33, -27, COLOR_LINE4},
    {"王家湾", 34, -27, COLOR_LINE4},
    {"玉龙路", 35, -27, COLOR_LINE4},
    {"永安堂", 36, -28, COLOR_LINE4},
    {"孟家铺", 36, -29, COLOR_LINE4},
    {"黄金口", 36, -30, COLOR_LINE4},
    {"新天", 37, -31, COLOR_LINE4},
    {"集贤", 37, -32, COLOR_LINE4},
    {"知音", 37, -33, COLOR_LINE4},
    {"新农", 37, -34, COLOR_LINE4},
    {"凤凰路", 38, -34, COLOR_LINE4},
    {"蔡甸广场", 38, -33, COLOR_LINE4},
    {"临嶂大道", 38, -32, COLOR_LINE4},
    {"新庙村", 38, -31, COLOR_LINE4},
    {"柏林", 39, -33, COLOR_LINE4},
};
static const MetroLine metro_lines[] = {
    {line_1, 32, COLOR_LINE1},
    {line_2, 38, COLOR_LINE2},
    {line_3, 24, COLOR_LINE3},
    {line_4, 37, COLOR_LINE4},
};


void create_metro_map(lv_obj_t* canvas) {
    lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);

    // 画所有线路
    for (int l = 0; l < 4; l++) {
        draw_line(canvas, &metro_lines[l]);
    }

    // 画所有站点
    for (int l = 0; l < 4; l++) {
        const MetroLine* line = &metro_lines[l];
        for (int i = 0; i < line->count; i++) {
            draw_station(canvas, &line->stations[i]);
        }
    }
}