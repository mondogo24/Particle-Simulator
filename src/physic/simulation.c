#include <stdlib.h>
#include <stddef.h>

#include "physic/simulation.h"
#include "physic/particle.h"
#include "particle_operation.h"

static double rand_from(double min_inclusive, double max_exclusive);

void simulation_init_random(struct particle* to_init, size_t length, struct random_init_specification* specification) {
    srand(specification->seed);
    for (size_t i = 0; i < length; i++) {
        to_init[i] = particle_init(
            vector2_init(
                rand_from(specification->min_position.x, specification->max_position.x),
                rand_from(specification->min_position.y, specification->max_position.y)
            ),
            vector2_init(
                rand_from(specification->min_velocity.x, specification->max_velocity.x),
                rand_from(specification->min_velocity.y, specification->max_velocity.y)
            ),
            rand_from(specification->min_mass, specification->max_mass)
        );
    }
}

static double rand_from(double min_inclusive, double max_exclusive) {
    // Source - https://stackoverflow.com/a/33059025
    // Posted by cp.engr, modified by community. See post 'Timeline' for change history
    // Retrieved 2026-10-06, License - CC BY-SA 3.0

    double range = (max_exclusive - min_inclusive); 
    double div = RAND_MAX / range;
    return min_inclusive + (rand() / div);
}

void simulation_start(struct particle* particles, size_t length, float fixed_time, char* stop) {
    while (!*stop) {
        particle_gravitation(particles, length, fixed_time);
    }
}