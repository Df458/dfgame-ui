#ifndef DF_UI_LAYOUT_ELEMENT
#define DF_UI_LAYOUT_ELEMENT
#include "math/vector.h"
#include "math/aabb.h"
#include "frame.h"

// Represents a single element in a larger UI layout
typedef struct layout_element {
    vec2 requested_dims;
    vec2 padding;
    aabb_2d calculated_bounds;
    alignment_2d align;
} layout_element;

#endif // DF_UI_LAYOUT_ELEMENT
