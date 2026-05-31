#include <stdio.h>
#include <stdlib.h>
#include "graphics/frame_buffer.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "model.h"

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

bool is_top_left_edge(int x0, int y0, int x1, int y1) {
    // Top edge
    if (y0 == y1 && x1 > x0) {
        return true;
    }

    // Left edge
    if (y1 > y0) {
        return true;
    }

    return false;
}

// TODO: Take floats instead?
void draw_triangle(struct frame_buffer *frame_buffer, struct vertex *v1,
                   struct vertex *v2, struct vertex *v3) {
    int v1_x = ((v1->x + 1) / 2) * frame_buffer->width;
    int v1_y = ((v1->y + 1) / 2) * frame_buffer->height;
    int v2_x = ((v2->x + 1) / 2) * frame_buffer->width;
    int v2_y = ((v2->y + 1) / 2) * frame_buffer->height;
    int v3_x = ((v3->x + 1) / 2) * frame_buffer->width;
    int v3_y = ((v3->y + 1) / 2) * frame_buffer->height;

    int min_x = v1_x < v2_x ? v1_x : v2_x;
    min_x = v3_x < min_x ? v3_x : min_x;

    int min_y = v1_y < v2_y ? v1_y : v2_y;
    min_y = v3_y < min_y ? v3_y : min_y;

    int max_x = v1_x > v2_x ? v1_x : v2_x;
    max_x = v3_x > max_x ? v3_x : max_x;

    int max_y = v1_y > v2_y ? v1_y : v2_y;
    max_y = v3_y > max_y ? v3_y : max_y;

    int edge1_bias = is_top_left_edge(v3_x, v3_y, v2_x, v2_y) ? 0 : -1;
    int edge2_bias = is_top_left_edge(v1_x, v1_y, v3_x, v3_y) ? 0 : -1;
    int edge3_bias = is_top_left_edge(v2_x, v2_y, v1_x, v1_y) ? 0 : -1;

    float area = (v3_x - v1_x) * (v2_y - v1_y) - (v3_y - v1_y) * (v2_x - v1_x);

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int edge1_distance = (y - v2_y) * (v3_x - v2_x) -
                                 (x - v2_x) * (v3_y - v2_y) + edge1_bias;
            int edge2_distance = (y - v3_y) * (v1_x - v3_x) -
                                 (x - v3_x) * (v1_y - v3_y) + edge2_bias;
            int edge3_distance = (y - v1_y) * (v2_x - v1_x) -
                                 (x - v1_x) * (v2_y - v1_y) + edge3_bias;

            int edge1_x = v3_x - v2_x;
            int edge1_y = v3_y - v2_y;
            int edge2_x = v1_x - v3_x;
            int edge2_y = v1_y - v3_y;
            int edge3_x = v2_x - v1_x;
            int edge3_y = v2_y - v1_y;

            if (edge1_distance >= 0 && edge2_distance >= 0 &&
                edge3_distance >= 0) {
                float b1 =
                    ((x - v2_x) * (v3_y - v2_y) - (y - v2_y) * (v3_x - v2_x)) /
                    area;

                float b2 =
                    ((x - v3_x) * (v1_y - v3_y) - (y - v3_y) * (v1_x - v3_x)) /
                    area;

                float b3 = 1 - b1 - b2;

                int r = (b1 * v1->r + b2 * v2->r + b3 * v3->r) * 255;
                int g = (b1 * v1->g + b2 * v2->g + b3 * v3->g) * 255;
                int b = (b1 * v1->b + b2 * v2->b + b3 * v3->b) * 255;

                frame_buffer_set_pixel(frame_buffer, x, y,
                                       (struct pixel){r, g, b});
            }
        }
    }
}

void draw_triangles(struct frame_buffer *frame_buffer, struct vertex *vertices,
                    unsigned int vertex_count) {
    // printf("%f %f %f\n", vertices[0].x, vertices[0].y, vertices[0].z);
    // printf("%f %f %f\n", vertices[1].x, vertices[1].y, vertices[1].z);
    // printf("%f %f %f\n", vertices[2].x, vertices[2].y, vertices[2].z);
    // draw_triangle(frame_buffer, &vertices[0], &vertices[1], &vertices[2]);
    for (int i = 0; i < vertex_count; i += 3) {
        draw_triangle(frame_buffer, &vertices[i], &vertices[i + 1],
                      &vertices[i + 2]);
    }
}

void draw_triangles_indices(struct frame_buffer *frame_buffer,
                            struct vertex *vertices, unsigned int *indices,
                            unsigned int index_count) {
    for (int i = 0; i < index_count; i += 3) {
        draw_triangle(frame_buffer, &vertices[indices[i]],
                      &vertices[indices[i + 1]], &vertices[indices[i + 2]]);
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

    struct model cube = model_from_obj("res/models/monkey.obj");

    for (int i = 0; i < cube.vertex_count; i++) {
        struct vertex v = cube.vertices[i];
        printf("%f, %f, %f, %f, %f, %f, %f, %f\n", v.x, v.y, v.z, v.texture_x,
               v.texture_y, v.r, v.g, v.b);
    }

    while (!window_should_close(&window)) {
        renderer_clear_buffers();

        window_update_delta_time(&window);
        window_update_input(&window);

        frame_buffer_begin(&frame_buffer);

        draw_triangles(&frame_buffer, cube.vertices, cube.vertex_count);

        frame_buffer_generate(&frame_buffer);
        frame_buffer_draw(&frame_buffer);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    window_shutdown();

    return 0;
}
