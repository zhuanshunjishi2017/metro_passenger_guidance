#include "favorites.h"
#include "ff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#define SEARCH_RECORD_FILE_PATH "0:/search_record.txt"

extern lv_obj_t* top_search_record_station[SEARCH_LIST_LEN];
extern lv_obj_t* top_search_record_line[SEARCH_LIST_LEN];
extern lv_obj_t* top_search_record_transfer[SEARCH_LIST_LEN];

uint16_t search_record_ids[SEARCH_LIST_LEN]; //用来储存被收藏的站点的唯一id
int search_record_count = 0; //用来计数有多少个搜索记录

bool search_record_contains(uint16_t only_id) //判断这个唯一id的车站是否被记录了
{
    for (size_t i = 0; i < search_record_count; i++) 
    {
        if (search_record_ids[i] == only_id) 
        {
            return true;
        }
    }
    return false;
}

void search_record_save_to_sd(void) //将搜索记录保存至sd卡
{
    FIL file;
    if (f_open(&file, SEARCH_RECORD_FILE_PATH, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) 
    {
        return;
    }

    char line[30]; //line为写入数据的缓冲区
    UINT written = 0; //记录实际写入的字节数
    for (size_t i = 0; i < search_record_count; i++)
    {
        int len = snprintf(line, sizeof(line), "%u\n", search_record_ids[i]);//将唯一id转化为字符串
        if (len <= 0) 
        {
            continue;
        }
        //写入文件
        if (f_write(&file, line, (UINT)len, &written) != FR_OK || written != (UINT)len) 
        {
            break;
        }
    }

    f_sync(&file);
    f_close(&file);
}

//从sd卡中读取数据
void search_record_load_from_sd(void)
{
    FIL file;
    FRESULT res = f_open(&file, SEARCH_RECORD_FILE_PATH, FA_READ);
    if (res != FR_OK) //未读取成功
    {
        search_record_count = 0;
        return;
    }

    char line[32]; //从文件中暂时存储站点的唯一id的字符串缓冲区
    search_record_count = 0;
    while (f_gets(line, sizeof(line), &file) != NULL) //逐行读取，如果文件没有读取完
    {
        char *end = NULL;
        unsigned long value = strtoul(line, &end, 10);//将字符串转换为数字
        if (end == line)
        {
            continue; //跳过\n或者无效行
        }
        if (search_record_contains((uint16_t)value)) 
        {
            continue;
        }
        if (search_record_count < SEARCH_LIST_LEN) 
        {
            search_record_ids[search_record_count++] = (uint16_t)value;
        }
    }

    f_close(&file);
}

void search_record_add(uint16_t only_id) //添加一个搜索记录
{
    if (search_record_count < SEARCH_LIST_LEN)
    {
        search_record_ids[search_record_count++] = only_id;
    }
    if (search_record_count >= SEARCH_LIST_LEN)
    {
        return;
    }
    search_record_save_to_sd();
}

void search_record_refresh(const char * text)
{
    if (text == NULL || text[0] == '\0') 
    {
        return;
    }
    const Station* station = find_station_by_name(text);
    if (station == NULL) 
    {
        return;
    }
    if (search_record_contains(station->only_id)) 
    {
        return;
    }
    search_record_add(station->only_id);

    lv_label_set_text(top_search_record_station[search_record_count - 1], station->name);
    lv_obj_clear_flag(top_search_record_station[search_record_count - 1], LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text_fmt(top_search_record_line[search_record_count - 1], "%d号线",station->line_belonged);
    lv_obj_set_style_bg_color(top_search_record_line[search_record_count - 1], get_line_color(station->line_belonged), 0);
    lv_obj_clear_flag(top_search_record_line[search_record_count - 1], LV_OBJ_FLAG_HIDDEN);

    if (station->is_transfer > 0) 
    {
        lv_label_set_text_fmt(top_search_record_transfer[search_record_count - 1], "%d号线", station->is_transfer);
        lv_obj_set_style_bg_color(top_search_record_transfer[search_record_count - 1], get_line_color(station->is_transfer), 0);
        lv_obj_clear_flag(top_search_record_transfer[search_record_count - 1], LV_OBJ_FLAG_HIDDEN);
    }
    
}

