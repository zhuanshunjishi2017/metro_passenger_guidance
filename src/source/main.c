#include "lv_porting.h"
#include "lvgl.h"
#include "gui.h"
#include "favorites.h"

void my_rtc_setup(void);
void keyBoard_event_cb(lv_event_t *e);
void metro_logo(lv_obj_t *, int);
void display_set(lv_obj_t *, int);
void timetable_init(void);
void time_set(void);
void top_ta_record_lb_init(lv_obj_t *);
void top_ta_result_lb_init(lv_obj_t *);
void top_search_record_init(void);
void ui_init(void);
void ta_init(void);
void display12_init(void);
void display11_init(void);
void favorite_station_init(void);
void favorite_station_show(void);
void favorite_route_show(void);
void display2_init(void);


lv_obj_t *display0, *display1, *display2;
lv_obj_t *canvas;
lv_obj_t *kb;
lv_obj_t *timetable1, *timetable2, *timetable3;
extern lv_obj_t *display11;

int main(void)
{
   /*  favorites_init();
    favorites_route_init(); */
    //my_rtc_setup();

    lv_porting_init();

    display0 = lv_obj_create(NULL);
    display1 = lv_obj_create(NULL);
    display2 = lv_obj_create(NULL);

    canvas = lv_canvas_create(display0);
    canvas_init(canvas);
    lv_scr_load(display0);

    lv_obj_clear_flag(lv_layer_top(), LV_OBJ_FLAG_SCROLLABLE);

    kb = lv_keyboard_create(lv_layer_top());
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);

    display_set(display0, 0);
    display_set(display1, 1);
    display_set(display2, 2);
    
    top_ta_record_lb_init(lv_layer_top());
    top_ta_result_lb_init(lv_layer_top());
    top_search_record_init();

    ui_init();
    ta_init();

   
    display12_init();
    display11_init();

     
    favorite_station_init();
    favorite_station_show();

    favorite_route_ui_init();
    favorite_route_show();
    lv_obj_move_foreground(display11);

    display2_init();
    timetable_init();      
  
    while (1) {
        time_set();
        lv_timer_handler();
        lv_porting_delay();
    }
}
