#ifndef FAVORITES_H
#define FAVORITES_H
#define MAX_FAVORITES 30
#define MAX_FAVORITES_BUTTONS 4
#define MAX_ROUTES 10
#include "canvas.h"
#include <stdbool.h>
#include <stdint.h>

void favorites_init(void);
void favorites_bind_button(lv_obj_t *btn, lv_obj_t *label, const Station *station);
void favorites_button_event_cb(lv_event_t *e);

void favorite_route_ui_init(void);
void favorites_route_init(void);
void favorites_route_toggle(uint16_t start_id, uint16_t end_id);
void favorites_route_bind_button(lv_obj_t *btn, lv_obj_t *label,
                                  const Station *st, const Station *end);
void favorites_route_button_event_cb(lv_event_t *e);
void favorites_route_invalidate_binding(lv_obj_t *btn);

typedef struct {
    uint16_t start_id; //起始站的唯一id
    uint16_t end_id;   //终点站的唯一id
} FavoriteRouteID;

extern FavoriteRouteID favorite_routes[MAX_ROUTES];
extern size_t favorite_route_count;

#endif
