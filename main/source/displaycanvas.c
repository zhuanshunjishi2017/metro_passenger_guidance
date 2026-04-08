#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <string.h>


#define CANVAS_X 60
#define CANVAS_Y 55
#define CANVAS_W 964
#define CANVAS_H 545
#define BUF_SIZE LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_W,CANVAS_H)


void *canvas_buf;//缓冲区
void pressing_canvas(lv_event_t *e);
void canvas_init(lv_obj_t *);

extern lv_coord_t origin_x, origin_y;

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

    create_metro_map(canvas);
}

void pressing_canvas(lv_event_t *e)
{
    lv_obj_t * obj = lv_event_get_target(e);//获取产生这个事件的对象
    lv_indev_t * indev = lv_indev_get_act();//获取输入设备

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);
/* 
    lv_coord_t x = lv_obj_get_x(obj) + vect.x;
    lv_coord_t y = lv_obj_get_y(obj) + vect.y;
    lv_obj_set_pos(obj, x, y);
 */
    origin_x += vect.x;
    origin_y += vect.y;

    //通过直接访问内存将画布重新填充为白色
    memset(canvas_buf, 255, BUF_SIZE);

    create_metro_map(obj);

}
