#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "gui.h"
void gpio_init(void);
void my_rtc_setup(void);
void keyBoard_event_cb(lv_event_t *e);
void metro_logo(lv_obj_t*,int);
void display_set(lv_obj_t*,int);
void timetable_init(void);
void time_set(void);
void top_ta_record_lb_init(lv_obj_t*);
void top_ta_result_lb_init(lv_obj_t*);

// void display0_init(lv_obj_t *);

lv_obj_t* display0,*display1,*display2;
lv_obj_t* canvas;
lv_obj_t* kb;
lv_obj_t* timetable1,*timetable2,*timetable3;
extern lv_obj_t* display11;
int main(void)
{
	sys_init();
	
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init();

  my_rtc_setup();
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();

    //display&canvas
  display0 = lv_obj_create(NULL);
  display1 = lv_obj_create(NULL);
  display2 = lv_obj_create(NULL);
  canvas = lv_canvas_create(display0);
  canvas_init(canvas);
  lv_scr_load(display0);
    
  //display0_gui
  kb = lv_keyboard_create(lv_layer_top());
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  
  display_set(display0,0);
  display_set(display1,1);
  display_set(display2,2);
  top_ta_record_lb_init(lv_layer_top());
  top_ta_result_lb_init(lv_layer_top());

  //display1_gui
  ui_init();
  ta_init();
  display12_init();
  display11_init();
  lv_obj_move_foreground(display11);
  
  //display2_gui
  // display2_ui_init();

  timetable_init();
  while(1)
	{
        time_set();

        lv_timer_handler();
        delay_us(40000);//刷新率25Hz
	}
}

void gpio_init(void)
{
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_60MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	gpio_bit_reset(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
}

