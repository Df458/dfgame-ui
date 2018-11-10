#include "application/game/mainloop.h"
#include "application/input.h"
#include "application/game/window.h"
#include "graphics/camera.h"
#include "graphics/shader.h"
#include "graphics/shader_init.h"
#include "graphics/text.h"
#include "resource/font_loader.h"
#include "resource/paths.h"

#include <math.h>

#include "frame.h"
#include "frame_io.h"

static window win;
static camera c_main;
static shader s_default;
static text info_text;
static frame f_test;
static frame_data f_data_test;
static vec2 dims = { .x = 100, .y = 100 };
static float timer = 0;

axis_id   a_width;
axis_id   a_height;

const char* frame_file = "frame.xml";
const char* font_file = "OpenSans-Regular.ttf";

const char* info_str =
"Controls\n"
"Left/Right: Adjust Width\n"
"Up/Down: Adjust Height";

bool loop(mainloop l, float dt) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    timer += dt;

    dims.x = clamp(dims.x + get_axis_value(a_width), 0, 800);
    dims.y = clamp(dims.y + get_axis_value(a_height), 0, 600);
    frame_set_dimensions(f_test, dims);
    frame_draw(f_test, s_default, camera_get_vp(c_main));

    // Draw text
    glUseProgram(s_default.id);
    vec2 offset = (vec2){ .x = -634, .y=354 };
    text_draw(info_text, s_default, mat4_mul(camera_get_vp(c_main), mat4_translate(mat4_ident, offset)));

    window_redraw(win);
    return !window_get_should_close(win);
}

int main(int argc, char** argv) {
    win = window_new(1280, 720, false, "UI Demo");
    init_base_resource_path(NULL);
    shaders_init();
    s_default  = shader_basic_tex_get();
    c_main = window_create_2d_camera(win);

    // Create frame
    load_frame(assets_path(frame_file, NULL), &f_data_test);
    f_test = frame_new(&f_data_test, dims);
    frame_set_align(f_test, FRAME_ALIGN_CENTER);

    // Create help text
    char* p = assets_path("OpenSans-Regular.ttf", NULL);
    info_text = text_new(load_font(p, 16), info_str);
    text_set_align(info_text, TEXT_ALIGN_BOTTOM_LEFT);
    sfree(p);

    // Input bindings
    a_width = input_add_key_axis(K_RIGHT, 4, 4, true);
    input_bind_key_axis(K_LEFT, a_width, -4);
    a_height = input_add_key_axis(K_DOWN, 5, 5, true);
    input_bind_key_axis(K_UP, a_height, -5);

    mainloop_create_run(loop);

    frame_free(f_test, false);
    text_free(info_text, true);

    camera_free(c_main);
    resource_path_free();
    window_free_final(win);
    shaders_cleanup();

    return 0;
}
