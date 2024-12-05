cmake_minimum_required(VERSION 3.28)
project(8080_intel_emulator CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_mixer_INCLUDE_DIRS})

include_directories(include/)

set(SourceCPU
    private/cpu/CPU_8080_Instructions.cpp
    private/cpu/CPU_8080.cpp
    include/cpu/CPU_8080.h
)

add_library(CPU_8080 STATIC ${SourceCPU})

set(SourcePlatform
#    private/cpu/CPU_8080_Instructions.cpp
#    private/cpu/CPU_8080.cpp
#    include/cpu/CPU_8080.h

    private/platform/SDL_display.cpp
    private/platform/Platform.cpp
    private/platform/InputBind.cpp
    private/platform/Sound.cpp

    include/platform/SDL_display.h
    include/platform/Platform.h
    include/platform/InputBind.h
    include/platform/Sound.h

    include/port_bits/space_invaders.h
)

add_library(Platform_obj STATIC ${SourcePlatform})
target_link_libraries(Platform_obj PUBLIC CPU_8080)
target_link_libraries(Platform_obj PUBLIC SDL2 SDL2_mixer)

add_custom_target(rom_files
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/rom/ ${CMAKE_BINARY_DIR}/rom/
    DEPENDS ${PROJECT_SOURCE_DIR}/rom/
)

add_executable(8080_intel_emulator main.cpp)
add_dependencies(8080_intel_emulator rom_files)
target_link_libraries(8080_intel_emulator PRIVATE Platform_obj)


add_executable(cpu_test tests/test.cpp)
target_link_libraries(cpu_test PUBLIC CPU_8080)
add_dependencies(cpu_test rom_files)
