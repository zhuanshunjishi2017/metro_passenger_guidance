#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <stdint.h>
#include "metro_line.h"
#include "gui.h"
#include "route_planner.h"
#include "canvas.h"

extern Route route_result;

extern void *canvas_buf;
extern lv_obj_t * canvas;
extern lv_obj_t * location_image;
extern MetroLine metro_lines[4];
extern lv_obj_t* end_img,* start_img;
extern const char* start_name, *end_name;
extern const Station* start_station, *end_station;
uint16_t magnify_size = 30;

//原点
lv_coord_t origin_x = -30;
lv_coord_t origin_y = -30;

int8_t plus = 1;
int8_t minus = -1;

//下面定义一些标志位
int8_t is_line_showing = 0;           //正在显示的线路，0为全部
int8_t is_station_clicked = 0;   //是否存在被点击的车站
int8_t is_station_info = 0;      //是否有站点详细信息显示
int8_t is_route_showing = 0;     //是否有规划的路线正在显示


Station station_clicked[2];

static void draw_route(lv_obj_t *canvas, const Route *route);



lv_coord_t geo_to_screen(lv_coord_t pos, lv_coord_t origin)
{
    lv_coord_t res = pos * magnify_size + origin;
    return res;
}

static void draw_metro_line(lv_obj_t* canvas, const MetroLine* line, int8_t state) 
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
            //处理中南路和洪山广场这一段并行线
            if (line->line_number == 2 && ((line->stations[i].id == 21)||(line->stations[i].id == 22)))
            {
                pts[j].x += 2;
                pts[j].y += 2;
            }
            else if (line->line_number == 4 && (line->stations[i].id == 25))
            {
                j++;
                pts[j].x = pts[j-1].x - 1;
                pts[j].y = pts[j-1].y - 1;
            }
            else if (line->line_number == 4 && (line->stations[i].id == 26))
            {
                pts[j].x -= 1;
                pts[j].y -= 1;
                j++;
                pts[j].x = pts[j-1].x + 1;
                pts[j].y = pts[j-1].y + 1;
            }
            j++;
        }
    }
    //绘制线路
    lv_canvas_draw_line(canvas, pts, line->draw_point_count, &dsc);
    //绘制车站
    for (int i = 0; i < line->count; i++)
        draw_station(canvas, line->stations + i, lv_color_hex(line->line_color), state);
}

static void draw_route(lv_obj_t *canvas, const Route *route)
{
    lv_draw_line_dsc_t dsc;

    //设置线条样式
    lv_draw_line_dsc_init(&dsc);
    //dsc.color = lv_color_hex(line->line_color);
    dsc.width = LINE_WIDTH;
    dsc.round_start = true;
    dsc.round_end = true;  //圆角
    dsc.opa = LV_OPA_COVER;  //不透明度

    //创建点列表
    lv_point_t pts[64];
    //if (line->count > 64) return;

    int8_t j = 0;


    //这里的线路要分段画
    for (int i = 0; i < route->total_stations; i++)
    {
        pts[j].x = geo_to_screen(route->steps[i].sta->geo_x, origin_x);
        pts[j].y = geo_to_screen(route->steps[i].sta->geo_y, origin_y);

        dsc.color = get_line_color(route->steps[i].line_number);
        //处理中南路和洪山广场这一段并行线
        if (route->steps[i].line_number == 2 && 
            ((route->steps[i].sta->only_id == 52)||(route->steps[i].sta->only_id == 53)))
        {
            pts[j].x += 2;
            pts[j].y += 2;
        }
        else if (route->steps[i].line_number == 4 && (route->steps[i].sta->only_id == 53))
        {
            j++;
            pts[j].x = pts[j-1].x - 1;
            pts[j].y = pts[j-1].y - 1;
            lv_canvas_draw_line(canvas, pts + j - 1, 2 , &dsc);
        }
        else if (route->steps[i].line_number == 4 && (route->steps[i].sta->only_id == 52))
        {
            pts[j].x -= 1;
            pts[j].y -= 1;
            j++;
            pts[j].x = pts[j-1].x + 1;
            pts[j].y = pts[j-1].y + 1;
            lv_canvas_draw_line(canvas, pts + j - 1 , 2 , &dsc);

        }
        
        if (j > 0)
        {    
            lv_canvas_draw_line(canvas, pts + j - 1, 2 , &dsc);
        }

        j++;
    }
    //绘制车站
    for (int i = 0; i < route->total_stations; i++)
        draw_station(canvas, 
                    route->steps[i].sta, 
                    get_line_color(route->steps[i].line_number), 
                    HIGHLIGHT_STATE);

}

