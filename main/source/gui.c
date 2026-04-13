#include "gui.h"

void btn1_cb(lv_event_t *e);
void btn2_cb(lv_event_t *e);
void btn3_cb(lv_event_t *e);
void kb_show_cb(lv_event_t *e);
void kb_hide_cb(lv_event_t *e);
void keyBoard_event_cb(lv_event_t *e);
void screen_load_event_cb(lv_event_t *e);


lv_obj_t* label_left,*label_top;
lv_obj_t* lb_t1;
lv_obj_t* btn1,*btn2,*btn3,*btn_search;
lv_obj_t* ta,*ta_lb1,*ta_lb2;
lv_obj_t* logo;
lv_obj_t* map_blue,*map_white;
lv_obj_t* line_blue,*line_white;
lv_obj_t* bell_blue,*bell_white;
lv_obj_t* map_lb,*bell_lb,*line_lb,*search_lb;
lv_obj_t* transparent = NULL;

static char time_buf[48];
void timetable_init(void)
{
	timetable1 = lv_label_create(display0);
	lv_obj_set_pos(timetable1,710,20);
	lv_obj_set_size(timetable1,280,24);
	lv_obj_set_style_text_font(timetable1,&heiti_20,LV_PART_MAIN);
	timetable2 = lv_label_create(display1);
	lv_obj_set_pos(timetable2,710,20);
	lv_obj_set_size(timetable2,280,24);
	lv_obj_set_style_text_font(timetable2,&heiti_20,LV_PART_MAIN);
	timetable3 = lv_label_create(display2);
	lv_obj_set_pos(timetable3,710,20);
	lv_obj_set_size(timetable3,280,24);
	lv_obj_set_style_text_font(timetable3,&heiti_20,LV_PART_MAIN);
}
void time_set(void)
{
	get_rtc_time_string(time_buf, sizeof(time_buf));
	lv_label_set_text(timetable1, time_buf);
	lv_label_set_text(timetable2, time_buf);
	lv_label_set_text(timetable3, time_buf);
}
void twocolumns(lv_obj_t* display0)
{
	label_left = create_simple_label(display0,0,55,60,545,"",NULL);
	lv_obj_set_style_bg_color(label_left, lv_color_hex(0x1a51), 0);
	lv_obj_set_style_bg_opa(label_left, LV_OPA_COVER, 0);

	label_top = create_simple_label(display0,0,0,1024,55,"",NULL);
	lv_obj_set_style_bg_color(label_top, lv_color_hex(0xd2dbe7), 0);
	lv_obj_set_style_bg_opa(label_top, LV_OPA_COVER,0);
}
void create_buttons(lv_obj_t* display0,int judge)
{
	btn1 = lv_btn_create(display0);
	lv_obj_set_pos(btn1, 6, 70);
	lv_obj_set_size(btn1, 50, 66);
	lv_obj_set_style_radius(btn1,4,LV_PART_MAIN);
	lv_obj_add_event_cb(btn1,btn1_cb,LV_EVENT_ALL,NULL);
	map_lb = create_simple_label(display0,13,107,33,19,"地图",&heiti_16);

	btn2 = lv_btn_create(display0);
	lv_obj_set_pos(btn2, 6, 144);
	lv_obj_set_size(btn2, 50, 66);
	lv_obj_set_style_radius(btn2,4,LV_PART_MAIN);
	lv_obj_add_event_cb(btn2,btn2_cb,LV_EVENT_ALL,NULL);
	line_lb = create_simple_label(display0,13,186,33,19,"线路",&heiti_16);

	btn3 = lv_btn_create(display0);
	lv_obj_set_pos(btn3, 6, 222);
	lv_obj_set_size(btn3, 50, 66);
	lv_obj_set_style_radius(btn3,4,LV_PART_MAIN);
	lv_obj_add_event_cb(btn3,btn3_cb,LV_EVENT_ALL,NULL);
	bell_lb = create_simple_label(display0,13,261,33,19,"提醒",&heiti_16);

	btn_search = create_simple_btn(display0,544,10,73,35,lv_color_hex(0x3f6ead));
	search_lb = lv_label_create(btn_search);
	lv_obj_set_flex_align(btn_search, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
	lv_label_set_text(search_lb,"搜索");
	lv_obj_set_style_text_font(search_lb,&heiti_20,LV_PART_MAIN);
	lv_obj_set_style_bg_color(search_lb, lv_color_hex(0xffffff), 0);

	switch (judge)
	{
	case 0:
		lv_obj_set_style_bg_color(btn1,lv_color_hex(0xd2dbe7),LV_PART_MAIN);
		lv_obj_set_style_bg_color(btn2,lv_color_hex(0x3b75),LV_PART_MAIN);
		lv_obj_set_style_bg_color(btn3,lv_color_hex(0x3b75),LV_PART_MAIN);
		lv_obj_set_style_text_color(map_lb,lv_color_hex(0x194888), 0);
		lv_obj_set_style_text_color(line_lb,lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_color(bell_lb,lv_color_hex(0xffffff), 0);
		break;
	case 1:
		lv_obj_set_style_bg_color(btn2,lv_color_hex(0xd2dbe7),LV_PART_MAIN);
		lv_obj_set_style_bg_color(btn1,lv_color_hex(0x3b75),LV_PART_MAIN);
		lv_obj_set_style_bg_color(btn3,lv_color_hex(0x3b75),LV_PART_MAIN);
		lv_obj_set_style_text_color(line_lb,lv_color_hex(0x194888), 0);
		lv_obj_set_style_text_color(map_lb,lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_color(bell_lb,lv_color_hex(0xffffff), 0);
		break;
	case 2:
		lv_obj_set_style_bg_color(btn3,lv_color_hex(0xd2dbe7),LV_PART_MAIN);
		lv_obj_set_style_bg_color(btn1,lv_color_hex(0x3b75),LV_PART_MAIN);
		lv_obj_set_style_bg_color(btn2,lv_color_hex(0x3b75),LV_PART_MAIN);
		lv_obj_set_style_text_color(bell_lb,lv_color_hex(0x194888), 0);
		lv_obj_set_style_text_color(line_lb,lv_color_hex(0xffffff), 0);
		lv_obj_set_style_text_color(map_lb,lv_color_hex(0xffffff), 0);
	default:
		break;
	}
}

void creat_top_ta(lv_obj_t* display0)
{
	ta = lv_textarea_create(display0);
	lv_obj_set_pos(ta,264,8);
	lv_obj_set_size(ta,266,34);
	lv_obj_set_style_radius(ta,4,LV_PART_MAIN);
	lv_textarea_set_one_line(ta, true);          // 强制单行
    lv_obj_set_scrollbar_mode(ta, LV_SCROLLBAR_MODE_OFF); // 关掉滚动条
	lv_textarea_set_placeholder_text(ta, "在此输入站点以检索");
	lv_obj_set_style_text_color(ta,lv_color_hex(COLOR_MID_GRAY),LV_PART_TEXTAREA_PLACEHOLDER);
	lv_obj_set_style_text_font(ta,&heiti_16, LV_PART_TEXTAREA_PLACEHOLDER);  //显示提示占位符
	lv_obj_add_event_cb(ta, kb_show_cb, LV_EVENT_CLICKED, kb);  //键盘弹出回调创建
}
void metro_logo(lv_obj_t* display0,int judge)
{
	logo = lv_img_create(display0);
	lv_img_set_src(logo, "0:/logo.bin");
	lv_obj_set_pos(logo,1,4);
	
	map_blue = lv_img_create(display0);
	lv_img_set_src(map_blue, "0:/map_blue.bin");
	lv_obj_set_pos(map_blue,16,77);
	lv_obj_add_flag(map_blue, LV_OBJ_FLAG_HIDDEN);
	map_white = lv_img_create(display0);
	lv_img_set_src(map_white, "0:/map_white.bin");
	lv_obj_set_pos(map_white,16,77);
	lv_obj_add_flag(map_white, LV_OBJ_FLAG_HIDDEN);  //隐藏map_white

	line_white = lv_img_create(display0);
	lv_img_set_src(line_white, "0:/line_white.bin");
	lv_obj_set_pos(line_white,16,153);
	lv_obj_add_flag(line_white, LV_OBJ_FLAG_HIDDEN);
	line_blue = lv_img_create(display0);
	lv_img_set_src(line_blue, "0:/line_blue.bin");
	lv_obj_set_pos(line_blue,16,153);
	lv_obj_add_flag(line_blue, LV_OBJ_FLAG_HIDDEN);  //隐藏line_blue

	bell_blue = lv_img_create(display0);
	lv_img_set_src(bell_blue, "0:/bell_blue.bin");
	lv_obj_set_pos(bell_blue,16,230);
	lv_obj_add_flag(bell_blue, LV_OBJ_FLAG_HIDDEN);  //隐藏bell_blue
	bell_white = lv_img_create(display0);
	lv_img_set_src(bell_white, "0:/bell_white.bin");
	lv_obj_set_pos(bell_white,16,230);
	lv_obj_add_flag(bell_white, LV_OBJ_FLAG_HIDDEN);

	lb_t1 = create_simple_label(display0,160,18,87,23,"乘客导视",&heiti_20);
	lv_obj_set_style_text_color(lb_t1,lv_color_hex(0x194888), 0);

	switch (judge)
	{
	case 0:
		lv_obj_clear_flag(map_blue,LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(line_white,LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(bell_white,LV_OBJ_FLAG_HIDDEN);
		break;
	case 1:
		lv_obj_clear_flag(map_white,LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(line_blue,LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(bell_white,LV_OBJ_FLAG_HIDDEN);
	case 2:
		lv_obj_clear_flag(map_white,LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(line_white,LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(bell_blue,LV_OBJ_FLAG_HIDDEN);
	default:
		break;
	}
}
void display_set(lv_obj_t* display,int judge)  //设置主界面
{
	twocolumns(display);
    create_buttons(display,judge);
    creat_top_ta(display);
    metro_logo(display,judge);
	lv_obj_add_event_cb(display, screen_load_event_cb, LV_EVENT_ALL, kb);  //界面转换回调函数
}
void transparent_init(lv_obj_t* display)  //生成隐藏的覆盖整个canvas的label
{
	transparent = create_simple_label(display,0,0,1024,600,"",NULL);
	lv_obj_set_style_bg_opa(transparent, LV_OPA_50, 0);
	lv_obj_set_style_bg_color(transparent,lv_color_hex(COLOR_MID_GRAY),LV_PART_MAIN);
	lv_obj_add_flag(transparent,LV_OBJ_FLAG_CLICKABLE);
	lv_obj_move_foreground(transparent);
	lv_obj_add_event_cb(transparent, kb_hide_cb, LV_EVENT_ALL,NULL);   //键盘隐藏回调创建
	lv_obj_add_event_cb(kb, keyBoard_event_cb, LV_EVENT_ALL, NULL);   //键盘输入回调创建
}
void top_ta_record_lb_init(lv_obj_t* display)  //初始化历史记录框
{
	ta_lb1 = create_simple_label(display,239,55,400,210,"",NULL);
	lv_obj_set_style_bg_color(ta_lb1,lv_color_hex(0xffffff),LV_PART_MAIN);
	lv_obj_set_style_bg_opa(ta_lb1, LV_OPA_100, 0);
	lv_obj_set_style_radius(ta_lb1,4,LV_PART_MAIN);
	lv_obj_add_flag(ta_lb1, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ta_lb1, LV_OBJ_FLAG_HIDDEN);
	lv_obj_move_foreground(ta_lb1);
	lv_obj_t* ta_lb1_lb1 = create_simple_label(ta_lb1,16,16,80,23,"历史记录",&heiti_20);
	lv_obj_set_style_text_color(ta_lb1_lb1,lv_color_hex(COLOR_DARK_BLUE), 0);

}
void top_ta_result_lb_init(lv_obj_t* display)
{
	ta_lb2 = create_simple_label(display,239,55,400,210,"",NULL);
	lv_obj_set_style_bg_color(ta_lb2,lv_color_hex(0xffffff),LV_PART_MAIN);
	lv_obj_set_style_bg_opa(ta_lb2, LV_OPA_100, 0);
	lv_obj_set_style_radius(ta_lb2,4,LV_PART_MAIN);
	lv_obj_add_flag(ta_lb2, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ta_lb2, LV_OBJ_FLAG_HIDDEN);
	lv_obj_move_foreground(ta_lb2);
	lv_obj_t* ta_lb2_lb1 = create_simple_label(ta_lb2,16,16,80,23,"搜索结果",&heiti_20);
	lv_obj_set_style_text_color(ta_lb2_lb1,lv_color_hex(COLOR_DARK_BLUE), 0);
}



/**
 * 回调函数
 */
void screen_load_event_cb(lv_event_t *e)  //切换界面后立即隐去kb（如果有）
{
	// 获取用户数据传入的键盘对象
    lv_obj_t * kb = (lv_obj_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    // 强制隐藏
    if(code == LV_EVENT_SCREEN_LOAD_START) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}
void btn1_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_PRESSED)
		lv_scr_load(display0);
}
void btn2_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_PRESSED)
		lv_scr_load(display1);
}
void btn3_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_PRESSED)
		lv_scr_load(display2);
}
void kb_show_cb(lv_event_t *e)
{
	lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
	kb_show(kb,ta);
	lv_obj_move_foreground(ta);
	lv_obj_clear_flag(ta_lb1, LV_OBJ_FLAG_HIDDEN);  //生成历史记录框
}
void kb_hide_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * transparent = lv_event_get_target(e);
	if(code == LV_EVENT_CLICKED)
	{
		kb_hide(kb);
		lv_obj_add_flag(ta_lb1, LV_OBJ_FLAG_HIDDEN);
		if(!lv_obj_has_flag(ta_lb2,LV_OBJ_FLAG_HIDDEN))
		{
			lv_obj_add_flag(ta_lb2, LV_OBJ_FLAG_HIDDEN);
		}
	}
}		
void keyBoard_event_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);           
    uint32_t id = 0;
    const char *text;
	if (code == LV_EVENT_VALUE_CHANGED)
	{
		lv_obj_add_flag(ta_lb1, LV_OBJ_FLAG_HIDDEN);
		lv_obj_clear_flag(ta_lb2, LV_OBJ_FLAG_HIDDEN);
		id = lv_btnmatrix_get_selected_btn(kb);
		text = lv_keyboard_get_btn_text(kb, id);
		if (strcmp(text, LV_SYMBOL_OK) == 0)
		{
			// const char *ta_text = lv_textarea_get_text(ta);
			// if(ta_text == NULL) ta_text = "";
			//...
			// lv_textarea_set_text(ta, "");
			kb_hide(kb);
			lv_obj_add_flag(ta_lb2, LV_OBJ_FLAG_HIDDEN);
		}
	}
}


/**
 * @brief 通用 Label 创建工具实现
 */
lv_obj_t* create_simple_label(lv_obj_t* parent, int x, int y, int w, int h, 
                              const char* text, const lv_font_t* font)
{
    lv_obj_t* lb = lv_label_create(parent);
    lv_obj_set_pos(lb, x, y);
    lv_obj_set_size(lb, w, h);
    lv_label_set_text(lb, text);
    
    // 设置字体
	if(text != NULL)
    	lv_obj_set_style_text_font(lb, font, LV_PART_MAIN);
    
    return lb; // 直接返回对象
}

/**
 * @brief 通用按钮创建工具实现
 */
lv_obj_t* create_simple_btn(lv_obj_t* parent, int x, int y, int w, int h, lv_color_t bg_color)
{
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_size(btn, w, h);
    lv_obj_set_style_radius(btn, BUTTON_RADIUS, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, bg_color, LV_PART_MAIN);
    
    return btn; // 直接返回对象
}

/**
 * @brief 键盘的弹出与隐去
 * 将kb与隐藏的transparent标签绑定
 * 实现弹出键盘同时生成transparent标签，隐去键盘同时删除transparent标签
 * 代替lv_obj_clear_flag，lv_obj_add_flag
 */
void kb_show(lv_obj_t* kb,lv_obj_t* ta)
{
	lv_keyboard_set_textarea(kb,ta);
	if (lv_obj_has_flag(kb,LV_OBJ_FLAG_HIDDEN))
	{
		transparent_init(lv_scr_act());  
	}
	lv_obj_clear_flag(kb,LV_OBJ_FLAG_HIDDEN);
	lv_obj_move_foreground(kb);
}
void kb_hide(lv_obj_t* kb)
{
	lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(transparent, LV_OBJ_FLAG_HIDDEN);
}
