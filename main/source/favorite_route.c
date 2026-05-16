#include "favorites.h"
#include "ff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#define FAVORITE_ROUTES_FILE_PATH "0:/favorite_routes.txt"
#define MAX_FAVORITE_ROUTES_BUTTONS 4

typedef struct {
    lv_obj_t *btn; //按钮指针
    lv_obj_t *label;    //按钮的标签指针
    uint16_t only_id_start;   //起始站的唯一id
    uint16_t only_id_end; //终点站的唯一id
    bool valid;         //是否有效
} FavoriteRouteBtn;

size_t favorite_route_count = 0; //用来计数有多少个收藏的路线
FavoriteRouteBtn route_bindings[MAX_FAVORITE_ROUTES_BUTTONS];
static bool favorites_ready = false;
FavoriteRouteID favorite_routes[MAX_ROUTES]; //用来储存被收藏的路线的起始站和终点站的唯一id

static bool favorites_route_contains(uint16_t start_id, uint16_t end_id) //判断这个起始站和终点站的组合是否被收藏了
{
    for (size_t i = 0; i < favorite_route_count; i++) {
        if (favorite_routes[i].start_id == start_id && favorite_routes[i].end_id == end_id) {
            return true;
        }
    }
    return false;
}

static void favorites_route_refresh_button(const FavoriteRouteBtn *binding) //检查这个路线是否被收藏了，并刷新按钮的显示
{
    if (!binding || !binding->valid || !binding->btn || !binding->label) {
        return;
    }
    if (favorites_route_contains(binding->only_id_start, binding->only_id_end))
    {
        lv_obj_remove_style(binding->btn, &blue_button_style, 0);
        lv_obj_remove_style(binding->btn, &btn_style, 0);
        lv_obj_add_style(binding->btn, &btn_style, 0);
        lv_label_set_text(binding->label, "已收藏");
        lv_obj_set_style_text_color(binding->label, lv_color_black(), 0);
    }
    else
    {
        lv_obj_remove_style(binding->btn, &blue_button_style, 0);
        lv_obj_remove_style(binding->btn, &btn_style, 0);
        lv_obj_add_style(binding->btn, &blue_button_style, 0);
        lv_label_set_text(binding->label, "收藏");
        lv_obj_set_style_text_color(binding->label, lv_color_white(), 0);
    }

    lv_obj_center(binding->label);
}

static void favorites_route_refresh_all_buttons(void) //刷新所有绑定的按钮
{
    for (size_t i = 0; i < MAX_FAVORITE_ROUTES_BUTTONS; i++) {
        favorites_route_refresh_button(&route_bindings[i]);
    }
}

