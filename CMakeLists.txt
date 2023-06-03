cmake_minimum_required(VERSION 3.0)

project(mas)

set(SRCS
    src/mas/main.c
    src/mas/agent.c
    src/mas/board.c
    src/mas/systems.c
    src/mas/world.c
)

set(HEADERS
    src/mas/mas.h
    src/mas/agent.h
    src/mas/board.h
    src/mas/environment.h
    src/mas/systems.h
    src/mas/utils.h
    src/mas/world.h
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(mas ${SRCS} ${HEADERS})
target_link_libraries(mas ${SDL2_LIBRARIES})


