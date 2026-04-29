#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <string.h>
#include <stdio.h>
#include "gui.h"
#include "metro_line.h"
#include "canvas.h"

void * sta_canvas_buf;//站点画布缓冲区

lv_coord_t sta_origin_x = 0;

lv_obj_t * station_info_disp;

lv_obj_t * change_btn, * change_label;
lv_obj_t * timetable_btn, * timetable_label;

lv_obj_t * sta_line_number_label;
lv_obj_t * sta_name_label;

lv_obj_t * sta_direction_label;
lv_obj_t * sta_first_train_label, * sta_last_train_label;

lv_obj_t * train_icon[5];
lv_obj_t * sta_line_canvas;

lv_obj_t * sta_star_btn, * sta_star_label;
lv_obj_t * sta_remind_add_btn, * sta_remind_add_label;

lv_style_t time_label_style;
TimeLabel time_labels[2];

lv_draw_rect_dsc_t rect_dsc_line;


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
    lv_obj_set_style_border_width(station_info_disp, 0, 0);

    sta_line_canvas = lv_canvas_create(station_info_disp);

    lv_obj_set_pos(sta_line_canvas, 0 ,100);
    lv_obj_set_size(sta_line_canvas,STATION_INFO_W, 156);

    sta_canvas_init(sta_line_canvas);


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

    if (!count) lv_label_set_text(tl->train_order_label, "第一班车" );
    else lv_label_set_text(tl->train_order_label, "第二班车" );


    lv_label_set_text(tl->train_direction_label, "佛祖岭 方向");
    lv_label_set_text(tl->remain_sta_label,      "还有2站");
    lv_label_set_text(tl->remain_time_label,     "03:15"  );
    lv_label_set_text(tl->arrive_time_label,     "预计 08:30" );

}


void sta_canvas_init(lv_obj_t * canvas)
{
    lv_obj_clear_flag(canvas, LV_OBJ_FLAG_SCROLLABLE);

    sta_canvas_buf = sdram_malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR(STATION_INFO_W,156));
    if (!sta_canvas_buf) while (1);

    lv_canvas_set_buffer(canvas, sta_canvas_buf, STATION_INFO_W, 156, LV_IMG_CF_TRUE_COLOR);

    lv_obj_add_flag(canvas, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(canvas, sta_canvas_cb, LV_EVENT_ALL, NULL);

    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);

    //初始化列车图片
    for (int8_t i = 0; i < 5; i++)
    {
        train_icon[i] = lv_img_create(canvas);
        lv_img_set_src(train_icon[i], "0:/train.bin");
        lv_obj_add_flag(train_icon[i], LV_OBJ_FLAG_HIDDEN);
    }

}


