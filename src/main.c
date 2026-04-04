#include <stdio.h>
#include "graphics/bo.h"
#include "graphics/renderer.h"
#include "graphics/shader.h"
#include "graphics/shader_program.h"
#include "graphics/vao.h"
#include "graphics/window.h"
#include "util/gl.h"

int main() {
    struct window window;
    window_init(&window, 800, 600, "Software Renderer!");
    window_reset_cursor(&window);

    renderer_init();

    renderer_set_viewport(0, 0, 800, 600);

    float vertices[] = {
        -1.0, -1.0, 0.0, 0.0, //
        1.0,  -1.0, 1.0, 0.0, //
        1.0,  1.0,  1.0, 1.0, //
        -1.0, 1.0,  0.0, 1.0, //
    };

    unsigned int indices[] = {
        0, 1, 2, //
        0, 2, 3, //
    };

    struct vao vao;
    vao_init(&vao);
    vao_bind(&vao);

    struct bo vbo;
    bo_init(&vbo, BO_TYPE_VERTEX);
    bo_upload(&vbo, sizeof(vertices), vertices, BO_USAGE_STATIC_DRAW);

    struct bo ibo;
    bo_init(&ibo, BO_TYPE_INDEX);
    bo_upload(&ibo, sizeof(indices), indices, BO_USAGE_STATIC_DRAW);

    vao_attrib(&vao, 0, 2, VAO_TYPE_FLOAT, false, 4 * sizeof(float), 0);
    vao_attrib(&vao, 1, 2, VAO_TYPE_FLOAT, false, 4 * sizeof(float),
               (void *)(2 * sizeof(float)));

    struct shader_program program;
    shader_program_from_files(&program, "res/shaders/framebuffer.vert",
                              "res/shaders/framebuffer.frag");
    shader_program_link(&program);

    while (!window_should_close(&window)) {
        renderer_clear_buffers();

        window_update_delta_time(&window);
        window_update_input(&window);

        unsigned int texture;
        GL_CALL(glGenTextures(1, &texture));
        GL_CALL(glBindTexture(GL_TEXTURE_2D, texture));

        unsigned char framebuffer[800 * 600 * 3];

        for (int i = 0; i < sizeof(framebuffer); i++) {
            framebuffer[i] = 100;
        }

        GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB,
                             GL_UNSIGNED_BYTE, framebuffer));

        shader_program_use(&program);
        vao_bind(&vao);

        renderer_draw_elements(DRAW_MODE_TRIANGLES, 6, INDEX_TYPE_UNSIGNED_INT);

        window_swap_buffers(&window);
    }

    window_destroy(&window);

    window_shutdown();

    return 0;
}
