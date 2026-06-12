#include "vec4d.h"

#include "math.h"

void vec4d_init(struct vec4d *v, double x, double y, double z, double w) {
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;
}

double vec4d_magnitude(struct vec4d v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2) + pow(v.w, 2));
}

void vec4d_normalise(struct vec4d *v) {
    double magnitude = vec4d_magnitude(*v);

    if (magnitude > EPSILON) {
        v->x /= magnitude;
        v->y /= magnitude;
        v->z /= magnitude;
        v->w /= magnitude;
    }
}

struct vec4d vec4d_normalised(struct vec4d v) {
    struct vec4d result = v;
    vec4d_normalise(&result);
    return result;
}

void vec4d_to_glm(struct vec4d v, vec4 *destination) {
    (*destination)[0] = v.x;
    (*destination)[1] = v.y;
    (*destination)[2] = v.z;
    (*destination)[3] = v.w;
}

void vec4d_add_to(struct vec4d v1, struct vec4d v2, struct vec4d *dest) {
    dest->x = v1.x + v2.x;
    dest->y = v1.y + v2.y;
    dest->z = v1.z + v2.z;
    dest->w = v1.w + v2.w;
}

struct vec4d vec4d_add(struct vec4d v1, struct vec4d v2) {
    struct vec4d result;
    vec4d_add_to(v1, v2, &result);
    return result;
}

void vec4d_sub_to(struct vec4d v1, struct vec4d v2, struct vec4d *dest) {
    dest->x = v1.x - v2.x;
    dest->y = v1.y - v2.y;
    dest->z = v1.z - v2.z;
    dest->w = v1.w - v2.w;
}

struct vec4d vec4d_sub(struct vec4d v1, struct vec4d v2) {
    struct vec4d result;
    vec4d_sub_to(v1, v2, &result);
    return result;
}

void vec4d_scalar_multiply_to(struct vec4d v, double scalar,
                              struct vec4d *dest) {
    dest->x = v.x * scalar;
    dest->y = v.y * scalar;
    dest->z = v.z * scalar;
    dest->w = v.w * scalar;
}

struct vec4d vec4d_scalar_multiply(struct vec4d v, double scalar) {
    struct vec4d result;
    vec4d_scalar_multiply_to(v, scalar, &result);
    return result;
}

void vec4d_product_to(struct vec4d v1, struct vec4d v2, struct vec4d *dest) {
    dest->x = v1.x * v2.x;
    dest->y = v1.y * v2.y;
    dest->z = v1.z * v2.z;
    dest->w = v1.w * v2.w;
}

struct vec4d vec4d_product(struct vec4d v1, struct vec4d v2) {
    struct vec4d result;
    vec4d_product_to(v1, v2, &result);
    return result;
}

double vec4d_dot_product(struct vec4d v1, struct vec4d v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

bool vec4d_equal(struct vec4d v1, struct vec4d v2) {
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w) {
        return true;
    } else {
        return false;
    }
}
