#include "gui.h"
#include "canvas.h"
#include "route_planner.h"
#include "lvgl/lvgl.h"
#include "favorites.h"
#include "route_planner.h"
void start_ta_kb_show_cb(lv_event_t *e);
void end_ta_kb_show_cb(lv_event_t *e);
void btn4_cb(lv_event_t *e); // 确定按钮回调
void cancelbtn_cb(lv_event_t *e); // 取消按钮回调
void search_result_click_cb(lv_event_t *e); // 搜索结果点击回调
void station_info_click_cb(lv_event_t *e); // 收藏站点信息点击回调
void route_info_click_cb(lv_event_t *e); // 收藏路线信息点击回调
void route_info_delete_click_cb(lv_event_t *e); // 收藏路线删除按钮点击回调
void clear_top_search_result_labels(void); // 清除顶部搜索结果标签
void del_pp_window(void);
// 全局变量，用于跨界面传递路线信息
Route route_result;

lv_obj_t* main_lb1;
lv_obj_t* start_lb,*end_lb;
lv_obj_t* start_ta,*end_ta;
lv_obj_t* station_prompt,*route_prompt;
lv_obj_t* btn4,*btn4_lb;
lv_obj_t* display11,*main_lb2,*station_lb,*route_lb;
lv_obj_t* display12,*display12_lb1;
lv_obj_t* search_result_show_label[SEARCH_LIST_LEN];
lv_obj_t* search_line_show_label[SEARCH_LIST_LEN];
lv_obj_t* search_line_transfer_show_label[SEARCH_LIST_LEN];
lv_obj_t* start_img,*end_img;

const char*  start_name,* end_name;
const Station*  start_station,*end_station;
lv_obj_t* pp_window;
lv_obj_t* star_bt_label;
lv_obj_t* star_bt,* route_name,*cancel_btn;
lv_obj_t* favorite_station_show_disp,*favorite_route_show_disp;
lv_obj_t* favorite_station_show_lb[30];
lv_obj_t* favorite_station_line_show_lb[30];
lv_obj_t* favorite_station_line_transfer_show_lb[30];
lv_obj_t* favorite_station_line_transfer_show_lb2[30];
lv_obj_t* favorite_route_show_lb[10];
lv_obj_t* favorite_route_dlt_btn[10];
lv_obj_t* favorite_route_station_lb1[10];
lv_obj_t* favorite_route_station_lb2[10];
extern lv_obj_t* top_search_station[SEARCH_LIST_LEN];
extern lv_obj_t* top_search_line[SEARCH_LIST_LEN];
extern lv_obj_t* top_search_transfer[SEARCH_LIST_LEN];
extern lv_style_t flame_style,blue_button_style,blue_label_style, btn_style;
extern uint16_t favorite_ids[MAX_FAVORITES];
extern FavoriteRouteID favorite_routes[MAX_ROUTES];
extern size_t favorite_count,favorite_route_count;
static lv_point_t line_points[] = {{352,55},{352,599}};
extern LineinfoBtn line_info_btns[2];

