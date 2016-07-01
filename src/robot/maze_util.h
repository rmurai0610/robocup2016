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
#define EAST_WALL_VIC_MASK  0x0010u
#define NORTH_WALL_VIC_MASK 0x0020u
#define VISITED_MASK        0x0040u
#define KEY_TILE_MASK      0x0080u

#endif