void sta_create_metro_line(lv_obj_t * canvas,MetroLine *line , Station *sta)
{
    lv_draw_line_dsc_t dsc_before, dsc_after;

    //设置线条样式
    lv_draw_line_dsc_init(&dsc_before);
    dsc_before.color = lv_color_hex(COLOR_MID_GRAY);
    dsc_before.width = STA_LINE_W;
    dsc_before.round_start = true;
    dsc_before.round_end = true;  //圆角
    dsc_before.opa = LV_OPA_COVER;  //不透明度

    lv_draw_line_dsc_init(&dsc_after);
    dsc_after.color = lv_color_hex(line->line_color);
    dsc_after.width = STA_LINE_W;
    dsc_after.round_start = true;
    dsc_after.round_end = true;  //圆角
    dsc_after.opa = LV_OPA_COVER;  //不透明度
    
    //设置站点样式
    lv_draw_rect_dsc_t rect_dsc_before;
    lv_draw_rect_dsc_init(&rect_dsc_before);

    rect_dsc_before.bg_color = lv_color_white();
    rect_dsc_before.outline_width = OUTLINE_WIDTH;
    rect_dsc_before.bg_opa = LV_OPA_COVER;
    rect_dsc_before.radius = STATION_RADIUS_NORM;
    rect_dsc_before.outline_color = lv_color_hex(COLOR_MID_GRAY);

    lv_draw_rect_dsc_t rect_dsc_now;
    lv_draw_rect_dsc_init(&rect_dsc_now);

    rect_dsc_now.bg_color = lv_color_white();
    rect_dsc_now.outline_width = 3;
    rect_dsc_now.bg_opa = LV_OPA_COVER;
    rect_dsc_now.radius = STATION_RADIUS_NOW;
    rect_dsc_now.outline_color = lv_color_hex(COLOR_MID_BLUE);

    lv_draw_rect_dsc_t rect_dsc_after;
    lv_draw_rect_dsc_init(&rect_dsc_after);

    rect_dsc_after.bg_color = lv_color_white();
    rect_dsc_after.outline_width = 2;
    rect_dsc_after.bg_opa = LV_OPA_COVER;
    rect_dsc_after.radius = STATION_RADIUS_NORM;
    rect_dsc_after.outline_color = lv_color_hex(line->line_color);

    lv_draw_label_dsc_t label_dsc_before;
    lv_draw_label_dsc_init(&label_dsc_before);

    label_dsc_before.color = lv_color_hex(COLOR_MID_GRAY);
    label_dsc_before.font = &heiti_16;
    label_dsc_before.align = LV_TEXT_ALIGN_CENTER;
    label_dsc_before.flag = LV_TEXT_FLAG_FIT;

    lv_draw_label_dsc_t label_dsc_now;
    lv_draw_label_dsc_init(&label_dsc_now);

    label_dsc_now.color = lv_color_hex(COLOR_MID_BLUE);
    label_dsc_now.font = &heiti_16;
    label_dsc_now.align = LV_TEXT_ALIGN_CENTER;
    label_dsc_now.flag = LV_TEXT_FLAG_FIT;

    lv_draw_label_dsc_t label_dsc_after;
    lv_draw_label_dsc_init(&label_dsc_after);

    label_dsc_after.color = lv_color_hex(line->line_color);
    label_dsc_after.font = &heiti_16;
    label_dsc_after.align = LV_TEXT_ALIGN_CENTER;
    label_dsc_after.flag = LV_TEXT_FLAG_FIT;

    lv_draw_rect_dsc_init(&rect_dsc_line);

    rect_dsc_line.outline_width = 0;
    rect_dsc_line.bg_opa = LV_OPA_COVER;
    rect_dsc_line.radius = BUTTON_RADIUS;
    
    //清空画布
    memset(sta_canvas_buf, 255, LV_CANVAS_BUF_SIZE_TRUE_COLOR(STATION_INFO_W,156));

    //创建点列表
    lv_point_t pts[3];

    pts[0].x = sta_coord_trans(1, sta_origin_x);
    pts[0].y = STATION_Y;

    pts[2].x = sta_coord_trans(line->count, sta_origin_x);
    pts[2].y = STATION_Y;

    if (!direction_state)//上行方向
    {
        pts[1].x = sta_coord_trans(sta->id, sta_origin_x);
        pts[1].y = STATION_Y;
    }
    else
    {
        pts[1].x = sta_coord_trans(line->count - sta->id + 1, sta_origin_x);
        pts[1].y = STATION_Y;
    }
    lv_canvas_draw_line(canvas, pts, 2, &dsc_before);
    lv_canvas_draw_line(canvas, pts + 1, 2, &dsc_after);

    lv_coord_t x , y;
    y = STATION_Y;
    int8_t after_state = 0;
    int8_t radius;
    //绘制站点
    if (!direction_state)//上行方向
    {
        for (int8_t i = 1; i <= line->count; i++)
        {
            x = sta_coord_trans(i , sta_origin_x);
            if (!after_state)
            {
                if(i == sta->id)
                {
                    radius = STATION_RADIUS_NOW;
                    lv_canvas_draw_rect(canvas,x - radius,y - radius,
                    radius * 2,radius * 2,&rect_dsc_now);  //站点形状

                    station_name_text_show(canvas, x, &label_dsc_now, &(line->stations[i-1]));

                    after_state = 1;
                }
                else
                {
                    radius = STATION_RADIUS_NORM;
                    lv_canvas_draw_rect(canvas,x - radius,y - radius,
                        radius * 2,radius * 2,&rect_dsc_before);
                    
                    station_name_text_show(canvas, x, &label_dsc_before, &(line->stations[i-1]));
                }
            }
            else
            {
                radius = STATION_RADIUS_NORM;
                lv_canvas_draw_rect(canvas,x - radius,y - radius,
                    radius * 2,radius * 2,&rect_dsc_after);  //站点形状
                
                station_name_text_show(canvas, x, &label_dsc_after, &(line->stations[i-1]));
            }
        }
        for (int i = 0; i < 1; i++)
        {
            lv_obj_set_pos(train_icon[i], sta_coord_trans(sta->id, sta_origin_x) - STATION_DIST/2 - 38, STATION_Y - 45);
            lv_obj_clear_flag(train_icon[i], LV_OBJ_FLAG_HIDDEN);
        }
    }
    else
    {
        for (int8_t i = line->count ; i >= 1; i--)
        {
            x = sta_coord_trans(line->count - i + 1, sta_origin_x);
            if (!after_state)
            {
                if(i == sta->id)
                {
                    radius = STATION_RADIUS_NOW;
                    lv_canvas_draw_rect(canvas,x - radius,y - radius,
                    radius * 2,radius * 2,&rect_dsc_now);  //站点形状

                    station_name_text_show(canvas, x, &label_dsc_now, &(line->stations[i-1]));

                    after_state = 1;
                }
                else
                {
                    radius = STATION_RADIUS_NORM;
                    lv_canvas_draw_rect(canvas,x - radius,y - radius,
                        radius * 2,radius * 2,&rect_dsc_before);
                    station_name_text_show(canvas, x, &label_dsc_before, &(line->stations[i-1]));
                }
            }
            else
            {
                radius = STATION_RADIUS_NORM;
                lv_canvas_draw_rect(canvas,x - radius,y - radius,
                    radius * 2,radius * 2,&rect_dsc_after);  //站点形状

                station_name_text_show(canvas, x, &label_dsc_after, &(line->stations[i-1]));
                
            }
        }
        //绘制电车图标
        for (int i = 0; i < 1; i++)
        {
            lv_obj_set_pos(train_icon[i], 
                sta_coord_trans(line->count - sta->id + 1, sta_origin_x) - STATION_DIST/2 - 38, 
                STATION_Y - 45);
            lv_obj_clear_flag(train_icon[i], LV_OBJ_FLAG_HIDDEN);
        }
    }
}

