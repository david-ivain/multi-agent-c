#include "board.h"
#include "agent.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Board Board_make(Environment* env)
{
    Board board;
    board.agents = malloc(env->nb_agents * sizeof(Agent));
    board.grid = calloc(env->grid_size.x * env->grid_size.y, sizeof(Agent*));
    board.grid_size = env->grid_size;
    return board;
}

void Board_delete(Board* board)
{
    free(board->agents);
    free(board->grid);
}

Agent* Board_get(Board* board, Vec2 position)
{
    return position.x >= board->grid_size.x || position.y >= board->grid_size.y || position.x < 0 || position.y < 0 ? NULL : board->grid[position.x + position.y * board->grid_size.x];
}

void Board_set(Board* board, Vec2 position, Agent* agent)
{
    board->grid[position.x + position.y * board->grid_size.x] = agent;
}