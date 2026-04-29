#include "gui.h"
#include "canvas.h"
#include "route_planner.h"
void start_ta_kb_show_cb(lv_event_t *e);
void end_ta_kb_show_cb(lv_event_t *e);
void btn4_cb(lv_event_t *e); // 确定按钮回调

// 全局变量，用于跨界面传递路线信息
RouteResult last_route_result;
char last_route_desc[1024] = {0};

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
extern lv_obj_t* top_search_station[SEARCH_LIST_LEN];
extern lv_obj_t* top_search_line[SEARCH_LIST_LEN];
extern lv_obj_t* top_search_transfer[SEARCH_LIST_LEN];
static lv_point_t line_points[] = {{352,55},{352,599}};
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
void route_design_result(const char* start_name, const char* end_name)
{
    start_img = lv_img_create(display0);
    lv_img_set_src(start_img, "0:/location_start.bin");
    lv_obj_add_flag(start_img, LV_OBJ_FLAG_HIDDEN);
    end_img = lv_img_create(display0);
    lv_img_set_src(end_img, "0:/location_end.bin");
    lv_obj_add_flag(end_img, LV_OBJ_FLAG_HIDDEN);

    const Station* start_station = find_station_by_name(start_name);
    const Station* end_station = find_station_by_name(end_name);
    lv_coord_t start_x = geo_to_screen(start_station->geo_x, origin_x);
    lv_coord_t start_y = geo_to_screen(start_station->geo_y, origin_y);
    lv_obj_set_pos(start_img, start_x - 38, start_y - 75);
    lv_obj_clear_flag(start_img, LV_OBJ_FLAG_HIDDEN);
    lv_coord_t end_x = geo_to_screen(end_station->geo_x, origin_x);
    lv_coord_t end_y = geo_to_screen(end_station->geo_y, origin_y);
    lv_obj_set_pos(end_img, end_x - 38, end_y - 75);
    lv_obj_clear_flag(end_img, LV_OBJ_FLAG_HIDDEN);
}

//回调函数
void start_ta_kb_show_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
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
        kb_show(kb,ta,lv_color_hex(0xffffff));
        lv_obj_set_style_bg_opa(transparent, 0, 0);
        lv_obj_move_foreground(ta);
        lv_obj_move_foreground(start_lb);
        lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(display11, LV_OBJ_FLAG_HIDDEN);
    }
}
void end_ta_kb_show_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
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
        kb_show(kb,ta,lv_color_hex(0xffffff));
        lv_obj_set_style_bg_opa(transparent, 0, 0);
        lv_obj_move_foreground(ta);
        lv_obj_move_foreground(end_lb);
        lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(display11, LV_OBJ_FLAG_HIDDEN);
    }
}

// 确定按钮回调函数 - 路线规划
void btn4_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED)
    {
        const char* start_name = lv_textarea_get_text(start_ta);
        const char* end_name = lv_textarea_get_text(end_ta);

        if (strlen(start_name) == 0 || strlen(end_name) == 0) {
            // 显示错误提示
            return;
        }

        // 查找路线
        RouteResult result = find_route(start_name, end_name);

        if (result.route_count > 0 && result.routes[0].step_count > 0) {
            // 保存路线结果到全局变量
            last_route_result = result;
            get_route_description(&result.routes[0], last_route_desc, sizeof(last_route_desc));

            // 跳转到主界面 (display0)
            route_design_result(start_name, end_name);
            lv_scr_load(display0);

            // 在主界面上显示路线结果
            // 这里需要在主界面的代码中处理显示路线结果
        } else {
            // 显示未找到路径提示
            lv_obj_add_flag(display11, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(display12, LV_OBJ_FLAG_HIDDEN);

            lv_obj_t* error_display = lv_obj_create(display1);
            lv_obj_set_size(error_display, 672, 545);
            lv_obj_set_pos(error_display, 352, 55);
            lv_obj_set_style_radius(error_display, 4, 0);
            lv_obj_set_style_bg_color(error_display, lv_color_hex(0xffffff), 0);

            lv_obj_t* error_label = lv_label_create(error_display);
            lv_obj_set_pos(error_label, 10, 250);
            lv_obj_set_size(error_label, 652, 50);
            lv_obj_set_style_text_color(error_label, lv_color_hex(COLOR_DARK_BLUE), 0);
            lv_obj_set_style_text_font(error_label, &heiti_20, 0);
            lv_label_set_text(error_label, "未找到有效路径，请检查站点名称");
        }
    }
}
