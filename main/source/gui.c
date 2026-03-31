#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include <stdio.h>

lv_obj_t *display1, *display2;

lv_obj_t * sw, * bu1, * bu2, *label1, *label2, *label;

lv_obj_t *bu3, *labelbu3, *bu4, *labelbu4;

lv_obj_t * keyboard,* text1,* label3, *img, *img2;

lv_img_dsc_t image_struct; 

lv_indev_t * position;

static int16_t dx = 10;
static int16_t dy = 1;
static int16_t angle = 0; 
	
void bu1_cb(lv_event_t *e);
void bu2_cb(lv_event_t *e);
void keyboard_activate(lv_event_t *e);
void keyboard_callback(lv_event_t *e);

void switchscreen2(lv_event_t *e);
void switchscreen1(lv_event_t *e);

void pressing_image(lv_event_t *e);

void keyboard_hid(lv_event_t *e);

void img_rotate(lv_timer_t * timer);

extern int read_file_to_array(const char* filename, uint8_t* buffer, uint32_t max_size);

void display(void)
{
	display1 = lv_obj_create(NULL);
	display2 = lv_obj_create(NULL);
	
	sw = lv_switch_create(display1);
	lv_obj_center(sw);
	/*
	label = lv_label_create(display1);
	lv_obj_set_pos(label,300,100);
	lv_obj_set_size(label,300,30);
	lv_label_set_text(label,"maguoteng U202514600");
	*/
	bu1 = lv_btn_create(display1);
	lv_obj_set_pos(bu1,100,100);
	lv_obj_set_size(bu1,100,50);
	
	label1 = lv_label_create(bu1);
	lv_label_set_text(label1,"BLUE");
	lv_obj_center(label1);
	lv_obj_set_style_bg_color(bu1,lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_PRESSED);
	
	
	bu2 = lv_btn_create(display1);
	lv_obj_set_pos(bu2,100,200);
	lv_obj_set_size(bu2,100,50);
	

	
	label2 = lv_label_create(bu2);
	lv_label_set_text(label2,"0");
	lv_obj_center(label2);
	lv_obj_add_flag(bu2, LV_OBJ_FLAG_CHECKABLE);
	
	
	bu3 = lv_btn_create(display1);
	lv_obj_set_pos(bu3,100,300);
	lv_obj_set_size(bu3,150,50);
	
	labelbu3 = lv_label_create(bu3);
	lv_label_set_text(labelbu3,"go to display2");
	lv_obj_center(labelbu3);
	

	
	//create a keyboard
	keyboard = lv_keyboard_create(display1);
	//hide the keyboard
	lv_obj_add_flag(keyboard,LV_OBJ_FLAG_HIDDEN);
	
	//create a label to display the text inputed
	label3 = lv_label_create(display2);
	lv_obj_set_pos(label3,300,200);
	lv_obj_set_size(label3,100,30);
	//lv_obj_set_style_bg_color(label3,lv_color_hex(0x999999), LV_PART_MAIN);
	
		
	bu4 = lv_btn_create(display2);
	lv_obj_set_pos(bu4,100,300);
	lv_obj_set_size(bu4,150,50);
	labelbu4 = lv_label_create(bu4);
	lv_label_set_text(labelbu4,"go to display1");
	lv_obj_center(labelbu4);
	
	
	//create an entry
	text1 = lv_textarea_create(display1);
	
	lv_obj_set_pos(text1,500,100);
	lv_obj_set_size(text1,300,50);
	
	
	img = lv_img_create(display2);
	img2 = lv_img_create(display2);
	lv_img_set_src(img, "0:/image.bin");
	
	uint8_t* image_buffer = sdram_malloc( 256 * 256 * 3 + 4 );
	read_file_to_array("0:/image.bin", image_buffer,  256 * 256 * 3 + 4 );
	
	image_struct.header.always_zero = 0;                  
	image_struct.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;  // 原始颜色格式
	image_struct.header.w = 256;                          // 图像宽度
	image_struct.header.h = 256;                          // 图像高度
	image_struct.header.reserved = 0;                     // 保留区域，须设为0
	image_struct.data_size = 256 * 256 * 3;               // 图像数据大小
	image_struct.data = image_buffer + 4;                 // 图像数据位置，须跳过文件头
	
	lv_img_set_src(img2, &image_struct);
	lv_obj_set_pos(img2,500,100);
	
	lv_img_set_src(img, &image_struct);
	lv_obj_set_pos(img,100,100);
	
	lv_timer_create(img_rotate,70,NULL);
	
	
	
	lv_obj_add_flag(img2,LV_OBJ_FLAG_CLICKABLE);
	
	lv_obj_add_event_cb(img2,pressing_image,LV_EVENT_PRESSING,NULL);

	
	
	lv_obj_add_event_cb(text1,keyboard_activate,LV_EVENT_CLICKED,NULL);
	
	lv_obj_add_event_cb(keyboard,keyboard_callback,LV_EVENT_ALL,NULL);
	
	lv_obj_add_event_cb(display1,keyboard_hid,LV_EVENT_CLICKED,NULL);
	
	lv_obj_add_event_cb(bu3,switchscreen2,LV_EVENT_ALL,NULL);	
	
	lv_obj_add_event_cb(bu4,switchscreen1,LV_EVENT_ALL,NULL);	
	
	lv_obj_add_event_cb(bu1, bu1_cb, LV_EVENT_ALL,NULL);
	lv_obj_add_event_cb(bu2, bu2_cb, LV_EVENT_ALL,NULL);
	
	lv_scr_load(display1);
	
}
void switchscreen2(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
      	case LV_EVENT_PRESSED:
			lv_scr_load(display2);
          	break;
		default:
			break;
    }
	
}

