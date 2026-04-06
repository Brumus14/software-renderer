#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"

enum texture_filter {
    TEXTURE_FILTER_LINEAR = 0,
    TEXTURE_FILTER_NEAREST,
};

enum texture_wrap {
    TEXTURE_WRAP_BORDER = 0,
    TEXTURE_WRAP_REPEAT,
};

struct texture {
    GLuint gl_id;
    enum texture_filter filter;
    enum texture_wrap wrap;
    int size_x;
    int size_y;
};

void texture_init(struct texture *texture, enum texture_filter filter,
                  enum texture_wrap wrap);
void texture_destroy(struct texture *texture);
void texture_bind(struct texture *texture);

#endif
