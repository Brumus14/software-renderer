#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include "bo.h"
#include "shader_program.h"
#include "texture.h"
#include "vao.h"

struct pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct frame_buffer {
    unsigned int width;
    unsigned int height;
    struct vao vao;
    struct bo vbo;
    struct bo ibo;
    struct texture texture;
    struct shader_program program;
};

void frame_buffer_init(struct frame_buffer *frame_buffer);
void frame_buffer_generate(struct frame_buffer *frame_buffer,
                           struct pixel *data);
void frame_buffer_draw(struct frame_buffer *frame_buffer);

#endif
