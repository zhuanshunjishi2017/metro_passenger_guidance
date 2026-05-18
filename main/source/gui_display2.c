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



RemindLabel remind_labels[MAX_REMIND];

lv_obj_t * remind_label_container,* no_remind_label;

lv_timer_t * reminder_timer;

void display2_init(void);

void remind_label_init(lv_obj_t * obj,RemindLabel * remind_label);

void remind_label_show(RemindLabel * remind_label,RemindInfo * info);

void remind_label_update(RemindLabel * remind_label,RemindInfo * info);

void reminder_info_handler(lv_timer_t *timer);

void remind_label_click_cb(lv_event_t * e);

void del_btn_cb(lv_event_t *e);

static lv_point_t line_point[2] = {{492,55},{492,599}};

extern void show_msgbox(lv_obj_t **obj);



void display2_init(void)
{

    lv_obj_t * top_label = lv_label_create(display2);
    lv_obj_set_size(top_label, 180, 30);
    lv_obj_set_pos(top_label, 90 , 86);
    lv_obj_set_style_text_align(top_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_text_font(top_label, &heiti_24, 0);

    lv_label_set_text(top_label, "已添加的提醒");

    remind_label_container = lv_obj_create(display2);

    lv_obj_set_style_pad_all(remind_label_container, 0, 0);
    lv_obj_set_scroll_dir(remind_label_container, LV_DIR_VER);

    lv_obj_set_pos(remind_label_container,90 , 144);
    lv_obj_set_size(remind_label_container, 364, 456);

    lv_obj_set_style_radius(remind_label_container, 0, 0);
    lv_obj_set_style_border_width(remind_label_container, 0, 0);
    lv_obj_set_scrollbar_mode(remind_label_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_bg_opa(remind_label_container, LV_OPA_0, 0);

    lv_obj_t * line2 = lv_line_create(display2);
    lv_line_set_points(line2, line_point, 2);
    lv_obj_set_style_line_width(line2, 1, LV_PART_MAIN);
    lv_obj_set_style_line_color(line2, lv_color_hex(COLOR_MID_GRAY), LV_PART_MAIN);
    lv_obj_clear_flag(display2, LV_OBJ_FLAG_SCROLLABLE);




    remind_label_init(remind_label_container, remind_labels);

    no_remind_label = lv_label_create(remind_label_container);

    lv_obj_set_style_text_align(no_remind_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(no_remind_label, &heiti_16, 0);
    lv_obj_set_style_text_color(no_remind_label, lv_color_hex(COLOR_MID_GRAY), 0);

    lv_label_set_text(no_remind_label, "暂无提醒\n\n从车站详情界面添加提醒\n\n\n");

    lv_obj_center(no_remind_label);

    remind_label_show(remind_labels,remind_info);


    reminder_timer = lv_timer_create(reminder_info_handler, 1000, NULL);


}

void reminder_info_handler(lv_timer_t *timer)
{
    remind_label_update(remind_labels,remind_info);
}


void remind_label_init(lv_obj_t * obj,RemindLabel * remind_label)
{

    for (int i = 0; i < MAX_REMIND; i++)
    {
        remind_label[i].label_frame  = lv_label_create(obj);

        lv_obj_set_pos(remind_label[i].label_frame , 0 , i * (116 + 10));
        lv_obj_set_size(remind_label[i].label_frame , 364, 116);
        lv_obj_set_style_bg_opa(remind_label[i].label_frame, LV_OPA_COVER, 0);
        lv_label_set_text(remind_label[i].label_frame, "");

        lv_obj_add_flag(remind_label[i].label_frame, LV_OBJ_FLAG_CLICKABLE);

        lv_obj_add_event_cb(remind_label[i].label_frame,remind_label_click_cb,
                            LV_EVENT_CLICKED, (void *)&remind_info[i]);

        lv_obj_add_style(remind_label[i].label_frame , &time_label_style, 0);
        
        remind_label[i].direction_label = lv_label_create(remind_label[i].label_frame);
        remind_label[i].line_label = lv_label_create(remind_label[i].label_frame);
        remind_label[i].station_label = lv_label_create(remind_label[i].label_frame);
        remind_label[i].remind_info_label = lv_label_create(remind_label[i].label_frame);
        remind_label[i].train_order_label = lv_label_create(remind_label[i].label_frame);

        remind_label[i].del_btn = lv_btn_create(remind_label[i].label_frame);


        lv_obj_set_pos(remind_label[i].line_label, 16, 16);
        lv_obj_set_size(remind_label[i].line_label, 63 , 30);

        lv_obj_set_style_bg_opa(remind_label[i].line_label, LV_OPA_COVER, 0);
        lv_obj_set_style_radius(remind_label[i].line_label, BUTTON_RADIUS, 0);
        lv_obj_set_style_text_color(remind_label[i].line_label, lv_color_white(), 0);
        lv_obj_set_style_text_font(remind_label[i].line_label, &heiti_16, 0);
        lv_obj_set_style_bg_color(remind_label[i].line_label, lv_color_hex(COLOR_LINE2), 0);
        lv_obj_set_style_pad_top(remind_label[i].line_label, 6, 0);
        lv_obj_set_style_text_align(remind_label[i].line_label, LV_TEXT_ALIGN_CENTER, 0);

        lv_label_set_text(remind_label[i].line_label, "2号线");

        
        // lv_obj_set_size( remind_label[i].station_label, 180, 30);
        lv_obj_set_pos( remind_label[i].station_label, 90 , 20);
        lv_obj_set_style_text_align( remind_label[i].station_label, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_style_text_font( remind_label[i].station_label, &heiti_16, 0);

        lv_label_set_text(remind_label[i].station_label, "街道口");


        lv_obj_set_size( remind_label[i].direction_label, 170, 16);
        lv_obj_set_pos( remind_label[i].direction_label, 175 , 20);
        lv_obj_set_style_text_color( remind_label[i].direction_label, lv_color_hex(COLOR_DARK_BLUE), 0);
        lv_obj_set_style_text_align( remind_label[i].direction_label, LV_TEXT_ALIGN_RIGHT, 0);
        lv_obj_set_style_text_font( remind_label[i].direction_label, &heiti_16, 0);

        lv_label_set_text(remind_label[i].direction_label, "佛祖岭 方向");


        lv_obj_set_size( remind_label[i].train_order_label, 190, 31);
        lv_obj_set_pos( remind_label[i].train_order_label, 16 , 64);
        lv_obj_set_style_text_color( remind_label[i].train_order_label, lv_color_hex(COLOR_DARK_BLUE), 0);
        lv_obj_set_style_text_align( remind_label[i].train_order_label, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_style_text_font( remind_label[i].train_order_label, &heiti_24, 0);

        lv_label_set_text(remind_label[i].train_order_label, " ");


        lv_obj_set_size( remind_label[i].remind_info_label, 150, 31);
        lv_obj_set_pos( remind_label[i].remind_info_label, 125 , 64);
        lv_obj_set_style_text_color( remind_label[i].remind_info_label, lv_color_black(), 0);
        lv_obj_set_style_text_align( remind_label[i].remind_info_label, LV_TEXT_ALIGN_LEFT, 0);
        lv_obj_set_style_text_font( remind_label[i].remind_info_label, &heiti_24, 0);

        lv_label_set_text(remind_label[i].remind_info_label, " ");

        
        
        
        lv_obj_set_pos(remind_label[i].del_btn, 303, 58);
        lv_obj_set_size(remind_label[i].del_btn, 42, 40);
        lv_obj_set_style_border_color(remind_label[i].del_btn, lv_color_hex(COLOR_DARK_BLUE), 0);
        lv_obj_set_style_border_width(remind_label[i].del_btn, 1, 0);
        lv_obj_set_style_bg_color(remind_label[i].del_btn, lv_color_hex(COLOR_LIGHT_BLUE), 0);
        lv_obj_set_style_bg_opa(remind_label[i].del_btn, LV_OPA_COVER, 0);
        lv_obj_set_style_radius(remind_label[i].del_btn, 4, 0);

        lv_obj_t* del_img = lv_img_create(remind_label[i].del_btn);
        lv_img_set_src(del_img, "0:/trash_bin.bin");
        lv_obj_center(del_img);

        lv_obj_add_event_cb(remind_label[i].del_btn,del_btn_cb,LV_EVENT_CLICKED,(void *)i);

        lv_obj_add_flag(remind_label[i].label_frame, LV_OBJ_FLAG_HIDDEN);

    }
}

void remind_label_show(RemindLabel * remind_label,RemindInfo * info)
{
    if (!info[0].flag){
        lv_label_set_text(no_remind_label,  "暂无提醒\n\n从车站详情界面添加提醒\n\n\n");
    }
    else{
        lv_label_set_text(no_remind_label, "");
    }
    for (int i = 0; i < MAX_REMIND; i++)
    {
        if (!info[i].flag)
        {
            lv_obj_add_flag(remind_label[i].label_frame, LV_OBJ_FLAG_HIDDEN);
        }
        else 
        {
            int8_t line_number = info[i].station->line_belonged;


            lv_label_set_text_fmt(remind_label[i].line_label, "%d号线", line_number);

            lv_obj_set_style_bg_color(remind_label[i].line_label, get_line_color(line_number), 0);

            lv_label_set_text_fmt(remind_label[i].station_label, "%s", info[i].station->name);
            
            lv_label_set_text_fmt(remind_label[i].direction_label,
                                "%s 方向", get_first_station(line_number, !info[i].direction)->name);
            
            lv_obj_clear_flag(remind_label[i].label_frame, LV_OBJ_FLAG_HIDDEN);            
        }
    }
    remind_label_update(remind_label, info);
}

void remind_label_update(RemindLabel * remind_label,RemindInfo * info)
{
    TimeStruct current_time;
    
    get_current_time(&current_time);

    for (int i = 0; i < MAX_REMIND; i++)
    { 
        if (!info[i].flag) return;

        //如果还没到时间
        if (timeCompare(&current_time, &info[i].start_time) < 0)
        {
            char start_time_str[10];
            timeToString(&info[i].start_time, start_time_str, HOUR_MIN_MODE);

            lv_label_set_text_fmt(remind_label[i].train_order_label, "从%s起提醒",start_time_str);
            lv_label_set_text(remind_label[i].remind_info_label, " ");
            
            continue;
        }


        lv_label_set_text(remind_label[i].train_order_label, "下一班车");

        TimeStruct depart_time, time_interval = {0,0,0}, temp_time = {0,0,0}, arrive_time;
        TimeStruct depart_time_interval;

        const MetroLine * line = get_metro_line(info[i].station->line_belonged);
                
        timeAdd(&(line->timetable->first_train_time),&time_interval, &depart_time);
        
        secondsToTimeStruct(line->timetable->depart_period, &depart_time_interval);

        //获取列车从始发站到选定车站需要多长时间
        if (!info[i].direction)
        {
            for (int j = 0; j < info[i].station->id - 1; j++)
            {
                secondsToTimeStruct(line->station_period[j] + STATION_STOP_TIME ,&temp_time);
                timeAdd(&time_interval,&temp_time,&time_interval);
            }
        }
        else
        {
            for (int j = line->count - 2; j > info[i].station->id - 2; j--)
            {
                secondsToTimeStruct(line->station_period[j] + STATION_STOP_TIME ,&temp_time);
                timeAdd(&time_interval,&temp_time,&time_interval);
            }
        } 

        int8_t is_find = 0;

        while (!is_find)
        {
            TimeStruct remain_time;
            char remain_time_str[6];

            timeAdd(&depart_time,&time_interval,&arrive_time);  //计算到达时间

            int time_period = timeCompare(&arrive_time, &current_time);

            if (time_period >= 0) //如果还没到站
            {
                is_find = 1;

                //这里是处理弹窗
                if (time_period < info[i].remind_sec && time_period >  1)
                {
                    if (!is_reminder_dialog_showing)
                    {
                        is_reminder_dialog_showing = 1;
                        reminder_dialog_show(&info[i], i);
                        show_msgbox(&reminder_dialog);
                    }
                    if (reminder_remain_time_label)
                        lv_label_set_text_fmt(reminder_remain_time_label, 
                            "列车还有%d秒到站",time_period); 
                }
                //超时则关闭窗口并自动忽略
                else if (time_period <=  1 && is_reminder_dialog_showing)
                {
                    is_reminder_dialog_showing = 0;
                    //如果窗口还存在的话
                    if (reminder_dialog)
                        {hide_msgbox(&reminder_dialog);
                        reminder_dialog_clear();

                        remind_add_msg_init(&remind_add_msg, "已忽略本班列车");
                        show_msgbox(&remind_add_msg);

                        if (!msg_hide_timer)
                        {
                            msg_hide_timer = lv_timer_create(close_msg_cb, 1000, &remind_add_msg);
                        }
                    }

                }


                //这里是处理标签
                if (time_period <  30)
                {
                    lv_label_set_text(remind_label[i].remind_info_label, "即将到站");
                }
                else
                {
                    timeDifference(&arrive_time, &current_time, &remain_time);
                    timeToString(&remain_time, remain_time_str, MIN_SEC_MODE);

                    lv_label_set_text_fmt(remind_label[i].remind_info_label,"还有%s到", remain_time_str);
                }

            }
            //将出发时间自增 从最初的时间
            timeAdd(&depart_time, &depart_time_interval, &depart_time);
        } 
    }

}

void remind_label_click_cb(lv_event_t * e)
{
    RemindInfo *info = (RemindInfo *)lv_event_get_user_data(e);

    if (!is_station_info)
    {
        is_station_info = 1;
    }
    direction_state = info->direction;

    station_info_show(info->station , true);

}

void del_btn_cb(lv_event_t *e)
{
    int index = (int)lv_event_get_user_data(e);

    del_remind(index);

    remind_label_show(remind_labels,remind_info);

}




