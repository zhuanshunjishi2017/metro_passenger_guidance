#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <string.h>
#include <stdio.h>
#include "gui.h"
#include "metro_line.h"

extern int8_t plus, minus;
extern MetroLine metro_lines[4];

extern int8_t is_showing;
extern lv_obj_t * kb;

void *canvas_buf;//缓冲区

int8_t para_numbers[5] = {0,1,2,3,4};  //妥协之举，传参只能传地址


void pressing_canvas(lv_event_t *e);
void canvas_init(lv_obj_t *);
void buttons_init(lv_obj_t *);
void adjust_magnify(lv_event_t * );
void clicked_canvas(lv_event_t *e);


void lines_selector_init(lv_obj_t * , MetroLine *);
void line_selector_cb(lv_event_t * e);

void lines_btn_init(lv_obj_t * btn ,lv_obj_t * labels, lv_obj_t *canvas , MetroLine *line);


lv_obj_t *btn_plus , *btn_minus;
lv_obj_t *lines_container;
lv_style_t btn_style, rec_style, selector_style, line_style;//创建样式
lv_obj_t * label_plus, *label_minus;
lv_obj_t line_btns[4];
lv_obj_t line_labels[4][2];

extern lv_coord_t origin_x, origin_y, magnify_size;
extern lv_font_t heiti_16;

