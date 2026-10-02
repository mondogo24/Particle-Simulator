#include <stddef.h>
#include <math.h>
#include <assert.h>

#include "test/test_util.h"
#include "physic/particle.h"
#include "util/vector2.h"

#define _FIXED_TIME 0.1f
#define _GRAVITATION_CONSTANT 6.6743e-11

static void init_basic_samples(struct particle* samples, size_t length);
static void init_normal_gravitational_particles(struct particle* samples, struct particle* expected, size_t length);
static void init_zero_division_gravitational(struct particle* samples, struct particle* expected, size_t length);

static void test_particle_gravitation_on_normal_conditions_should_calculate_values_correctly(
  struct particle* samples, 
  struct particle* expecteds,
  size_t length,
  float fixed_time
);
static void test_particle_gravitation_on_zero_divison_should_cancel_operation(
  struct particle* samples,
  struct particle* expecteds,
  size_t length,
  float fixed_time
);

int main() {
    size_t length = 2;
    struct particle samples[length];
    struct particle expecteds[length];

    init_normal_gravitational_particles(samples, expecteds, length);
    test_particle_gravitation_on_normal_conditions_should_calculate_values_correctly(
      samples, expecteds, length, _FIXED_TIME
    );

    init_zero_division_gravitational(samples, expecteds, length);
    test_particle_gravitation_on_zero_divison_should_cancel_operation(
      samples, expecteds, length, _FIXED_TIME
    );

    FINALIZE_TESTING;
}

static void test_particle_gravitation_on_normal_conditions_should_calculate_values_correctly(
  struct particle* samples, 
  struct particle* expecteds,
  size_t length,
  float fixed_time
) {
    for (size_t i = 0; i < length; i++) {
        particle_gravitation(samples, length, fixed_time);

        ASSERT(vector2_equals(samples[i].position, expecteds[i].position),
          "Positions weren't correctly setted.");
        ASSERT(vector2_equals(samples[i].velocity, expecteds[i].velocity),
          "Velocities weren't correctly setted.");
        ASSERT(particle_equals(&samples[i], &expecteds[i]),
          "Something else changed when it shoundn't");
    }
}

static void test_particle_gravitation_on_zero_divison_should_cancel_operation(
  struct particle* samples,
  struct particle* expecteds,
  size_t length,
  float fixed_time
) {
    for (size_t i = 0; i < length; i++) {
        ASSERT(particle_equals(&samples[i], &expecteds[i]),
          "Particles's state changed");
    }
}

static void init_basic_samples(struct particle* samples, size_t length) {
    assert(length == 2 && "Length for init must be 2");

    samples[0] = particle_init(
      vector2_init(0.0, 0.0),
      vector2_init(0.0, 0.0),
      10e10
    );
    samples[1] = particle_init(
      vector2_init(5.0, 5.0),
      vector2_init(0.0, 0.0),
      10e5
    );
}

static void init_normal_gravitational_particles(struct particle* samples, struct particle* expected, size_t length) {
    assert(length == 2 && "Length for init must be 2");

    init_basic_samples(samples, length);

    struct vector2 direction_0 = vector2_distance(samples[0].position, samples[1].position);
    double distance_0 = vector2_module(direction_0);
    double acceleration_0 = (-_GRAVITATION_CONSTANT * samples[0].mass * samples[1].mass / pow(distance_0 , 2)) /
      samples[0].mass;
    struct vector2 normalized_direction_0 = vector2_normalize(direction_0);
    struct vector2 velocity_0 = vector2_multiplyd(normalized_direction_0, acceleration_0 * _FIXED_TIME);
    struct vector2 position_0 = vector2_add(samples[0].position, vector2_multiplyd(velocity_0, _FIXED_TIME));

    struct vector2 direction_1 = vector2_distance(samples[1].position, samples[0].position);
    double distance_1 = vector2_module(direction_1);
    double acceleration_1 = (-_GRAVITATION_CONSTANT * samples[0].mass * samples[1].mass / pow(distance_1 , 2)) /
      samples[1].mass;
    struct vector2 normalized_direction_1 = vector2_normalize(direction_1);
    struct vector2 velocity_1 = vector2_multiplyd(normalized_direction_1, acceleration_1 * _FIXED_TIME);
    struct vector2 position_1 = vector2_add(samples[1].position, vector2_multiplyd(velocity_1, _FIXED_TIME));

    expected[0] = particle_init(
      position_0,
      velocity_0,
      10e10
    );
    expected[1] = particle_init(
      position_1,
      velocity_1,
      10e5
    );
}

static void init_zero_division_gravitational(struct particle* samples, struct particle* expected, size_t length) {
    assert(length == 2 && "Length for init must be 2");
    init_basic_samples(samples, length);

    expected[0] = samples[0];
    expected[1] = samples[1];
}