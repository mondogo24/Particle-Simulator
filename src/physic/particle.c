#include <stddef.h>
#include <stdlib.h>
#include <math.h>

#include "physic/particle.h"
#include "util/vector2.h"

#define _GRAVITATION_CONSTANT 6.6743e-11

struct particle particle_init(struct vector2 position, struct vector2 velocity, double mass) {
    return (struct particle) {
        .position = position,
        .velocity = velocity,
        .mass = mass
    };
}

char particle_equals(struct particle* a, struct particle* b) {
    return vector2_equals(a->position, b->position) &&
      vector2_equals(a->velocity, b->velocity) &&
      a->mass == b->mass;
}


void particle_gravitation(struct particle* restrict particles, size_t length, float fixed_time) {
    struct particle* restrict next_states = (struct particle*) malloc(sizeof(struct particle) * length);
    for (size_t i = 0; i < length; i++)
        next_states[i] = particles[i];

    for (size_t i = 0; i < length; i++) {
        for (size_t j = 0; j < length; j++) {
            if (i == j)
                continue;
            
            struct vector2 distance = VECTOR2_DISTANCE(particles[i].position, particles[j].position);
            double distance_module = VECTOR2_MODULE(distance);
            if (distance_module == 0.0) //Avoid 0 divisions
                continue;
                        
            double gravitational_force = -_GRAVITATION_CONSTANT * particles[i].mass * particles[j].mass / pow(distance_module, 2);
            double acceleration = gravitational_force / particles[i].mass;

            struct vector2 direction = VECTOR2_NORMALIZE(distance, distance_module);
            
            struct vector2 next_velocity = VECTOR2_MULTIPLYD(direction, acceleration * fixed_time);
            next_states[i].velocity = VECTOR2_ADD(next_states[i].velocity, next_velocity);

            struct vector2 position_vector = VECTOR2_MULTIPLYD(next_states[i].velocity, fixed_time);
            next_states[i].position = VECTOR2_ADD(next_states[i].position, position_vector);
        }
    }

    for (size_t i = 0; i < length; i++)
        particles[i] = next_states[i];

    free(next_states);
}