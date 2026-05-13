#ifndef CANVAS
#define CANVAS

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "metro_line.h"
#include "route_planner.h"


extern int8_t plus, minus;
extern MetroLine metro_lines[4];

extern int8_t is_line_showing;
extern int8_t is_route_showing;

extern lv_obj_t  *display0;

extern int8_t is_station_clicked;
extern Station station_clicked[2];

extern int8_t is_station_info ;

extern lv_obj_t * station_info_disp;

extern lv_coord_t origin_x, origin_y;
extern uint16_t magnify_size;

extern lv_style_t btn_style, black_label_style, blue_label_style;
extern lv_style_t blue_button_style;

extern lv_obj_t *location_image, *pop_window;


extern lv_timer_t * station_timer;

extern int8_t is_start_selected, is_end_selected;

extern int8_t is_reminder_set_showing;

extern lv_obj_t* end_img,* start_img;

extern Route route_result;
extern const char* start_name, *end_name;

extern int8_t direction_state;

extern lv_obj_t * reminder_disp;



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


void station_copy(Station *dist,const Station *source);

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

void hide_pop_window(void);

void start_end_btn_cb(lv_event_t *e);


/*
下面是有关站点屏幕的函数声明
*/

lv_color_t get_line_color(int8_t line_number);

void station_info_init(lv_obj_t * canvas);
void time_label_init(lv_obj_t * obj ,TimeLabel *tl, int8_t count);
void sta_canvas_init(lv_obj_t * canvas);

void timetable_label_init(lv_obj_t * obj,TimeLabel * tl);


void sta_create_metro_line(lv_obj_t * canvas,MetroLine *line , Station *sta);
void station_name_text_show(lv_obj_t * canvas, lv_coord_t x, lv_draw_label_dsc_t* dsc,const Station *sta);
lv_coord_t sta_coord_trans(lv_coord_t pos, lv_coord_t origin);

void sta_click_canvas(lv_indev_t *indev, MetroLine *line);

void station_info_show(const Station *sta, int8_t);

void station_info_refresh(int8_t);

void change_btn_cb(lv_event_t * e);
void timetable_btn_cb(lv_event_t * e);
void remind_add_btn_cb(lv_event_t *e);

void sta_canvas_cb(lv_event_t * e);
void sta_pressing_canvas(lv_indev_t *);

void timetable_show(MetroLine *line);
void station_timer_cb(lv_timer_t * timer);

void time_label_update(MetroLine *line, Station * sta);

void draw_train_icon(MetroLine *line, Station * sta, int8_t remain_sta_count, int remain_sec, int count);


void del_time_label_text(TimeLabel *tl);

//获取某个站点某个方向的首末班车时间
void get_first_last_train(Station *sta, char (*result)[7], int8_t direction);

int get_station_interval(MetroLine *line, int8_t id_1, int8_t id_2);

/*
提醒添加里面的函数声明
*/
void remind_entry_init(lv_obj_t * obj);

void reminder_set_show(Station *sta);

void remind_cancel_btn_cb(lv_event_t * e);

/*
gui里的函数声明
*/

void route_design_result(void);
void pop_search_result_window_init(void);
void pop_search_result_window_show(Route* route);


#endif
