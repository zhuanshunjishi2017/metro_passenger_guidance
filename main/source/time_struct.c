#include "drivers.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "lvgl.h"
#include <stdint.h>
#include "metro_line.h"
#include "gui.h"
#include "timestruct.h"
#include <stdint.h>
#include <stdlib.h>   // for abs()

static int timeToSeconds(const TimeStruct* t);
static void secondsToTime(int totalSeconds, TimeStruct* result) ;


// 辅助函数：将 TimeStruct 转换为从午夜开始的秒数（int 类型）
static int timeToSeconds(const TimeStruct* t) {
    return t->hour * 3600 + t->min * 60 + t->sec;
}

// 辅助函数：将秒数（int 类型）规范化为 TimeStruct（模 86400，范围 0~86399）
static void secondsToTime(int totalSeconds, TimeStruct* result) {
    // 模一天，并处理负数
    totalSeconds %= 86400;
    if (totalSeconds < 0) {
        totalSeconds += 86400;
    }
    result->hour = totalSeconds / 3600;
    result->min  = (totalSeconds % 3600) / 60;
    result->sec  = totalSeconds % 60;
}

/**
 * 1. 将 int8_t 类型的秒数转化为 TimeStruct，结果保存在 result 指针中。
 *    注：由于秒数范围小，直接计算即可。
 */
void secondsToTimeStruct(int seconds, TimeStruct* result) {
    int total = seconds;  // 提升为 int 避免溢出
    result->hour = total / 3600;   // 对于 |seconds| <= 127，hour 始终为 0
    result->min  = (total % 3600) / 60;
    result->sec  = total % 60;
}

/**
 * 2. 计算两个 TimeStruct 的时间差（绝对值），结果保存在 result 指针中。
 *    注：结果非负，且规范化在 0 ~ 23:59:59 范围内。
 */
void timeDifference(const TimeStruct* t1, const TimeStruct* t2, TimeStruct* result) {
    int sec1 = timeToSeconds(t1);
    int sec2 = timeToSeconds(t2);
    int diff = abs(sec1 - sec2);   // 绝对时间差（秒）
    secondsToTime(diff, result);
}

/**
 * 3. 计算当前时间加上时间间隔后的新时间，结果保存在 result 指针中。
 *    注：结果自动规范化到一天之内（模 86400）。
 */
void timeAdd(const TimeStruct* current, const TimeStruct* interval, TimeStruct* result) {
    int curSec  = timeToSeconds(current);
    int intSec  = timeToSeconds(interval);
    int totalSec = curSec + intSec;
    secondsToTime(totalSec, result);
}

/* 
比较两个时间的大小，如果第一个时间大则返回大于0的结果
*/

int timeCompare(const TimeStruct *t1, const TimeStruct *t2)
{
    int sec1 = timeToSeconds(t1);
    int sec2 = timeToSeconds(t2);

    return sec1 - sec2;
}

