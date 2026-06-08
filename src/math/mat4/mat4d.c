#include "mat4d.h"

struct mat4d mat4d_multiply_matrix(struct mat4d *m1, struct mat4d *m2) {
    struct mat4d result = MAT4D_ZERO;

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            for (int i = 0; i < 4; i++) {
                result.values[r][c] += m1->values[r][i] * m2->values[i][c];
            }
        }
    }

    return result;
}

struct vec4d mat4d_multiply_vector(struct mat4d *m, struct vec4d *v) {
    struct vec4d result;

    result.x = m->values[0][0] * v->x + m->values[0][1] * v->y +
               m->values[0][2] * v->z + m->values[0][3] * v->w;
    result.y = m->values[1][0] * v->x + m->values[1][1] * v->y +
               m->values[1][2] * v->z + m->values[1][3] * v->w;
    result.z = m->values[2][0] * v->x + m->values[2][1] * v->y +
               m->values[2][2] * v->z + m->values[2][3] * v->w;
    result.w = m->values[3][0] * v->x + m->values[3][1] * v->y +
               m->values[3][2] * v->z + m->values[3][3] * v->w;

    return result;
}

void mat4d_zero(struct mat4d *matrix) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            matrix->values[r][c] = 0;
        }
    }
}

void mat4d_identity(struct mat4d *matrix) {
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == c) {
                matrix->values[r][c] = 1;
            } else {
                matrix->values[r][c] = 0;
            }
        }
    }
}
