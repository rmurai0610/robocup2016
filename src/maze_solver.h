#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include "maze.h"
#include "movement.h"
void turn_to_new_tile(uint8, Robot *);
uint8 maze_solver(Maze, Robot *);
void go_to_next_unvisited(Maze, Robot *, uint8, uint8);
uint8 find_unvisited(Maze, Robot *);
#endif
