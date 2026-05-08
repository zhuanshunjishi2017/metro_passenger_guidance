#ifndef FAVORITES_H
#define FAVORITES_H

#include "canvas.h"
#include <stdbool.h>
#include <stdint.h>

void favorites_init(void);
void favorites_bind_button(lv_obj_t *btn, lv_obj_t *label, const Station *station);
void favorites_button_event_cb(lv_event_t *e);

#endif
