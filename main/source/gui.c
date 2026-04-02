#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include <stdio.h>

lv_obj_t *display1;
void display(void);
void display(void)
{
	display1 = lv_obj_create(NULL);
	
	lv_scr_load(display1);
	
}
