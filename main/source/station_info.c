#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <string.h>
#include <stdio.h>
#include "gui.h"
#include "metro_line.h"
#include "canvas.h"


lv_obj_t * station_info_disp;

lv_obj_t * change_btn, * change_label;
lv_obj_t * timetable_btn, * timetable_label;

lv_obj_t * sta_line_number_label;
lv_obj_t * sta_name_label;

lv_obj_t * sta_direction_label;
lv_obj_t * sta_first_train_label, * sta_last_train_label;

lv_obj_t * train_icon;
lv_obj_t * sta_line_canvas;

lv_obj_t * sta_star_btn, * sta_star_label;
lv_obj_t * sta_remind_add_btn, * sta_remind_add_label;

lv_style_t time_label_style;
TimeLabel time_labels[2];

int8_t direction_state = 0;

static Station showing_station;


void station_info_init(lv_obj_t * canvas)
{
    //设置样式
    lv_style_init(&time_label_style);

    lv_style_set_bg_color(&time_label_style, lv_color_hex(COLOR_BG_BLUE));
    lv_style_set_border_color(&time_label_style, lv_color_hex(COLOR_MID_BLUE));
    lv_style_set_border_width(&time_label_style, 1);
    lv_style_set_radius(&time_label_style, BUTTON_RADIUS);
    lv_style_set_opa(&time_label_style, LV_OPA_COVER);
    lv_style_set_pad_all(&time_label_style, 0);
    lv_style_set_shadow_opa(&time_label_style, LV_OPA_0);


    //初始化界面
    station_info_disp = lv_obj_create(canvas);

    lv_obj_set_style_pad_all(station_info_disp, 0, 0);
    lv_obj_move_foreground(station_info_disp);
    lv_obj_clear_flag(station_info_disp, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_pos(station_info_disp, STATION_INFO_X, STATION_INFO_Y);
    lv_obj_set_size(station_info_disp, STATION_INFO_W, STATION_INFO_H);

    lv_obj_set_style_radius(station_info_disp, 0, 0);


    //初始化线路标签
    sta_line_number_label = lv_label_create(station_info_disp);
    lv_obj_set_pos(sta_line_number_label, 22, 17);
    lv_obj_set_size(sta_line_number_label, 63 , 30);

    lv_obj_set_style_bg_opa(sta_line_number_label, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(sta_line_number_label, BUTTON_RADIUS, 0);
    lv_obj_set_style_text_color(sta_line_number_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(sta_line_number_label, &heiti_16, 0);
    lv_obj_set_style_bg_color(sta_line_number_label, lv_color_hex(COLOR_LINE2), 0);
    lv_obj_set_style_pad_top(sta_line_number_label, 6, 0);
    lv_obj_set_style_text_align(sta_line_number_label, LV_TEXT_ALIGN_CENTER, 0);

    lv_label_set_text(sta_line_number_label, "2号线");

    //初始化车站名称标签
    
    sta_name_label = lv_label_create(station_info_disp);
    lv_obj_set_size(sta_name_label, 150, 30);
    lv_obj_set_pos(sta_name_label, 22 , 61);
    lv_obj_set_style_text_align(sta_name_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(sta_name_label, &heiti_24, 0);

    lv_label_set_text(sta_name_label, "街道口");


    sta_direction_label = lv_label_create(station_info_disp);
    lv_obj_set_size(sta_direction_label, 200, 30);
    lv_obj_set_pos(sta_direction_label, 210 , 22);
    lv_obj_set_style_text_color(sta_direction_label, lv_color_black(), 0);
    lv_obj_set_style_text_align(sta_direction_label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_font(sta_direction_label, &heiti_20, 0);

    lv_label_set_text(sta_direction_label, "佛祖岭 方向");

    
    sta_first_train_label = lv_label_create(station_info_disp);
    lv_obj_set_size(sta_first_train_label, 200, 18);
    lv_obj_set_pos(sta_first_train_label, 210 , 51);
    lv_obj_set_style_text_color(sta_first_train_label, lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_obj_set_style_text_align(sta_first_train_label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_font(sta_first_train_label, &heiti_16, 0);

    lv_label_set_text(sta_first_train_label, "首班 06:15");

    
    sta_last_train_label = lv_label_create(station_info_disp);
    lv_obj_set_size(sta_last_train_label, 200, 18);
    lv_obj_set_pos(sta_last_train_label, 210 , 75);
    lv_obj_set_style_text_color(sta_last_train_label, lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_obj_set_style_text_align(sta_last_train_label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_font(sta_last_train_label, &heiti_16, 0);

    lv_label_set_text(sta_last_train_label, "末班 23:15");


    change_btn = lv_btn_create(station_info_disp);
    
    lv_obj_set_pos(change_btn, 432 ,22);
    lv_obj_set_size(change_btn, 73, 30);

    change_label = lv_label_create(change_btn);
    lv_label_set_text(change_label, "换向");
    lv_obj_set_style_text_font(change_label, &heiti_16, 0);
    lv_obj_set_style_text_color(change_label, lv_color_black(), 0);
    lv_obj_center(change_label);

    lv_obj_add_style(change_btn, &btn_style, 0);

    lv_obj_add_event_cb(change_btn, change_btn_cb, LV_EVENT_CLICKED, NULL);


    timetable_btn = lv_btn_create(station_info_disp);
    
    lv_obj_set_pos(timetable_btn, 432 ,63);
    lv_obj_set_size(timetable_btn, 73 , 30);

    timetable_label = lv_label_create(timetable_btn);
    lv_label_set_text(timetable_label, "时间表");
    lv_obj_set_style_text_font(timetable_label, &heiti_16, 0);
    lv_obj_set_style_text_color(timetable_label, lv_color_black(), 0);
    lv_obj_center(timetable_label);

    lv_obj_add_style(timetable_btn, &btn_style, 0);

    for (int8_t i = 0; i < 2; i++)
        time_label_init(station_info_disp, time_labels + i, i);
    


    sta_star_btn = lv_btn_create(station_info_disp);
    
    lv_obj_set_pos(sta_star_btn, 308 , 485);
    lv_obj_set_size(sta_star_btn,73, 35);

    sta_star_label = lv_label_create(sta_star_btn);
    lv_label_set_text(sta_star_label, "收藏");
    lv_obj_set_style_text_font(sta_star_label, &heiti_16, 0);
    lv_obj_set_style_text_color(sta_star_label, lv_color_white(), 0);
    lv_obj_center(sta_star_label);

    lv_obj_add_style(sta_star_btn, &blue_button_style, 0);
    
    
    
    sta_remind_add_btn = lv_btn_create(station_info_disp);
    
    lv_obj_set_pos(sta_remind_add_btn, 397 , 485);
    lv_obj_set_size(sta_remind_add_btn,106, 35);

    sta_remind_add_label = lv_label_create(sta_remind_add_btn);
    lv_label_set_text(sta_remind_add_label, "添加提醒");
    lv_obj_set_style_text_font(sta_remind_add_label, &heiti_16, 0);
    lv_obj_set_style_text_color(sta_remind_add_label, lv_color_white(), 0);
    lv_obj_center(sta_remind_add_label);

    lv_obj_add_style(sta_remind_add_btn, &blue_button_style, 0);
    
    lv_obj_add_flag(station_info_disp, LV_OBJ_FLAG_HIDDEN);

}

void time_label_init(lv_obj_t * obj ,TimeLabel *tl, int8_t count)
{
    tl->label_frame = lv_label_create(obj);

    lv_obj_set_pos(tl->label_frame , TIME_LABEL_X , TIME_LABEL_Y + count * (TIME_LABEL_H + 10));
    lv_obj_set_size(tl->label_frame , TIME_LABEL_W, TIME_LABEL_H);
    lv_obj_set_style_bg_opa(tl->label_frame, LV_OPA_COVER, 0);
    lv_label_set_text(tl->label_frame, "");


    lv_obj_add_style(tl->label_frame , &time_label_style, 0);
/* 
    btn->line_number_label = lv_label_create(tl->label_frame);
    lv_obj_set_pos(btn->line_number_label, 0, 0);
    lv_obj_set_size(btn->line_number_label, 63 , 29);

    lv_obj_set_style_bg_opa(btn->line_number_label, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(btn->line_number_label, BUTTON_RADIUS, 0);
    lv_obj_set_style_text_color(btn->line_number_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(btn->line_number_label, &heiti_16, 0);
    lv_obj_set_style_bg_color(btn->line_number_label, lv_color_hex(COLOR_LINE2), 0);
    lv_obj_set_style_pad_top(btn->line_number_label, 6, 0);
    lv_obj_set_style_text_align(btn->line_number_label, LV_TEXT_ALIGN_CENTER, 0);
 */
    //lv_label_set_text(btn->line_number_label, "2号线");
    
    tl->remain_sta_label = lv_label_create(tl->label_frame);
    tl->remain_time_label = lv_label_create(tl->label_frame);
    tl->arrive_time_label = lv_label_create(tl->label_frame);
    tl->train_order_label = lv_label_create(tl->label_frame);
    tl->train_direction_label = lv_label_create(tl->label_frame);

    lv_obj_add_style(tl->train_direction_label, &blue_label_style,  0);
    lv_obj_add_style(tl->remain_sta_label,      &blue_label_style,  0);
    lv_obj_add_style(tl->train_order_label,     &black_label_style, 0);
    lv_obj_add_style(tl->arrive_time_label,     &black_label_style, 0);
    lv_obj_add_style(tl->remain_time_label,     &black_label_style, 0);

    lv_obj_set_style_text_font(tl->arrive_time_label, &heiti_24, 0);
    lv_obj_set_style_text_font(tl->remain_time_label, &heiti_32, 0);

    lv_obj_set_style_text_align(tl->remain_sta_label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_align(tl->arrive_time_label, LV_TEXT_ALIGN_RIGHT, 0);

    lv_obj_set_size(tl->remain_sta_label, 96, 16);
    lv_obj_set_size(tl->arrive_time_label, 122, 31);


    lv_obj_set_pos(tl->train_direction_label, 94 , 14);
    lv_obj_set_pos(tl->remain_sta_label,      362, 14);  
    lv_obj_set_pos(tl->train_order_label,     14 , 14); 
    lv_obj_set_pos(tl->remain_time_label,     16 , 40);   
    lv_obj_set_pos(tl->arrive_time_label,     338, 46);

    lv_label_set_text(tl->train_direction_label, "佛祖岭 方向");
    lv_label_set_text(tl->remain_sta_label,      "还有2站");
    lv_label_set_text(tl->train_order_label,     "第一班车"  );
    lv_label_set_text(tl->remain_time_label,     "03:15"  );
    lv_label_set_text(tl->arrive_time_label,     "预计 08:30" );

}

void station_info_show(Station *sta)
{
    showing_station.name = sta->name;
    showing_station.line_belonged = sta->line_belonged;
    
    lv_obj_clear_flag(station_info_disp, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text(sta_name_label, sta->name);


    int8_t line_number = showing_station.line_belonged;
    char * sta1 = metro_lines[line_number - 1].stations[0].name;
    char * sta2 = metro_lines[line_number - 1].stations[metro_lines[line_number - 1].count - 1].name;
    char line_str[10], sta1_str[24], sta2_str[24];
    snprintf(line_str, sizeof(line_str), "%d号线",line_number);
    snprintf(sta1_str, sizeof(sta1_str), "%s 方向",sta1);
    snprintf(sta2_str, sizeof(sta2_str), "%s 方向",sta2);

    lv_label_set_text(sta_line_number_label, line_str);

    lv_color_t color;
    switch (line_number)
    {
        case 1:
            color = lv_color_hex(COLOR_LINE1);
            break;
        case 2:
            color = lv_color_hex(COLOR_LINE2);
            break;
        case 3:
            color = lv_color_hex(COLOR_LINE3);
            break;
        case 4:
            color = lv_color_hex(COLOR_LINE4);
            break;
        default:
            break;
    }

    lv_obj_set_style_bg_color(sta_line_number_label, color, 0);

    if (!direction_state) lv_label_set_text(sta_direction_label, sta1_str);
    else lv_label_set_text(sta_direction_label, sta2_str);

    lv_label_set_text(sta_first_train_label, "首班 06:15");
    lv_label_set_text(sta_last_train_label, "末班 23:15");

}

void station_info_refresh(void)
{
    int8_t line_number = showing_station.line_belonged;
    char * sta1 = metro_lines[line_number - 1].stations[0].name;
    char * sta2 = metro_lines[line_number - 1].stations[metro_lines[line_number - 1].count - 1].name;
    char sta1_str[24], sta2_str[24];
    snprintf(sta1_str, sizeof(sta1_str), "%s 方向",sta1);
    snprintf(sta2_str, sizeof(sta2_str), "%s 方向",sta2);

    if (!direction_state) lv_label_set_text(sta_direction_label, sta1_str);
    else lv_label_set_text(sta_direction_label, sta2_str);

    lv_label_set_text(sta_first_train_label, "首班 06:15");
    lv_label_set_text(sta_last_train_label, "末班 23:15");

}

void change_btn_cb(lv_event_t * e)
{
    if (direction_state) direction_state = 0;
    else direction_state = 1;
    
    station_info_refresh();
}