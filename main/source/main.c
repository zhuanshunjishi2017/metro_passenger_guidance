#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"

void gpio_init(void);

lv_obj_t *label;

int main(void)
{
	sys_init();
	
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init();
	
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();

	display();

    lv_obj_t * btn;
    btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn,200,50);
    lv_obj_center(btn);


    label = lv_label_create(btn);
    lv_label_set_text(label, "");
    lv_obj_center(label); 
    lv_label_set_text(label,"start to configure RTC time\n\r");

    my_rtc_setup();
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
