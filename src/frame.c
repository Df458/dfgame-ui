// Log category, used to filter logs
#define LOG_CATEGORY "UI"

#include "frame.h"

#include "core/check.h"
#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "math/matrix.h"

// Create a new frame with the given texture data and dimensions
frame frame_new(frame_data* data, vec2 dims) {
    frame f = mscalloc(1, struct frame);
    f->data = data;
    f->align = FRAME_ALIGN_DEFAULT;
    f->dims = dims;
    f->m = NULL;

    f->is_dirty = true;

    return f;
}

// Frees the frame
void _frame_free(frame f, bool deep) {
    check_return(f != NULL, "Frame is NULL", );

    if(deep) {
        frame_data_cleanup(f->data);
        sfree(f->data);
    }
    if(f->m) {
        mesh_free(f->m);
    }

    sfree(f);
}

void build_box(vt_pt* verts, aabb_2d box, aabb_2d uv_box) {
    verts[0] = (vt_pt) {
        .position = (vec3){ .x = box.position.x, .y = box.position.y, .z = 0 },
        .uv = uv_box.position
    };

    verts[1] = (vt_pt) {
        .position = (vec3){ .x = box.position.x + box.dimensions.x, .y = box.position.y, .z = 0 },
        .uv = uv_box.position
    };
    verts[1].uv.x += uv_box.dimensions.x;

    verts[2] = (vt_pt) {
        .position = (vec3){ .x = box.position.x + box.dimensions.x, .y = box.position.y + box.dimensions.y, .z = 0 },
        .uv = vec2_add(uv_box.position, uv_box.dimensions)
    };

    verts[3] = (vt_pt) {
        .position = (vec3){ .x = box.position.x, .y = box.position.y, .z = 0 },
        .uv = uv_box.position
    };

    verts[4] = (vt_pt) {
        .position = (vec3){ .x = box.position.x + box.dimensions.x, .y = box.position.y + box.dimensions.y, .z = 0 },
        .uv = vec2_add(uv_box.position, uv_box.dimensions)
    };

    verts[5] = (vt_pt) {
        .position = (vec3){ .x = box.position.x, .y = box.position.y + box.dimensions.y, .z = 0 },
        .uv = uv_box.position
    };
    verts[5].uv.y += uv_box.dimensions.y;
}

