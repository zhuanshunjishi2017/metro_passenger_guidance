#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <string.h>
#include "gui.h"

extern int8_t plus, minus;



void *canvas_buf;//缓冲区


void pressing_canvas(lv_event_t *e);
void canvas_init(lv_obj_t *);
void buttons_init(lv_obj_t *);
void adjust_magnify(lv_event_t * );

lv_obj_t *btn_plus , *btn_minus;
lv_obj_t *lines_container;
lv_style_t btn_style;//创建样式
lv_obj_t * label_plus, *label_minus;

extern lv_coord_t origin_x, origin_y, magnify_size;

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
    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);

    buttons_init(canvas);

    create_metro_map();
}

void buttons_init(lv_obj_t *canvas)
{


    lv_style_init(&btn_style);

    lv_style_set_bg_color(&btn_style, lv_color_hex(COLOR_LIGHT_GRAY));
    lv_style_set_border_color(&btn_style, lv_color_hex(COLOR_MID_GRAY));
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


void pressing_canvas(lv_event_t *e)
{
    lv_obj_t * obj = lv_event_get_target(e);//获取产生这个事件的对象
    lv_indev_t * indev = lv_indev_get_act();//获取输入设备

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    origin_x += vect.x;
    origin_y += vect.y;

    create_metro_map();

}

void adjust_magnify(lv_event_t * e)
{
    int8_t *state = lv_event_get_user_data(e);

    uint16_t pre_size = magnify_size;
    magnify_size += (uint16_t)((*state) * STEP);

    if (magnify_size < 15) 
    {
        magnify_size = 15;
        return;
    }
    if (magnify_size > 150) 
    {    
        magnify_size = 150;
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
