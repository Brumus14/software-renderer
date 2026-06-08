#ifndef MODEL_H
#define MODEL_H

#include "vertex.h"
#include "renderer.h"
#include "camera.h"

struct vertex_coordinate {
    float x;
    float y;
    float z;
};

struct vertex_texture_coordinate {
    float x;
    float y;
};

struct vertex_normal {
    float x;
    float y;
    float z;
};

struct face_element {
    unsigned int coordinate_index;
    unsigned int texture_coordinate_index;
    unsigned int normal_index;
};

struct face {
    struct face_element *face_elements;
    unsigned int face_element_count;
};

struct model {
    struct vertex *vertices;
    unsigned int vertex_count;
    struct vertex *projected_vertices;
};

struct model model_from_obj(const char *path);
void model_draw(struct model *model, struct camera *camera,
                struct frame_buffer *frame_buffer);
void model_destroy(struct model *model);

#endif
