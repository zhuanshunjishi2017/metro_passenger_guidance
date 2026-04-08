#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <stdint.h>
#include "metro_line.h"
#include "gui.h"

void pressing_canvas(lv_event_t *e);
extern void *canvas_buf;

int8_t plus = 1;
int8_t minus = -1;

extern lv_obj_t * canvas;



static lv_coord_t geo_to_screen(lv_coord_t pos, lv_coord_t origin)
{
    lv_coord_t res = pos * magnify_size + origin;
    return res;
}

static void draw_metro_line(lv_obj_t* canvas, const MetroLine* line) 
{
    lv_draw_line_dsc_t dsc;

    //设置线条样式
    lv_draw_line_dsc_init(&dsc);
    dsc.color = lv_color_hex(line->line_color);
    dsc.width = LINE_WIDTH;
    dsc.round_start = true;
    dsc.round_end = true;  //圆角
    dsc.opa = LV_OPA_COVER;  //不透明度

    //创建点列表
    lv_point_t pts[64];
    if (line->count > 64) return;
    int8_t j = 0;
    for (int i = 0; i < line->count; i++)
    {
        if (line->stations[i].is_draw_point)
        {
            pts[j].x = geo_to_screen(line->stations[i].geo_x, origin_x);
            pts[j].y = geo_to_screen(line->stations[i].geo_y, origin_y);
            j++;
        }
    }
    //绘制线路
    lv_canvas_draw_line(canvas, pts, line->draw_point_count, &dsc);
    //绘制车站
    for (int i = 0; i < line->count; i++)
        draw_station(canvas, line->stations + i, lv_color_hex(line->line_color));
}

static void draw_station(lv_obj_t* canvas, const Station* s, lv_color_t color) 
{
    //已经画过的就不画了
    if (!s->horizon_offset && !s->vertical_offset) return;

    //设置站点样式
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);

    rect_dsc.bg_color = lv_color_white();
    rect_dsc.outline_width = OUTLINE_WIDTH;
    rect_dsc.bg_opa = LV_OPA_COVER;

    int8_t radius;
    if (s->is_transfer) {
        rect_dsc.outline_color = lv_color_black();
        radius = TRANSFER_STATION_RADIUS;
        rect_dsc.radius = TRANSFER_STATION_RADIUS;
    } else {
        radius = STATION_RADIUS;
        rect_dsc.radius = STATION_RADIUS;
        rect_dsc.outline_color = color;
    }

    //绘制站点
    lv_coord_t x = geo_to_screen(s->geo_x, origin_x);
    lv_coord_t y = geo_to_screen(s->geo_y, origin_y);

    lv_canvas_draw_rect(canvas,x - radius,y - radius,
                        radius * 2,radius * 2,&rect_dsc);  //站点形状

    //设置标签样式
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);

    label_dsc.color = lv_color_black();
    label_dsc.font = &heiti_14;
    label_dsc.align = LV_TEXT_ALIGN_LEFT;

    lv_coord_t text_x, text_y;
    lv_coord_t tw = lv_txt_get_width(s->name, strlen(s->name), label_dsc.font,0,0);
    
    text_x = x + LABEL_OFFSET * s->horizon_offset;
    text_y = y + LABEL_OFFSET * s->vertical_offset;

    if (s->horizon_offset == -1)
        text_x -= (int8_t)tw;
    if (s->vertical_offset == -1)
        text_y -= 14;
    if (!s->horizon_offset && s->vertical_offset)
    {
        text_x -= (int8_t)tw / 2;
        text_y += s->vertical_offset * 2;
    }
    if (s->horizon_offset && !s->vertical_offset)
    { 
        text_y -= 7;
        text_x += s->horizon_offset * 2;
    }
        
    lv_canvas_draw_text(canvas, text_x, text_y, TEXT_MAX, &label_dsc, s->name); //站点名
}

void create_metro_map(void) 
{
    //通过直接访问内存将画布重新填充为白色
    memset(canvas_buf, 255, BUF_SIZE);
    for (int i = 3; i >= 0; i--) 
        draw_metro_line(canvas, &metro_lines[i]);
}


