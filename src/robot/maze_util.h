#ifndef MAZE_UTIL_H
#define MAZE_UTIL_H
#include "defs.h"
#define MAZE_SIZE 20
#define MAZE_X MAZE_SIZE
#define MAZE_Y MAZE_SIZE
#define MAZE_Z 2

#define WEST_WALL_MASK      0x0001u
#define SOUTH_WALL_MASK     0x0002u
#define WEST_WALL_VIC_MASK  0x0004u
#define SOUTH_WALL_VIC_MASK 0x0008u
#define VISITED_MASK        0x0010u
#define CHECKPOINT_MASK     0x0020u
#define SAVED_TILE_MASK     0x0040u
#define RAMP_TILE_MASK      0x0080u

void shift_maze_right(Maze, Robot*);
void shift_maze_up(Maze, Robot*);

uint8 get_west_wall(Maze, uint8, uint8, uint8);
uint8 get_south_wall(Maze, uint8, uint8, uint8);
uint8 get_east_wall(Maze, uint8, uint8, uint8);
uint8 get_north_wall(Maze, uint8, uint8, uint8);

uint8 is_visited(Maze, uint8, uint8, uint8);
uint8 is_checkpoint(Maze, uint8, uint8, uint8);
uint8 is_saved_tile(Maze, uint8, uint8, uint8);
uint8 is_ramp_tile(Maze, uint8, uint8, uint8);

uint8 get_val(Maze, uint8, uint8, uint8);

void set_west_wall(Maze, uint8, uint8, uint8, uint8);
void set_south_wall(Maze, uint8, uint8, uint8, uint8);
void set_east_wall(Maze, uint8, uint8, uint8, uint8);
void set_north_wall(Maze, uint8, uint8, uint8, uint8);

void set_visited(Maze, uint8, uint8, uint8, uint8);
void set_checkpoint(Maze, uint8, uint8, uint8, uint8);
void set_saved_tile(Maze, uint8, uint8, uint8, uint8);
void set_ramp_tile(Maze, uint8, uint8, uint8, uint8);

void set_val(Maze, uint8, uint8, uint8, uint8);

#endif
