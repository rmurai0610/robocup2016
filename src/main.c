#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "defs.h"
#include "maze.h"

#define DEBUG

void init_robot(Robot *robot) {
  robot->x = 0;
  robot->y = 0;
  robot->z = 0;
  robot->start_tile_x = 0;
  robot->start_tile_y = 0;
}

void print_test(char const *s) {
  printf("========== %*s%*s ==========\n",10 + ((int) strlen(s)) / 2, s, 10 - ((int) strlen(s)) / 2, "");
}

int main(void) {
  /* We are going to allocate each node uint16 number which is going to be
   * storing the wall data of SOUTH AND WEST of the tile, whether there is
   * victim on these wall.
   * Im going to assume that we are not having any map bigger than 20x20
   * uint16 looks like
   * |val|ramp_start_tile|saved|checkpoint|is_visited|south_victim|west_victim|south_wall|west_wall|
   */

  //initialise the maze, set them all to 255 for the val and 0 for the flags
  uint16 maze_floor_1[MAZE_SIZE * MAZE_SIZE];
  uint16 maze_floor_2[MAZE_SIZE * MAZE_SIZE];
  memset(maze_floor_1, 0xFF00u, sizeof(maze_floor_1));
  memset(maze_floor_2, 0xFF00u, sizeof(maze_floor_2));
  uint16 *maze[] = {maze_floor_1, maze_floor_2};
  //set the value of first tile to zero
  maze[0][0] = 0;
  //initialise the robot
  Robot robot;
  init_robot(&robot);

#ifdef DEBUG
  /*========== testing starts from here ==========*/
  print_test("Running Test Suite");

  print_test("Testing maze.c");

  set_east_wall(maze, 0,0,0,1);
  assert(get_east_wall(maze, 0,0,0));
  set_west_wall(maze, 0,0,0,1);
  assert(get_west_wall(maze, 0,0,0));
  set_north_wall(maze, 0,0,0,1);
  assert(get_north_wall(maze, 0,0,0));
  set_south_wall(maze, 0,0,0,1);
  assert(get_south_wall(maze, 0,0,0));

  set_east_wall(maze, 0,0,0,0);
  assert(!get_east_wall(maze, 0,0,0));
  set_west_wall(maze, 0,0,0,0);
  assert(!get_west_wall(maze, 0,0,0));
  set_north_wall(maze, 0,0,0,0);
  assert(!get_north_wall(maze, 0,0,0));
  set_south_wall(maze, 0,0,0,0);
  assert(!get_south_wall(maze, 0,0,0));

  set_east_wall(maze, 1,2,1,1);
  assert(get_east_wall(maze, 1,2,1));
  set_west_wall(maze, 10,10,0,1);
  assert(get_west_wall(maze, 10,10,0));
  set_north_wall(maze, 18,9,0,1);
  assert(get_north_wall(maze, 18,9,0));
  set_south_wall(maze, 9,18,1,1);
  assert(get_south_wall(maze,9,18,1));

  shift_maze_right(maze, &robot);
  robot.z = 1;
  shift_maze_right(maze, &robot);
  assert(get_east_wall(maze, 2, 2, 1));
  assert(get_west_wall(maze, 11, 10, 0));
  assert(get_north_wall(maze, 19, 9, 0));
  assert(get_south_wall(maze, 10, 18, 1));
  robot.z = 0;
  shift_maze_up(maze, &robot);
  robot.z = 1;
  shift_maze_up(maze, &robot);
  robot.z = 0;
  assert(get_east_wall(maze, 2, 3, 1));
  assert(get_west_wall(maze, 11, 11, 0));
  assert(get_north_wall(maze, 19, 10, 0));
  assert(get_south_wall(maze, 10, 19, 1));

  set_east_wall(maze, 2, 3, 1, 0);
  assert(!get_east_wall(maze, 2, 3, 1));
  set_west_wall(maze, 11, 11, 0, 0);
  assert(!get_west_wall(maze, 10, 10, 0));
  set_north_wall(maze, 19, 10, 0, 0);
  assert(!get_north_wall(maze, 10, 10, 0));
  set_south_wall(maze, 10, 19, 1, 0);
  assert(!get_south_wall(maze, 10, 19, 1));
  assert(get_val(maze, 10, 10, 0) != 0xFFu);

  print_test("Passed All Tests!");
#endif
  return 0;
}