void switchscreen1(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
      case LV_EVENT_PRESSED:
			lv_scr_load(display1);
			break;
		default:
			break;
    }
}

void img_rotate(lv_timer_t * timer)
{
    lv_coord_t x = lv_obj_get_x(img);
    lv_coord_t y = lv_obj_get_y(img);
    lv_coord_t scr_w = lv_obj_get_width(lv_scr_act());
    lv_coord_t scr_h = lv_obj_get_height(lv_scr_act());
    lv_coord_t img_w = lv_obj_get_width(img);
    lv_coord_t img_h = lv_obj_get_height(img);
    int16_t new_x = x + dx;
    int16_t new_y = y + dy;
    if (new_x + img_w > scr_w) 
	{
        new_x = scr_w - img_w;
        dx = -dx;                
    }
	else if (new_x < 0) 
	{
        new_x = 0; 
        dx = -dx;
    }
    if (new_y + img_h > scr_h)
	{
        new_y = scr_h - img_h; 
        dy = -dy;
    }
	else if (new_y < 0)
	{
        new_y = 0;
        dy = -dy;
    }
    lv_obj_set_pos(img, new_x, new_y);

    angle += 50;
    if (angle >= 3600) angle = 0;
    lv_img_set_angle(img, angle);
}

void pressing_image(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	
	position = lv_indev_get_act();
	
	lv_point_t vect;
	lv_indev_get_vect(position, &vect);
	
	lv_obj_set_x(img2,lv_obj_get_x(img2)+vect.x);
	lv_obj_set_y(img2,lv_obj_get_y(img2)+vect.y);
	
}

void LED_control(void)
{
	if (lv_obj_has_state(sw,LV_STATE_CHECKED))
		gpio_bit_write(GPIOA, GPIO_PIN_0, RESET);	
	else
		gpio_bit_write(GPIOA, GPIO_PIN_0, SET);	
}

void bu1_cb(lv_event_t *e)                 //第一个按钮
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code)
    {
        case LV_EVENT_PRESSED:
				gpio_bit_write(GPIOA, GPIO_PIN_1, RESET);	
            break;
				case LV_EVENT_RELEASED:
					gpio_bit_write(GPIOA, GPIO_PIN_1, SET);
					break;
				default:
					break;
    }
}

void bu2_cb(lv_event_t *e)                 //第二个按钮
{
  static int count = 0;  
	lv_event_code_t code = lv_event_get_code(e);
  switch (code)
  {
        case LV_EVENT_VALUE_CHANGED:
					count++;
					if (count%2)
					{
						gpio_bit_write(GPIOA, GPIO_PIN_2, RESET);	
					}
					else
					{
						gpio_bit_write(GPIOA, GPIO_PIN_2, SET);	
					}
					char * str;
					sprintf(str, "%d", count);
	
					lv_label_set_text(label2,str);
					break;
				default:
					break;
  }
}

void keyboard_activate(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	lv_keyboard_set_textarea(keyboard,text1);
	
	lv_obj_clear_flag(keyboard,LV_OBJ_FLAG_HIDDEN);
}

void keyboard_hid(lv_event_t *e)
{
	lv_event_code_t code = lv_event_get_code(e);
	
	lv_obj_add_flag(keyboard,LV_OBJ_FLAG_HIDDEN);
}


void keyboard_callback(lv_event_t * e)
{
	lv_event_code_t code = lv_event_get_code(e);     // 获取事件
	
  uint32_t id = 0;
  const char *text, *content;
	//static int keyboard_flag;
	

  if (code == LV_EVENT_VALUE_CHANGED)
  {
    id = lv_btnmatrix_get_selected_btn(keyboard);  // 获取按下按钮的ID
    text = lv_keyboard_get_btn_text(keyboard, id); // 获取按下按钮的名称
    if (!strcmp(text, LV_SYMBOL_KEYBOARD))
    {	
			if (LV_KEYBOARD_MODE_TEXT_LOWER == lv_keyboard_get_mode(keyboard))
				lv_keyboard_set_mode(keyboard,LV_KEYBOARD_MODE_NUMBER);
			else lv_keyboard_set_mode(keyboard,LV_KEYBOARD_MODE_TEXT_LOWER);
    }
		if (!strcmp(text, LV_SYMBOL_OK))
    {
			content = lv_textarea_get_text(text1);
			
			lv_label_set_text(label3,content);
			
			lv_textarea_set_text(text1,"\0");
			
			lv_obj_add_flag(keyboard,LV_OBJ_FLAG_HIDDEN);
    }
  }
}

