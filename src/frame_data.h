#ifndef DF_UI_FRAME_DATA
#define DF_UI_FRAME_DATA

#include "core/types.h"
#include "graphics/texture.h"
#include "math/aabb.h"

typedef struct frame_data {
    gltex texture;
    aabb_2d uvs[9];
    uint16 margin;

    char* asset_path;
} frame_data;

void frame_data_new_default(frame_data* f, gltex tex, aabb_2d frame_box, uint16 margin);
void frame_data_cleanup(frame_data* f);

#endif // DF_UI_FRAME_DATA
