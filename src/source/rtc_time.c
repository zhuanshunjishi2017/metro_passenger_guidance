#include "timestruct.h"

#include <stdio.h>
#include <time.h>

static int get_local_time(struct tm *result)
{
    time_t now;

    if (!result) {
        return -1;
    }

    now = time(NULL);
    if (now == (time_t)-1) {
        return -1;
    }

#if defined(_WIN32)
    return localtime_s(result, &now);
#else
    return localtime_r(&now, result) == NULL ? -1 : 0;
#endif
}

void get_rtc_time_string(char *buf, size_t len)
{
    static const char *weekdays[] = {
        "\xE5\x91\xA8\xE6\x97\xA5",
        "\xE5\x91\xA8\xE4\xB8\x80",
        "\xE5\x91\xA8\xE4\xBA\x8C",
        "\xE5\x91\xA8\xE4\xB8\x89",
        "\xE5\x91\xA8\xE5\x9B\x9B",
        "\xE5\x91\xA8\xE4\xBA\x94",
        "\xE5\x91\xA8\xE5\x85\xAD",
    };
    struct tm local_time;

    if (!buf || len == 0) {
        return;
    }

    if (get_local_time(&local_time) != 0) {
        snprintf(buf, len, "----/--/-- --:--:--");
        return;
    }

    snprintf(buf,
             len,
             "%04d" "\xE5\xB9\xB4" "%02d" "\xE6\x9C\x88" "%02d"
             "\xE6\x97\xA5" " %s %02d:%02d:%02d",
             local_time.tm_year + 1900,
             local_time.tm_mon + 1,
             local_time.tm_mday,
             weekdays[local_time.tm_wday],
             local_time.tm_hour,
             local_time.tm_min,
             local_time.tm_sec);
}

void get_current_time(TimeStruct *result)
{
    struct tm local_time;

    if (!result) {
        return;
    }

    if (get_local_time(&local_time) != 0) {
        result->hour = 0;
        result->min = 0;
        result->sec = 0;
        return;
    }

    result->hour = (int8_t)local_time.tm_hour;
    result->min = (int8_t)local_time.tm_min;
    result->sec = (int8_t)local_time.tm_sec;
}

void my_rtc_setup(void)
{
}
