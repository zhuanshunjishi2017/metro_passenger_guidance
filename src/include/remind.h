#ifndef REMIND_H
#define REMIND_H

#include "metro_line.h"
#include "timestruct.h"

#define MAX_REMIND 10

typedef struct
{
  const Station *station;
  int8_t direction;
  TimeStruct start_time;
  uint16_t remind_sec;
  int8_t flag;

} RemindInfo;

typedef struct {
    lv_obj_t * label_frame;
    lv_obj_t * station_label;
    lv_obj_t * line_label;
    lv_obj_t * direction_label;
    lv_obj_t * remind_info_label;
    lv_obj_t * train_order_label;
    lv_obj_t * del_btn;
} RemindLabel;



extern RemindInfo remind_info[MAX_REMIND];
extern RemindLabel remind_labels[MAX_REMIND];


extern lv_obj_t * reminder_dialog;
extern lv_obj_t * reminder_remain_time_label;

extern lv_obj_t * remind_add_msg;

extern lv_timer_t * msg_hide_timer;


extern int8_t is_reminder_dialog_showing;

void del_remind(int index);
void reminder_dialog_show(RemindInfo *info, int index);
void reminder_dialog_clear(void);

void close_msg_cb(lv_timer_t *t);



#endif
