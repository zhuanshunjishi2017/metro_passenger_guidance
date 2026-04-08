#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"



void gpio_init(void);
// void display0_init(lv_obj_t *);

lv_obj_t* display0, *display1, *display2;

lv_obj_t* canvas;

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
    //display_init(display0);
    canvas = lv_canvas_create(display0);
    canvas_init(canvas);

    lv_scr_load(display0);
    
    while(1)
	{
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

