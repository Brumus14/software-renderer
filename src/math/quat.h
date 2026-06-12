#ifndef QUAT_H
#define QUAT_H

#include "vec3/vec3d.h"

struct quat {
    double a;
    double b;
    double c;
    double d;
};

void quat_multiply_to(struct quat q1, struct quat q2, struct quat *dest);
struct quat quat_multiply(struct quat q1, struct quat q2);
struct quat quat_conjugate(struct quat q);
struct vec3d quat_conjugation_vec3(struct quat q, struct vec3d p);
void quat_normalise(struct quat *q);
struct quat quat_normalised(struct quat q);

#endif
