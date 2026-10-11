add_executable(physic_particle_test)
target_sources(physic_particle_test PRIVATE 
    src/physic/particle.test.c
        
    PUBLIC FILE_SET HEADERS
    BASE_DIRS test_include
    FILES test_include/test/test_util.h)
target_link_libraries(physic_particle_test core_fast_physic core_util)

add_test(
    NAME physic_particle
    COMMAND physic_particle_test
)