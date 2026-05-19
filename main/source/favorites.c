#include "favorites.h"
#include "ff.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gui.h"
#define FAVORITES_FILE_PATH "0:/favorites.txt"
#define MAX_FAVORITES 30
#define MAX_FAVORITES_BUTTONS 4

typedef struct {
    lv_obj_t *btn; //按钮指针
    lv_obj_t *label;    //按钮的标签指针
    uint16_t only_id;   //车站的唯一id
    bool valid;         //是否有效
} FavoriteBtn;

uint16_t favorite_ids[MAX_FAVORITES]; //用来储存被收藏的站点的唯一id
size_t favorite_count = 0; //用来计数有多少个收藏的车站
static FavoriteBtn bindings[MAX_FAVORITES_BUTTONS]; 
static bool favorites_ready = false;
extern lv_obj_t* favorite_station_show_lb[MAX_FAVORITES];
extern lv_obj_t* station_prompt,*route_prompt;

static bool favorites_contains(uint16_t only_id) //判断这个唯一id的车站是否被收藏了
{
    for (size_t i = 0; i < favorite_count; i++) {
        if (favorite_ids[i] == only_id) {
            return true;
        }
    }
    return false;
}

static void favorites_refresh_button(const FavoriteBtn *binding) //检查这个车站是否被收藏了，并刷新按钮的显示
{
    if (!binding || !binding->valid || !binding->btn || !binding->label) {
        return;
    }
    if (favorites_contains(binding->only_id))
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

static void favorites_refresh_all_buttons(void) //刷新所有绑定的按钮
{
    for (size_t i = 0; i < MAX_FAVORITES_BUTTONS; i++) {
        favorites_refresh_button(&bindings[i]);
    }
}

static void favorites_save_to_sd(void) //将收藏保存至sd卡
{
    FIL file;
    FRESULT res = f_open(&file, FAVORITES_FILE_PATH, FA_CREATE_ALWAYS | FA_WRITE); //以写入模式打开文件，如果文件不存在就创建，存在就覆盖
    if (res != FR_OK) {
        return;
    }

    char line[16]; //line为写入数据的缓冲区
    UINT written = 0; //记录实际写入的字节数
    for (size_t i = 0; i < favorite_count; i++)
    {
        int len = snprintf(line, sizeof(line), "%u\n", favorite_ids[i]);//将唯一id转化为字符串
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
static void favorites_load_from_sd(void)
{
    FIL file;
    FRESULT res = f_open(&file, FAVORITES_FILE_PATH, FA_READ);
    if (res != FR_OK) //未读取成功
    {
        favorite_count = 0;
        return;
    }

    char line[32]; //从文件中暂时存储站点的唯一id的字符串缓冲区
    favorite_count = 0;
    while (f_gets(line, sizeof(line), &file) != NULL) //逐行读取，如果文件没有读取完
    {
        char *end = NULL;
        unsigned long value = strtoul(line, &end, 10);//将字符串转换为数字
        if (end == line) {
            continue; //跳过\n或者无效行
        }
        if (value > UINT16_MAX) {
            continue;
        }
        if (favorites_contains((uint16_t)value)) 
        {
            continue;
        }
        if (favorite_count < MAX_FAVORITES) {
            favorite_ids[favorite_count++] = (uint16_t)value;
        }
    }

    f_close(&file);
}

static FavoriteBtn *favorites_find_binding(lv_obj_t *btn) //根据按钮指针查找绑定关系
{
    for (size_t i = 0; i < MAX_FAVORITES_BUTTONS; i++) {
        if (bindings[i].valid && bindings[i].btn == btn) {
            return &bindings[i];
        }
    }
    return NULL;
}

//添加收藏
static bool favorites_add(uint16_t only_id)
{
    //如果已经存在或数量达到上限
    if (favorites_contains(only_id) || favorite_count >= MAX_FAVORITES) {
        return false;
    }
    favorite_ids[favorite_count++] = only_id;
    return true;
}

static bool favorites_remove(uint16_t only_id)
{
    for (size_t i = 0; i < favorite_count; i++) {
        if (favorite_ids[i] == only_id) 
        {
            //将后面的数据前移
            for (size_t j = i + 1; j < favorite_count; j++) {
                favorite_ids[j - 1] = favorite_ids[j];
            }
            favorite_count--;
            return true;
        }
    }
    return false;
}

//切换收藏状态
static void favorites_toggle(uint16_t only_id) //如果已经收藏了就取消收藏，没收藏就添加收藏
{
    if (favorites_contains(only_id)) {
        favorites_remove(only_id);
    } else {
        favorites_add(only_id);
    }
    //每次状态的切换都会保存至sd卡
    favorites_save_to_sd();
}

void favorites_init(void)
{
    if (favorites_ready) {
        return;
    }
    //清空结构体数据
    memset(bindings, 0, sizeof(bindings));
    favorites_load_from_sd();
    favorites_ready = true;
}

//将按钮和标签与车站绑定，在选定车站并显示弹窗时调用
void favorites_bind_button(lv_obj_t *btn, lv_obj_t *label, const Station *station)
{
    if (!btn || !label || !station) {
        return;
    }
    //查找这个按钮有没有绑定
    FavoriteBtn *binding = favorites_find_binding(btn);

    //如果没有就绑定这个按钮
    if (!binding) {
        for (size_t i = 0; i < MAX_FAVORITES_BUTTONS; i++) {
            if (!bindings[i].valid) //如果这个位置还没有被使用
            {
                binding = &bindings[i];
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

    //事实上，这个地方才是重点，前面的都是初始化，我也可以一开始就绑定
    //每次打开一个站点，都把绑定的站点id替换成新打开的
    binding->only_id = (uint16_t)station->only_id;
    favorites_refresh_button(binding);
}

void favorites_button_event_cb(lv_event_t *e)
{
    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }

    lv_obj_t *btn = lv_event_get_target(e);
    FavoriteBtn *binding = favorites_find_binding(btn);
    if (!binding) {
        return;
    }

    favorites_toggle(binding->only_id);
    favorites_refresh_all_buttons();
    
    favorite_station_show(); //刷新收藏界面
    hide_pop_window(); //切换完收藏状态后关闭弹窗
    favorite_station_lb_style_reset(); //重置收藏界面标签样式
}
