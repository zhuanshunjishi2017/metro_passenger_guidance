#ifndef TIME_STRUCT_H
#define TIME_STRUCT_H

#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <stdint.h>

#define HOUR_MIN_MODE  0
#define MIN_SEC_MODE   1

#define STATION_STOP_TIME 20

typedef struct{
    int8_t hour;
    int8_t min;
    int8_t sec;
} TimeStruct;



/* 下面是与处理时间有关的函数定义 */
void secondsToTimeStruct(int seconds, TimeStruct* result);
void timeDifference(const TimeStruct* t1, const TimeStruct* t2, TimeStruct* result);
void timeAdd(const TimeStruct* current, const TimeStruct* interval, TimeStruct* result);
int timeCompare(const TimeStruct *t1, const TimeStruct *t2);
void timeToString(const TimeStruct* t, char* buffer, int mode) ;



void get_current_time(TimeStruct *result);


#endif