void station_name_text_show(lv_obj_t * canvas, lv_coord_t x, lv_draw_label_dsc_t* dsc, const Station *sta)
{
    lv_coord_t line_height = 18; /* 行高 */ 

    const char * ptr = sta->name;
    int8_t i = 0;
    if (sta->is_transfer)
    {
        int8_t transfer_line = sta->is_transfer;
        if (transfer_line < 0) transfer_line = - transfer_line;
        lv_color_t color;
        switch (transfer_line)
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

        rect_dsc_line.bg_color = color;

        lv_canvas_draw_rect(canvas, x+1, STATION_Y + 10,19,54,&rect_dsc_line);

        char line_str[3];
        snprintf(line_str, sizeof(line_str), "%d",transfer_line);

        lv_draw_label_dsc_t label_dsc_transfer;
        lv_draw_label_dsc_init(&label_dsc_transfer);

        label_dsc_transfer.color = lv_color_white();
        label_dsc_transfer.font = &heiti_16;
        label_dsc_transfer.align = LV_TEXT_ALIGN_CENTER;
        label_dsc_transfer.flag = LV_TEXT_FLAG_FIT;
        
        lv_canvas_draw_text(canvas, x + 2, STATION_Y + 10+1, 18, &label_dsc_transfer, line_str);
        lv_canvas_draw_text(canvas, x + 2, STATION_Y + 10+17, 18, &label_dsc_transfer, "号");
        lv_canvas_draw_text(canvas, x + 2, STATION_Y + 10+34, 18, &label_dsc_transfer, "线");

        while (*ptr) {
        char single_char[5] = {0}; /* UTF-8 最多占用4个字节+结束符 */

        /* 获取当前UTF-8字符的字节长度，并拷贝 */
        uint32_t char_len = 3;//]lv_utf8_get_bytes_len(ptr);
        if (char_len == 0) break;
        strncpy(single_char, ptr, char_len);
        single_char[char_len] = '\0';

        lv_canvas_draw_text(canvas, x - 17, STATION_Y + 10 + line_height * i, 18, dsc, single_char);
        
        ptr += char_len;
        i++;
        }
    }
    else
    {
        while (*ptr) 
        {
            char single_char[5] = {0}; /* UTF-8 最多占用4个字节+结束符 */

            /* 获取当前UTF-8字符的字节长度，并拷贝 */
            uint32_t char_len = 3;//]lv_utf8_get_bytes_len(ptr);
            if (char_len == 0) break;
            strncpy(single_char, ptr, char_len);
            single_char[char_len] = '\0';

            lv_canvas_draw_text(canvas, x- 8, STATION_Y + 10 + line_height * i, 18, dsc, single_char);
            
            ptr += char_len;
            i++;
        }
    }
}


void sta_canvas_cb(lv_event_t * e)
{
    lv_obj_t * canvas = lv_event_get_target(e);//获取产生这个事件的对象
    lv_indev_t * indev = lv_indev_get_act();//获取输入设备
    lv_event_code_t code = lv_event_get_code(e);//获取事件名称

    switch (code)
    {
        case LV_EVENT_PRESSING:
            sta_pressing_canvas(indev);
            break;
        case LV_EVENT_SHORT_CLICKED:
            sta_click_canvas(indev, &metro_lines[showing_station.line_belonged - 1]);
            break;
        default:
            break;
    }

}


void sta_pressing_canvas(lv_indev_t * indev)
{
    int8_t line_len = metro_lines[showing_station.line_belonged - 1].count;
    
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    if (!vect.x && !vect.y) return;

    sta_origin_x += vect.x;

    //设置边界

    if (sta_origin_x > 30) sta_origin_x = 30;
    else if (sta_origin_x < - line_len * STATION_DIST + STATION_INFO_W - 10)
        sta_origin_x = - line_len * STATION_DIST + STATION_INFO_W - 10;

    sta_create_metro_line(sta_line_canvas ,&metro_lines[showing_station.line_belonged - 1] , &showing_station);
}

