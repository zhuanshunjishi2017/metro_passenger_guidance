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
extern Station station_clicked;

extern lv_coord_t origin_x, origin_y, magnify_size;



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


#endif
