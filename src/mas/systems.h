#pragma once

#include <stddef.h>
typedef struct World World;

void Particle_system(World* world, void (*callback)(World* world, size_t current_tick));
