#include "agent.h"
#include "board.h"
#include "environment.h"
#include "mas.h"
#include "systems.h"
#include "utils.h"
#include "world.h"
#include <SDL2/SDL.h>
#include <ranlib.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// #define BENCHMARK

#define SCALING 12

#define NB_PARTCLES (int)(250000 / SCALING / SCALING)
#define GRID_SIZE_X (int)(3360 / SCALING)
#define GRID_SIZE_Y (int)(2100 / SCALING)
#define GRID_IS_VISIBLE (SCALING > 8)
#ifndef BENCHMARK
#define DELAY 66
#define TICKS 0
#else
#define DELAY 0
#define TICKS 100
#endif

SDL_Renderer* renderer;

void callback(World* world, size_t currentTick)
{
#ifndef BENCHMARK
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            world->quit = true;
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);
    // printf("Rendering ctx size.\n %d %d", w, h);
    int pw = w / world->env.grid_size.x;
    int ph = h / world->env.grid_size.y;
    // printf("Agent size %d %d.\n", pw, ph);
    for (size_t i = 0; i < world->env.nb_agents; i++) {
        SDL_Rect rect = {
            .x = world->board.agents[i].position.x * pw,
            .y = world->board.agents[i].position.y * ph,
            .w = pw,
            .h = ph
        };
        if (world->board.agents[i].collision)
            SDL_SetRenderDrawColor(renderer, 128, 0, 0, SDL_ALPHA_OPAQUE);
        else
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &rect);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    if (world->env.grid_is_visible) {
        for (size_t i = 0; i < world->env.grid_size.x; i++) {
            SDL_RenderDrawLine(renderer, i * pw, 0, i * pw, h);
        }
        for (size_t i = 0; i < world->env.grid_size.y; i++) {
            SDL_RenderDrawLine(renderer, 0, i * ph, w, i * ph);
        }
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(world->env.delay);
#endif
}

int main(int argc, char** argv)
{
    Environment env = {
        .delay = DELAY,
        .grid_is_visible = GRID_IS_VISIBLE,
        .grid_size = {
            .x = GRID_SIZE_X,
            .y = GRID_SIZE_Y },
        .nb_agents = NB_PARTCLES,
        .scheduling = SCHEDULING_SEQUENTIAL,
        .ticks = TICKS,
        .torus = false
    };
    World world = World_make(env);
    srand(time(NULL));
    for (size_t i = 0; i < NB_PARTCLES; i++) {
        Vec2 position = {
            .x = (unsigned)rand() % env.grid_size.x,
            .y = (unsigned)rand() % env.grid_size.y
        };
        while (Board_get(&world.board, position) != NULL) {
            position.x = (unsigned)rand() % env.grid_size.x;
            position.y = (unsigned)rand() % env.grid_size.y;
        }
        Vec2 step = {
            .x = rand() % 3 - 1,
            .y = rand() % 3 - 1
        };
        if (step.x + step.y == 0) {
            if (rand() % 2)
                step.x = (rand() & 2) - 1;
            else
                step.y = (rand() & 2) - 1;
        }
        Agent agent = Agent_make(AGENT_PARTICLE, position, step);
        world.board.agents[i] = agent;
        Board_set(&world.board, position, &world.board.agents[i]);
    }

#ifndef BENCHMARK
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "Particles",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        3360,
        2100,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    Particle_system(&world, callback);
    SDL_DestroyWindow(window);
    SDL_Quit();
#else
    clock_t begin = clock();
    Particle_system(&world, callback);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time for %d particles and %d ticks: %.2fs\n", NB_PARTCLES, TICKS, time_spent);
    printf("Time / Iteration: %.2fms\n", time_spent / TICKS * 1E3);
    printf("Time / Particle: %.2fns\n", time_spent / TICKS / NB_PARTCLES * 1E9);
#endif

    World_delete(&world);

    return 0;
}