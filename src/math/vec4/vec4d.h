#ifndef VEC4D_H
#define VEC4D_H

#include "cglm/cglm.h"

#define EPSILON 1e-6

struct vec4d {
    double x;
    double y;
    double z;
    double w;
};

#define VEC4D_ZERO (struct vec4d){0.0, 0.0, 0.0, 0.0}

void vec4d_init(struct vec4d *v, double x, double y, double z, double w);
double vec4d_magnitude(struct vec4d v);
void vec4d_normalise(struct vec4d *v);
struct vec4d vec4d_normalised(struct vec4d v);
void vec4d_to_glm(struct vec4d v, vec4 *destination);
void vec4d_add_to(struct vec4d v1, struct vec4d v2, struct vec4d *dest);
struct vec4d vec4d_add(struct vec4d v1, struct vec4d v2);
void vec4d_sub_to(struct vec4d v1, struct vec4d v2, struct vec4d *dest);
struct vec4d vec4d_sub(struct vec4d v1, struct vec4d v2);
void vec4d_scalar_multiply_to(struct vec4d v, double scalar,
                              struct vec4d *dest);
struct vec4d vec4d_scalar_multiply(struct vec4d v, double scalar);
void vec4d_product_to(struct vec4d v1, struct vec4d v2, struct vec4d *dest);
struct vec4d vec4d_product(struct vec4d v1, struct vec4d v2);
double vec4d_dot_product(struct vec4d v1, struct vec4d v2);
bool vec4d_equal(struct vec4d v1, struct vec4d v2);

#endif
