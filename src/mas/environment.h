#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "utils.h"

typedef enum Scheduling {
    SCHEDULING_SEQUENTIAL,
    SCHEDULING_FAIR,
    SCHEDULING_RANDOM
} Scheduling;

typedef struct Environment {
    unsigned delay;
    Scheduling scheduling;
    size_t ticks;
    Vec2 grid_size;
    bool torus;
    bool grid_is_visible;
    size_t nb_agents;
} Environment;