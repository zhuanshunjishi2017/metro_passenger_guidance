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
extern lv_obj_t* display0;
extern lv_obj_t* display1;
extern lv_obj_t* display2;
extern lv_obj_t* canvas;
extern lv_obj_t* logo;
static int flag;

void twocolumns(lv_obj_t* display0,lv_obj_t* label_left,lv_obj_t* label_top)
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
void create_buttons(lv_obj_t* display0,lv_obj_t* btn1,lv_obj_t* btn2,lv_obj_t* btn3)
{
	btn1 = lv_btn_create(display0);
	lv_obj_set_pos(btn1, 6, 70);
	lv_obj_set_size(btn1, 50, 66);
	lv_obj_set_style_radius(btn1,4,LV_PART_MAIN);
	lv_obj_set_style_bg_color(btn1,lv_color_hex(0xd2dbe7),LV_PART_MAIN);
	lv_obj_add_event_cb(btn1,btn1_cb,LV_EVENT_ALL,NULL);

	btn2 = lv_btn_create(display0);
	lv_obj_set_pos(btn2, 6, 144);
	lv_obj_set_size(btn2, 50, 66);
	lv_obj_set_style_radius(btn2,4,LV_PART_MAIN);
	lv_obj_set_style_bg_color(btn2,lv_color_hex(0x3b75),LV_PART_MAIN);
	lv_obj_add_event_cb(btn2,btn2_cb,LV_EVENT_ALL,NULL);

	btn3 = lv_btn_create(display0);
	lv_obj_set_pos(btn3, 6, 222);
	lv_obj_set_size(btn3, 50, 66);
	lv_obj_set_style_radius(btn3,4,LV_PART_MAIN);
	lv_obj_set_style_bg_color(btn3,lv_color_hex(0x3b75),LV_PART_MAIN);
	lv_obj_add_event_cb(btn3,btn3_cb,LV_EVENT_ALL,NULL);
}
void creat_top_ta(lv_obj_t* display0,lv_obj_t* ta,lv_obj_t* kb)
{
	ta = lv_textarea_create(display0);
	lv_obj_set_pos(ta,264,12);
	lv_obj_set_size(ta,266,34);
	lv_obj_set_style_radius(ta,4,LV_PART_MAIN);
	kb = lv_keyboard_create(display0);
	lv_obj_add_flag(kb,LV_OBJ_FLAG_HIDDEN);
	lv_obj_add_event_cb(ta, kb_show_cb, LV_EVENT_CLICKED, kb);
	lv_obj_add_event_cb(canvas, kb_hide_cb, LV_EVENT_CLICKED, kb);
	lv_obj_add_event_cb(kb, keyBoard_event_cb, LV_EVENT_ALL, ta);
}
void metro_logo(lv_obj_t* lb_top)
{
	logo = lv_img_create(lb_top);
	lv_img_set_src(logo, "0:/logo.bin");
	lv_obj_set_pos(logo,2,2);
	
}
void btn1_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *btn1 = lv_event_get_target(e);
	if(code == LV_EVENT_PRESSED)
	{
		flag = 0;
		lv_obj_set_style_bg_color(btn1,lv_color_hex(0xd2dbe7),LV_PART_MAIN);
		lv_scr_load(display0);
	}
	if(flag != 0)
	{
		lv_obj_set_style_bg_color(btn1,lv_color_hex(0x3b75),LV_PART_MAIN);
	}
}

void btn2_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *btn2 = lv_event_get_target(e);
	if(code == LV_EVENT_PRESSED)
	{
		flag = 1;
		lv_obj_set_style_bg_color(btn2,lv_color_hex(0xd2dbe7),LV_PART_MAIN);
		lv_scr_load(display1);
	}
	if(flag != 1)
	{
		lv_obj_set_style_bg_color(btn2,lv_color_hex(0x3b75),LV_PART_MAIN);
	}
}

void btn3_cb(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t *btn3 = lv_event_get_target(e);
	if(code == LV_EVENT_PRESSED)
	{
		flag = 2;
		lv_obj_set_style_bg_color(btn3,lv_color_hex(0xd2dbe7),LV_PART_MAIN);
		lv_scr_load(display2);
	}
	if(flag != 2)
	{
		lv_obj_set_style_bg_color(btn3,lv_color_hex(0x3b75),LV_PART_MAIN);
	}
}
void kb_show_cb(lv_event_t *e)
{
	lv_obj_t *ta = lv_event_get_target(e);
	lv_obj_t *kb = (lv_obj_t *)lv_event_get_user_data(e);
	lv_keyboard_set_textarea(kb,ta);
	lv_obj_clear_flag(kb,LV_OBJ_FLAG_HIDDEN);
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


