#ifndef CAMERA_H
#define CAMERA_H

#include "../math/vec3.h"
#include "../math/mat4.h"

struct camera {
    struct vec3d position;
    struct vec3d rotation;
    double fov;
    double aspect_ratio;
    double near_plane_distance;
    double far_plane_distance;
    struct mat4d view_matrix;
    struct mat4d projection_matrix;
    bool view_matrix_stale;
    bool projection_matrix_stale;
    struct mat4d view_projection_matrix;
};

void camera_init(struct camera *camera, struct vec3d position,
                 struct vec3d rotation, double fov, double aspect_ratio,
                 double near_plane, double far_plane);
struct vec3d camera_get_direction(struct camera *camera);
void camera_set_position(struct camera *camera, struct vec3d position);
void camera_move(struct camera *camera, struct vec3d movement_delta);
void camera_set_aspect_ratio(struct camera *camera, double aspect_ratio);
void camera_set_rotation(struct camera *camera, struct vec3d rotation);
void camera_set_fov(struct camera *camera, double fov);
void camera_rotate(struct camera *camera, struct vec3d rotation_delta);
void camera_update(struct camera *camera);
struct vec3d camera_project(struct camera *camera, struct vec3d point);

#endif
