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

    frame_buffer->new_width = width;
    frame_buffer->new_height = height;
    frame_buffer->resized = true;
}

// Gradient 0 to 1
void draw_line_low(struct frame_buffer *frame_buffer, int x0, int y0, int x1,
                   int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int increment = 1;

    if (dy < 0) {
        dy *= -1;
        increment *= -1;
    }

    int fraction = (2 * dy) - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++) {
        frame_buffer_set_pixel(frame_buffer, x, y,
                               (struct pixel){255, 255, 255});

        if (fraction > 0) {
            y += increment;
            fraction += 2 * (dy - dx);
        } else {
            fraction += 2 * dy;
        }
    }
}

// Gradient greater than 1
void draw_line_high(struct frame_buffer *frame_buffer, int x0, int y0, int x1,
                    int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int increment = 1;

    if (dx < 0) {
        dx *= -1;
        increment *= -1;
    }

    int fraction = (2 * dx) - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++) {
        frame_buffer_set_pixel(frame_buffer, x, y,
                               (struct pixel){255, 255, 255});

        if (fraction > 0) {
            x += increment;
            fraction += 2 * (dx - dy);
        } else {
            fraction += 2 * dx;
        }
    }
}

void draw_line(struct frame_buffer *frame_buffer, int x0, int y0, int x1,
               int y1) {
    if (abs(y1 - y0) < abs(x1 - x0)) {
        if (x0 > x1) {
            draw_line_low(frame_buffer, x1, y1, x0, y0);
        } else {
            draw_line_low(frame_buffer, x0, y0, x1, y1);
        }
    } else {
        if (y0 > y1) {
            draw_line_high(frame_buffer, x1, y1, x0, y0);
        } else {
            draw_line_high(frame_buffer, x0, y0, x1, y1);
        }
    }
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

    while (!window_should_close(&window)) {
        renderer_clear_buffers();

        window_update_delta_time(&window);
        window_update_input(&window);

        frame_buffer_begin(&frame_buffer);

        draw_line(&frame_buffer, 100, 150, 600, 400);
        draw_line(&frame_buffer, 100, 150, 400, 200);
        draw_line(&frame_buffer, 400, 200, 600, 400);
        draw_line(&frame_buffer, 0, 400, 100, 600);

        frame_buffer_generate(&frame_buffer);
        frame_buffer_draw(&frame_buffer);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    window_shutdown();

    return 0;
}
