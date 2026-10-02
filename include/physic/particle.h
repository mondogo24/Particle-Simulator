#ifndef _PHYSIC_PARTICLE_H
#define _PHYSIC_PARTICLE_H

#include <stddef.h>

#include "util/vector2.h"

struct particle {
    struct vector2 position;
    struct vector2 velocity;
    double mass;
};


struct particle particle_init(struct vector2 position, struct vector2 velocity, double mass);

char particle_equals(struct particle* a, struct particle* b);

void particle_gravitation(struct particle* restrict particles, size_t length, float fixed_time);

#endif