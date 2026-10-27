#ifndef _PHYSIC_PARTICLE_OPERATION_H
#define _PHYSIC_PARTICLE_OPERATION_H
#include "physic/particle.h"

void particle_gravitation(struct particle* restrict particles, size_t length, float fixed_time);
#endif