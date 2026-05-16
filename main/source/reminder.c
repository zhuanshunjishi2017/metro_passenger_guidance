#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <string.h>
#include <stdio.h>
#include "gui.h"
#include "metro_line.h"
#include "canvas.h"
#include "timestruct.h"
#include "remind.h"


lv_obj_t * reminder_disp;

lv_obj_t * remind_add_msg;

lv_obj_t * hour_entry, *min_entry ,* remind_time_entry;
lv_obj_t * cancel_button, * add_button;
lv_obj_t *line_num,*direc, *sta_name;

lv_timer_t * msg_hide_timer = NULL;

RemindInfo remind_info[MAX_REMIND];

extern const Station *showing_station;

static void close_msg_cb(lv_timer_t *t);





void remind_entry_init(lv_obj_t * obj)
{
    char min[256] = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\
                    \n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\
                    \n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59";
    char hour[256] = "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\
                    \n21\n22\n23";
    char remind_time[32] = "30秒\n60秒\n120秒";
    
    
    lv_obj_set_size(obj, REMINDER_SET_W, STATION_INFO_H);
    lv_obj_set_pos(obj, REMINDER_SET_X + CANVAS_X, STATION_INFO_Y + CANVAS_Y);

    lv_obj_set_style_pad_all(obj, 0, 0);
    lv_obj_move_foreground(obj);

    //lv_obj_set_scroll_dir(station_info_disp, LV_DIR_VER);
    lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);


    lv_obj_set_style_radius(obj, 0, 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    
    
    hour_entry = lv_dropdown_create(obj);
    lv_obj_set_pos(hour_entry,30,233);
    lv_obj_set_size(hour_entry,89,40);
    lv_obj_set_style_radius(hour_entry,4,LV_PART_MAIN);
    //lv_obj_add_event_cb(hour_entry, hour_entry_kb_show_cb, LV_EVENT_ALL, kb);
    lv_obj_set_style_text_font(hour_entry, &heiti_16, 0);
    lv_dropdown_set_options(hour_entry, hour);


    min_entry = lv_dropdown_create(obj);
    lv_obj_set_pos(min_entry,151,233);
    lv_obj_set_size(min_entry,89,40);
    lv_obj_set_style_radius(min_entry,4,LV_PART_MAIN);
    //lv_obj_add_event_cb(hour_entry, hour_entry_kb_show_cb, LV_EVENT_ALL, kb);
    lv_obj_set_style_text_font(min_entry, &heiti_16, 0);
    lv_dropdown_set_options(min_entry, min);

    remind_time_entry = lv_dropdown_create(obj);
    lv_obj_set_pos(remind_time_entry,30,323);
    lv_obj_set_size(remind_time_entry,212,40);
    lv_obj_set_style_radius(remind_time_entry,4,LV_PART_MAIN);
    //lv_obj_add_event_cb(hour_entry, hour_entry_kb_show_cb, LV_EVENT_ALL, kb);
    lv_obj_set_style_text_font(remind_time_entry, &heiti_16, 0);
    lv_dropdown_set_options(remind_time_entry, remind_time);

    
    cancel_button = lv_btn_create(obj);
    
    lv_obj_set_pos(cancel_button, 30 , 413);
    lv_obj_set_size(cancel_button,212, 38);

    lv_obj_t *cancel_button_label = lv_label_create(cancel_button);
    lv_label_set_text(cancel_button_label, "取消");
    lv_obj_set_style_text_font(cancel_button_label, &heiti_16, 0);
    lv_obj_set_style_text_color(cancel_button_label, lv_color_black(), 0);
    lv_obj_center(cancel_button_label);

    lv_obj_add_style(cancel_button, &btn_style, 0);
    lv_obj_add_event_cb(cancel_button, remind_cancel_btn_cb, LV_EVENT_CLICKED, NULL);


    add_button = lv_btn_create(obj);
    
    lv_obj_set_pos(add_button, 30 , 464);
    lv_obj_set_size(add_button,212, 38);

    lv_obj_t * add_button_label = lv_label_create(add_button);
    lv_label_set_text(add_button_label, "添加提醒");
    lv_obj_set_style_text_font(add_button_label, &heiti_16, 0);
    lv_obj_set_style_text_color(add_button_label, lv_color_white(), 0);
    lv_obj_center(add_button_label);

    lv_obj_add_style(add_button, &blue_button_style, 0);
    lv_obj_add_event_cb(add_button, confirm_remind_add_btn_cb, LV_EVENT_CLICKED, NULL);

    
    
    lv_obj_t * title = lv_label_create(obj);
    lv_obj_set_pos(title, 30, 30);
    lv_obj_set_style_text_font(title, &heiti_24, 0);
    lv_label_set_text(title, "添加提醒");

    lv_obj_t * discrip = lv_label_create(obj);
    lv_obj_set_pos(discrip, 30, 65);
    lv_obj_set_style_text_font(discrip, &heiti_16, 0);
    lv_obj_set_style_text_color(discrip, lv_color_hex(COLOR_DARK_BLUE), 0);
    lv_label_set_text(discrip, "在指定列车进站前通知");

    sta_name = lv_label_create(obj);
    lv_obj_set_pos(sta_name, 30, 115);
    lv_obj_set_style_text_font(sta_name, &heiti_20, 0);
    
            

    direc = lv_label_create(obj);
    line_num = lv_label_create(obj);

    lv_obj_set_pos(direc, 107, 156);
    lv_obj_set_style_text_font(direc, &heiti_16, 0);
    lv_obj_set_pos(line_num, 30, 150);
    lv_obj_set_size(line_num, 63 , 30);

    lv_obj_set_style_bg_opa(line_num, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(line_num, BUTTON_RADIUS, 0);
    lv_obj_set_style_text_color(line_num, lv_color_white(), 0);
    lv_obj_set_style_text_font(line_num, &heiti_16, 0);
    lv_obj_set_style_pad_top(line_num, 6, 0);
    lv_obj_set_style_text_align(line_num, LV_TEXT_ALIGN_CENTER, 0);




    lv_obj_t * discrip_1 = lv_label_create(obj);
    lv_obj_set_pos(discrip_1, 30, 203);
    lv_obj_set_style_text_font(discrip_1, &heiti_16, 0);
    lv_label_set_text(discrip_1, "提醒起始时间");

    lv_obj_t * discrip_2 = lv_label_create(obj);
    lv_obj_set_pos(discrip_2, 30, 297);
    lv_obj_set_style_text_font(discrip_2, &heiti_16, 0);
    lv_label_set_text(discrip_2, "提前提醒秒数");

    lv_obj_t * dot = lv_label_create(obj);
    lv_obj_set_pos(dot, 132, 244);
    lv_obj_set_style_text_font(dot, &heiti_16, 0);
    lv_label_set_text(dot, ":");

    


    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
    
}

void reminder_set_show(const Station *sta)
{
    int8_t line_number =  sta->line_belonged;   
    lv_color_t color =get_line_color(sta->line_belonged);


    char * sta1 = metro_lines[line_number - 1].stations[0].name;
    char * sta2 = metro_lines[line_number - 1].stations[metro_lines[line_number - 1].count - 1].name;
    
    TimeStruct current_time;
    char * hour_str, *min_str;
    get_current_time(&current_time);

    sprintf(hour_str, "%d" , current_time.hour);
    sprintf(min_str, "%d" , current_time.min);

    // lv_textarea_set_text(hour_entry,hour_str);
    // lv_textarea_set_text(min_entry,min_str);
    // lv_textarea_set_text(remind_time_entry,"30");

    lv_dropdown_set_selected(hour_entry, current_time.hour);
    lv_dropdown_set_selected(min_entry, current_time.min);
    lv_dropdown_set_selected(remind_time_entry, 0);

    lv_obj_set_style_bg_color(line_num, color, 0);

    lv_label_set_text_fmt(line_num, "%d号线", line_number);

    if (direction_state)
        lv_label_set_text_fmt(direc, "%s 方向", sta1);
    else
        lv_label_set_text_fmt(direc, "%s 方向", sta2);

    lv_label_set_text(sta_name, sta->name);


}

void remind_cancel_btn_cb(lv_event_t * e)
{
    is_reminder_set_showing = 0;
    lv_obj_add_flag(reminder_disp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(station_info_disp, LV_OBJ_FLAG_HIDDEN);
}

void confirm_remind_add_btn_cb(lv_event_t * e)
{
    
    if (remind_info[MAX_REMIND - 1].flag)
    {
        remind_add_msg_init(&remind_add_msg,"提醒数量已达上限!");
        show_msgbox(&remind_add_msg);
    }
    
    else  
    {
        for (int i = 0; i < MAX_REMIND; i++)
        {
            if (!remind_info[i].flag)
            {
                TimeStruct start_time_struct;
                uint16_t hour, min, remind_sec;
                hour = lv_dropdown_get_selected(hour_entry);
                min = lv_dropdown_get_selected(min_entry);

                remind_sec =( lv_dropdown_get_selected(remind_time_entry) + 1) * 30;

                secondsToTimeStruct(hour * 3600 + min * 60, &start_time_struct);

                remind_info[i].station = get_station(showing_station);
                remind_info[i].direction = direction_state;
                remind_info[i].start_time = start_time_struct;
                remind_info[i].remind_sec = remind_sec;
                remind_info[i].flag = 1;
                
                remind_add_msg_init(&remind_add_msg,"提醒添加成功!");
                show_msgbox(&remind_add_msg);
                break;
            }
        }
    }
        
    if (!msg_hide_timer)
    {
        msg_hide_timer = lv_timer_create(close_msg_cb, 1000, &remind_add_msg);
    }

    is_reminder_set_showing = 0;
    lv_obj_add_flag(reminder_disp, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(station_info_disp, LV_OBJ_FLAG_HIDDEN);
    
}

static void close_msg_cb(lv_timer_t *t)
{
    lv_obj_t ** obj = (lv_obj_t **) t->user_data;
    hide_msgbox(obj);
    
    lv_timer_del(msg_hide_timer);
    msg_hide_timer = NULL;
}


