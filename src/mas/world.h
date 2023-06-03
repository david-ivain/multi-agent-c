#pragma once

#include "agent.h"
#include "board.h"
#include "environment.h"

typedef struct World {
    Environment env;
    Board board;
    bool quit;
} World;

World World_make(Environment env);

void World_delete(World* world);