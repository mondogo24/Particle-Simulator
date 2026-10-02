#ifndef _PHYSIC_SIMULATION_H
#define _PHYSIC_SIMULATION_H

#include <stddef.h>
#include "physic/particle.h"

/*
Params:
    fixed_time: Should be in seconds.
*/
void simulation_start(struct particle* restrict init_particles, size_t length, float fixed_time, char* stop);
#endif