static void favorites_route_save_to_sd(void) //将收藏的路线保存至sd卡
{
    FIL file;
    if (f_open(&file, FAVORITE_ROUTES_FILE_PATH, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
        return;
    }

    char line[30]; //line为写入数据的缓冲区
    UINT written = 0; //记录实际写入的字节数
    for (size_t i = 0; i < favorite_route_count; i++)
    {
        int len = snprintf(line, sizeof(line), "%u,%u\n", favorite_routes[i].start_id, favorite_routes[i].end_id);//将唯一id转化为字符串
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

static void favorites_route_load_from_sd(void) //从sd卡中读取数据
{
    FIL file;
    if (f_open(&file, FAVORITE_ROUTES_FILE_PATH, FA_READ) != FR_OK)
    {
        favorite_route_count = 0;
        memset(favorite_routes, 0, sizeof(favorite_routes));
        return;
    }

    char line[30];
    favorite_route_count = 0;
    memset(favorite_routes, 0, sizeof(favorite_routes));
    while (f_gets(line, sizeof(line), &file) != NULL) //逐行读取，如果文件没有读取完
    {
        char *comma = strchr(line, ',');
        if (!comma) 
        {
            continue; //无效行
        }
        *comma = '\0'; //分割字符串
        char *end1 = NULL;
        char *end2 = NULL;
        unsigned long start_id = strtoul(line, &end1, 10);
        unsigned long end_id = strtoul(comma + 1, &end2, 10);
        if (end1 == line || end2 == comma + 1) {
            continue; //跳过无效行
        }
        if (start_id > UINT16_MAX || end_id > UINT16_MAX) {
            continue;
        }
        if (favorites_route_contains((uint16_t)start_id, (uint16_t)end_id)) 
        {
            continue;
        }
        if (favorite_route_count < MAX_ROUTES) {
            favorite_routes[favorite_route_count].start_id = (uint16_t)start_id;
            favorite_routes[favorite_route_count].end_id = (uint16_t)end_id;
            favorite_route_count++;
        }
    }

    f_close(&file);
}

FavoriteRouteBtn* favorites_route_find_binding(lv_obj_t *btn) //根据按钮指针找到对应的绑定
{
    for (size_t i = 0; i < MAX_FAVORITE_ROUTES_BUTTONS; i++) {
        if (route_bindings[i].valid && route_bindings[i].btn == btn) {
            return &route_bindings[i];
        }
    }
    return NULL;
}

void favorites_route_invalidate_binding(lv_obj_t *btn)  
{
    for (size_t i = 0; i < MAX_FAVORITE_ROUTES_BUTTONS; i++) {
        if (route_bindings[i].valid && route_bindings[i].btn == btn) {
            route_bindings[i].valid = false;
            return;
        }
    }
}

static bool favorites_route_add(uint16_t start_id, uint16_t end_id) //添加收藏
{
    if (favorites_route_contains(start_id, end_id) || favorite_route_count >= MAX_ROUTES) {
        return false;
    }
    favorite_routes[favorite_route_count].start_id = start_id;
    favorite_routes[favorite_route_count].end_id = end_id;
    favorite_route_count++;
    return true;
}

static bool favorites_route_remove(uint16_t start_id, uint16_t end_id) //移除收藏
{
    for (size_t i = 0; i < favorite_route_count; i++) {
        if (favorite_routes[i].start_id == start_id && favorite_routes[i].end_id == end_id) 
        {
            for (size_t j = i + 1; j < favorite_route_count; j++) {
                favorite_routes[j - 1] = favorite_routes[j];
            }
            favorite_routes[favorite_route_count - 1] = (FavoriteRouteID){0, 0};
            favorite_route_count--;
            return true;
        }
    }
    return false;
}

//切换收藏状态
void favorites_route_toggle(uint16_t start_id, uint16_t end_id)
{
    if (favorites_route_contains(start_id, end_id)) {
        favorites_route_remove(start_id, end_id);
    } else {
        favorites_route_add(start_id, end_id);
    }
    //每次状态的切换都会保存至sd卡
    favorites_route_save_to_sd();
}

void favorites_route_init(void)
{
    if (favorites_ready) {
        return;
    }
    //清空结构体数据
    memset(route_bindings, 0, sizeof(route_bindings));
    favorites_route_load_from_sd();
    favorites_ready = true;
}

void favorites_route_bind_button(lv_obj_t *btn, lv_obj_t *label, const Station *st_station,const Station *end_station) //将按钮和标签与路线绑定，在选定路线并显示弹窗时调用
{
    if (!btn || !label || !st_station || !end_station) {
        return;
    }
    //查找这个按钮有没有绑定
    FavoriteRouteBtn *binding = favorites_route_find_binding(btn);

    //如果没有就绑定这个按钮
    if (!binding) 
    {
        for (size_t i = 0; i < MAX_FAVORITE_ROUTES_BUTTONS; i++) {
            if (!route_bindings[i].valid) //如果这个位置还没有被使用
            {
                binding = &route_bindings[i];
                binding->btn = btn;
                binding->valid = true;
                break;
            }
        }
    }

    if (!binding) {
        return;
    }

    binding->label = label;

    //每次打开一个路线，都把绑定的起始站和终点站的唯一id替换成新打开的
    binding->only_id_start = (uint16_t)st_station->only_id;
    binding->only_id_end = (uint16_t)end_station->only_id;
    favorites_route_refresh_button(binding);
}

void favorites_route_button_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) return;

    lv_obj_t *btn = lv_event_get_target(e);
    FavoriteRouteBtn *binding = favorites_route_find_binding(btn);
    if (!binding) return;

    favorites_route_toggle(binding->only_id_start, binding->only_id_end);
    favorites_route_refresh_all_buttons();
    favorite_route_show();
}


