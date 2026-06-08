#ifndef MAT4D_H
#define MAT4D_H

#include "../vec4/vec4d.h"

#define MAT4D_ZERO                                                            \
    (struct mat4d) {                                                          \
        {                                                                     \
            {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, {0.0, 0.0, 0.0, 0.0}, \
                {0.0, 0.0, 0.0, 0.0},                                         \
        }                                                                     \
    }

struct mat4d {
    double values[4][4];
};

struct mat4d mat4d_multiply_matrix(struct mat4d *m1, struct mat4d *m2);
struct vec4d mat4d_multiply_vector(struct mat4d *m, struct vec4d *v);
void mat4d_zero(struct mat4d *matrix);
void mat4d_identity(struct mat4d *matrix);

#endif
