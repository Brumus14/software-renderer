#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "bo.h"
#include "shader_program.h"
#include "texture.h"
#include "vao.h"
#include <stdbool.h>

struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct frame_buffer {
    unsigned int width;
    unsigned int height;
    bool resized;
    unsigned int new_width;
    unsigned int new_height;
    struct pixel *pixels;
    struct pixel clear_colour;
    struct vao vao;
    struct bo vbo;
    struct bo ibo;
    struct texture texture;
    struct shader_program program;
};

void frame_buffer_init(struct frame_buffer *frame_buffer, unsigned int width,
                       unsigned int height);
void frame_buffer_begin(struct frame_buffer *frame_buffer);
void frame_buffer_generate(struct frame_buffer *frame_buffer);
void frame_buffer_draw(struct frame_buffer *frame_buffer);
bool frame_buffer_get_pixel(struct frame_buffer *frame_buffer, unsigned int x,
                            unsigned int y, struct pixel *result);
bool frame_buffer_set_pixel(struct frame_buffer *frame_buffer, unsigned int x,
                            unsigned int y, struct pixel value);

#endif
