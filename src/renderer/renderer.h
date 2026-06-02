#ifndef RENDERER_H
#define RENDERER_H

#include "frame_buffer.h"
#include "vertex.h"

void renderer_draw_triangle(struct frame_buffer *frame_buffer,
                            struct vertex *v1, struct vertex *v2,
                            struct vertex *v3);
void renderer_draw_triangles(struct frame_buffer *frame_buffer,
                             struct vertex *vertices,
                             unsigned int vertex_count);
void renderer_draw_triangles_indices(struct frame_buffer *frame_buffer,
                                     struct vertex *vertices,
                                     unsigned int *indices,
                                     unsigned int index_count);

#endif
