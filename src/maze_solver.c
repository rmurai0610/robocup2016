#include "maze.h"
#include <string.h>

#define curr_node(x, y) (x + y * MAZE_X)
/* Finds the next unvisited tile move the robot to the tile
 * If new tile is not found, it will go to the starting tile or
 * the ramp tile
 * returns
 * 0 -> reached new tile
 * 1 -> reached ramp tile
 * 2 -> reached start tile
 */
uint8 find_unvisited(Maze maze_ptr, Robot *robot_ptr) {
  uint8 z = robot_ptr->z;
  uint8 init_x = robot_ptr->x;
  uint8 init_y = robot_ptr->y;

  uint8 x = init_x;
  uint8 y = init_y;

  return x + y + z + maze_ptr[0][0];
}
