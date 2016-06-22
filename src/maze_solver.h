#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include "maze.h"
#include "movement.h"
#include <string.h>

uint8 find_unvisited(Maze maze_ptr, Robot *robot_ptr);
void turn_to_new_tile(uint8 i, Robot *robot_ptr);

#endif
