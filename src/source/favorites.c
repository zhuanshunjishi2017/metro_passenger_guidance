#include "favorites.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif

#include "gui.h"

#define FAVORITES_FILE_PATH "user_data/favorites.txt"
#define MAX_FAVORITES_BUTTONS 4

typedef struct {
    lv_obj_t *btn;
    lv_obj_t *label;
    uint16_t only_id;
    bool valid;
} FavoriteBtn;

uint16_t favorite_ids[MAX_FAVORITES];
size_t favorite_count = 0;
static FavoriteBtn bindings[MAX_FAVORITES_BUTTONS];
static bool favorites_ready = false;

extern lv_obj_t *favorite_station_show_lb[MAX_FAVORITES];
extern lv_obj_t *station_prompt, *route_prompt;
extern void favorite_station_show(void);
extern void favorite_station_lb_style_reset(void);

static void ensure_user_data_dir(void)
{
#if defined(_WIN32)
    if (_mkdir("user_data") != 0 && errno != EEXIST) {
        return;
    }
#else
    if (mkdir("user_data", 0755) != 0 && errno != EEXIST) {
        return;
    }
#endif
}

static bool favorites_contains(uint16_t only_id)
{
    for (size_t i = 0; i < favorite_count; i++) {
        if (favorite_ids[i] == only_id) {
            return true;
        }
    }
    return false;
}

static void favorites_refresh_button(const FavoriteBtn *binding)
{
    if (!binding || !binding->valid || !binding->btn || !binding->label) {
        return;
    }

    lv_obj_remove_style(binding->btn, &blue_button_style, 0);
    lv_obj_remove_style(binding->btn, &btn_style, 0);

    if (favorites_contains(binding->only_id)) {
        lv_obj_add_style(binding->btn, &btn_style, 0);
        lv_label_set_text(binding->label, "\xE5\xB7\xB2\xE6\x94\xB6\xE8\x97\x8F");
        lv_obj_set_style_text_color(binding->label, lv_color_black(), 0);
    } else {
        lv_obj_add_style(binding->btn, &blue_button_style, 0);
        lv_label_set_text(binding->label, "\xE6\x94\xB6\xE8\x97\x8F");
        lv_obj_set_style_text_color(binding->label, lv_color_white(), 0);
    }

    lv_obj_center(binding->label);
}

static void favorites_refresh_all_buttons(void)
{
    for (size_t i = 0; i < MAX_FAVORITES_BUTTONS; i++) {
        favorites_refresh_button(&bindings[i]);
    }
}

static void favorites_save_to_sd(void)
{
    FILE *file;

    ensure_user_data_dir();
    file = fopen(FAVORITES_FILE_PATH, "w");
    if (!file) {
        return;
    }

    for (size_t i = 0; i < favorite_count; i++) {
        fprintf(file, "%u\n", favorite_ids[i]);
    }

    fflush(file);
    fclose(file);
}

static void favorites_load_from_sd(void)
{
    FILE *file = fopen(FAVORITES_FILE_PATH, "r");
    char line[32];

    favorite_count = 0;
    if (!file) {
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *end = NULL;
        unsigned long value = strtoul(line, &end, 10);

        if (end == line || value > UINT16_MAX) {
            continue;
        }
        if (favorites_contains((uint16_t)value)) {
            continue;
        }
        if (favorite_count < MAX_FAVORITES) {
            favorite_ids[favorite_count++] = (uint16_t)value;
        }
    }

    fclose(file);
}

static FavoriteBtn *favorites_find_binding(lv_obj_t *btn)
{
    for (size_t i = 0; i < MAX_FAVORITES_BUTTONS; i++) {
        if (bindings[i].valid && bindings[i].btn == btn) {
            return &bindings[i];
        }
    }
    return NULL;
}

static bool favorites_add(uint16_t only_id)
{
    if (favorites_contains(only_id) || favorite_count >= MAX_FAVORITES) {
        return false;
    }
    favorite_ids[favorite_count++] = only_id;
    return true;
}

static bool favorites_remove(uint16_t only_id)
{
    for (size_t i = 0; i < favorite_count; i++) {
        if (favorite_ids[i] == only_id) {
            for (size_t j = i + 1; j < favorite_count; j++) {
                favorite_ids[j - 1] = favorite_ids[j];
            }
            favorite_count--;
            return true;
        }
    }
    return false;
}

static void favorites_toggle(uint16_t only_id)
{
    if (favorites_contains(only_id)) {
        favorites_remove(only_id);
    } else {
        favorites_add(only_id);
    }
    favorites_save_to_sd();
}

void favorites_init(void)
{
    if (favorites_ready) {
        return;
    }
    memset(bindings, 0, sizeof(bindings));
    favorites_load_from_sd();
    favorites_ready = true;
}

void favorites_bind_button(lv_obj_t *btn, lv_obj_t *label, const Station *station)
{
    FavoriteBtn *binding;

    if (!btn || !label || !station) {
        return;
    }

    binding = favorites_find_binding(btn);
    if (!binding) {
        for (size_t i = 0; i < MAX_FAVORITES_BUTTONS; i++) {
            if (!bindings[i].valid) {
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
    binding->only_id = (uint16_t)station->only_id;
    favorites_refresh_button(binding);
}

void favorites_button_event_cb(lv_event_t *e)
{
    lv_obj_t *btn;
    FavoriteBtn *binding;

    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }

    btn = lv_event_get_target(e);
    binding = favorites_find_binding(btn);
    if (!binding) {
        return;
    }

    favorites_toggle(binding->only_id);
    favorites_refresh_all_buttons();
    favorite_station_show();
    hide_pop_window();
    favorite_station_lb_style_reset();
}
