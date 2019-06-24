#include "menu.h"

#include "core/check.h"
#include "graphics/font.h"

menu menu_new(font fnt) {
    check_return(fnt != NULL, "Font is NULL", NULL);

    menu m = mscalloc(1, struct menu);
    m->entries = array_mnew_ordered(menu_entry, 8);
    m->fnt = fnt;

    return m;
}

container_index menu_add_entry(menu m, const char* label, menu submenu, menu_activate_event* event) {
    check_return(m != NULL, "Menu is NULL", CONTAINER_INDEX_INVALID);

    menu_entry entry = {
        .label = text_new(m->fnt, label),
        .submenu = submenu,
    };

    bind_event(entry.activate, event);

    array_add(m->entries, entry);

    return array_get_length(m->entries) - 1;
}

container_index menu_set_cursor(menu m, container_index index) {
    check_return(m != NULL, "Menu is NULL", CONTAINER_INDEX_INVALID);
    
    if(index < array_get_length(m->entries))
    {
        m->cursor = index;
    }
    else
    {
        m->cursor = CONTAINER_INDEX_INVALID;
    }

    return m->cursor;
}
container_index menu_move_cursor(menu m, int16 offset) {
    check_return(m != NULL, "Menu is NULL", CONTAINER_INDEX_INVALID);

    container_index entry_count = array_get_length(m->entries);
    if(entry_count == 0) {
        return CONTAINER_INDEX_INVALID;
    }
    
    if(offset < 0 && m->cursor < -offset) {
        if(!m->can_wrap) {
            m->cursor = 0;
        } else {
            do {
                offset += entry_count;
            } while(m->cursor < -offset);
            m->cursor += offset;
        }
    } else if(offset > 0 && m->cursor + offset >= entry_count) {
        if(!m->can_wrap) {
            m->cursor = entry_count - 1;
        } else {
            do {
                offset -= entry_count;
            } while(m->cursor + offset > entry_count);
            m->cursor += offset;
        }
    } else {
        m->cursor += offset;
    }
    return m->cursor;
}
menu menu_activate(menu m) {
    check_return(m != NULL, "Menu is NULL", NULL);

    if(m->cursor == CONTAINER_INDEX_INVALID)
    {
        return NULL;
    }

    menu_entry* entry = array_get(m->entries, m->cursor);

    if(entry == NULL)
    {
        return NULL;
    }

    call_event(entry->activate, m);

    return entry->submenu;
}
menu_entry* menu_get_entry(menu m, container_index index) {
    return array_get(m->entries, index);
}

void menu_clear(menu m) {
    check_return(m != NULL, "Menu is NULL", );

    array_foreach(m->entries, iter) {
        menu_entry* entry = iter.data;
        text_free(entry->label, false);
        array_remove_iter(m->entries, &iter);
    }

    m->cursor = 0;
}

void menu_draw_entry(menu m, array_iter iter, shader s, mat4 vp) {
    menu_entry* entry = iter.data;

    vec3 vec = vec3_zero;
    vec = vec_mul(m->offset, iter.index);
    vec.y += font_get_height(m->fnt) * iter.index;

    text_draw(entry->label, s, mat4_mul(vp, mat4_translate(mat4_ident, vec)));
}

vec2 menu_calculate_dims(menu m) {
    vec2 bounds = vec2_zero;
    float height = font_get_height(m->fnt);

    array_foreach(m->entries, iter) {
        menu_entry* entry = iter.data;

        vec2 t_bounds = text_get_bounds(entry->label);
        t_bounds.x += iter.index * m->offset.x;
        t_bounds.y += iter.index * (m->offset.y + height);

        bounds.x = max(bounds.x, t_bounds.x);
        bounds.y = max(bounds.y, t_bounds.y);
    }

    return bounds;
}

void menu_free(menu m) {
    check_return(m != NULL, "Menu is NULL", );

    array_foreach(m->entries, iter) {
        menu_entry* entry = iter.data;
        text_free(entry->label, false);
    }
    array_free(m->entries);
    sfree(m);
}
