#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <string.h>
#include <stdio.h>
#include "gui.h"
#include "metro_line.h"
#include "canvas.h"


void *canvas_buf;//缓冲区

int8_t para_numbers[5] = {0,1,2,3,4};  //妥协之举，传参只能传地址


lv_obj_t *btn_plus , *btn_minus;
lv_obj_t *lines_container;
lv_style_t btn_style, rec_style, selector_style, line_style;//创建样式
lv_obj_t * label_plus, *label_minus;
lv_obj_t line_btns[4];
lv_obj_t line_labels[4][2];
lv_obj_t * location_image;//位置的照片

lv_obj_t * pop_window;//弹出的窗口
lv_obj_t *line_info_btn1;//线路信息
lv_obj_t *line_number_label, *dire_label1, *dire_label2, *time_label1, *time_label2;
lv_obj_t * start_btn, *end_btn, *start_label, *end_label;
lv_obj_t * star_btn, *star_label, *pop_top_label;

lv_style_t flame_style, line_info_style, blue_label_style, black_label_style;
lv_style_t blue_button_style, white_button_style;


void canvas_init(lv_obj_t *canvas)
{
    lv_obj_clear_flag(canvas, LV_OBJ_FLAG_SCROLLABLE);
    
    //申请缓冲区以设置像素
    canvas_buf = sdram_malloc(BUF_SIZE);
    if (!canvas_buf) while (1);
    
    lv_obj_set_pos(canvas,CANVAS_X,CANVAS_Y);
    lv_obj_set_size(canvas,CANVAS_W,CANVAS_H);

    lv_canvas_set_buffer(canvas, canvas_buf, CANVAS_W, CANVAS_H, LV_IMG_CF_TRUE_COLOR);

    lv_obj_add_flag(canvas, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_event_cb(canvas, canvas_cb, LV_EVENT_ALL, NULL);

    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);

    buttons_init(canvas);

    location_pic_init(canvas);

    lines_selector_init(canvas, metro_lines);

    pop_window_init(canvas);

    create_metro_map();
}
void location_pic_init(lv_obj_t *canvas)
{
    location_image = lv_img_create(canvas);
	lv_img_set_src(location_image, "0:/location.bin");
    lv_obj_add_flag(location_image, LV_OBJ_FLAG_HIDDEN);

}

void buttons_init(lv_obj_t *canvas)
{


    lv_style_init(&btn_style);

    lv_style_set_bg_color(&btn_style, lv_color_hex(COLOR_LIGHT_GRAY));
    lv_style_set_border_color(&btn_style, lv_color_hex(COLOR_MID_GRAY));
    lv_style_set_border_width(&btn_style, 1);
    lv_style_set_radius(&btn_style, BUTTON_RADIUS);
    lv_style_set_opa(&btn_style, LV_OPA_COVER);


    btn_plus = lv_btn_create(canvas);
    lv_obj_set_pos(btn_plus , BUTTON_X, BUTTON_Y);
    lv_obj_set_size(btn_plus, BUTTON_LEN, BUTTON_LEN);


    label_plus = lv_label_create(btn_plus);
    lv_label_set_text(label_plus, "+");
    lv_obj_center(label_plus);
    lv_obj_set_style_text_color(label_plus, lv_color_black(),0);

    btn_minus = lv_btn_create(canvas);
    lv_obj_set_pos(btn_minus , BUTTON_X , BUTTON_Y + BUTTON_LEN + 10);
    lv_obj_set_size(btn_minus, BUTTON_LEN, BUTTON_LEN);

    label_minus = lv_label_create(btn_minus);
    lv_label_set_text(label_minus, "-");
    lv_obj_center(label_minus);
    lv_obj_set_style_text_color(label_minus, lv_color_black(),0);

    lv_obj_add_style(btn_plus, &btn_style, 0);
    lv_obj_add_style(btn_minus, &btn_style, 0);

    lv_obj_add_event_cb(btn_plus, magnify_button_cb, LV_EVENT_PRESSED, &plus);
    lv_obj_add_event_cb(btn_minus, magnify_button_cb, LV_EVENT_PRESSED, &minus);
}

