#ifndef _PHYSIC_SIMULATION_H
#define _PHYSIC_SIMULATION_H

#include <stddef.h>
#include "physic/particle.h"
#include "util/vector2.h"

struct random_init_specification {
    unsigned int seed;
    struct vector2 max_position;//exclusive
    struct vector2 min_position;//inclusive
    struct vector2 max_velocity;//exclusive
    struct vector2 min_velocity;//inclusive
    double max_mass;
    double min_mass;
};

void simulation_init_random(struct particle* to_init, size_t length, struct random_init_specification* specification);

/*
Params:
    fixed_time: Should be in seconds.
*/
void simulation_start(struct particle* particles, size_t length, float fixed_time, char* stop);
#endif