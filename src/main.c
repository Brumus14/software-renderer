#include <stdio.h>
#include <stdlib.h>
#include "input/keyboard.h"
#include "math/math_util.h"
#include "math/quat.h"
#include "renderer/camera.h"
#include "math/vec3/vec3d.h"
#include "math/vec4/vec4d.h"
#include "renderer/frame_buffer.h"
#include "renderer/renderer.h"
#include "renderer/camera.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "renderer/model.h"

struct window_resize_callback_args {
    struct frame_buffer *frame_buffer;
    struct camera *camera;
};

void resize_callback(struct window *window, int width, int height, void *arg) {
    struct window_resize_callback_args *args = arg;
    struct frame_buffer *frame_buffer = args->frame_buffer;
    struct camera *camera = args->camera;

    frame_buffer->new_width = width;
    frame_buffer->new_height = height;
    frame_buffer->resized = true;

    camera_set_aspect_ratio(camera, window_get_aspect_ratio(window));
}

int main() {
    struct window window;
    window_init(&window, 800, 600, "Software Renderer!");

    window_reset_cursor(&window);

    renderer_init();

    struct frame_buffer frame_buffer;
    frame_buffer_init(&frame_buffer, 800, 600);

    struct camera camera;
    camera_init(&camera, (struct vec3d){0, 0, 5}, VEC3D_ZERO, 90,
                window_get_aspect_ratio(&window), 0.1, 1000);

    struct window_resize_callback_args *resize_callback_args =
        malloc(sizeof(struct window_resize_callback_args));
    resize_callback_args->frame_buffer = &frame_buffer;
    resize_callback_args->camera = &camera;

    window_set_resize_callback(&window, resize_callback, resize_callback_args);

    struct model model = model_from_obj("res/models/high_res/lucy.obj",
                                        (struct vec3d){}, VEC3D_ZERO);

    while (!window_should_close(&window)) {
        renderer_clear_buffers();

        window_update_delta_time(&window);
        window_update_input(&window);

        if (keyboard_key_down(&window.keyboard, KEYCODE_W)) {
            camera_move(&camera, (struct vec3d){0, 0, -0.1});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_S)) {
            camera_move(&camera, (struct vec3d){0, 0, 0.1});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_A)) {
            camera_move(&camera, (struct vec3d){-0.1, 0, 0});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_D)) {
            camera_move(&camera, (struct vec3d){0.1, 0, 0});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_SPACE)) {
            camera_move(&camera, (struct vec3d){0, 0.1, 0});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_LEFT_SHIFT)) {
            camera_move(&camera, (struct vec3d){0, -0.1, 0});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_ARROW_UP)) {
            camera_rotate(&camera, (struct vec3d){1, 0, 0});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_ARROW_DOWN)) {
            camera_rotate(&camera, (struct vec3d){-1, 0, 0});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_ARROW_LEFT)) {
            camera_rotate(&camera, (struct vec3d){0, -1, 0});
        }

        if (keyboard_key_down(&window.keyboard, KEYCODE_ARROW_RIGHT)) {
            camera_rotate(&camera, (struct vec3d){0, 1, 0});
        }

        frame_buffer_begin(&frame_buffer);

        camera_update(&camera);

        model_rotate(&model, (struct vec3d){0, 10, 0});
        model_draw(&model, &camera, &frame_buffer);

        frame_buffer_generate(&frame_buffer);
        frame_buffer_draw(&frame_buffer);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    window_shutdown();

    return 0;
}
