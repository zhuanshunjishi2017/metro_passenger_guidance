#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include <stdio.h>
#include "gui.h"

void btn1_cb(lv_event_t *e);
void btn2_cb(lv_event_t *e);
void btn3_cb(lv_event_t *e);
void kb_show_cb(lv_event_t *e);
void kb_hide_cb(lv_event_t *e);
void keyBoard_event_cb(lv_event_t *e);
void screen_load_event_cb(lv_event_t *e);
extern const lv_font_t heiti_14;
extern const lv_font_t heiti_16;
extern const lv_font_t heiti_20;
extern const lv_font_t heiti_24;
extern lv_obj_t* display0;
extern lv_obj_t* display1;
extern lv_obj_t* display2;
extern lv_obj_t* canvas;
extern lv_obj_t* timetable1,*timetable2,*timetable3;
lv_obj_t* label_left,*label_top;
lv_obj_t* lb_t1;
lv_obj_t* btn1,*btn2,*btn3;
lv_obj_t* ta,*kb;
lv_obj_t* logo;
lv_obj_t* map_blue,*map_white;
lv_obj_t* line_blue,*line_white;
lv_obj_t* bell_blue,*bell_white;
lv_obj_t* map_lb,*bell_lb,*line_lb;

static int flag;
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
	label_left = lv_label_create(display0);
	lv_obj_set_style_bg_color(label_left, lv_color_hex(0x1a51), 0);
	lv_obj_set_style_bg_opa(label_left, LV_OPA_COVER, 0);
	lv_obj_set_pos(label_left, 0, 55);
	lv_obj_set_size(label_left, 60,545);
	lv_label_set_text(label_left, "");
	label_top = lv_label_create(display0);
	lv_obj_set_style_bg_color(label_top, lv_color_hex(0xd2dbe7), 0);
	lv_obj_set_style_bg_opa(label_top, LV_OPA_COVER,0);
	lv_obj_set_pos(label_top, 0, 0);
	lv_obj_set_size(label_top, 1024,55);
	lv_label_set_text(label_top, "");
}
void create_buttons(lv_obj_t* display0,int judge)
{
	btn1 = lv_btn_create(display0);
	lv_obj_set_pos(btn1, 6, 70);
	lv_obj_set_size(btn1, 50, 66);
	lv_obj_set_style_radius(btn1,4,LV_PART_MAIN);
	lv_obj_add_event_cb(btn1,btn1_cb,LV_EVENT_ALL,NULL);
	map_lb = lv_label_create(display0);
	lv_obj_set_pos(map_lb,13,107);
	lv_obj_set_size(map_lb,33,19);
	lv_label_set_text(map_lb,"地图");
	lv_obj_set_style_text_font(map_lb,&heiti_16,LV_PART_MAIN);

	btn2 = lv_btn_create(display0);
	lv_obj_set_pos(btn2, 6, 144);
	lv_obj_set_size(btn2, 50, 66);
	lv_obj_set_style_radius(btn2,4,LV_PART_MAIN);
	lv_obj_add_event_cb(btn2,btn2_cb,LV_EVENT_ALL,NULL);
	line_lb = lv_label_create(display0);
	lv_obj_set_pos(line_lb,13,186);
	lv_obj_set_size(line_lb,33,19);
	lv_label_set_text(line_lb,"线路");
	lv_obj_set_style_text_font(line_lb,&heiti_16,LV_PART_MAIN);


	btn3 = lv_btn_create(display0);
	lv_obj_set_pos(btn3, 6, 222);
	lv_obj_set_size(btn3, 50, 66);
	lv_obj_set_style_radius(btn3,4,LV_PART_MAIN);
	lv_obj_add_event_cb(btn3,btn3_cb,LV_EVENT_ALL,NULL);
	bell_lb = lv_label_create(display0);
	lv_obj_set_pos(bell_lb,13,261);
	lv_obj_set_size(bell_lb,33,19);
	lv_label_set_text(bell_lb,"提醒");
	lv_obj_set_style_text_font(bell_lb,&heiti_16,LV_PART_MAIN);

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
	lv_obj_set_pos(ta,264,12);
	lv_obj_set_size(ta,266,34);
	lv_obj_set_style_radius(ta,4,LV_PART_MAIN);

	kb = lv_keyboard_create(display0);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_event_cb(ta, kb_show_cb, LV_EVENT_CLICKED, kb);
	lv_obj_add_event_cb(display0, kb_hide_cb, LV_EVENT_CLICKED, kb);
	lv_obj_add_event_cb(kb, keyBoard_event_cb, LV_EVENT_ALL, ta);
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

	lb_t1 = lv_label_create(display0);
	lv_obj_set_pos(lb_t1,160,18);
	lv_obj_set_size(lb_t1,87,23);
	lv_label_set_text(lb_t1,"乘客导视");
	lv_obj_set_style_text_font(lb_t1,&heiti_20,LV_PART_MAIN);
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
void display_set(lv_obj_t* display,int judge)
{
	twocolumns(display);
    create_buttons(display,judge);
    creat_top_ta(display);
    metro_logo(display,judge);
	lv_obj_add_event_cb(display, screen_load_event_cb, LV_EVENT_ALL, kb);
}



void screen_load_event_cb(lv_event_t *e)
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
	{
		lv_scr_load(display0);
	}
}

void btn2_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_PRESSED)
	{
		lv_scr_load(display1);
	}
}

void btn3_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	if(code == LV_EVENT_PRESSED)
	{
		lv_scr_load(display2);
	}
}
void kb_show_cb(lv_event_t *e)
{
	lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
	lv_keyboard_set_textarea(kb,ta);
	lv_obj_clear_flag(kb,LV_OBJ_FLAG_HIDDEN);
	lv_obj_move_foreground(kb);
}
void kb_hide_cb(lv_event_t *e)
{
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
	lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
}		
void keyBoard_event_cb(lv_event_t *e)
{
	lv_obj_t *ta = (lv_obj_t *)lv_event_get_user_data(e);
	lv_event_code_t code = lv_event_get_code(e);     
    lv_obj_t *kb = lv_event_get_target(e);       
    uint32_t id = 0;
    const char *text;
	if (code == LV_EVENT_VALUE_CHANGED)
	{
		id = lv_btnmatrix_get_selected_btn(kb);
		text = lv_keyboard_get_btn_text(kb, id);
		if (strcmp(text, LV_SYMBOL_OK) == 0)
		{
			const char *ta_text = lv_textarea_get_text(ta);
			if(ta_text == NULL) ta_text = "";
			//...
			lv_textarea_set_text(ta, "");
			lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
		}
	}
}