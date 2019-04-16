#include "layout.h"
#include "layout_element.h"

#include "core/log/log.h"

// Initialize a new layout
void layout_init(layout* l, vec2 bounds, layout_type type) {
    l->bounds = (layout_element) {
        .requested_dims = bounds,
        .calculated_bounds = {
            .position = vec2_zero,
            .dimensions = bounds
        },
        .align = ALIGN_DEFAULT
    };
    l->children = array_mnew_ordered(layout_element*, 4);
    l->type = type;
}

// Add a new element to a layout
void layout_add_element(layout* l, layout_element* elem) {
    array_add(l->children, elem);
}

// Update a layout, recalculating the bounds of its children
void layout_update(layout* l) {
    switch(l->type) {
        // Free layout type: Children don't interact, and attach to the layout based on their alignment
        case LAYOUT_FREE:
            array_foreach(l->children, it) {
                layout_element* elem = array_iter_data(it, layout_element*);

                aabb_2d box = {
                    .position = vec2_zero,
                    .dimensions = vec2_add(elem->requested_dims, vec2_mul(elem->padding, 2))
                };

                box.dimensions.x = clamp(box.dimensions.x, 0, l->bounds.calculated_bounds.dimensions.x);
                box.dimensions.y = clamp(box.dimensions.y, 0, l->bounds.calculated_bounds.dimensions.y);
                aabb_2d bounds = aabb_align_box_2d(box, l->bounds.calculated_bounds, elem->align);
            
                elem->calculated_bounds = (aabb_2d) {
                    .position=vec2_add(bounds.position, elem->padding),
                    .dimensions=vec2_sub(box.dimensions, vec2_mul(elem->padding, 2))
                };
            }
        break;
        // Horizontal layout type: Children are placed next to each other in a horizontal line, and resize proportionally if the line overflows
        case LAYOUT_STACK_HORIZONTAL: {
            float width_factor = 0.0f;
            array_foreach(l->children, it) {
                width_factor += array_iter_data(it, layout_element*)->requested_dims.x;
            }

            if(l->bounds.calculated_bounds.x <= width_factor) {
                width_factor = 1;
            } else if(l->bounds.calculated_bounds.x == 0) {
                width_factor = 0;
            } else {
                width_factor /= l->bounds.calculated_bounds.x;
            }

            float x_counter = l->bounds.calculated_bounds.position.x;
            array_foreach(l->children, it) {
                layout_element* elem = array_iter_data(it, layout_element*);

                aabb_2d box = aabb_2d_zero;
                box.position.x = x_counter + elem->padding.x;
                box.position.y = l->bounds.calculated_bounds.position.y + elem->padding.y;
                box.dimensions.x = clamp(elem->requested_dims.x, 0, l->bounds.calculated_bounds.dimensions.x - elem->padding.x * 2);
                box.dimensions.y = clamp(elem->requested_dims.y, 0, l->bounds.calculated_bounds.dimensions.y - elem->padding.y * 2);
                elem->calculated_bounds = box;

                x_counter += box.dimensions.x + elem->padding.x * 2;
            }

            if(x_counter - l->bounds.calculated_bounds.position.x > l->bounds.calculated_bounds.dimensions.x) {
                float ratio = l->bounds.calculated_bounds.dimensions.x / (x_counter - l->bounds.calculated_bounds.position.x);
                x_counter = l->bounds.calculated_bounds.position.x;
                array_foreach(l->children, it) {
                    layout_element* elem = array_iter_data(it, layout_element*);
                    elem->calculated_bounds.position.x = x_counter + elem->padding.x;
                    elem->calculated_bounds.dimensions.x *= ratio;
                    x_counter += elem->calculated_bounds.dimensions.x + elem->padding.x * 2;
                }
            }
        } break;
        // Vertical layout type: Children are placed next to each other in a vertical line, and resize proportionally if the line overflows
        case LAYOUT_STACK_VERTICAL: {
            float height_factor = 0.0f;
            array_foreach(l->children, it) {
                height_factor += array_iter_data(it, layout_element*)->requested_dims.y;
            }

            if(l->bounds.calculated_bounds.y <= height_factor) {
                height_factor = 1;
            } else if(l->bounds.calculated_bounds.y == 0) {
                height_factor = 0;
            } else {
                height_factor /= l->bounds.calculated_bounds.y;
            }

            float y_counter = l->bounds.calculated_bounds.position.y;
            array_foreach(l->children, it) {
                layout_element* elem = array_iter_data(it, layout_element*);

                aabb_2d box = aabb_2d_zero;
                box.position.x = l->bounds.calculated_bounds.position.x + elem->padding.x;
                box.position.y = y_counter + elem->padding.y;
                box.dimensions.x = clamp(elem->requested_dims.x, 0, l->bounds.calculated_bounds.dimensions.x - elem->padding.x * 2);
                box.dimensions.y = clamp(elem->requested_dims.y, 0, l->bounds.calculated_bounds.dimensions.y - elem->padding.y * 2);
                elem->calculated_bounds = box;

                y_counter += box.dimensions.y + elem->padding.y * 2;
            }

            if(y_counter - l->bounds.calculated_bounds.position.y > l->bounds.calculated_bounds.dimensions.y) {
                float ratio = l->bounds.calculated_bounds.dimensions.y / (y_counter - l->bounds.calculated_bounds.position.y);
                y_counter = l->bounds.calculated_bounds.position.y;
                array_foreach(l->children, it) {
                    layout_element* elem = array_iter_data(it, layout_element*);
                    elem->calculated_bounds.position.y = y_counter + elem->padding.y;
                    elem->calculated_bounds.dimensions.y *= ratio;
                    y_counter += elem->calculated_bounds.dimensions.y + elem->padding.y * 2;
                }
            }
        } break;
    }
}

// Cleanup a layout, freeing resources
void layout_cleanup(layout* l) {
    array_free(l->children);
}
