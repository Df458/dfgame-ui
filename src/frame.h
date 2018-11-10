#ifndef DF_UI_FRAME
#define DF_UI_FRAME

#include "frame_data.h"

#include "graphics/mesh.hd"
#include "graphics/shader.hd"
#include "math/matrix.hd"

typedef struct frame {
    frame_data* data;

    vec2 dims;
    mesh m;
    bool is_dirty;
}* frame;

// Create a new frame with the given texture data and dimensions
frame frame_new(frame_data* data, vec2 dims);

// Frees the frame
#define frame_free(f, deep) { _frame_free(f, deep); f = NULL; }
void _frame_free(frame f, bool deep);

// Rebuilds the mesh data for f
void frame_rebuild_mesh(frame f);

// Gets/sets the frame's texture data
const frame_data* frame_get_data(frame f);
void frame_set_data(frame f, frame_data* data);

// Gets/sets the dimensions of the frame
vec2 frame_get_dimensions(frame f);
void frame_set_dimensions(frame f, vec2 dims);

// Return the frame's generated mesh
const mesh frame_get_mesh(frame f);

// Helper function to render a frame
void frame_draw(frame f, shader s, mat4 m);

#endif // DF_UI_FRAME
