#include "physic/simulation.h"
#include "physic/particle.h"


void simulation_start(struct particle* restrict init_particles, size_t length, float fixed_time, char* stop) {
    while (!*stop) {
        particle_gravitation(init_particles, length, fixed_time);
    }
}