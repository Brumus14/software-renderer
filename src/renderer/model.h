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
    struct vec3d position;
    struct vec3d rotation;
    struct vertex *vertices;
    unsigned int vertex_count;
    struct vertex *projected_vertices;
    struct mat4d model_matrix;
    bool model_matrix_stale;
};

struct model model_from_obj(const char *path, struct vec3d position,
                            struct vec3d rotation);
void model_destroy(struct model *model);
void model_draw(struct model *model, struct camera *camera,
                struct frame_buffer *frame_buffer);
struct vec3d model_get_direction(struct model *model);
void model_set_position(struct model *model, struct vec3d position);
void model_move(struct model *model, struct vec3d movement_delta);
void model_set_rotation(struct model *model, struct vec3d rotation);
void model_rotate(struct model *model, struct vec3d rotation_delta);

#endif
