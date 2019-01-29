#ifndef DF_UI_LAYOUT
#define DF_UI_LAYOUT
#include "core/container/array.h"

#include "layout_element.h"

// Represents the way elements are arranged in a layout
typedef enum layout_type {
    LAYOUT_FREE,
    LAYOUT_STACK_HORIZONTAL,
    LAYOUT_STACK_VERTICAL,
} layout_type;

// Represents a set of layout elements in a particular arrangement
typedef struct layout {
    array children;
    layout_element bounds;
    layout_type type;
} layout;

// Initialize a new layout
void layout_init(layout* l, vec2 bounds, layout_type type);

// Add a new element to a layout
void layout_add_element(layout* l, layout_element* elem);

// Update a layout, recalculating the bounds of its children
void layout_update(layout* l);

// Cleanup a layout, freeing resources
void layout_cleanup(layout* l);

#endif
