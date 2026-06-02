#include <stdio.h>
#include <stdlib.h>
#include "renderer/frame_buffer.h"
#include "renderer/renderer.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "renderer/model.h"

struct window_resize_callback_args {
    struct frame_buffer *frame_buffer;
};

void resize_callback(struct window *window, int width, int height, void *arg) {
    struct window_resize_callback_args *args = arg;
    struct frame_buffer *frame_buffer = args->frame_buffer;

    frame_buffer->new_width = width;
    frame_buffer->new_height = height;
    frame_buffer->resized = true;
}

int main() {
    struct window window;
    window_init(&window, 800, 600, "Software Renderer!");

    window_reset_cursor(&window);

    renderer_init();

    struct frame_buffer frame_buffer;
    frame_buffer_init(&frame_buffer, 800, 600);

    struct window_resize_callback_args *resize_callback_args =
        malloc(sizeof(struct window_resize_callback_args));
    resize_callback_args->frame_buffer = &frame_buffer;

    window_set_resize_callback(&window, resize_callback, resize_callback_args);

    struct model dragon = model_from_obj("res/models/dragon.obj");

    while (!window_should_close(&window)) {
        renderer_clear_buffers();

        window_update_delta_time(&window);
        window_update_input(&window);

        frame_buffer_begin(&frame_buffer);

        model_draw(&dragon, &frame_buffer);

        frame_buffer_generate(&frame_buffer);
        frame_buffer_draw(&frame_buffer);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    window_shutdown();

    return 0;
}
