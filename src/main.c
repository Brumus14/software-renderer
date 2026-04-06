#include <stdio.h>
#include "graphics/frame_buffer.h"
#include "graphics/renderer.h"
#include "graphics/window.h"

void window_resize_callback(struct window *window, int width, int height) {
}

int main() {
    struct window window;
    window_init(&window, 800, 600, "Software Renderer!");
    window_set_framebuffer_size_callback(&window, window_resize_callback);

    window_reset_cursor(&window);

    renderer_init();

    renderer_set_viewport(0, 0, 800, 600);

    struct frame_buffer frame_buffer;
    frame_buffer_init(&frame_buffer);

    while (!window_should_close(&window)) {
        renderer_clear_buffers();

        window_update_delta_time(&window);
        window_update_input(&window);

        struct pixel framebuffer[800 * 600];

        for (int i = 0; i < 800 * 600; i++) {
            framebuffer[i] = (struct pixel){255, 0, 0};

            if (i % 2 == 1) {
                framebuffer[i].green = 150;
            }
        }

        frame_buffer_generate(&frame_buffer, 800, 600, framebuffer);
        frame_buffer_draw(&frame_buffer);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    window_shutdown();

    return 0;
}
