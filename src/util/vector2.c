#include "util/vector2.h"

struct vector2 vector2_init(double x, double y) {
    return (struct vector2) {
        .x = x,
        .y = y
    };
}

char vector2_equals(struct vector2 a, struct vector2 b) {
    return a.x == b.x && a.y == b.y;
}

struct vector2 vector2_distance(const struct vector2 a, const struct vector2 b) {
    return (struct vector2) {
        .x = b.x - a.x,
        .y = b.y - a.y
    };
}

struct vector2 vector2_normalize(struct vector2 a) {
    double module = vector2_module(a);
    return (struct vector2) {
        .x = a.x / module,
        .y = a.y / module
    };
}

double vector2_module(struct vector2 vector) {
    return sqrt(pow(vector.x, 2) + pow(vector.y, 2));
}

struct vector2 vector2_multiplyd(struct vector2 a, double b) {
    return (struct vector2) {
        .x = a.x * b,
        .y = a.y * b
    };
}

struct vector2 vector2_addd(struct vector2 a, double b) {
    return (struct vector2) {
        .x = a.x + b,
        .y = a.y + b
    };
}

struct vector2 vector2_add(struct vector2 a, struct vector2 b) {
    return (struct vector2) {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
}