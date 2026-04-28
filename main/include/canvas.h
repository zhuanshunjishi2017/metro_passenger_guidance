#ifndef CANVAS
#define CANVAS

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "metro_line.h"


extern int8_t plus, minus;
extern MetroLine metro_lines[4];

extern int8_t is_showing;
extern lv_obj_t  *display0;

extern int8_t is_station_clicked;
extern Station station_clicked[2];

extern int8_t is_station_info ;

extern lv_obj_t * station_info_disp;

extern lv_coord_t origin_x, origin_y, magnify_size;

extern lv_style_t btn_style, black_label_style, blue_label_style;
extern lv_style_t blue_button_style;

typedef struct {
    lv_obj_t * label_frame;
    lv_obj_t * train_order_label;
    lv_obj_t * train_direction_label;
    lv_obj_t * remain_time_label;
    lv_obj_t * remain_sta_label;
    lv_obj_t * arrive_time_label;
} TimeLabel;

//定义一个显示站点简略信息的按钮结构体

typedef struct {
    lv_obj_t * line_info_btn;
    lv_obj_t * line_number_label;
    lv_obj_t * direction_label1;
    lv_obj_t * direction_label2;
    lv_obj_t * time_label1;
    lv_obj_t * time_label2;
} LineinfoBtn;


/*
下面是有关画布的函数声明
*/

void canvas_init(lv_obj_t *);
void buttons_init(lv_obj_t *);
void location_pic_init(lv_obj_t *);
void pop_window_init(lv_obj_t * );


void magnify_button_cb(lv_event_t *e);
void canvas_cb(lv_event_t * e);

void pressing_canvas(lv_indev_t *);
void clicked_canvas(lv_indev_t *, MetroLine *);

void lines_selector_init(lv_obj_t * , MetroLine *);
void lines_selector_cb(lv_event_t * e);

void lines_btn_init(lv_obj_t * btn ,lv_obj_t * labels, lv_obj_t *canvas , MetroLine *line);
void line_info_btn_init(lv_obj_t * obj, LineinfoBtn * btn, int8_t count);

void pop_window_move(Station *sta);
void pop_window_show(Station *sta, LineinfoBtn * btn);

void line_info_btn_cb(lv_event_t * e);


/*
下面是有关站点屏幕的函数声明
*/

void station_info_init(lv_obj_t * canvas);
void time_label_init(lv_obj_t * obj ,TimeLabel *tl, int8_t count);

void station_info_show(Station *sta);

void station_info_refresh(void);

void change_btn_cb(lv_event_t * e);






#endif
