#include <stdio.h>
#include <stdlib.h>
#include "graphics/frame_buffer.h"
#include "graphics/renderer.h"
#include "graphics/window.h"

struct window_resize_callback_args {
    struct frame_buffer *frame_buffer;
};

void resize_callback(struct window *window, int width, int height, void *arg) {
    struct window_resize_callback_args *args = arg;
    struct frame_buffer *frame_buffer = args->frame_buffer;

    frame_buffer->width = width;
    frame_buffer->height = height;
}

int main() {
    struct window window;
    window_init(&window, 800, 600, "Software Renderer!");

    window_reset_cursor(&window);

    renderer_init();

    struct frame_buffer frame_buffer;
    frame_buffer_init(&frame_buffer);

    struct window_resize_callback_args *resize_callback_args =
        malloc(sizeof(struct window_resize_callback_args));
    resize_callback_args->frame_buffer = &frame_buffer;

    window_set_resize_callback(&window, resize_callback, resize_callback_args);

    while (!window_should_close(&window)) {
        renderer_clear_buffers();

        window_update_delta_time(&window);
        window_update_input(&window);

        struct pixel *framebuffer = malloc(
            sizeof(struct pixel) * frame_buffer.width * frame_buffer.height);

        for (int x = 0; x < frame_buffer.width; x++) {
            for (int y = 0; y < frame_buffer.height; y++) {
                framebuffer[x + y * frame_buffer.width] =
                    (struct pixel){255, 0, 0};

                if ((x / 10) % 2 == 1) {
                    framebuffer[x + y * frame_buffer.width].green = 255;
                }

                if ((y / 5) % 2 == 1) {
                    framebuffer[x + y * frame_buffer.width].red = 0;
                }
            }
        }

        frame_buffer_generate(&frame_buffer, framebuffer);
        frame_buffer_draw(&frame_buffer);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    window_shutdown();

    return 0;
}
