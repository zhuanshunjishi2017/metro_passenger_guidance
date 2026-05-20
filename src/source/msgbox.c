
#include "lvgl.h"
#include <string.h>
#include <stdio.h>
#include "gui.h"
#include "metro_line.h"
#include "canvas.h"
#include "timestruct.h"
#include "remind.h"


lv_obj_t * transparent_bg = NULL;


/**
 * @brief 显示一个顶层的消息弹窗
 * @param obj 需要显示的窗口
 **/

void show_msgbox(lv_obj_t **obj)
{
    if (transparent_bg == NULL)
    {
        transparent_bg = lv_obj_create(lv_layer_top());
        lv_obj_set_size(transparent_bg, SCREEN_W, SCREEN_H);
        lv_obj_set_style_bg_color(transparent_bg, lv_color_hex(COLOR_MID_GRAY),0);
        lv_obj_set_style_border_width(transparent_bg, 0 ,0);
        lv_obj_set_style_opa(transparent_bg, LV_OPA_50, 0);
    }

    lv_obj_clear_flag(*obj, LV_OBJ_FLAG_HIDDEN);
    
    lv_obj_move_foreground(*obj);
    lv_obj_center(*obj);

}


/**
 * @brief 隐藏一个顶层的消息弹窗
 * @param obj 需要隐藏的窗口
 **/
void hide_msgbox(lv_obj_t **obj)
{
    if (transparent_bg)
    {
        lv_obj_del_async(transparent_bg);
        transparent_bg = NULL;
    }
    if (*obj)
    {
        lv_obj_add_flag(*obj, LV_OBJ_FLAG_HIDDEN);
        lv_obj_del_async(*obj);
        *obj = NULL;
    }
}

void remind_add_msg_init(lv_obj_t **obj, const char * text)
{
    *obj = lv_obj_create(lv_layer_top());

    lv_obj_t * msg = lv_label_create(*obj);

    lv_obj_set_size(*obj, 250, 100);

    lv_obj_set_style_text_font(msg, &heiti_16, 0);
    lv_obj_set_style_text_align(msg, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(msg);

    lv_label_set_text(msg, text);
    lv_obj_add_style(*obj,&flame_style,0);

}
