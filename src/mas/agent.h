#pragma once

#include <stdbool.h>

#include "utils.h"

typedef struct World World;

typedef enum AgentType {
    AGENT_PARTICLE
} AgentType;

typedef struct Agent {
    AgentType type;
    Vec2 position;
    Vec2 step;
    bool collision;
} Agent;

Agent Agent_make(AgentType type, Vec2 position, Vec2 step);

void Agent_prepare(Agent* this);

void Agent_decide(Agent* this, World* world);

void Agent_update(Agent* this, World* world);
