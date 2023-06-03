#pragma once

#include <stdlib.h>

#include "environment.h"
#include "utils.h"

typedef struct Agent Agent;

typedef struct Board {
    Vec2 grid_size;
    Agent* agents;
    Agent** grid;
} Board;

Board Board_make(Environment* env);

void Board_delete(Board* board);

Agent* Board_get(Board* board, Vec2 position);

void Board_set(Board* board, Vec2 position, Agent* agent);