// Rebuilds the mesh data for f
void frame_rebuild_mesh(frame f) {
    check_return(f != NULL, "Frame is NULL", );

    if(f->m != NULL) {
        mesh_free(f->m);
    }

    if(f->data != NULL) {
        vt_pt verts[54];
        uint8 len = 0;

        check_return(f->data->texture.width != 0 && f->data->texture.height != 0, "Texture dimensions are invalid", );
        float uv_x_transform = 1.0f / f->data->texture.width;
        float uv_y_transform = 1.0f / f->data->texture.height;

        for(uint8 i = 0; i < 9; ++i) {
            // If the box has no size, skip it
            if(eq0(vec2_len_squared(f->data->uvs[i].dimensions))) {
                continue;
            }

            aabb_2d box = (aabb_2d) {
                .position = vec2_zero,
                .dimensions = f->dims
            };
            aabb_2d uv_box = f->data->uvs[i];
            uv_box.position.x *= uv_x_transform;
            uv_box.dimensions.x *= uv_x_transform;
            uv_box.position.y *= uv_y_transform;
            uv_box.dimensions.y *= uv_y_transform;

            // For edges, make the box size fixed
            if(i % 3 == 0) { // Left column
                box.dimensions.x = f->data->uvs[i].dimensions.x;
                box.position.x -= box.dimensions.x;
            } else if (i % 3 == 2) { // Right column
                box.position.x += box.dimensions.x;
                box.dimensions.x = f->data->uvs[i].dimensions.x;
            }
            if(i / 3 == 0) { // Top row
                box.dimensions.y = f->data->uvs[i].dimensions.y;
                box.position.y -= box.dimensions.y;
            } else if (i / 3 == 2) { // Bottom row
                box.position.y += box.dimensions.y;
                box.dimensions.y = f->data->uvs[i].dimensions.y;
            }

            build_box(verts + (len), box, uv_box);

            len += 6;
        }

        // Calculate and apply offset for alignment
        vec2 offset = vec2_zero;
        if(f->align == FRAME_ALIGN_TOP || f->align == FRAME_ALIGN_CENTER || f->align == FRAME_ALIGN_BOTTOM) {
            offset.x = f->dims.x * -0.5f;
        } else if(f->align == FRAME_ALIGN_TOP_RIGHT || f->align == FRAME_ALIGN_RIGHT || f->align == FRAME_ALIGN_BOTTOM_RIGHT) {
            offset.x = f->dims.x * -1.0f;
        }
        if(f->align == FRAME_ALIGN_LEFT || f->align == FRAME_ALIGN_CENTER || f->align == FRAME_ALIGN_RIGHT) {
            offset.y = f->dims.y * -0.5f;
        } else if(f->align == FRAME_ALIGN_BOTTOM_LEFT || f->align == FRAME_ALIGN_BOTTOM || f->align == FRAME_ALIGN_BOTTOM_RIGHT) {
            offset.y = f->dims.y * -1.0f;
        }
        for(uint8 i = 0; i < len; ++i) {
            verts[i].position.xy = vec2_add(verts[i].position.xy, offset);
        }

        if(f->m) {
            mesh_free(f->m);
        }
        if(len != 0) {
            f->m = mesh_new(len, verts, NULL);
        }

        f->is_dirty = false;
    }
}

// Gets/sets the frame's texture data
const frame_data* frame_get_data(frame f) {
    check_return(f != NULL, "Frame is NULL", NULL);

    return f->data;
}
void frame_set_data(frame f, frame_data* data) {
    check_return(f != NULL, "Frame is NULL", );

    f->data = data;
    f->is_dirty = true;
}

// Gets/sets the dimensions of the frame
vec2 frame_get_dimensions(frame f) {
    check_return(f != NULL, "Frame is NULL", vec2_zero);

    return f->dims;
}
void frame_set_dimensions(frame f, vec2 dims) {
    check_return(f != NULL, "Frame is NULL", );

    f->dims = dims;
    f->is_dirty = true;
}

// Gets/sets the alignment of the frame
frame_alignment frame_get_align(frame f) {
    check_return(f != NULL, "Frame is NULL", FRAME_ALIGN_DEFAULT);

    return f->align;
}
void frame_set_align(frame f, frame_alignment align) {
    check_return(f != NULL, "Frame is NULL", );
    check_return(align <= FRAME_ALIGN_LAST, "Invalid frame alignment 0x%x", , align);

    f->align = align;
    f->is_dirty = true;
}

// Return the frame's generated mesh
const mesh frame_get_mesh(frame f) {
    check_return(f != NULL, "Frame is NULL", NULL);

    if(f->is_dirty || f->m == NULL) {
        frame_rebuild_mesh(f);
    }

    return f->m;
}

// Helper function to render a frame
void frame_draw(frame f, shader s, mat4 m) {
    mesh final_mesh = frame_get_mesh(f);

    if(final_mesh != NULL) {
        glUseProgram(s.id);

        shader_bind_uniform_name(s, "u_transform", m);
        shader_bind_uniform_texture_name(s, "u_texture", f->data->texture, GL_TEXTURE0);
        vec2 v0 = vec2_zero;
        vec2 v1 = (vec2){.x=1,.y=1};
        shader_bind_uniform_name(s, "uv_offset", v0);
        shader_bind_uniform_name(s, "uv_scale", v1);
        mesh_render(s, final_mesh, GL_TRIANGLES, "i_pos", VT_POSITION, "i_uv", VT_TEXTURE);
    }
}
