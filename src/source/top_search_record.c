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

#define SEARCH_RECORD_FILE_PATH "user_data/search_record.txt"

extern lv_obj_t *top_search_record_station[SEARCH_LIST_LEN];
extern lv_obj_t *top_search_record_line[SEARCH_LIST_LEN];
extern lv_obj_t *top_search_record_transfer1[SEARCH_LIST_LEN];
extern lv_obj_t *top_search_record_transfer2[SEARCH_LIST_LEN];

void search_record_load_from_sd(void);
void search_record_save_to_sd(void);

uint16_t search_record_ids[SEARCH_LIST_LEN];
int search_record_count = 0;

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

bool search_record_contains(uint16_t only_id)
{
    for (int i = 0; i < search_record_count; i++) {
        if (search_record_ids[i] == only_id) {
            return true;
        }
    }
    return false;
}

void search_record_save_to_sd(void)
{
    FILE *file;

    ensure_user_data_dir();
    file = fopen(SEARCH_RECORD_FILE_PATH, "w");
    if (!file) {
        return;
    }

    for (int i = 0; i < search_record_count; i++) {
        fprintf(file, "%u\n", search_record_ids[i]);
    }

    fflush(file);
    fclose(file);
}

void search_record_load_from_sd(void)
{
    FILE *file = fopen(SEARCH_RECORD_FILE_PATH, "r");
    char line[32];

    search_record_count = 0;
    memset(search_record_ids, 0, sizeof(search_record_ids));
    if (!file) {
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *end = NULL;
        unsigned long value = strtoul(line, &end, 10);

        if (end == line || value > UINT16_MAX) {
            continue;
        }
        if (search_record_contains((uint16_t)value)) {
            continue;
        }
        if (search_record_count < SEARCH_LIST_LEN) {
            search_record_ids[search_record_count++] = (uint16_t)value;
        }
    }

    fclose(file);
}

void search_record_add(uint16_t only_id)
{
    if (search_record_count >= SEARCH_LIST_LEN) {
        return;
    }

    search_record_ids[search_record_count++] = only_id;
    search_record_save_to_sd();
}

void search_record_refresh(const char *text)
{
    const Station *station;
    int record_index;

    if (text == NULL || text[0] == '\0') {
        return;
    }

    station = find_station_by_name(text);
    if (station == NULL) {
        return;
    }
    if (search_record_contains(station->only_id) || search_record_count >= SEARCH_LIST_LEN) {
        return;
    }

    search_record_add(station->only_id);
    record_index = search_record_count - 1;

    lv_label_set_text(top_search_record_station[record_index], station->name);
    lv_obj_clear_flag(top_search_record_station[record_index], LV_OBJ_FLAG_HIDDEN);

    lv_label_set_text_fmt(top_search_record_line[record_index],
                          "%d" "\xE5\x8F\xB7\xE7\xBA\xBF",
                          station->line_belonged);
    lv_obj_set_style_bg_color(top_search_record_line[record_index], get_line_color(station->line_belonged), 0);
    lv_obj_clear_flag(top_search_record_line[record_index], LV_OBJ_FLAG_HIDDEN);

    if (station->transfer_line[0] > 0) {
        lv_label_set_text_fmt(top_search_record_transfer1[record_index],
                              "%d" "\xE5\x8F\xB7\xE7\xBA\xBF",
                              station->transfer_line[0]);
        lv_obj_set_style_bg_color(top_search_record_transfer1[record_index], get_line_color(station->transfer_line[0]), 0);
        lv_obj_clear_flag(top_search_record_transfer1[record_index], LV_OBJ_FLAG_HIDDEN);
    }
    if (station->transfer_line[1] > 0) {
        lv_label_set_text_fmt(top_search_record_transfer2[record_index],
                              "%d" "\xE5\x8F\xB7\xE7\xBA\xBF",
                              station->transfer_line[1]);
        lv_obj_set_style_bg_color(top_search_record_transfer2[record_index], get_line_color(station->transfer_line[1]), 0);
        lv_obj_clear_flag(top_search_record_transfer2[record_index], LV_OBJ_FLAG_HIDDEN);
    }
}
