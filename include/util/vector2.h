#ifndef _UTIL_VECTOR2_H
#define _UTIL_VECTOR2_H

#include <stddef.h>
#include <math.h>

struct vector2 {
    double x;
    double y;
};


#define VECTOR2_DISTANCE(vector2_a, vector2_b) (struct vector2) {\
    .x = vector2_b.x - vector2_a.x, \
    .y = vector2_b.y - vector2_a.y \
}

#define VECTOR2_MODULE(vector2_a) sqrt(pow(vector2_a.x, 2) + pow(vector2_a.y, 2))

#define VECTOR2_NORMALIZE(vector2_a, module) (struct vector2) {\
    .x = vector2_a.x / module, \
    .y = vector2_a.y / module \
}

#define VECTOR2_ADD(vector2_a, vector2_b) (struct vector2) {\
    .x = vector2_a.x + vector2_b.x, \
    .y = vector2_a.y + vector2_b.y \
}

#define VECTOR2_MULTIPLYD(vector2_a, double_b) (struct vector2) {\
    .x = vector2_a.x * double_b, \
    .y = vector2_a.y * double_b \
};

struct vector2 vector2_init(double x, double y);

char vector2_equals(struct vector2 a, struct vector2 b);

struct vector2 vector2_distance(struct vector2 a, struct vector2 b);

struct vector2 vector2_normalize(struct vector2 a);

double vector2_module(struct vector2 vector);

struct vector2 vector2_multiplyd(struct vector2 a, double b);

struct vector2 vector2_addd(struct vector2 a, double b);

struct vector2 vector2_add(struct vector2 a, struct vector2 b);
#endif