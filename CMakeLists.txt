cmake_minimum_required(VERSION 3.28)

project(particle_simulator LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
endif()

# DEFAULT BUILD FLAGS

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_FLAGS_RELEASE -O2)
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# EXTERNAL LIBRARIES/FRAMEWORKS

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk4>=4.14)
pkg_check_modules(gmodule-export-2.0 REQUIRED IMPORTED_TARGET gmodule-export-2.0>=2.80)

# PROJECT SOURCE CODE

# EXECUTABLE

add_executable(Particle_Simulator)
target_sources(Particle_Simulator
    PRIVATE src/main.c
)
add_dependencies(Particle_Simulator gtk_resources)
target_link_libraries(Particle_Simulator
    PRIVATE
        core_util
        core_fast_physic
        core_physic
        ui_src

        PkgConfig::GTK
)

# PROJECT LIBRARIES

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
        BASE_DIRS include src
        FILES 
            include/physic/particle.h
            src/physic/particle_operation.h
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

add_library(ui_src)
target_sources(ui_src
    PRIVATE
        src/ui/signal.c

    INTERFACE FILE_SET HEADERS
        FILES 
            src/ui/identifier.h
            src/ui/signal.h
)
# These compile and link options should be only for linux/unix systems
target_link_options(ui_src PRIVATE -Wl,--export-dynamic)
target_link_libraries(ui_src PRIVATE PkgConfig::gmodule-export-2.0 PkgConfig::GTK)

# PROJECT ASSETS

file(GLOB_RECURSE _GTK_RESOURCE_FILES ${CMAKE_SOURCE_DIR}/gtk/*)
add_custom_command(OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gtk
    COMMAND cp -r ${CMAKE_SOURCE_DIR}/gtk ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
    DEPENDS ${_GTK_RESOURCE_FILES}
    VERBATIM
)
add_custom_target(gtk_resources DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/gtk)

# TESTING AREA

option(BUILD_TESTING "Allow cmake to handle source code testing" OFF)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()