#ifndef DF_UI_MENU
#define DF_UI_MENU
#include "core/container/array.h"
#include "math/matrix.h"
#include "graphics/shader.h"
#include "graphics/text.h"

typedef struct menu {
    container_index cursor;
    array entries;
    font fnt;
    vec3 offset;
    bool can_wrap;
}* menu;

event(menu_activate_event, menu m);

typedef struct menu_entry {
    text label;
    menu submenu;
    menu_activate_event* activate;
} menu_entry;

menu menu_new(font fnt);
container_index menu_add_entry(menu m, const char* label, menu submenu, menu_activate_event* event);
container_index menu_set_cursor(menu m, container_index index);
container_index menu_move_cursor(menu m, int16 offset);
menu menu_activate(menu m);
menu_entry* menu_get_entry(menu m, container_index index);

void menu_draw_entry(menu m, array_iter entry, shader s, mat4 vp);
vec2 menu_calculate_dims(menu m);

void menu_free(menu m);

#endif