static void draw_station(lv_obj_t* canvas, const Station* s, lv_color_t color, int8_t state)
{
    //已经画过的就不画了
    if (!state && s->is_transfer > 0) return;

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
    } else {
        radius = STATION_RADIUS;
        rect_dsc.outline_color = color;
    }
    rect_dsc.radius = radius;

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
    //lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
    
    for (int i = 0; i < 4; i++) 
        draw_metro_line(canvas, &metro_lines[i], NORMAL_STATE);
    
    if (is_line_showing)
    {
        draw_transparent_rect(canvas, lv_color_white());
        for (int i = 0; i < 4; i++)
        {
            if (is_line_showing == metro_lines[i].line_number)
            {
                draw_metro_line(canvas, &metro_lines[i], HIGHLIGHT_STATE);
            }
        }
    }
    if (is_station_clicked)
    {
        pop_window_move(station_clicked);
    } 
    if (is_route_showing)
    {
        draw_transparent_rect(canvas, lv_color_white());
        draw_route(canvas, &route_result);
        start_end_pin_move();
    }
    if (is_station_info)
    {
        draw_transparent_rect(canvas, lv_color_hex(COLOR_MID_GRAY));
    }
    draw_line_container(canvas);
    
}

//绘制盛放按钮的区域
void draw_line_container(lv_obj_t * canvas)
{
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);

    rect_dsc.bg_color = lv_color_white();
    rect_dsc.outline_width = 1;
    rect_dsc.bg_opa = LV_OPA_COVER;

    rect_dsc.outline_color = lv_color_hex(COLOR_MID_GRAY);
    rect_dsc.radius = 4;

    lv_canvas_draw_rect(canvas, REC_X, REC_Y, REC_W, REC_H, &rect_dsc);
   
}

//绘制半透明遮罩
void draw_transparent_rect(lv_obj_t * canvas, lv_color_t color)
{
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);

    rect_dsc.bg_color = color;
    rect_dsc.outline_width = 0;
    rect_dsc.bg_opa = LV_OPA_70;

    rect_dsc.radius = 0;

    lv_canvas_draw_rect(canvas,0 ,0, CANVAS_W ,CANVAS_H ,&rect_dsc); 

}

void start_end_pin_move(void)
{
    
    lv_coord_t start_x = geo_to_screen(start_station->geo_x, origin_x);
    lv_coord_t start_y = geo_to_screen(start_station->geo_y, origin_y);
    lv_obj_set_pos(start_img, start_x - 38, start_y - 75);
    //lv_obj_move_foreground(start_img);
    lv_obj_clear_flag(start_img, LV_OBJ_FLAG_HIDDEN);


    lv_coord_t end_x = geo_to_screen(end_station->geo_x, origin_x);
    lv_coord_t end_y = geo_to_screen(end_station->geo_y, origin_y);
    lv_obj_set_pos(end_img, end_x - 38, end_y - 75);
    //lv_obj_move_foreground(end_img);
    lv_obj_clear_flag(end_img, LV_OBJ_FLAG_HIDDEN);

}

//拷贝一个站点信息
void station_copy(Station *dist,const Station *source)
{
    dist->geo_x = source->geo_x;
    dist->geo_y = source->geo_y;

    dist->horizon_offset = source->horizon_offset;
    dist->vertical_offset = source->vertical_offset;

    dist->id = source->id;
    dist->only_id = source->only_id;
    dist->transfer_id = source->transfer_id;

    dist->is_transfer = source->is_transfer;
    dist->line_belonged = source->line_belonged;

    dist->name = source->name;
    dist->name_pinyin = source->name_pinyin;
}