int is_both_ta_filled = 0;
void ui_init(void)
{
    create_simple_label(&main_lb1,display1,100,86,96,27,"路线规划",&heiti_24);
    create_simple_label(&start_lb,display1,100,136,40,23,"起点",&heiti_20);
    create_simple_label(&end_lb,display1,100,226,40,23,"终点",&heiti_20);
    lv_obj_set_style_text_color(start_lb, lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_obj_set_style_text_color(end_lb, lv_color_hex(COLOR_DARK_BLUE), 0);

    create_simple_btn(&btn4,display1,100,336,212,38, lv_color_hex(COLOR_DARK_BLUE));
    btn4_lb = lv_label_create(btn4);
	lv_obj_set_flex_align(btn4, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_label_set_text(btn4_lb,"确定");
	lv_obj_set_style_text_font(btn4_lb,&heiti_16,LV_PART_MAIN);
	lv_obj_set_style_bg_color(btn4_lb, lv_color_hex(0xffffff), 0);
	lv_obj_add_event_cb(btn4, btn4_cb, LV_EVENT_CLICKED, NULL);

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
    lv_obj_set_style_text_font(start_ta, &heiti_16, 0);

    end_ta = lv_textarea_create(display1);
    lv_obj_set_pos(end_ta,101,255);
    lv_obj_set_size(end_ta,211,40);
    lv_obj_set_style_radius(end_ta,4,LV_PART_MAIN);
    lv_obj_add_event_cb(end_ta, end_ta_kb_show_cb, LV_EVENT_ALL, kb);
    lv_textarea_set_one_line(end_ta, true);          // 强制单行
    lv_obj_set_scrollbar_mode(end_ta, LV_SCROLLBAR_MODE_OFF); // 关掉滚动条
    lv_obj_set_style_text_font(end_ta, &heiti_16, 0);

}
void display11_init(void)
{
    display11 = lv_obj_create(display1);
    lv_obj_set_size(display11, 672, 545);
    lv_obj_set_pos(display11, 352, 55);
    lv_obj_set_style_bg_opa(display11, LV_OPA_0, 0);
    lv_obj_set_style_border_width(display11, 0, 0);
    lv_obj_set_style_border_width(display11, 0, 0);
    lv_obj_set_style_pad_all(display11, 0, 0);
    lv_obj_set_style_outline_width(display11, 0, 0);

    create_simple_label(&main_lb2,display11,43, 31, 96, 27,"我的收藏",&heiti_24);
    create_simple_label(&station_lb,display11, 44, 94, 40, 23,"车站",&heiti_20);
    create_simple_label(&route_lb,display11, 350, 94, 40, 23,"线路",&heiti_20);
    lv_obj_set_style_text_color(station_lb, lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_obj_set_style_text_color(route_lb, lv_color_hex(COLOR_DARK_BLUE), 0);
    create_simple_label(&station_prompt,display11, 42, 150, 140, 23,"暂无收藏车站!",&heiti_20);
    create_simple_label(&route_prompt,display11, 350, 150, 140, 23,"暂无收藏线路!",&heiti_20);
    lv_obj_move_foreground(display11);

}
void display12_init(void)
{
    display12 = lv_obj_create(display1);
    lv_obj_set_size(display12, 672, 545);
    lv_obj_set_pos(display12, 352, 55);
    lv_obj_set_style_bg_opa(display12, LV_OPA_0, 0);
    lv_obj_set_style_border_width(display12, 0, 0);
    lv_obj_set_style_pad_all(display12, 0, 0);
    lv_obj_set_style_outline_width(display12, 0, 0);

    create_simple_label(&display12_lb1,display12,17,17,80,23,"搜索结果",&heiti_20);
    lv_obj_set_style_text_color(display12_lb1,lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
}

void search_result_show_label_init(lv_obj_t* display)
{
    if (search_result_show_label[0] == NULL || search_line_show_label[0] == NULL)
    {
        for(int i = 0; i < SEARCH_LIST_LEN; i++) {
            search_result_show_label[i] = lv_label_create(display);
            lv_obj_set_style_border_color(search_result_show_label[i], lv_color_hex(COLOR_MID_GRAY), 0);
            lv_obj_set_style_border_width(search_result_show_label[i], 1, 0);
            lv_obj_set_style_pad_left(search_result_show_label[i], 16, 0);
            lv_obj_set_style_pad_top(search_result_show_label[i], 16, 0);
    
            lv_obj_set_pos(search_result_show_label[i], -1, 60 + i * 54);
            lv_obj_set_size(search_result_show_label[i], 673, 55);
            lv_obj_add_flag(search_result_show_label[i], LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_text_font(search_result_show_label[i], &heiti_16, LV_PART_MAIN);
            lv_label_set_text(search_result_show_label[i], "");
            lv_obj_add_flag(search_result_show_label[i],LV_OBJ_FLAG_HIDDEN);

            lv_obj_add_event_cb(search_result_show_label[i], search_result_click_cb, LV_EVENT_CLICKED, NULL);

            search_line_show_label[i] = lv_label_create(display);
            lv_obj_set_pos(search_line_show_label[i], 590, 71 + i * 54);
            lv_obj_set_size(search_line_show_label[i], 63, 28);
            lv_obj_set_style_radius(search_line_show_label[i],4,LV_PART_MAIN);
            lv_obj_set_style_bg_color(search_line_show_label[i], lv_color_hex(0xffffff), 0);
            lv_obj_set_style_bg_opa(search_line_show_label[i], LV_OPA_COVER, 0);
            lv_obj_add_flag(search_line_show_label[i],LV_OBJ_FLAG_HIDDEN);
            
            lv_obj_set_style_text_align(search_line_show_label[i], LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_pad_top(search_line_show_label[i],4, 0);
            lv_obj_set_style_text_font(search_line_show_label[i], &heiti_16, LV_PART_MAIN);
            lv_label_set_text(search_line_show_label[i], "");
            lv_obj_set_style_text_color(search_line_show_label[i],lv_color_hex(0xffffff), 0);

            search_line_transfer_show_label[i] = lv_label_create(display);
            lv_obj_set_pos(search_line_transfer_show_label[i], 517, 71 + i * 54);
            lv_obj_set_size(search_line_transfer_show_label[i], 63, 28);
            lv_obj_set_style_radius(search_line_transfer_show_label[i],4,LV_PART_MAIN);
            lv_obj_set_style_bg_color(search_line_transfer_show_label[i], lv_color_hex(0xffffff), 0);
            lv_obj_set_style_bg_opa(search_line_transfer_show_label[i], LV_OPA_COVER, 0);
            lv_obj_add_flag(search_line_transfer_show_label[i],LV_OBJ_FLAG_HIDDEN);
            
            lv_obj_set_style_text_align(search_line_transfer_show_label[i], LV_TEXT_ALIGN_CENTER, 0);
            lv_obj_set_style_pad_top(search_line_transfer_show_label[i],4, 0);
            lv_obj_set_style_text_font(search_line_transfer_show_label[i], &heiti_16, LV_PART_MAIN);
            lv_label_set_text(search_line_transfer_show_label[i], "");
            lv_obj_set_style_text_color(search_line_transfer_show_label[i],lv_color_hex(0xffffff), 0);
        }
    }
    else
    {
        for(int i = 0; i < SEARCH_LIST_LEN; i++) 
        {
            lv_label_set_text(search_result_show_label[i], "");
            lv_obj_add_flag(search_result_show_label[i],LV_OBJ_FLAG_HIDDEN);

            lv_label_set_text(search_line_show_label[i], "");
            // lv_obj_set_style_bg_color(search_line_show_label[i], lv_color_hex(0xffffff), 0);
            // lv_obj_set_style_bg_opa(search_line_show_label[i], LV_OPA_COVER, 0);
            lv_obj_add_flag(search_line_show_label[i],LV_OBJ_FLAG_HIDDEN);

            lv_label_set_text(search_line_transfer_show_label[i], "");
            // lv_obj_set_style_bg_color(search_line_transfer_show_label[i], lv_color_hex(0xffffff), 0);
            // lv_obj_set_style_bg_opa(search_line_transfer_show_label[i], LV_OPA_COVER, 0);
            lv_obj_add_flag(search_line_transfer_show_label[i],LV_OBJ_FLAG_HIDDEN);
        }
    }
}   

void search_result_show_label_set_text(char * text)
{
    int index = 0;
    search_result_show_label_init(display12);

    if (!*text){
        lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(display11, LV_OBJ_FLAG_HIDDEN);
        return;
    }
    const char* text_cstr = text; // 确保text是以'\0'结尾的字符串
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < metro_lines[i].count; j++)
        {
            if (!strncmp(metro_lines[i].stations[j].name_pinyin, text_cstr, strlen(text_cstr))
                && metro_lines[i].stations[j].is_transfer >= 0) 
            {
                lv_obj_clear_flag(search_result_show_label[index], LV_OBJ_FLAG_HIDDEN);                
                lv_label_set_text(search_result_show_label[index], metro_lines[i].stations[j].name);

                lv_label_set_text(search_line_show_label[index], 
                                            metro_lines[i].line_number == 1 ? "1号线" :
                                            metro_lines[i].line_number == 2 ? "2号线" :
                                            metro_lines[i].line_number == 3 ? "3号线" : "4号线");
                lv_obj_set_style_bg_color(search_line_show_label[index], lv_color_hex(metro_lines[i].line_color), 0);
                lv_obj_clear_flag(search_line_show_label[index], LV_OBJ_FLAG_HIDDEN);
                lv_obj_move_foreground(search_line_show_label[index]);

                if (metro_lines[i].stations[j].is_transfer)
                {
                    int8_t transfer_line = metro_lines[i].stations[j].is_transfer;
                    lv_label_set_text(search_line_transfer_show_label[index], 
                                            transfer_line == 1 ? "1号线" :
                                            transfer_line == 2 ? "2号线" :
                                            transfer_line == 3 ? "3号线" : "4号线");
                    lv_obj_set_style_bg_color(search_line_transfer_show_label[index], lv_color_hex(metro_lines[transfer_line-1].line_color), 0);
                    lv_obj_clear_flag(search_line_transfer_show_label[index], LV_OBJ_FLAG_HIDDEN);
                    lv_obj_move_foreground(search_line_transfer_show_label[index]);
                }

                index++;
                if (index >= SEARCH_LIST_LEN) 
                {
                    return; // 最多显示SEARCH_LIST_LEN条结果
                }
            }
        }
    }
}

void route_design_result(void)
{
    if (start_img == NULL || end_img == NULL)
    {
        start_img = lv_img_create(canvas);
        lv_img_set_src(start_img, "0:/location_start.bin");
        end_img = lv_img_create(canvas);
        lv_img_set_src(end_img, "0:/location_end.bin");
    }
    else
    {
        lv_obj_clear_flag(start_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(end_img, LV_OBJ_FLAG_HIDDEN);
    }
    start_station = find_station_by_name(start_name);
    end_station = find_station_by_name(end_name);

    lv_coord_t start_x = geo_to_screen(start_station->geo_x, origin_x);
    lv_coord_t start_y = geo_to_screen(start_station->geo_y, origin_y);
    lv_obj_clear_flag(start_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_pos(start_img, start_x - 38, start_y - 75);

    lv_coord_t end_x = geo_to_screen(end_station->geo_x, origin_x);
    lv_coord_t end_y = geo_to_screen(end_station->geo_y, origin_y);
    lv_obj_clear_flag(end_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_pos(end_img, end_x - 38, end_y - 75);

}
void pop_search_result_window_lineinfo_init(const Station *st1, const Station *st2,int index,int linebelonged)
{
    lv_obj_t* lineinfo = lv_obj_create(pp_window);
    lv_obj_set_size(lineinfo, 371, 168);
    lv_obj_set_pos(lineinfo, 0, 65 + index * 168);
    lv_obj_set_style_border_width(lineinfo, 0, 0);        
    lv_obj_set_style_pad_all(lineinfo, 0, 0);

    int pp_x = lv_obj_get_x(pp_window);  // 弹窗在屏幕的 X
    int pp_y = lv_obj_get_y(pp_window);  // 弹窗在屏幕的 Y
    //按钮的纵坐标
    int btn_y  =  btn_y = pp_y + 195 + index * 168;

    lv_obj_set_pos(cancel_btn, pp_x + 259, btn_y);
    lv_obj_move_foreground(cancel_btn);
    
    lv_obj_t* line_cl = lv_obj_create(lineinfo);
    lv_obj_set_size(line_cl, 6, 141);
    lv_obj_set_pos(line_cl, 30, 16);
    lv_obj_set_style_bg_color(line_cl, lv_color_hex(metro_lines[linebelonged-1].line_color), 0);
    lv_obj_set_style_border_width(line_cl, 0, 0);    
    lv_obj_set_style_bg_opa(line_cl, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(line_cl, 4, 0);

    lv_obj_t* st1_lb = lv_label_create(lineinfo);
    lv_obj_set_pos(st1_lb, 50, 16);
    lv_obj_set_style_text_font(st1_lb, &heiti_20, 0);
    lv_label_set_text(st1_lb, st1->name);

    lv_obj_t* st2_lb = lv_label_create(lineinfo);
    lv_obj_set_pos(st2_lb, 50, 16 + 141 -20);
    lv_obj_set_style_text_font(st2_lb, &heiti_20, 0);
    lv_label_set_text(st2_lb, st2->name);

    lv_obj_t* line_lb = lv_label_create(lineinfo);
    lv_obj_set_pos(line_lb, 50, 50);
    lv_obj_set_size(line_lb, 63, 29);
    lv_obj_set_style_radius(line_lb, 4, 0);
    lv_obj_set_style_bg_color(line_lb, lv_color_hex(metro_lines[linebelonged-1].line_color), 0);
    lv_obj_set_style_bg_opa(line_lb, LV_OPA_COVER, 0);
    lv_obj_set_style_text_align(line_lb, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_top(line_lb,5, 0);
    lv_obj_set_style_text_font(line_lb, &heiti_16, 0);
    lv_obj_set_style_text_color(line_lb,lv_color_hex(0xffffff), 0);
    lv_label_set_text(line_lb, 
                        metro_lines[linebelonged-1].line_number == 1 ? "1号线" :
                        metro_lines[linebelonged-1].line_number == 2 ? "2号线" :
                        metro_lines[linebelonged-1].line_number == 3 ? "3号线" : "4号线");
    
    lv_obj_t* direction = lv_label_create(lineinfo);
    lv_obj_set_pos(direction, 130, 55);
    lv_obj_set_style_text_font(direction, &heiti_16, 0);

    
    int id1,id2;
    for (int i = 0; i < metro_lines[linebelonged-1].count; i++)
    {
        if (metro_lines[linebelonged-1].stations[i].name_pinyin == st1->name_pinyin)
        {
            id1 = metro_lines[linebelonged-1].stations[i].id;
        }
        if (metro_lines[linebelonged-1].stations[i].name_pinyin == st2->name_pinyin)
        {
            id2 = metro_lines[linebelonged-1].stations[i].id;
        }
    }
    if (id2 > id1)
    {
        lv_label_set_text_fmt(direction, "%s 方向",
        metro_lines[linebelonged-1].stations[metro_lines[linebelonged-1].count - 1].name);
    }
    else
    {
        lv_label_set_text_fmt(direction, "%s 方向",
        metro_lines[linebelonged-1].stations[0].name);
    }

    lv_obj_t* time_lb = lv_label_create(lineinfo);
    lv_obj_set_pos(time_lb, 50, 100);
    lv_obj_set_style_text_font(time_lb, &heiti_16, 0);

    int sec = get_station_interval(&(metro_lines[linebelonged-1]), id1, id2);
    //timeToString(&period, period_str, MIN_MODE);

    lv_label_set_text_fmt(time_lb, "%d站 (%d分钟)",abs(id2 - id1), sec/60);
}
void pop_search_result_window_init(void)
{
    pp_window = lv_obj_create(display0);
    lv_obj_add_style(pp_window, &flame_style, 0);
    lv_obj_set_pos(pp_window, 100, 95);
    lv_obj_set_size(pp_window,371,451);
    lv_obj_add_flag(pp_window, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_scroll_dir(pp_window, LV_DIR_TOP | LV_DIR_BOTTOM);

    route_name = lv_label_create(pp_window);
    lv_obj_set_pos(route_name, 0, 0);
    lv_obj_set_size(route_name, 371, 54);
    lv_obj_set_style_bg_color(route_name, lv_color_hex(COLOR_LIGHT_BLUE), 0);
    lv_obj_set_style_bg_opa(route_name, LV_OPA_COVER, 0);
    lv_obj_set_style_text_align(route_name, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_pad_top(route_name, 17, 0);
    lv_obj_set_style_pad_left(route_name, 13 , 0);
    lv_obj_set_style_text_font(route_name, &heiti_20, 0);

    star_bt = lv_btn_create(pp_window);
    lv_obj_add_style(star_bt, &blue_button_style, 0);
    lv_obj_set_pos(star_bt, 286, 12);
    lv_obj_set_size(star_bt, 73, 30);
    lv_obj_add_event_cb(star_bt, favorites_route_button_event_cb, LV_EVENT_CLICKED, NULL);

    star_bt_label = lv_label_create(star_bt);
    lv_label_set_text(star_bt_label, "收藏");
    lv_obj_center(star_bt_label);
    lv_obj_set_style_text_font(star_bt_label, &heiti_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(star_bt_label,lv_color_hex(0xffffff), 0);

    cancel_btn = lv_btn_create(pp_window);
    lv_obj_set_size(cancel_btn, 90, 38);
    // lv_obj_align(cancel_btn, LV_ALIGN_BOTTOM_RIGHT, -22, -25);
    lv_obj_add_style(cancel_btn, &btn_style,0);
    lv_obj_move_foreground(cancel_btn);
    lv_obj_add_event_cb(cancel_btn, cancelbtn_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *label = lv_label_create(cancel_btn);
    lv_label_set_text(label, "完成");
    lv_obj_set_style_text_font(label, &heiti_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(label,lv_color_hex(COLOR_BLACK), 0);
    lv_obj_center(label);
}
void pop_search_result_window_show(Route* route)
{
    int flag = 1;//总共需要乘坐的线路数量
    const Station *route_show_result[MAX_TRANSFER_COUNT];
    int8_t linebelonged[MAX_TRANSFER_COUNT];

    if (route->step_count == 0) return;

    route_show_result[0] = find_station_by_name(route->steps[0].station_name);
    for (int i = 0; i < route->step_count; i++)
    {
        if (route->steps[i].action == 2) // 换乘站才显示在结果里
        {
            const Station *st = find_station_by_name(route->steps[i].station_name);
            linebelonged[flag - 1] = route->steps[i].line_number;
            route_show_result[flag++] = st;
        }
    }
    if (flag == 1)
    {
        lv_obj_set_size(pp_window,371,260);

    }

    linebelonged[flag - 1] = route->steps[route->step_count - 1].line_number;
    route_show_result[flag] = find_station_by_name(route->steps[route->step_count - 1].station_name);
    lv_label_set_text_fmt(route_name, "%s-%s", route_show_result[0]->name, route_show_result[flag]->name);

    for (int i = 0; i + 1 <= flag; i++)
    {
        pop_search_result_window_lineinfo_init(route_show_result[i], route_show_result[i+1],i ,linebelonged[i]);
    }
    lv_obj_clear_flag(pp_window, LV_OBJ_FLAG_HIDDEN);
    
    lv_obj_clear_flag(start_img, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(end_img, LV_OBJ_FLAG_HIDDEN);

    //更新标志位
    is_start_selected = 1;
    is_end_selected = 1;
    is_route_showing = 1;

    favorites_route_bind_button(star_bt, star_bt_label,
                                 route_show_result[0], route_show_result[flag]);
}
void favorite_station_init(void)
{
    favorite_station_show_disp = lv_obj_create(display11);
    lv_obj_set_pos(favorite_station_show_disp, 44, 144);
    lv_obj_set_size(favorite_station_show_disp, 263, 336);
    lv_obj_set_style_bg_color(favorite_station_show_disp, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_border_width(favorite_station_show_disp, 0, 0);
    lv_obj_set_scroll_dir(favorite_station_show_disp, LV_DIR_TOP | LV_DIR_BOTTOM);
    lv_obj_set_style_bg_opa(favorite_station_show_disp, LV_OPA_0, 0);
    lv_obj_set_style_pad_all(favorite_station_show_disp, 0, 0);
    lv_obj_set_scrollbar_mode(favorite_station_show_disp, LV_SCROLLBAR_MODE_OFF);

    for(int i = 0; i < 30; i++) 
    {
        favorite_station_show_lb[i] = lv_label_create(favorite_station_show_disp);
        lv_obj_set_style_border_color(favorite_station_show_lb[i], lv_color_hex(COLOR_MID_BLUE), 0);
        lv_obj_set_style_text_color(favorite_station_show_lb[i], lv_color_black(),0);
        lv_obj_set_style_bg_color(favorite_station_show_lb[i], lv_color_hex(COLOR_BG_BLUE), 0);
        lv_obj_set_style_bg_opa(favorite_station_show_lb[i], LV_OPA_100, 0);
        lv_obj_set_style_border_width(favorite_station_show_lb[i], 1, 0);
        lv_obj_set_style_pad_left(favorite_station_show_lb[i], 16, 0);
        lv_obj_set_style_pad_top(favorite_station_show_lb[i], 16, 0);
        lv_obj_set_style_radius(favorite_station_show_lb[i],4,LV_PART_MAIN);

        lv_obj_set_pos(favorite_station_show_lb[i], 0, i * 64);
        lv_obj_set_size(favorite_station_show_lb[i], 263, 55);
        lv_obj_add_flag(favorite_station_show_lb[i], LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_text_font(favorite_station_show_lb[i], &heiti_20, LV_PART_MAIN);
        lv_label_set_text(favorite_station_show_lb[i], "");
        lv_obj_add_flag(favorite_station_show_lb[i],LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_event_cb(favorite_station_show_lb[i], station_info_click_cb, LV_EVENT_CLICKED, NULL);

        favorite_station_line_show_lb[i] = lv_label_create(favorite_station_show_disp);
        lv_obj_set_pos(favorite_station_line_show_lb[i], 185, 15 + i * 64);
        lv_obj_set_size(favorite_station_line_show_lb[i], 63, 29);
        lv_obj_set_style_radius(favorite_station_line_show_lb[i],4,LV_PART_MAIN);
        lv_obj_set_style_bg_color(favorite_station_line_show_lb[i], lv_color_hex(0xffffff), 0);
        lv_obj_set_style_bg_opa(favorite_station_line_show_lb[i], LV_OPA_COVER, 0);
        
        lv_obj_set_style_text_align(favorite_station_line_show_lb[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_top(favorite_station_line_show_lb[i],4, 0);
        lv_obj_set_style_text_font(favorite_station_line_show_lb[i], &heiti_16, LV_PART_MAIN);
        lv_label_set_text(favorite_station_line_show_lb[i], "");
        lv_obj_set_style_text_color(favorite_station_line_show_lb[i],lv_color_hex(0xffffff), 0);

        favorite_station_line_transfer_show_lb[i] = lv_label_create(favorite_station_show_disp);
        lv_obj_set_pos(favorite_station_line_transfer_show_lb[i], 182, 15 + i * 64);
        lv_obj_set_size(favorite_station_line_transfer_show_lb[i], 29, 29);
        lv_obj_set_style_radius(favorite_station_line_transfer_show_lb[i],4,LV_PART_MAIN);
        lv_obj_set_style_bg_color(favorite_station_line_transfer_show_lb[i], lv_color_hex(0xffffff), 0);
        lv_obj_set_style_bg_opa(favorite_station_line_transfer_show_lb[i], LV_OPA_COVER, 0);
        
        lv_obj_set_style_text_align(favorite_station_line_transfer_show_lb[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_top(favorite_station_line_transfer_show_lb[i],4, 0);
        lv_obj_set_style_text_font(favorite_station_line_transfer_show_lb[i], &heiti_16, LV_PART_MAIN);
        lv_label_set_text(favorite_station_line_transfer_show_lb[i], "");
        lv_obj_set_style_text_color(favorite_station_line_transfer_show_lb[i],lv_color_hex(0xffffff), 0);

        favorite_station_line_transfer_show_lb2[i] = lv_label_create(favorite_station_show_disp);
        lv_obj_set_pos(favorite_station_line_transfer_show_lb2[i], 221, 15 + i * 64);
        lv_obj_set_size(favorite_station_line_transfer_show_lb2[i], 29, 29);
        lv_obj_set_style_radius(favorite_station_line_transfer_show_lb2[i],4,LV_PART_MAIN);
        lv_obj_set_style_bg_color(favorite_station_line_transfer_show_lb2[i], lv_color_hex(0xffffff), 0);
        lv_obj_set_style_bg_opa(favorite_station_line_transfer_show_lb2[i], LV_OPA_COVER, 0);
        
        lv_obj_set_style_text_align(favorite_station_line_transfer_show_lb2[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_top(favorite_station_line_transfer_show_lb2[i],4, 0);
        lv_obj_set_style_text_font(favorite_station_line_transfer_show_lb2[i], &heiti_16, LV_PART_MAIN);
        lv_label_set_text(favorite_station_line_transfer_show_lb2[i], "");
        lv_obj_set_style_text_color(favorite_station_line_transfer_show_lb2[i],lv_color_hex(0xffffff), 0);
    }
}
void favorite_station_show(void)
{
    for (int i = 0; i < 30; i++)
    {
        lv_obj_add_flag(favorite_station_show_lb[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(favorite_station_line_show_lb[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(favorite_station_line_transfer_show_lb[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(favorite_station_line_transfer_show_lb2[i], LV_OBJ_FLAG_HIDDEN);
    }

    for (int i = 0; i < favorite_count && i < 30; i++)
    {
        const Station* station_temp = find_station_by_id(favorite_ids[i]);

        if (station_temp == NULL) 
        {
            continue;
        }

        lv_obj_clear_flag(favorite_station_show_lb[i], LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(favorite_station_show_lb[i], station_temp->name);

        int line_belonged = station_temp->line_belonged;
        if (line_belonged > 0) 
        {
            lv_label_set_text(favorite_station_line_show_lb[i],
                                        line_belonged == 1 ? "1号线" :
                                        line_belonged == 2 ? "2号线" :
                                        line_belonged == 3 ? "3号线" : "4号线");
            lv_obj_set_style_bg_color(favorite_station_line_show_lb[i], lv_color_hex(metro_lines[line_belonged-1].line_color), 0);
            lv_obj_clear_flag(favorite_station_line_show_lb[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_foreground(favorite_station_line_show_lb[i]);
        }

        if (station_temp->is_transfer) 
        {
            int transfer_line = station_temp->is_transfer;
            lv_obj_add_flag(favorite_station_line_show_lb[i], LV_OBJ_FLAG_HIDDEN);

            lv_label_set_text_fmt(favorite_station_line_transfer_show_lb2[i], "%d", transfer_line);
            lv_label_set_text_fmt(favorite_station_line_transfer_show_lb[i], "%d", line_belonged);
            lv_obj_set_style_bg_color(favorite_station_line_transfer_show_lb2[i], lv_color_hex(metro_lines[transfer_line-1].line_color), 0);
            lv_obj_set_style_bg_color(favorite_station_line_transfer_show_lb[i], lv_color_hex(metro_lines[line_belonged-1].line_color), 0);
            lv_obj_clear_flag(favorite_station_line_transfer_show_lb[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(favorite_station_line_transfer_show_lb2[i], LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_foreground(favorite_station_line_transfer_show_lb[i]);
        }
    }
    if (favorite_count == 0)
    {
        lv_obj_clear_flag(station_prompt, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(station_prompt, LV_OBJ_FLAG_HIDDEN);
    }
}


void favorite_station_lb_style_reset(void)
{
    for (int i = 0; i < 30; i++)
    {
        lv_obj_set_style_bg_color(favorite_station_show_lb[i], lv_color_hex(COLOR_BG_BLUE), 0);
        lv_obj_set_style_text_color(favorite_station_show_lb[i], lv_color_black(),0);
    }
}

void favorite_route_ui_init(void)
{
    favorite_route_show_disp = lv_obj_create(display11);
    lv_obj_set_pos(favorite_route_show_disp, 350, 144);
    lv_obj_set_size(favorite_route_show_disp, 285, 336);
    lv_obj_set_style_bg_color(favorite_route_show_disp, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_border_width(favorite_route_show_disp, 0, 0);
    lv_obj_set_scroll_dir(favorite_route_show_disp, LV_DIR_TOP | LV_DIR_BOTTOM);
    lv_obj_set_style_bg_opa(favorite_route_show_disp, LV_OPA_0, 0);
    lv_obj_set_style_pad_all(favorite_route_show_disp, 0, 0);
    lv_obj_set_scrollbar_mode(favorite_route_show_disp, LV_SCROLLBAR_MODE_OFF);

    for(int i = 0; i < 10; i++) 
    {
        favorite_route_show_lb[i] = lv_label_create(favorite_route_show_disp);
        lv_obj_set_style_border_color(favorite_route_show_lb[i], lv_color_hex(COLOR_MID_BLUE), 0);
        // lv_obj_set_style_text_color(favorite_route_show_lb[i], lv_color_black(),0);
        lv_obj_set_style_bg_color(favorite_route_show_lb[i], lv_color_hex(COLOR_BG_BLUE), 0);
        lv_obj_set_style_bg_opa(favorite_route_show_lb[i], LV_OPA_100, 0);
        lv_obj_set_style_border_width(favorite_route_show_lb[i], 1, 0);
        // lv_obj_set_style_pad_left(favorite_route_show_lb[i], 16, 0);
        // lv_obj_set_style_pad_top(favorite_route_show_lb[i], 16, 0);
        lv_obj_set_style_radius(favorite_route_show_lb[i],4,LV_PART_MAIN);

        lv_obj_set_pos(favorite_route_show_lb[i], 0, i * 104);
        lv_obj_set_size(favorite_route_show_lb[i], 285, 98);
        lv_obj_add_flag(favorite_route_show_lb[i], LV_OBJ_FLAG_CLICKABLE);
        lv_label_set_text(favorite_route_show_lb[i], "");
        lv_obj_add_flag(favorite_route_show_lb[i],LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_event_cb(favorite_route_show_lb[i], route_info_click_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t* qidian;
        create_simple_label(&qidian,favorite_route_show_lb[i], 18, 22,33,18,"起点",&heiti_16);
        lv_obj_set_style_text_color(qidian, lv_color_hex(COLOR_DARK_BLUE), 0);
        lv_obj_t* zhongdian;
        create_simple_label(&zhongdian,favorite_route_show_lb[i], 18, 56,33,18,"终点",&heiti_16);
        lv_obj_set_style_text_color(zhongdian, lv_color_hex(COLOR_DARK_BLUE), 0);

        favorite_route_dlt_btn[i] = lv_btn_create(favorite_route_show_lb[i]);
        lv_obj_set_pos(favorite_route_dlt_btn[i], 222, 29);
        lv_obj_set_size(favorite_route_dlt_btn[i], 42, 40);
        lv_obj_set_style_border_color(favorite_route_dlt_btn[i], lv_color_hex(COLOR_DARK_BLUE), 0);
        lv_obj_set_style_border_width(favorite_route_dlt_btn[i], 1, 0);
        lv_obj_set_style_bg_color(favorite_route_dlt_btn[i], lv_color_hex(COLOR_LIGHT_BLUE), 0);
        lv_obj_set_style_bg_opa(favorite_route_dlt_btn[i], LV_OPA_COVER, 0);
        lv_obj_set_style_radius(favorite_route_dlt_btn[i], 4, 0);
        lv_obj_add_flag(favorite_route_dlt_btn[i], LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_event_cb(favorite_route_dlt_btn[i], route_info_delete_click_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_t* dlt_img = lv_img_create(favorite_route_dlt_btn[i]);
        lv_img_set_src(dlt_img, "0:/trash_bin.bin");
        lv_obj_center(dlt_img);

        favorite_route_station_lb1[i] = lv_label_create(favorite_route_show_lb[i]);
        lv_obj_set_pos(favorite_route_station_lb1[i], 67, 19);
        lv_obj_set_size(favorite_route_station_lb1[i], 160, 23);
        lv_label_set_text(favorite_route_station_lb1[i], "");
        lv_obj_set_style_text_font(favorite_route_station_lb1[i], &heiti_20, LV_PART_MAIN);
        lv_obj_add_flag(favorite_route_station_lb1[i], LV_OBJ_FLAG_HIDDEN);

        favorite_route_station_lb2[i] = lv_label_create(favorite_route_show_lb[i]);
        lv_obj_set_pos(favorite_route_station_lb2[i], 67, 54);
        lv_obj_set_size(favorite_route_station_lb2[i], 160, 23);
        lv_label_set_text(favorite_route_station_lb2[i], "");
        lv_obj_set_style_text_font(favorite_route_station_lb2[i], &heiti_20, LV_PART_MAIN);
        lv_obj_add_flag(favorite_route_station_lb2[i], LV_OBJ_FLAG_HIDDEN);
    }
}
void favorite_route_show(void)
{
    for (int i = 0; i < 10; i++)
    {
        lv_obj_add_flag(favorite_route_show_lb[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(favorite_route_dlt_btn[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(favorite_route_station_lb1[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(favorite_route_station_lb2[i], LV_OBJ_FLAG_HIDDEN);
    }

    lv_obj_scroll_to_y(favorite_route_show_disp, 0, LV_ANIM_OFF);

    for (int i = 0; i < favorite_route_count && i < 10; i++)
    {
        const Station* station_temp1 = find_station_by_id(favorite_routes[i].start_id);
        const Station* station_temp2 = find_station_by_id(favorite_routes[i].end_id);

        if (station_temp1 == NULL || station_temp2 == NULL)
        { 
            continue;
        }

        lv_label_set_text_fmt(favorite_route_station_lb1[i], "%s", station_temp1->name);
        lv_label_set_text_fmt(favorite_route_station_lb2[i], "%s", station_temp2->name);

        lv_obj_clear_flag(favorite_route_show_lb[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(favorite_route_dlt_btn[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(favorite_route_station_lb1[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(favorite_route_station_lb2[i], LV_OBJ_FLAG_HIDDEN);
    }

    if (favorite_route_count == 0)
    {
        lv_obj_clear_flag(route_prompt, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
        lv_obj_add_flag(route_prompt, LV_OBJ_FLAG_HIDDEN);
    }
}
//回调函数
void start_ta_kb_show_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        clear_top_search_result_labels();
        kb_show(kb,ta,lv_color_hex(0xffffff));
        lv_obj_set_style_bg_opa(transparent, 0, 0);
        lv_obj_move_foreground(ta);
        lv_obj_move_foreground(start_lb);
        lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(display11, LV_OBJ_FLAG_HIDDEN);

        hide_pop_window();
        favorite_station_lb_style_reset();
    }
}
void end_ta_kb_show_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        clear_top_search_result_labels();
        kb_show(kb,ta,lv_color_hex(0xffffff));
        lv_obj_set_style_bg_opa(transparent, 0, 0);
        lv_obj_move_foreground(ta);
        lv_obj_move_foreground(end_lb);
        lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(display11, LV_OBJ_FLAG_HIDDEN);

        hide_pop_window();
        favorite_station_lb_style_reset();
    }
}

void search_result_click_cb(lv_event_t *e)
{
    lv_obj_t* label = lv_event_get_target(e);
    const char* station_name = lv_label_get_text(label);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < metro_lines[i].count; j++)
        {
            if (!strcmp(metro_lines[i].stations[j].name, station_name))
            {
                //station_name = metro_lines[i].stations[j].name_pinyin;
                station_name = metro_lines[i].stations[j].name;
                 break;
            }
        }
    }
    lv_obj_t * current_ta = lv_keyboard_get_textarea(kb);
    if (current_ta == start_ta)
    {
        lv_textarea_set_text(start_ta, station_name);
    }
    if (current_ta == end_ta)
    {
        lv_textarea_set_text(end_ta, station_name);
    }
    lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(display11, LV_OBJ_FLAG_HIDDEN);
}
void station_info_click_cb(lv_event_t *e)
{
    lv_obj_t* label = lv_event_get_target(e);
    const char* station_name = lv_label_get_text(label);
    const Station* station = find_station_by_name(station_name);
    if (station == NULL) {
        return;
    }
    if (!is_station_clicked)
    {
        lv_obj_set_style_bg_color(label, lv_color_hex(COLOR_DARK_BLUE), 0);
        
        lv_obj_set_style_text_color(label, lv_color_white(),0);


        station_clicked[0] = station;
        station_clicked[2] = get_transfer_station(station, &station_clicked[1]);

        pop_window_show(station_clicked, line_info_btns);
        lv_obj_clear_flag(pop_window, LV_OBJ_FLAG_HIDDEN);
        
        lv_obj_set_x(pop_window, 681);
        lv_obj_set_y(pop_window, 197);
        is_station_clicked = 1;
    }
    else
    {
        if (lv_obj_get_style_bg_color(label, LV_PART_MAIN).full == lv_color_hex(COLOR_DARK_BLUE).full 
                && !lv_obj_has_flag(pop_window, LV_OBJ_FLAG_HIDDEN))
        {
            lv_obj_set_style_bg_color(label, lv_color_hex(COLOR_BG_BLUE), 0);
            lv_obj_set_style_text_color(label, lv_color_black(),0);

            lv_obj_add_flag(pop_window, LV_OBJ_FLAG_HIDDEN);
            is_station_clicked = 0;
        }
        else
        {
            for (int i = 0; i < 30; i++)
            {
                lv_obj_set_style_bg_color(favorite_station_show_lb[i], lv_color_hex(COLOR_BG_BLUE), 0);
                lv_obj_set_style_text_color(favorite_station_show_lb[i], lv_color_black(),0);

            }
            lv_obj_set_style_bg_color(label, lv_color_hex(COLOR_DARK_BLUE), 0);
            lv_obj_set_style_text_color(label, lv_color_white(),0);

            station_clicked[0] = station;
            station_clicked[2] = get_transfer_station(station, &station_clicked[1]);

            pop_window_show(station_clicked, line_info_btns);
            lv_obj_clear_flag(pop_window, LV_OBJ_FLAG_HIDDEN);
    
            lv_obj_set_x(pop_window, 681);
            lv_obj_set_y(pop_window, 197);
            is_station_clicked = 1;
            
        }
        
    }
    
}
void route_info_click_cb(lv_event_t *e)
{
    lv_obj_t* label = lv_event_get_target(e);
    lv_obj_t* station_lb1 = lv_obj_get_child(label, 3);
    lv_obj_t* station_lb2 = lv_obj_get_child(label, 4);
    char* start_name = lv_label_get_text(station_lb1);
    char* end_name = lv_label_get_text(station_lb2);

    if (strlen(start_name) == 0 || strlen(end_name) == 0) 
    {
        return;
    }
    
    lv_textarea_set_text(end_ta, end_name);
    lv_textarea_set_text(start_ta, start_name);
}
void route_info_delete_click_cb(lv_event_t *e)
{
    lv_obj_t* btn = lv_event_get_target(e);
    lv_obj_t* station_lb1 = lv_obj_get_child(lv_obj_get_parent(btn), 3);
    lv_obj_t* station_lb2 = lv_obj_get_child(lv_obj_get_parent(btn), 4);
    char* start_name = lv_label_get_text(station_lb1);
    char* end_name = lv_label_get_text(station_lb2);
    const Station* start_station = find_station_by_name(start_name);
    const Station* end_station = find_station_by_name(end_name);

    if (strlen(start_name) == 0 || strlen(end_name) == 0) 
    {
        // 显示错误提示
        return;
    }

    favorites_route_toggle(start_station->only_id, end_station->only_id);
    favorite_route_show();
}
// 确定按钮回调函数 - 路线规划
void btn4_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        if (lv_obj_has_flag(pp_window,LV_OBJ_FLAG_HIDDEN))  //判断ppwindow是否已经自动生成
        {
            start_name = lv_textarea_get_text(start_ta);
            end_name = lv_textarea_get_text(end_ta);
    
            if (strlen(start_name) == 0 || strlen(end_name) == 0) {
                // 显示错误提示
                return;
            }
    
            // 查找路线
            is_both_ta_filled = 1;
            find_route(start_name, end_name, &route_result);
            // 设计并显示路线结果界面
            
            if (route_result.step_count > 0) 
            {
                // 跳转到主界面 (display0)
                // 在主界面上显示路线结果
                route_design_result();
                pop_search_result_window_init();
                pop_search_result_window_show(&route_result);
                lv_scr_load(display0);
            } 
            else 
            {
                // // 显示未找到路径提示
                // lv_obj_add_flag(display11, LV_OBJ_FLAG_HIDDEN);
                // lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
    
                // lv_obj_t* error_display = lv_obj_create(display1);
                // lv_obj_set_size(error_display, 672, 545);
                // lv_obj_set_pos(error_display, 352, 55);
                // lv_obj_set_style_bg_color(error_display, lv_color_hex(0xffffff), 0);
    
                // lv_obj_t* error_label = lv_label_create(error_display);
                // lv_obj_set_pos(error_label, 10, 250);
                // lv_obj_set_size(error_label, 652, 50);
                // lv_obj_set_style_text_color(error_label, lv_color_hex(COLOR_DARK_BLUE), 0);
                // lv_obj_set_style_text_font(error_label, &heiti_20, 0);
                // lv_label_set_text(error_label, "未找到有效路径，请检查站点名称");
                return;
            }
        }
        else
        {
            lv_scr_load(display0);
        }
        
    }
}
void cancelbtn_cb(lv_event_t *e)
{
    del_pp_window();

    if (start_img != NULL && end_img != NULL)
    {
        lv_obj_add_flag(start_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(end_img, LV_OBJ_FLAG_HIDDEN);
    }

    is_route_showing = 0;
    is_start_selected = 0;
    is_end_selected = 0;

    create_metro_map();//强制重绘
}

void clear_top_search_result_labels(void) 
{
    for (int i = 0; i < SEARCH_LIST_LEN; i++)
	{
		if (top_search_station[i] != NULL || top_search_line[i] != NULL || top_search_transfer[i] != NULL)
		{
			lv_obj_del_async(top_search_station[i]);
			top_search_station[i] = NULL;
			lv_obj_del_async(top_search_line[i]);
			top_search_line[i] = NULL;
			lv_obj_del_async(top_search_transfer[i]);
			top_search_transfer[i] = NULL;
    	}
	}
}

void del_pp_window()
{
    if (pp_window != NULL)
    {
        favorites_route_invalidate_binding(star_bt);
        lv_obj_del_async(pp_window);
        pp_window = NULL;
        route_name = NULL;
        cancel_btn = NULL;
        star_bt = NULL;
        star_bt_label = NULL;
    }
}
