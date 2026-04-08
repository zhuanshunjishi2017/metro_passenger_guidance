#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

void gpio_init(void);
void display0_init(lv_obj_t *);
void canvas_init(lv_obj_t *);
void create_metro_map(lv_obj_t* );
void twocolumns(lv_obj_t* );
void create_buttons(lv_obj_t* ,lv_obj_t* ,lv_obj_t* ,lv_obj_t*);

lv_obj_t *label;
lv_obj_t* display0;
lv_obj_t* display1;
lv_obj_t* display2;
lv_obj_t* canvas;

int main(void)
{
	sys_init();
	
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init();
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();

    //display&canvas
    display0 = lv_obj_create(NULL);
    display0_init(display0);
    display1 = lv_obj_create(NULL);
    display2 = lv_obj_create(NULL);
    lv_scr_load(display0);
    
    canvas = lv_canvas_create(display0);
    canvas_init(canvas);
        
    //rtc_time
    lv_obj_t * btn;
    btn = lv_btn_create(display0);
    lv_obj_set_size(btn,268,23);
    lv_obj_set_pos(btn,754,13);

    label = lv_label_create(btn);
    lv_label_set_text(label, "");
    lv_obj_center(label); 
    lv_label_set_text(label,"start to configure RTC time\n\r");

    my_rtc_setup();

    //maps
    create_metro_map(canvas);
    //canvas上的组件
    twocolumns(canvas);
    lv_obj_t* btn1;
    lv_obj_t* btn2;
    lv_obj_t* btn3;
    create_buttons(canvas,btn1,btn2,btn3);
    lv_obj_t *top_ta;
    lv_obj_t *kb;

    char time_buf[32];
    while(1)
	{
        get_rtc_time_string(time_buf, sizeof(time_buf));
        lv_label_set_text(label, time_buf); // 更新显示
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