void lines_selector_init(lv_obj_t *canvas, MetroLine *lines)
{

    lv_style_init(&selector_style);

    lv_style_set_bg_color(&selector_style, lv_color_white());
    //lv_style_set_border_color(&selector_style, lv_color_hex(COLOR_MID_GRAY));
    lv_style_set_border_width(&selector_style, 0);
    lv_style_set_radius(&selector_style, BUTTON_RADIUS);
    lv_style_set_opa(&selector_style, LV_OPA_COVER);
    lv_style_set_shadow_opa(&selector_style, LV_OPA_0);


    lv_style_init(&line_style);
    for (int i = 0; i < 4;i++)
    {
        lines_btn_init(line_btns + i, line_labels[i], canvas, lines + i);
    }

}

//绘制按钮
void lines_btn_init(lv_obj_t * btn ,lv_obj_t * labels, lv_obj_t *canvas ,  MetroLine *line)
{
    char * str, longstr[10];
    int8_t count = line->line_number;
    sprintf(str, "%d", count);
    
    btn = lv_btn_create(canvas);

    lv_obj_set_pos(btn, REC_X + 10 , REC_Y + 10 + 33 *(count -1));
    lv_obj_set_size(btn, 84 , 24);

    lv_obj_add_style(btn, &selector_style, 0);

    labels = lv_label_create(btn);
    lv_obj_set_pos(labels, -20 ,-11);
    lv_obj_set_size(labels, 24, 24);

    lv_obj_set_style_text_font(labels, &heiti_16, 0);
    lv_obj_set_style_text_color(labels, lv_color_white(), 0);
    lv_obj_set_style_bg_color(labels, lv_color_hex(line->line_color), 0);
    lv_obj_set_style_bg_opa(labels, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(labels, 4, 0);
    lv_obj_set_style_text_align(labels, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_top(labels, 4, 0);
    
    
    lv_label_set_text(labels, str);

    lv_obj_t * label2 = labels + 1;

    label2 = lv_label_create(btn);
    
    lv_obj_set_pos(label2, 14 ,-8);
    lv_obj_set_size(label2, 48, 18);
    lv_obj_set_style_text_font(label2, &heiti_16, 0);
    lv_obj_set_style_text_color(label2, lv_color_black(), 0);
    lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_LEFT, 0);


    snprintf(longstr, sizeof(longstr), "%d号线",count);
    lv_label_set_text(label2, longstr);

    lv_obj_add_event_cb(btn, lines_selector_cb, LV_EVENT_PRESSED, para_numbers + count);

}


void lines_selector_cb(lv_event_t * e)
{
    int8_t *para_number = lv_event_get_user_data(e);

    if (*para_number == is_showing) is_showing = 0;
    else is_showing = *para_number;

    create_metro_map();
}

void canvas_cb(lv_event_t * e)
{
    lv_obj_t * canvas = lv_event_get_target(e);//获取产生这个事件的对象
    lv_indev_t * indev = lv_indev_get_act();//获取输入设备
    lv_event_code_t code = lv_event_get_code(e);//获取事件名称

    switch (code)
    {
        case LV_EVENT_PRESSING:
            pressing_canvas(indev);
            break;
        case LV_EVENT_SHORT_CLICKED:
            clicked_canvas(indev, metro_lines);
            break;
        default:
            break;
    }

}

void pressing_canvas(lv_indev_t * indev)
{
    lv_point_t vect;
    lv_indev_get_vect(indev, &vect);

    origin_x += vect.x;
    origin_y += vect.y;

    //设置边界

    if (origin_x > CANVAS_W/3) origin_x = CANVAS_W/3;
    if (origin_y > CANVAS_H/3) origin_y = CANVAS_H/3;

    if (origin_x < CANVAS_W/3*2 - GEO_X_MAX * magnify_size) 
        origin_x = CANVAS_W/3*2 - GEO_X_MAX * magnify_size;
    if (origin_y < CANVAS_H/3*2 - GEO_Y_MAX * magnify_size) 
        origin_y = CANVAS_H/3*2 - GEO_Y_MAX * magnify_size;


    create_metro_map();

}

void magnify_button_cb(lv_event_t * e)
{
    int8_t *state = lv_event_get_user_data(e);

    uint16_t pre_size = magnify_size;
    magnify_size += (uint16_t)((*state) * STEP);

    if (magnify_size < 20) 
    {
        magnify_size = 20;
        return;
    }
    if (magnify_size > 100) 
    {    
        magnify_size = 100;
        return;
    }
    //以画布中心为原点 计算偏移量
    lv_coord_t dist_x = origin_x - CANVAS_W/2;
    lv_coord_t dist_y = origin_y - CANVAS_H/2;
    

    dist_x = (lv_coord_t)((float)magnify_size/pre_size * dist_x);
    dist_y = (lv_coord_t)((float)magnify_size/pre_size * dist_y);


    origin_x = CANVAS_W/2 + dist_x;
    origin_y = CANVAS_H/2 + dist_y;

    create_metro_map();
}


//按下画布显示车站
void clicked_canvas(lv_indev_t *indev, MetroLine *lines)
{
    lv_point_t pos;
    lv_indev_get_point(indev, &pos);

    //转化为canvas的坐标
    pos.x -= CANVAS_X;
    pos.y -= CANVAS_Y;

    for (int8_t i = 0; i < 4; i++)
    {
        for (int8_t j = 0; j < lines[i].count; j++)
        {
            lv_coord_t x = geo_to_screen(lines[i].stations[j].geo_x ,origin_x);
            lv_coord_t y = geo_to_screen(lines[i].stations[j].geo_y ,origin_y);
            if ((pos.x - x < TOUCH_RANGE && pos.x - x > -TOUCH_RANGE) && 
                (pos.y - y < TOUCH_RANGE && pos.y - y > -TOUCH_RANGE))
            {
                station_clicked.geo_x = lines[i].stations[j].geo_x;
                station_clicked.geo_y = lines[i].stations[j].geo_y;
                station_clicked.line_belonged = (int8_t)(i + 1);
                station_clicked.is_transfer = lines[i].stations[j].is_transfer;
                station_clicked.name = lines[i].stations[j].name;

                pop_window_show(&station_clicked);

                if (!is_station_clicked)
                {
                    lv_obj_clear_flag(location_image, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(pop_window, LV_OBJ_FLAG_HIDDEN);
                    is_station_clicked = 1;
                }
                return;
            }        
        }
    }
    if (is_station_clicked)
    {
        lv_obj_add_flag(location_image, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(pop_window, LV_OBJ_FLAG_HIDDEN);
        is_station_clicked = 0;
    }
}

void pop_window_init(lv_obj_t * obj)
{
    //设置样式
    lv_style_init(&line_info_style);

    lv_style_set_bg_color(&line_info_style, lv_color_white());
    lv_style_set_border_color(&line_info_style, lv_color_hex(COLOR_MID_GRAY));
    lv_style_set_border_width(&line_info_style, 1);
    lv_style_set_radius(&line_info_style, BUTTON_RADIUS);
    lv_style_set_opa(&line_info_style, LV_OPA_COVER);
    lv_style_set_pad_all(&line_info_style, 9);
    lv_style_set_shadow_opa(&line_info_style, LV_OPA_0);

    //lv_style_set_shadow_spread(&line_info_style, 10);


    lv_style_init(&flame_style);

    lv_style_set_bg_color(&flame_style, lv_color_white());
    lv_style_set_border_color(&flame_style, lv_color_hex(COLOR_MID_GRAY));
    lv_style_set_border_width(&flame_style, 1);
    lv_style_set_radius(&flame_style, BUTTON_RADIUS);
    lv_style_set_opa(&flame_style, LV_OPA_COVER);
    lv_style_set_bg_opa(&flame_style, LV_OPA_COVER);
    lv_style_set_pad_all(&flame_style, 0);
    lv_style_set_shadow_opa(&flame_style, LV_OPA_30);
    lv_style_set_shadow_ofs_x(&flame_style, 5);
    lv_style_set_shadow_ofs_y(&flame_style, 5);
    lv_style_set_shadow_width(&flame_style, 15);
    lv_style_set_shadow_color(&flame_style, lv_color_black());
    //lv_style_set_shadow_spread(&flame_style, 100);
    //lv_style_set_sha

    lv_style_init(&blue_label_style);
    lv_style_set_text_font(&blue_label_style, &heiti_16);
    lv_style_set_text_align(&blue_label_style, LV_TEXT_ALIGN_LEFT);
    lv_style_set_text_color(&blue_label_style, lv_color_hex(COLOR_DARK_BLUE));

    lv_style_init(&black_label_style);
    lv_style_set_text_font(&black_label_style, &heiti_16);
    lv_style_set_text_align(&black_label_style, LV_TEXT_ALIGN_LEFT);
    lv_style_set_text_color(&black_label_style, lv_color_black());

    lv_style_init(&blue_button_style);

    lv_style_set_bg_color(&blue_button_style, lv_color_hex(COLOR_MID_BLUE));
    lv_style_set_border_color(&blue_button_style, lv_color_hex(COLOR_DARK_BLUE));
    lv_style_set_border_width(&blue_button_style, 1);
    lv_style_set_radius(&blue_button_style, BUTTON_RADIUS);
    lv_style_set_opa(&blue_button_style, LV_OPA_COVER);


    pop_window = lv_obj_create(obj);
    lv_obj_move_foreground(pop_window);
    lv_obj_add_style(pop_window, &flame_style, 0);
    lv_obj_clear_flag(pop_window, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_pos(pop_window, 200, 200);
    lv_obj_set_size(pop_window, POP_WINDOW_W,POP_WINDOW_H_NORM);
    
    pop_top_label = lv_label_create(pop_window);
    lv_obj_set_size(pop_top_label, POP_WINDOW_W, TOP_BAR_H);
    lv_obj_set_pos(pop_top_label,0 , 0);

    lv_obj_set_style_bg_color(pop_top_label, lv_color_hex(COLOR_LIGHT_BLUE), 0);
    lv_obj_set_style_bg_opa(pop_top_label, LV_OPA_COVER, 0);
    lv_obj_set_style_text_align(pop_top_label, LV_TEXT_ALIGN_LEFT, 0);
    lv_obj_set_style_pad_top(pop_top_label, 12, 0);
    lv_obj_set_style_pad_left(pop_top_label, 12 , 0);
    lv_obj_set_style_text_font(pop_top_label, &heiti_20, 0);

    char *str = "街道口";
    lv_label_set_text(pop_top_label, str);

    line_info_btn_init(pop_window, line_info_btn1 ,0);
    
    start_btn = lv_btn_create(pop_window);
    
    lv_obj_set_pos(start_btn, 10 ,BOTTOM_BTN_Y_NORM);
    lv_obj_set_size(start_btn,BOTTOM_BTN_W, BOTTOM_BTN_H);

    start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "设为起点");
    lv_obj_set_style_text_font(start_btn, &heiti_16, 0);
    lv_obj_set_style_text_color(start_label, lv_color_black(), 0);

    lv_obj_center(start_label);

    lv_obj_add_style(start_btn, &btn_style, 0);

    end_btn = lv_btn_create(pop_window);
    
    lv_obj_set_pos(end_btn, BOTTOM_BTN_X ,BOTTOM_BTN_Y_NORM);
    lv_obj_set_size(end_btn,BOTTOM_BTN_W, BOTTOM_BTN_H);

    end_label = lv_label_create(end_btn);
    lv_label_set_text(end_label, "设为终点");
    lv_obj_set_style_text_font(end_label, &heiti_16, 0);
    lv_obj_set_style_text_color(end_label, lv_color_black(), 0);
    lv_obj_center(end_label);

    lv_obj_add_style(end_btn, &btn_style, 0);

    star_btn = lv_btn_create(pop_window);
    
    lv_obj_set_pos(star_btn, 206 , 8);
    lv_obj_set_size(star_btn,TOP_BTN_W, TOP_BTN_H);

    star_label = lv_label_create(star_btn);
    lv_label_set_text(star_label, "收藏");
    lv_obj_set_style_text_font(star_label, &heiti_16, 0);
    lv_obj_set_style_text_color(star_label, lv_color_white(), 0);
    lv_obj_center(star_label);

    lv_obj_add_style(star_btn, &blue_button_style, 0);
    
    lv_obj_add_flag(pop_window, LV_OBJ_FLAG_HIDDEN);
}


void line_info_btn_init(lv_obj_t * obj, lv_obj_t * btn, int8_t count)
{
    btn = lv_btn_create(obj);

    lv_obj_set_pos(btn, 10 , 56 + count * (LINE_BTN_H + 10));
    lv_obj_set_size(btn, LINE_BTN_W, LINE_BTN_H);

    lv_obj_add_style(btn, &line_info_style, 0);

    line_number_label = lv_label_create(btn);
    lv_obj_set_pos(line_number_label, 0, 0);
    lv_obj_set_size(line_number_label, 63 , 29);

    lv_obj_set_style_bg_opa(line_number_label, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(line_number_label, BUTTON_RADIUS, 0);
    lv_obj_set_style_text_color(line_number_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(line_number_label, &heiti_16, 0);
    lv_obj_set_style_bg_color(line_number_label, lv_color_hex(COLOR_LINE2), 0);
    lv_obj_set_style_pad_top(line_number_label, 6, 0);
    lv_obj_set_style_text_align(line_number_label, LV_TEXT_ALIGN_CENTER, 0);

    lv_label_set_text(line_number_label, "2号线");

    dire_label1 = lv_label_create(btn);
    dire_label2 = lv_label_create(btn);
    time_label1 = lv_label_create(btn);
    time_label2 = lv_label_create(btn);

    lv_obj_add_style(dire_label1, &black_label_style, 0);
    lv_obj_add_style(dire_label2, &black_label_style, 0);
    lv_obj_add_style(time_label1, &blue_label_style, 0);
    lv_obj_add_style(time_label2, &blue_label_style, 0);

    lv_obj_set_pos(dire_label1, 72, 0);
    lv_obj_set_pos(time_label1, 72, 24);
    lv_obj_set_pos(dire_label2, 72, 60);
    lv_obj_set_pos(time_label2, 72, 60 + 24);

    lv_label_set_text(dire_label1 , "佛祖岭 方向");
    lv_label_set_text(dire_label2 , "天河机场 方向");
    
    lv_label_set_text(time_label1 , "首班 06:10  末班 23:10");
    lv_label_set_text(time_label2 , "首班 06:10  末班 23:10");
    
}

void pop_window_move(Station *sta)
{
    lv_coord_t x = geo_to_screen(sta->geo_x, origin_x);
    lv_coord_t y = geo_to_screen(sta->geo_y, origin_y);

    lv_obj_set_pos(location_image, x - 38, y - 75);

    x += 25;
    if (sta->is_transfer)
    {
        y -= 70 + POP_WINDOW_H_TRANS;
        if (y > CANVAS_H - 10 - POP_WINDOW_H_TRANS)
            y = CANVAS_H - 10 - POP_WINDOW_H_TRANS;
    }
    else 
    {   
        y -= 75 + POP_WINDOW_H_NORM;
        if (y > CANVAS_H - 10 - POP_WINDOW_H_NORM)
            y = CANVAS_H - 10 - POP_WINDOW_H_NORM;
    }

    if (x < 10) x = 10;
    if (x > CANVAS_W - 10 - POP_WINDOW_W) x = CANVAS_W - 10 - POP_WINDOW_W;
    if (y < 10) y = 10;
    

    //x += CANVAS_X;
    //y += CANVAS_Y;
    lv_obj_set_pos(pop_window, x , y );
    
}
    
void pop_window_show(Station *sta)
{
    pop_window_move(sta);

    if (sta->is_transfer) lv_obj_set_size(pop_window, POP_WINDOW_W,POP_WINDOW_H_TRANS);
    else lv_obj_set_size(pop_window, POP_WINDOW_W,POP_WINDOW_H_NORM);

    lv_label_set_text(pop_top_label, sta->name);
    //lv_label_set_text(pop_top_label, "循礼门");

    int8_t line_number = sta->line_belonged;
    char * sta1 = metro_lines[line_number - 1].stations[0].name;
    char * sta2 = metro_lines[line_number - 1].stations[metro_lines[line_number - 1].count - 1].name;
    char line_str[10], sta1_str[24], sta2_str[24];
    snprintf(line_str, sizeof(line_str), "%d号线",line_number);
    snprintf(sta1_str, sizeof(sta1_str), "%s 方向",sta1);
    snprintf(sta2_str, sizeof(sta2_str), "%s 方向",sta2);

    lv_label_set_text(line_number_label, line_str);
    //lv_label_set_text(line_number_label, "1号线");

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

    lv_obj_set_style_bg_color(line_number_label, color, 0);

    lv_label_set_text(dire_label1, sta1_str);
    lv_label_set_text(dire_label2, sta2_str);

}

