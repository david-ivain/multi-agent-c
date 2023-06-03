#include "world.h"
#include "board.h"

World World_make(Environment env)
{
    World world;
    world.env = env;
    world.board = Board_make(&world.env);
    world.quit = false;
    return world;
}

void World_delete(World* world)
{
    Board_delete(&world->board);
}
