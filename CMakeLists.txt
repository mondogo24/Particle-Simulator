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
pkg_check_modules(libadwaita-1 REQUIRED IMPORTED_TARGET libadwaita-1>=1.5)

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
        core_physic
        ui
)

# PROJECT LIBRARIES

file(GLOB_RECURSE _CORE_UTIL_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.c)
file(GLOB_RECURSE _CORE_UTIL_PUBLIC_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/util/*.h)
add_library(core_util)
target_sources(core_util 
    PRIVATE ${_CORE_UTIL_SRC}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES ${_CORE_UTIL_PUBLIC_HEADERS}
)
target_link_libraries(core_util PRIVATE m)

set(_CORE_PHYSIC_OPTIMIZED_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/physic/particle.c)
set(_CORE_PHYSIC_OPTIMIZED_PUBLIC_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/physic/particle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/physic/particle_operation.h
)
add_library(_core_physic_optimized)
target_sources(_core_physic_optimized 
    PRIVATE ${_CORE_PHYSIC_OPTIMIZED_SRC}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include src
        FILES ${_CORE_PHYSIC_OPTIMIZED_PUBLIC_HEADERS}
)
target_compile_options(_core_physic_optimized PRIVATE $<$<CONFIG:Release>:-O3> $<$<CONFIG:Release>:-fopt-info-vec-all>)
target_link_libraries(_core_physic_optimized PUBLIC core_util)

file(GLOB_RECURSE _CORE_PHYSIC_NOT_OPTIMIZED_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/physic/*.c)
list(REMOVE_ITEM _CORE_PHYSIC_NOT_OPTIMIZED_SRC ${_CORE_PHYSIC_OPTIMIZED_SRC})
file(GLOB_RECURSE _CORE_PHYSIC_NOT_OPTIMIZED_PUBLIC_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/physic/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/physic/*.h
)
list(REMOVE_ITEM _CORE_PHYSIC_NOT_OPTIMIZED_PUBLIC_HEADERS ${_CORE_PHYSIC_OPTIMIZED_PUBLIC_HEADERS})
add_library(_core_physic_not_optimized)
target_sources(_core_physic_not_optimized 
    PRIVATE ${_CORE_PHYSIC_NOT_OPTIMIZED_SRC}
    PUBLIC FILE_SET HEADERS
        BASE_DIRS include
        FILES ${_CORE_PHYSIC_NOT_OPTIMIZED_PUBLIC_HEADERS}
)
target_link_libraries(_core_physic_not_optimized PUBLIC _core_physic_optimized core_util)

add_library(core_physic INTERFACE)
target_link_libraries(core_physic INTERFACE _core_physic_optimized _core_physic_not_optimized)

file(GLOB_RECURSE _UI_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.c)
file(GLOB_RECURSE _UI_PUBLIC_HEADERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/*.h
)
add_library(ui)
target_sources(ui
    PRIVATE ${_UI_SRC}
    PUBLIC FILE_SET HEADERS
        FILES ${_UI_PUBLIC_HEADERS}
)
# These compile and link options should be only for linux/unix systems
target_link_options(ui PRIVATE -Wl,--export-dynamic)
target_link_libraries(ui 
    PRIVATE 
        PkgConfig::gmodule-export-2.0 
    PUBLIC
        PkgConfig::GTK
        PkgConfig::libadwaita-1
)

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