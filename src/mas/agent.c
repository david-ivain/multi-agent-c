#include "agent.h"
#include "utils.h"
#include "world.h"
#include <stdbool.h>
#include <stdio.h>

/*
 * private definitions
 */

Vec2 calculate_next_point(Agent* this, Vec2 grid_size, bool torus);

/*
 * implementations
 */

Agent Agent_make(AgentType type, Vec2 position, Vec2 step)
{
    Agent agent = {
        .type = type,
        .position = position,
        .step = step,
        .collision = false
    };
    return agent;
}

void Agent_prepare(Agent* this)
{
    this->collision = false;
}

void Agent_decide(Agent* this, World* world)
{
    Vec2 next_point = calculate_next_point(this, world->env.grid_size, world->env.torus);
    Agent* target_particle = Board_get(&world->board, next_point);
    if (target_particle != NULL) {
        Vec2 step = target_particle->step;
        target_particle->step = this->step;
        this->step = step;
        this->collision = true;
        target_particle->collision = true;
    }
    next_point = calculate_next_point(this, world->env.grid_size, world->env.torus);
    if (next_point.x >= world->env.grid_size.x || next_point.x < 0) {
        this->step.x = -this->step.x;
        this->collision = true;
    }
    if (next_point.y >= world->env.grid_size.y || next_point.y < 0) {
        this->step.y = -this->step.y;
        this->collision = true;
    }
}

void Agent_update(Agent* this, World* world)
{
    if (Board_get(&world->board, this->position) == this)
        Board_set(&world->board, this->position, NULL);
    this->position = calculate_next_point(this, world->env.grid_size, world->env.torus);
    Board_set(&world->board, this->position, this);
}

Vec2 calculate_next_point(Agent* this, Vec2 grid_size, bool torus)
{
    Vec2 new_position = {
        .x = this->position.x + this->step.x,
        .y = this->position.y + this->step.y
    };
    if (torus) {
        new_position.x = new_position.x % grid_size.x;
        new_position.y = new_position.y % grid_size.y;
    } else {
        new_position.x = new_position.x > grid_size.x ? grid_size.x : new_position.x;
        new_position.y = new_position.y > grid_size.y ? grid_size.y : new_position.y;
    }
    return new_position;
}
