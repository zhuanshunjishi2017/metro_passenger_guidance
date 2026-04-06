#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#define CANVAS_SIZE 4096    // 画布边长 4096x4096（可根据内存改 2048/8192）
#define COLOR_FORMAT LV_IMG_CF_TRUE_COLOR  // 16位色，占用内存小

static lv_color_t canvas_buf[CANVAS_SIZE * CANVAS_SIZE];

void display0_init(lv_obj_t * display0)
{
    lv_obj_set_style_border_width(display0, 0, LV_PART_MAIN);
	lv_obj_set_style_bg_opa(display0, 0, LV_PART_MAIN);
	lv_obj_set_scrollbar_mode(display0, LV_SCROLLBAR_MODE_OFF);
	lv_obj_set_scroll_dir(display0, LV_DIR_ALL);
	lv_obj_set_scroll_snap(display0, LV_SCROLL_SNAP_NONE);
	//无边界滚动
	lv_obj_set_scroll_limit(display0, LV_DIR_TOP, -LV_COORD_MAX);
    lv_obj_set_scroll_limit(display0, LV_DIR_BOTTOM, LV_COORD_MAX);
    lv_obj_set_scroll_limit(display0, LV_DIR_LEFT, -LV_COORD_MAX);
    lv_obj_set_scroll_limit(display0, LV_DIR_RIGHT, LV_COORD_MAX);
}

void canvas_init(lv_obj_t *canvas)
{
	lv_canvas_set_buffer(canvas, canvas_buf, CANVAS_SIZE, CANVAS_SIZE, COLOR_FORMAT);
    lv_obj_center(canvas);
	lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
}