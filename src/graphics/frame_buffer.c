#include "frame_buffer.h"

#include "renderer.h"
#include "vao.h"
#include "../util/gl.h"
#include <stdlib.h>

// clang-format off
const float vertices[] = {
    -1.0, -1.0, 0.0, 0.0,
     1.0, -1.0, 1.0, 0.0,
     1.0,  1.0, 1.0, 1.0,
    -1.0,  1.0, 0.0, 1.0,
};

const unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3,
};
// clang-format on

// Pass in width and height?
void frame_buffer_init(struct frame_buffer *frame_buffer, unsigned int width,
                       unsigned int height) {
    frame_buffer->width = width;
    frame_buffer->height = height;
    frame_buffer->resized = false;
    frame_buffer->new_width = 0;
    frame_buffer->new_height = 0;
    frame_buffer->pixels = malloc(frame_buffer->height * frame_buffer->width *
                                  sizeof(struct pixel));
    frame_buffer->clear_colour = (struct pixel){0, 0, 0};

    vao_init(&frame_buffer->vao);
    vao_bind(&frame_buffer->vao);

    bo_init(&frame_buffer->vbo, BO_TYPE_VERTEX);
    bo_upload(&frame_buffer->vbo, sizeof(vertices), vertices,
              BO_USAGE_STATIC_DRAW);

    bo_init(&frame_buffer->ibo, BO_TYPE_INDEX);
    bo_upload(&frame_buffer->ibo, sizeof(indices), indices,
              BO_USAGE_STATIC_DRAW);

    vao_attrib(&frame_buffer->vao, 0, 2, VAO_TYPE_FLOAT, false,
               4 * sizeof(float), 0);
    vao_attrib(&frame_buffer->vao, 1, 2, VAO_TYPE_FLOAT, false,
               4 * sizeof(float), (void *)(2 * sizeof(float)));

    shader_program_from_files(&frame_buffer->program,
                              "res/shaders/framebuffer.vert",
                              "res/shaders/framebuffer.frag");
    shader_program_link(&frame_buffer->program);

    texture_init(&frame_buffer->texture, TEXTURE_FILTER_NEAREST,
                 TEXTURE_WRAP_BORDER);
}

void frame_buffer_begin(struct frame_buffer *frame_buffer) {
    if (frame_buffer->resized) {
        frame_buffer->width = frame_buffer->new_width;
        frame_buffer->height = frame_buffer->new_height;

        frame_buffer->pixels = realloc(
            frame_buffer->pixels,
            frame_buffer->height * frame_buffer->width * sizeof(struct pixel));

        frame_buffer->resized = false;
    }

    for (int i = 0; i < frame_buffer->height * frame_buffer->width; i++) {
        frame_buffer->pixels[i] = frame_buffer->clear_colour;
    }
}

void frame_buffer_generate(struct frame_buffer *frame_buffer) {
    texture_bind(&frame_buffer->texture);
    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_buffer->width,
                         frame_buffer->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         frame_buffer->pixels));
}

void frame_buffer_draw(struct frame_buffer *frame_buffer) {
    shader_program_use(&frame_buffer->program);
    vao_bind(&frame_buffer->vao);
    texture_bind(&frame_buffer->texture);

    renderer_draw_elements(DRAW_MODE_TRIANGLES, 6, INDEX_TYPE_UNSIGNED_INT);
}

bool frame_buffer_get_pixel(struct frame_buffer *frame_buffer, unsigned int x,
                            unsigned int y, struct pixel *result) {
    if (x >= frame_buffer->width || y >= frame_buffer->height) {
        return false;
    }

    *result = frame_buffer->pixels[x + y * frame_buffer->width];
    return true;
}

bool frame_buffer_set_pixel(struct frame_buffer *frame_buffer, unsigned int x,
                            unsigned int y, struct pixel value) {
    if (x >= frame_buffer->width || y >= frame_buffer->height) {
        return false;
    }

    frame_buffer->pixels[x + y * frame_buffer->width] = value;
    return true;
}
