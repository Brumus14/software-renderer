#include "camera.h"

#include "../math/math_util.h"

void generate_view_matrix(struct camera *camera) {
    struct vec3d forward = camera_get_direction(camera);
    struct vec3d world_up = (struct vec3d){0, 1, 0};
    struct vec3d right =
        vec3d_normalised(vec3d_cross_product(forward, world_up));
    struct vec3d up = vec3d_cross_product(right, forward);

    camera->view_matrix = (struct mat4d){{
        {right.x, right.y, right.z,
         -vec3d_dot_product(right, camera->position)},
        {up.x, up.y, up.z, -vec3d_dot_product(up, camera->position)},
        {forward.x, forward.y, forward.z,
         -vec3d_dot_product(forward, camera->position)},
        {0, 0, 0, 1},
    }};
}

void generate_projection_matrix(struct camera *camera) {
    camera->projection_matrix = (struct mat4d){{
        {
            1 / (tan(radians(camera->fov / 2)) * camera->aspect_ratio),
            0,
            0,
            0,
        },
        {
            0,
            1 / tan(radians(camera->fov / 2)),
            0,
            0,
        },
        {
            0,
            0,
            (camera->near_plane_distance + camera->far_plane_distance) /
                (camera->far_plane_distance - camera->near_plane_distance),
            (2 * camera->far_plane_distance * camera->near_plane_distance) /
                (camera->far_plane_distance - camera->near_plane_distance),
        },
        {
            0,
            0,
            1,
            0,
        },
    }};
}

void camera_init(struct camera *camera, struct vec3d position,
                 struct vec3d rotation, double fov, double aspect_ratio,
                 double near_plane_distance, double far_plane_distance) {
    camera->position = position;
    camera->rotation = rotation;
    camera->fov = fov;
    camera->aspect_ratio = aspect_ratio;
    camera->near_plane_distance = near_plane_distance;
    camera->far_plane_distance = far_plane_distance;
    camera->view_matrix_stale = true;
    camera->projection_matrix_stale = true;
}

struct vec3d camera_get_direction(struct camera *camera) {
    return rotation_to_direction(camera->rotation);
}

void camera_set_position(struct camera *camera, struct vec3d position) {
    camera->position = position;
    camera->view_matrix_stale = true;
}

void camera_move(struct camera *camera, struct vec3d movement_delta) {
    struct vec3d position_delta = VEC3D_ZERO;

    struct vec3d forwards = camera_get_direction(camera);
    forwards.y = 0.0;
    vec3d_normalise(&forwards);

    position_delta = vec3d_add(
        position_delta, vec3d_scalar_multiply(forwards, -movement_delta.z));

    struct vec3d up;
    vec3d_init(&up, 0.0, 1.0, 0.0);

    position_delta =
        vec3d_add(position_delta,
                  vec3d_scalar_multiply(vec3d_normalised(vec3d_cross_product(
                                            camera_get_direction(camera), up)),
                                        movement_delta.x));

    position_delta.y += movement_delta.y;

    camera->position = vec3d_add(camera->position, position_delta);
    vec3d_print(camera->position);
    putchar('\n');
    camera->view_matrix_stale = true;
}

void camera_set_aspect_ratio(struct camera *camera, double aspect_ratio) {
    camera->aspect_ratio = aspect_ratio;
    camera->projection_matrix_stale = true;
}

void camera_set_rotation(struct camera *camera, struct vec3d rotation) {
    camera->rotation = rotation;
    camera->view_matrix_stale = true;
}

void camera_set_fov(struct camera *camera, double fov) {
    camera->fov = fov;
    camera->view_matrix_stale = true;
}

void camera_rotate(struct camera *camera, struct vec3d rotation_delta) {
    struct vec3d new_rotation = vec3d_add(camera->rotation, rotation_delta);
    camera_set_rotation(camera, new_rotation);
}

void camera_update(struct camera *camera) {
    bool view_projection_matrix_stale = false;

    if (camera->view_matrix_stale) {
        generate_view_matrix(camera);
        camera->view_matrix_stale = false;
        view_projection_matrix_stale = true;
    }

    if (camera->projection_matrix_stale) {
        generate_projection_matrix(camera);
        camera->projection_matrix_stale = false;
        view_projection_matrix_stale = true;
    }

    if (view_projection_matrix_stale) {
        camera->view_projection_matrix = mat4d_multiply_matrix(
            &camera->projection_matrix, &camera->view_matrix);
    }
}

struct vec3d camera_project(struct camera *camera, struct vec3d point) {
    struct vec4d vector =
        mat4d_multiply_vector(&camera->view_projection_matrix,
                              &(struct vec4d){point.x, point.y, point.z, 1.0});

    return (struct vec3d){vector.x / vector.w, vector.y / vector.w,
                          vector.z / vector.w};
}