void sta_click_canvas(lv_indev_t *indev, MetroLine *line)
{
    lv_point_t pos;
    lv_indev_get_point(indev, &pos);

    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    if (vect.x || vect.y) return;

    //转化为canvas的坐标
    pos.x -= CANVAS_X + STATION_INFO_X;
    pos.y -= CANVAS_Y + 100;
    if(!direction_state)
    { 
        for (int8_t i = 0; i < line->count; i++)
        {
            lv_coord_t x = sta_coord_trans(i + 1, sta_origin_x);
            if((pos.y >= STATION_Y && pos.y < 156) && (pos.x - x < TOUCH_RANGE && pos.x - x > -TOUCH_RANGE))
            {

                if (line->stations[i].id == showing_station.id && line->stations[i].is_transfer)
                {
                    int8_t transfer_line = line->stations[i].is_transfer;
                    int8_t id = line->stations[i].transfer_id;
                    if (transfer_line < 0) transfer_line = - transfer_line;

                    station_info_show(&(metro_lines[transfer_line - 1].stations[id - 1]),true);
                    return;
                }
                else
                {
                    station_info_show(&(line->stations[i]), false);
                    return;
                }
            }
        }
    }
    else{
        for (int8_t i = line->count - 1; i >= 0; i--)
        {
            lv_coord_t x = sta_coord_trans(line->count - i, sta_origin_x);
            if((pos.y >= STATION_Y && pos.y < 156) && (pos.x - x < TOUCH_RANGE && pos.x - x > -TOUCH_RANGE))
            {
                if (line->stations[i].id == showing_station.id && line->stations[i].is_transfer)
                {
                    int8_t transfer_line = line->stations[i].is_transfer;
                    int8_t id = line->stations[i].transfer_id;
                    if (transfer_line < 0) transfer_line = - transfer_line;

                    station_info_show(&(metro_lines[transfer_line - 1].stations[id - 1]),true);
                    return;
                }
                else
                {
                    station_info_show(&(line->stations[i]), false);
                    return;
                }
            }
        }
    }

}

lv_coord_t sta_coord_trans(lv_coord_t pos, lv_coord_t origin)
{
    lv_coord_t res = (pos-1) * STATION_DIST + origin;
    return res;
}



void station_info_show(const Station *sta, int8_t is_init)
{
    showing_station.name = sta->name;
    showing_station.line_belonged = sta->line_belonged;
    showing_station.id = sta->id;

    
    lv_obj_clear_flag(station_info_disp, LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text(sta_name_label, sta->name);


    int8_t line_number = showing_station.line_belonged;

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

    station_info_refresh(is_init);

}

void station_info_refresh(int8_t is_init)
{
    int8_t line_number = showing_station.line_belonged;
    char * sta1 = metro_lines[line_number - 1].stations[0].name;
    char * sta2 = metro_lines[line_number - 1].stations[metro_lines[line_number - 1].count - 1].name;
    char line_str[10], sta1_str[24], sta2_str[24];
    snprintf(line_str, sizeof(line_str), "%d号线",line_number);
    snprintf(sta1_str, sizeof(sta1_str), "%s 方向",sta1);
    snprintf(sta2_str, sizeof(sta2_str), "%s 方向",sta2);

    lv_label_set_text(sta_line_number_label, line_str);

    //调整原点使得目标车站显示在适当位置
    if (is_init)
    {
        int8_t line_len = metro_lines[showing_station.line_belonged - 1].count;
        if (direction_state) 
        {
            lv_label_set_text(sta_direction_label, sta1_str);
            sta_origin_x = - (line_len - showing_station.id) * STATION_DIST + 250;
        }
        else 
        {
            lv_label_set_text(sta_direction_label, sta2_str);
            sta_origin_x = - (showing_station.id - 1) * STATION_DIST + 250;
        }
        if (sta_origin_x > 30) sta_origin_x = 30;
        else if (sta_origin_x < - line_len * STATION_DIST + STATION_INFO_W - 10)
            sta_origin_x = - line_len * STATION_DIST + STATION_INFO_W - 10;
    }

    lv_label_set_text(sta_first_train_label, "首班 06:15");
    lv_label_set_text(sta_last_train_label, "末班 23:15");

    
    sta_create_metro_line(sta_line_canvas ,&metro_lines[showing_station.line_belonged - 1] , &showing_station);

}

void change_btn_cb(lv_event_t * e)
{
    if (direction_state) direction_state = 0;
    else direction_state = 1;
    
    station_info_refresh(true);
}

