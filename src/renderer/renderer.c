#include "renderer.h"

#include "frame_buffer.h"
#include <stdlib.h>

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
        frame_buffer_set_pixel(frame_buffer, x, y, 0,
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
        frame_buffer_set_pixel(frame_buffer, x, y, 0,
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
void renderer_draw_triangle(struct frame_buffer *frame_buffer,
                            struct vertex *v1, struct vertex *v2,
                            struct vertex *v3) {
    int v1_x = ((v1->x + 1) / 2) * frame_buffer->width;
    int v1_y = ((v1->y + 1) / 2) * frame_buffer->height;
    int v2_x = ((v2->x + 1) / 2) * frame_buffer->width;
    int v2_y = ((v2->y + 1) / 2) * frame_buffer->height;
    int v3_x = ((v3->x + 1) / 2) * frame_buffer->width;
    int v3_y = ((v3->y + 1) / 2) * frame_buffer->height;

    int min_x = v1_x < v2_x ? v1_x : v2_x;
    min_x = v3_x < min_x ? v3_x : min_x;
    min_x = min_x < 0 ? 0 : min_x;

    int min_y = v1_y < v2_y ? v1_y : v2_y;
    min_y = v3_y < min_y ? v3_y : min_y;
    min_y = min_y < 0 ? 0 : min_y;

    int max_x = v1_x > v2_x ? v1_x : v2_x;
    max_x = v3_x > max_x ? v3_x : max_x;
    max_x = max_x >= frame_buffer->width ? frame_buffer->width - 1 : max_x;

    int max_y = v1_y > v2_y ? v1_y : v2_y;
    max_y = v3_y > max_y ? v3_y : max_y;
    max_y = max_y >= frame_buffer->height ? frame_buffer->height - 1 : max_y;

    int edge1_bias = is_top_left_edge(v3_x, v3_y, v2_x, v2_y) ? 0 : -1;
    int edge2_bias = is_top_left_edge(v1_x, v1_y, v3_x, v3_y) ? 0 : -1;
    int edge3_bias = is_top_left_edge(v2_x, v2_y, v1_x, v1_y) ? 0 : -1;

    float area = (v3_x - v1_x) * (v2_y - v1_y) - (v3_y - v1_y) * (v2_x - v1_x);
    float inverse_area = 1 / area;

    int edge1_x = v3_x - v2_x;
    int edge1_y = v3_y - v2_y;
    int edge2_x = v1_x - v3_x;
    int edge2_y = v1_y - v3_y;
    int edge3_x = v2_x - v1_x;
    int edge3_y = v2_y - v1_y;

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            int edge1_distance = (y - v2_y) * (v3_x - v2_x) -
                                 (x - v2_x) * (v3_y - v2_y) + edge1_bias;
            int edge2_distance = (y - v3_y) * (v1_x - v3_x) -
                                 (x - v3_x) * (v1_y - v3_y) + edge2_bias;
            int edge3_distance = (y - v1_y) * (v2_x - v1_x) -
                                 (x - v1_x) * (v2_y - v1_y) + edge3_bias;

            // Clockwise:
            // (edge1_distance <= 0 && edge2_distance <= 0 &&
            //  edge3_distance <= 0)

            if (edge1_distance >= 0 && edge2_distance >= 0 &&
                edge3_distance >= 0) {
                float b1 =
                    ((x - v2_x) * (v3_y - v2_y) - (y - v2_y) * (v3_x - v2_x)) *
                    inverse_area;

                float b2 =
                    ((x - v3_x) * (v1_y - v3_y) - (y - v3_y) * (v1_x - v3_x)) *
                    inverse_area;

                float b3 = 1 - b1 - b2;

                int r = (b1 * v1->r + b2 * v2->r + b3 * v3->r) * 255;
                int g = (b1 * v1->g + b2 * v2->g + b3 * v3->g) * 255;
                int b = (b1 * v1->b + b2 * v2->b + b3 * v3->b) * 255;

                float z = b1 * v1->z + b2 * v2->z + b3 * v3->z;
                float depth = -z;

                frame_buffer_set_pixel(frame_buffer, x, y, depth,
                                       (struct pixel){r, g, b});
            }
        }
    }
}

void renderer_draw_triangles(struct frame_buffer *frame_buffer,
                             struct vertex *vertices,
                             unsigned int vertex_count) {
    for (int i = 0; i < vertex_count; i += 3) {
        renderer_draw_triangle(frame_buffer, &vertices[i], &vertices[i + 1],
                               &vertices[i + 2]);
    }
}

void renderer_draw_triangles_indices(struct frame_buffer *frame_buffer,
                                     struct vertex *vertices,
                                     unsigned int *indices,
                                     unsigned int index_count) {
    for (int i = 0; i < index_count; i += 3) {
        renderer_draw_triangle(frame_buffer, &vertices[indices[i]],
                               &vertices[indices[i + 1]],
                               &vertices[indices[i + 2]]);
    }
}
