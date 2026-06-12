#include "quat.h"

void quat_multiply_to(struct quat q1, struct quat q2, struct quat *dest) {
    dest->a = q1.a * q2.a - q1.b * q2.b - q1.c * q2.c - q1.d * q2.d;
    dest->b = q1.a * q2.b + q1.b * q2.a + q1.c * q2.d - q1.d * q2.c;
    dest->c = q1.a * q2.c - q1.b * q2.d + q1.c * q2.a + q1.d * q2.b;
    dest->d = q1.a * q2.d + q1.b * q2.c - q1.c * q2.b + q1.d * q2.a;
}

struct quat quat_multiply(struct quat q1, struct quat q2) {
    struct quat result;
    quat_multiply_to(q1, q2, &result);
    return result;
}

struct quat quat_conjugate(struct quat q) {
    return (struct quat){q.a, -q.b, -q.c, -q.d};
}

struct vec3d quat_conjugation_vec3(struct quat q, struct vec3d p) {
    struct quat result = quat_multiply(q, (struct quat){0, p.x, p.y, p.z});
    quat_multiply_to(result, quat_conjugate(q), &result);
    return (struct vec3d){result.b, result.c, result.d};
}

void quat_normalise(struct quat *q) {
    double magnitude =
        sqrt(pow(q->a, 2) + pow(q->b, 2) + pow(q->c, 2) + pow(q->d, 2));

    if (magnitude > EPSILON) {
        q->a /= magnitude;
        q->b /= magnitude;
        q->c /= magnitude;
        q->d /= magnitude;
    }
}

struct quat quat_normalised(struct quat q) {
    quat_normalise(&q);
    return q;
}
