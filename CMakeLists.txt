cmake_minimum_required(VERSION 3.28)

project(particle_simulator LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
endif()

# DEFAULT BUILD FLAGS

set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")

# PROJECT SOURCE CODE

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_library(core_util)
target_sources(core_util 
    PRIVATE src/util/vector2.c
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES include/util/vector2.h
)
target_link_libraries(core_util PRIVATE m)

add_library(core_fast_physic)
target_sources(core_fast_physic 
    PRIVATE src/physic/particle.c
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES include/physic/particle.h
)
target_compile_options(core_fast_physic PRIVATE $<$<CONFIG:Release>:-O3> $<$<CONFIG:Release>:-fopt-info-vec-all>)
target_link_libraries(core_fast_physic PUBLIC core_util)

add_library(core_physic)
target_sources(core_physic 
    PRIVATE src/physic/simulation.c
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES include/physic/simulation.h
)
target_link_libraries(core_physic PUBLIC core_fast_physic core_util)

# EXTERNAL LIBRARIES/FRAMEWORKS

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk4>=4.14)

add_executable(Particle_Simulator)
target_sources(Particle_Simulator
    PRIVATE src/main.c
)

target_link_libraries(Particle_Simulator
    PRIVATE
        core_util
        core_fast_physic
        core_physic
        PkgConfig::GTK
)

# TESTING AREA

option(BUILD_TESTING "Allow cmake to handle source code testing" OFF)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()