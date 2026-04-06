#include "texture.h"

#include "../util/gl.h"

GLenum to_gl_filter(enum texture_filter filter) {
    switch (filter) {
    case TEXTURE_FILTER_LINEAR:
        return GL_LINEAR;
    case TEXTURE_FILTER_NEAREST:
        return GL_NEAREST;
    default:
        fprintf(stderr, "struct texture filter not recognised\n");
        return GL_NEAREST;
    }
}

GLenum to_gl_wrap(enum texture_wrap wrap) {
    switch (wrap) {
    case TEXTURE_WRAP_BORDER:
        return GL_CLAMP_TO_BORDER;
    case TEXTURE_WRAP_REPEAT:
        return GL_REPEAT;
    default:
        fprintf(stderr, "struct texture wrap not recognised\n");
        return GL_CLAMP_TO_BORDER;
    }
}

void texture_init(struct texture *texture, enum texture_filter filter,
                  enum texture_wrap wrap) {
    GL_CALL(glGenTextures(1, &texture->gl_id));
    texture->filter = filter;
    texture->wrap = wrap;
}

void texture_destroy(struct texture *texture) {
    GL_CALL(glDeleteTextures(1, &texture->gl_id));
}

void texture_bind(struct texture *texture) {
    GL_CALL(glActiveTexture(GL_TEXTURE0));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, texture->gl_id));

    GLenum gl_filter = to_gl_filter(texture->filter);
    GLenum gl_wrap = to_gl_wrap(texture->wrap);

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_wrap));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_wrap));
}
