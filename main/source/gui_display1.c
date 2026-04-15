#include "gui.h"
void start_ta_kb_show_cb(lv_event_t *e);
void end_ta_kb_show_cb(lv_event_t *e);

lv_obj_t* main_lb1;
lv_obj_t* start_lb,*end_lb;
lv_obj_t* start_ta,*end_ta;
lv_obj_t* btn4,*btn4_lb;
lv_obj_t* display11,*main_lb2,*station_lb,*route_lb;
lv_obj_t* display12,*display12_lb1;
static lv_point_t line_points[] = {{352,55},{352,599}};
void ui_init(void)
{
    main_lb1 = create_simple_label(display1,100,86,96,27,"路线规划",&heiti_24);
    start_lb = create_simple_label(display1,100,136,40,23,"起点",&heiti_20);
    end_lb = create_simple_label(display1,100,226,40,23,"终点",&heiti_20);
    lv_obj_set_style_text_color(start_lb, lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_obj_set_style_text_color(end_lb, lv_color_hex(COLOR_DARK_BLUE), 0);

    btn4 = create_simple_btn(display1,100,336,212,38, lv_color_hex(COLOR_DARK_BLUE));
    btn4_lb = lv_label_create(btn4);
	lv_obj_set_flex_align(btn4, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_label_set_text(btn4_lb,"确定");
	lv_obj_set_style_text_font(btn4_lb,&heiti_16,LV_PART_MAIN);
	lv_obj_set_style_bg_color(btn4_lb, lv_color_hex(0xffffff), 0);

    lv_obj_t * line1 = lv_line_create(display1);
    lv_line_set_points(line1, line_points, 2);
    lv_obj_set_style_line_width(line1, 2, LV_PART_MAIN);
    lv_obj_set_style_line_color(line1, lv_color_hex(COLOR_MID_GRAY), LV_PART_MAIN);
    lv_obj_clear_flag(display1, LV_OBJ_FLAG_SCROLLABLE);
}
void ta_init(void)
{
    start_ta = lv_textarea_create(display1);
    lv_obj_set_pos(start_ta,101,167);
    lv_obj_set_size(start_ta,211,40);
    lv_obj_set_style_radius(start_ta,4,LV_PART_MAIN);
    lv_obj_add_event_cb(start_ta, start_ta_kb_show_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_one_line(start_ta, true);          // 强制单行
    lv_obj_set_scrollbar_mode(start_ta, LV_SCROLLBAR_MODE_OFF); // 关掉滚动条

    end_ta = lv_textarea_create(display1);
    lv_obj_set_pos(end_ta,101,255);
    lv_obj_set_size(end_ta,211,40);
    lv_obj_set_style_radius(end_ta,4,LV_PART_MAIN);
    lv_obj_add_event_cb(end_ta, end_ta_kb_show_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_one_line(end_ta, true);          // 强制单行
    lv_obj_set_scrollbar_mode(end_ta, LV_SCROLLBAR_MODE_OFF); // 关掉滚动条
}
void display11_init(void)
{
    display11 = lv_obj_create(display1);
    lv_obj_set_size(display11, 672, 545);
    lv_obj_set_pos(display11, 354, 55);
    lv_obj_set_style_bg_opa(display11, LV_OPA_100, 0);
    lv_obj_set_style_border_width(display11, 0, 0);
    main_lb2 = create_simple_label(display11,43, 31, 96, 27,"我的收藏",&heiti_24);
    station_lb = create_simple_label(display11, 44, 94, 40, 23,"车站",&heiti_20);
    route_lb = create_simple_label(display11, 350, 94, 40, 23,"线路",&heiti_20);
    lv_obj_set_style_text_color(station_lb, lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_obj_set_style_text_color(route_lb, lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_obj_t* station_prompt = create_simple_label(display11, 42, 150, 140, 23,"暂无收藏车站!",&heiti_20);
    lv_obj_t* route_prompt = create_simple_label(display11, 350, 150, 140, 23,"暂无收藏线路!",&heiti_20);
    lv_obj_move_foreground(display11);
}
void display12_init(void)
{
    display12 = lv_obj_create(display1);
    lv_obj_set_size(display12, 672, 545);
    lv_obj_set_pos(display12, 354, 55);
    lv_obj_set_style_bg_opa(display12, LV_OPA_100, 0);
    lv_obj_set_style_border_width(display12, 0, 0);
    display12_lb1 = create_simple_label(display12,20,18,80,23,"搜索结果",&heiti_20);
    lv_obj_set_style_text_color(display12_lb1,lv_color_hex(COLOR_DARK_BLUE), 0);
    // lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
}




void start_ta_kb_show_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        kb_show(kb,ta,lv_color_hex(0xffffff));
        lv_obj_set_style_bg_opa(transparent, 0, 0);
        lv_obj_move_foreground(ta);
        lv_obj_move_foreground(start_lb);
        lv_obj_clear_flag(display11, LV_OBJ_FLAG_HIDDEN);
    }
    if (code == LV_EVENT_VALUE_CHANGED)
	{
        lv_obj_add_flag(display11, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(display12, LV_OBJ_FLAG_HIDDEN);
	}
}
void end_ta_kb_show_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        kb_show(kb,ta,lv_color_hex(0xffffff));
        lv_obj_set_style_bg_opa(transparent, 0, 0);
        lv_obj_move_foreground(ta);
        lv_obj_move_foreground(end_lb);
    }
    if (code == LV_EVENT_VALUE_CHANGED)
	{
        lv_obj_add_flag(display11, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(display12, LV_OBJ_FLAG_HIDDEN);
	}
}