void canvas_init(lv_obj_t *canvas)
{
	    //申请缓冲区以设置像素
    canvas_buf = sdram_malloc(BUF_SIZE);
    if (!canvas_buf) while (1);
    
    lv_obj_set_pos(canvas,CANVAS_X,CANVAS_Y);
    lv_obj_set_size(canvas,CANVAS_W,CANVAS_H);

    lv_canvas_set_buffer(canvas, canvas_buf, CANVAS_W, CANVAS_H, LV_IMG_CF_TRUE_COLOR);

    lv_obj_add_flag(canvas, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(canvas, pressing_canvas, LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(canvas,clicked_canvas, LV_EVENT_PRESSED, NULL);


    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);

    buttons_init(canvas);

    lines_selector_init(canvas, metro_lines);

    create_metro_map();
}

void buttons_init(lv_obj_t *canvas)
{


    lv_style_init(&btn_style);

    lv_style_set_bg_color(&btn_style, lv_color_hex(COLOR_LIGHT_GRAY));
    lv_style_set_border_color(&btn_style, lv_color_hex(COLOR_MID_GRAY));
    lv_style_set_border_width(&btn_style, 1);
    lv_style_set_radius(&btn_style, BUTTON_RADIUS);
    lv_style_set_opa(&btn_style, LV_OPA_COVER);


    btn_plus = lv_btn_create(canvas);
    lv_obj_set_pos(btn_plus , BUTTON_X, BUTTON_Y);
    lv_obj_set_size(btn_plus, BUTTON_LEN, BUTTON_LEN);


    label_plus = lv_label_create(btn_plus);
    lv_label_set_text(label_plus, "+");
    lv_obj_center(label_plus);
    lv_obj_set_style_text_color(label_plus, lv_color_black(),0);

    btn_minus = lv_btn_create(canvas);
    lv_obj_set_pos(btn_minus , BUTTON_X , BUTTON_Y + BUTTON_LEN + 10);
    lv_obj_set_size(btn_minus, BUTTON_LEN, BUTTON_LEN);

    label_minus = lv_label_create(btn_minus);
    lv_label_set_text(label_minus, "-");
    lv_obj_center(label_minus);
    lv_obj_set_style_text_color(label_minus, lv_color_black(),0);

    lv_obj_add_style(btn_plus, &btn_style, 0);
    lv_obj_add_style(btn_minus, &btn_style, 0);

    lv_obj_add_event_cb(btn_plus, adjust_magnify, LV_EVENT_PRESSED, &plus);
    lv_obj_add_event_cb(btn_minus, adjust_magnify, LV_EVENT_PRESSED, &minus);
}

void lines_selector_init(lv_obj_t *canvas, MetroLine *lines)
{

    lv_style_init(&selector_style);

    lv_style_set_bg_color(&selector_style, lv_color_white());
    //lv_style_set_border_color(&selector_style, lv_color_hex(COLOR_MID_GRAY));
    lv_style_set_border_width(&selector_style, 0);
    lv_style_set_radius(&selector_style, BUTTON_RADIUS);
    lv_style_set_opa(&selector_style, LV_OPA_COVER);
    lv_style_set_shadow_opa(&selector_style, LV_OPA_0);


    lv_style_init(&line_style);
    for (int i = 0; i < 4;i++)
    {
        lines_btn_init(line_btns + i, line_labels[i], canvas, lines + i);
    }

}
void lines_btn_init(lv_obj_t * btn ,lv_obj_t * labels, lv_obj_t *canvas ,  MetroLine *line)
{
    char * str, longstr[10];
    int8_t count = line->line_number;
    sprintf(str, "%d", count);
    
    btn = lv_btn_create(canvas);

    lv_obj_set_pos(btn, REC_X + 10 , REC_Y + 10 + 33 *(count -1));
    lv_obj_set_size(btn, 84 , 24);

    lv_obj_add_style(btn, &selector_style, 0);

    labels = lv_label_create(btn);
    lv_obj_set_pos(labels, -20 ,-11);
    lv_obj_set_size(labels, 24, 24);

    lv_obj_set_style_text_font(labels, &heiti_16, 0);
    lv_obj_set_style_text_color(labels, lv_color_white(), 0);
    lv_obj_set_style_bg_color(labels, lv_color_hex(line->line_color), 0);
    lv_obj_set_style_bg_opa(labels, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(labels, 4, 0);
    lv_obj_set_style_text_align(labels, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_top(labels, 4, 0);
    
    
    lv_label_set_text(labels, str);

    lv_obj_t * label2 = labels + 1;

    label2 = lv_label_create(btn);
    
    lv_obj_set_pos(label2, 14 ,-8);
    lv_obj_set_size(label2, 48, 18);
    lv_obj_set_style_text_font(label2, &heiti_16, 0);
    lv_obj_set_style_text_color(label2, lv_color_black(), 0);
    lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_LEFT, 0);


    snprintf(longstr, sizeof(longstr), "%d号线",count);
    lv_label_set_text(label2, longstr);

    lv_obj_add_event_cb(btn, line_selector_cb, LV_EVENT_PRESSED, para_numbers + count);

}


void line_selector_cb(lv_event_t * e)
{
    int8_t *para_number = lv_event_get_user_data(e);

    if (*para_number == is_showing) is_showing = 0;
    else is_showing = *para_number;

    create_metro_map();
}

void pressing_canvas(lv_event_t *e)
{
    lv_obj_t * obj = lv_event_get_target(e);//获取产生这个事件的对象
    lv_indev_t * indev = lv_indev_get_act();//获取输入设备

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    origin_x += vect.x;
    origin_y += vect.y;

    //设置边界

    if (origin_x > CANVAS_W/2) origin_x = CANVAS_W/2;
    if (origin_y > CANVAS_H/2) origin_y = CANVAS_H/2;

    if (origin_x < CANVAS_W/2 - GEO_X_MAX * magnify_size) 
        origin_x = CANVAS_W/2 - GEO_X_MAX * magnify_size;
    if (origin_y < CANVAS_H/2 - GEO_Y_MAX * magnify_size) 
        origin_y = CANVAS_H/2 - GEO_Y_MAX * magnify_size;


    create_metro_map();

}

void adjust_magnify(lv_event_t * e)
{
    int8_t *state = lv_event_get_user_data(e);

    uint16_t pre_size = magnify_size;
    magnify_size += (uint16_t)((*state) * STEP);

    if (magnify_size < 20) 
    {
        magnify_size = 20;
        return;
    }
    if (magnify_size > 100) 
    {    
        magnify_size = 100;
        return;
    }
    //以画布中心为原点 计算偏移量
    lv_coord_t dist_x = origin_x - CANVAS_W/2;
    lv_coord_t dist_y = origin_y - CANVAS_H/2;

    dist_x = (lv_coord_t)((float)magnify_size/pre_size * dist_x);
    dist_y = (lv_coord_t)((float)magnify_size/pre_size * dist_y);


    origin_x = CANVAS_W/2 + dist_x;
    origin_y = CANVAS_H/2 + dist_y;

    create_metro_map();
}

void clicked_canvas(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
}