// Log category, used to filter logs
#define LOG_CATEGORY "UI"

#include "frame_data.h"

#include "core/check.h"

void frame_data_new_default(frame_data* f, gltex tex, aabb_2d frame_box, uint16 margin) {
    check_return(f != NULL, "Frame data is NULL", );

    f->texture = tex;
    f->margin = margin;
    for(uint8 i = 0; i < 9; ++i) {
        f->uvs[i] = frame_box;

        switch(i % 3) { // Column UVs
            case 0: // Left column
                f->uvs[i].dimensions.x = margin;
            break;
            case 1: // Center column
                f->uvs[i].position.x += margin;
                f->uvs[i].dimensions.x -= margin * 2;
            break;
            case 2: // Right column
                f->uvs[i].position.x += f->uvs[i].dimensions.x - margin;
                f->uvs[i].dimensions.x = margin;
            break;
        }
        switch(i / 3) { // Row UVs
            case 0: // Top row
                f->uvs[i].dimensions.y = margin;
            break;
            case 1: // Center row
                f->uvs[i].position.y += margin;
                f->uvs[i].dimensions.y -= margin * 2;
            break;
            case 2: // Bottom column
                f->uvs[i].position.y += f->uvs[i].dimensions.y - margin;
                f->uvs[i].dimensions.y = margin;
            break;
        }
    }

    f->asset_path = NULL;
}
void frame_data_cleanup(frame_data* f) {
    gltex_cleanup(&f->texture);

    if(f != NULL) {
        sfree(f->asset_path);
    }
}
