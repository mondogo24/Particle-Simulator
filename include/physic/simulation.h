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

/*
Randomly initialize a particle array based on some specifications.

Params:
    to_init: The particle array to be initialized.
    length: The particle array's length.
    specification: The specifications of how the initialization must proceed.
*/
void simulation_init_random(struct particle* to_init, size_t length, struct random_init_specification* specification);

/*
Starts a simulation over a pre-initialized particle array. Only gravitational pulls/operations are simulated.

Params:
    particles: The particle array that is going to be operated on.
    length: the particle array's length.
    fixed_time: The physical time on which operations are going to be executed. Should be in seconds.
    stop: If the dereference value is: True; the simulation loop will stop executing, False; The simulation
        loop will continue.
*/
void simulation_start(struct particle* particles, size_t length, float fixed_time, char* stop);
#endif