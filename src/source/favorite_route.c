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

#define FAVORITE_ROUTES_FILE_PATH "user_data/favorite_routes.txt"
#define MAX_FAVORITE_ROUTES_BUTTONS 4

typedef struct {
    lv_obj_t *btn;
    lv_obj_t *label;
    uint16_t only_id_start;
    uint16_t only_id_end;
    bool valid;
} FavoriteRouteBtn;

size_t favorite_route_count = 0;
FavoriteRouteBtn route_bindings[MAX_FAVORITE_ROUTES_BUTTONS];
static bool favorites_ready = false;
FavoriteRouteID favorite_routes[MAX_ROUTES];

void favorite_route_show(void);

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

static bool favorites_route_contains(uint16_t start_id, uint16_t end_id)
{
    for (size_t i = 0; i < favorite_route_count; i++) {
        if (favorite_routes[i].start_id == start_id && favorite_routes[i].end_id == end_id) {
            return true;
        }
    }
    return false;
}

static void favorites_route_refresh_button(const FavoriteRouteBtn *binding)
{
    if (!binding || !binding->valid || !binding->btn || !binding->label) {
        return;
    }

    lv_obj_remove_style(binding->btn, &blue_button_style, 0);
    lv_obj_remove_style(binding->btn, &btn_style, 0);

    if (favorites_route_contains(binding->only_id_start, binding->only_id_end)) {
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

static void favorites_route_refresh_all_buttons(void)
{
    for (size_t i = 0; i < MAX_FAVORITE_ROUTES_BUTTONS; i++) {
        favorites_route_refresh_button(&route_bindings[i]);
    }
}

static void favorites_route_save_to_sd(void)
{
    FILE *file;

    ensure_user_data_dir();
    file = fopen(FAVORITE_ROUTES_FILE_PATH, "w");
    if (!file) {
        return;
    }

    for (size_t i = 0; i < favorite_route_count; i++) {
        fprintf(file, "%u,%u\n", favorite_routes[i].start_id, favorite_routes[i].end_id);
    }

    fflush(file);
    fclose(file);
}

static void favorites_route_load_from_sd(void)
{
    FILE *file = fopen(FAVORITE_ROUTES_FILE_PATH, "r");
    char line[30];

    favorite_route_count = 0;
    memset(favorite_routes, 0, sizeof(favorite_routes));
    if (!file) {
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *comma = strchr(line, ',');
        char *end1 = NULL;
        char *end2 = NULL;
        unsigned long start_id;
        unsigned long end_id;

        if (!comma) {
            continue;
        }

        *comma = '\0';
        start_id = strtoul(line, &end1, 10);
        end_id = strtoul(comma + 1, &end2, 10);

        if (end1 == line || end2 == comma + 1) {
            continue;
        }
        if (start_id > UINT16_MAX || end_id > UINT16_MAX) {
            continue;
        }
        if (favorites_route_contains((uint16_t)start_id, (uint16_t)end_id)) {
            continue;
        }
        if (favorite_route_count < MAX_ROUTES) {
            favorite_routes[favorite_route_count].start_id = (uint16_t)start_id;
            favorite_routes[favorite_route_count].end_id = (uint16_t)end_id;
            favorite_route_count++;
        }
    }

    fclose(file);
}

FavoriteRouteBtn *favorites_route_find_binding(lv_obj_t *btn)
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

static bool favorites_route_add(uint16_t start_id, uint16_t end_id)
{
    if (favorites_route_contains(start_id, end_id) || favorite_route_count >= MAX_ROUTES) {
        return false;
    }
    favorite_routes[favorite_route_count].start_id = start_id;
    favorite_routes[favorite_route_count].end_id = end_id;
    favorite_route_count++;
    return true;
}

static bool favorites_route_remove(uint16_t start_id, uint16_t end_id)
{
    for (size_t i = 0; i < favorite_route_count; i++) {
        if (favorite_routes[i].start_id == start_id && favorite_routes[i].end_id == end_id) {
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

void favorites_route_toggle(uint16_t start_id, uint16_t end_id)
{
    if (favorites_route_contains(start_id, end_id)) {
        favorites_route_remove(start_id, end_id);
    } else {
        favorites_route_add(start_id, end_id);
    }
    favorites_route_save_to_sd();
}

void favorites_route_init(void)
{
    if (favorites_ready) {
        return;
    }
    memset(route_bindings, 0, sizeof(route_bindings));
    favorites_route_load_from_sd();
    favorites_ready = true;
}

void favorites_route_bind_button(lv_obj_t *btn, lv_obj_t *label, const Station *st_station, const Station *end_station)
{
    FavoriteRouteBtn *binding;

    if (!btn || !label || !st_station || !end_station) {
        return;
    }

    binding = favorites_route_find_binding(btn);
    if (!binding) {
        for (size_t i = 0; i < MAX_FAVORITE_ROUTES_BUTTONS; i++) {
            if (!route_bindings[i].valid) {
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
    binding->only_id_start = (uint16_t)st_station->only_id;
    binding->only_id_end = (uint16_t)end_station->only_id;
    favorites_route_refresh_button(binding);
}

void favorites_route_button_event_cb(lv_event_t *e)
{
    lv_obj_t *btn;
    FavoriteRouteBtn *binding;

    if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
        return;
    }

    btn = lv_event_get_target(e);
    binding = favorites_route_find_binding(btn);
    if (!binding) {
        return;
    }

    favorites_route_toggle(binding->only_id_start, binding->only_id_end);
    favorites_route_refresh_all_buttons();
    favorite_route_show();
}
