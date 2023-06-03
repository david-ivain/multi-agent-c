#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "environment.h"
#include "systems.h"
#include "world.h"

// typedef struct Thread {
// } Thread;

typedef struct ThreadPool {
    pthread_t threads[6];
} ThreadPool;

bool condition_always_true(size_t current_tick, size_t ticks)
{
    return true;
}

bool condition_current_tick_less_than_ticks(size_t current_tick, size_t ticks)
{
    return current_tick < ticks;
}

void Particle_system(World* world, void (*callback)(World* world, size_t current_tick))
{
    size_t ticks = world->env.ticks;
    size_t current_tick = 0;
    bool (*condition)(size_t, size_t) = ticks == 0 ? condition_always_true : condition_current_tick_less_than_ticks;
    Agent* agents = world->board.agents;
    while (condition(current_tick, ticks) && !world->quit) {
        for (size_t i = 0; i < world->env.nb_agents; i++) {
            Agent_prepare(&agents[i]);
        }
        if (world->env.scheduling == SCHEDULING_RANDOM)
            printf("TODO: RANDOM");
        for (size_t i = 0; i < world->env.nb_agents; i++) {
            Agent_decide(&agents[i], world);
            Agent_update(&agents[i], world);
        }
        current_tick++;
        callback(world, current_tick);
    }
}