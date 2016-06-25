#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "defs.h"
#include "maze.h"
#include "maze_solver.h"

#define DEBUG
void init_robot(Robot *robot) {
  robot->x = 0;
  robot->y = 0;
  robot->z = 0;
  robot->d = N;
  robot->start_tile_x = 0;
  robot->start_tile_y = 0;
}
#ifdef DEBUG
void print_test(char const *s) {
  printf("========== %*s%*s ==========\n",10 + ((int) strlen(s)) / 2, s, 10 - ((int) strlen(s)) / 2, "");
}

void print_maze(Maze maze, Robot *robot_ptr) {
  int y = MAZE_SIZE - 1;
  while(y --> 0) {
    for (int x = 0; x < MAZE_SIZE; ++x) {
      if(x == robot_ptr->x && y == robot_ptr->y) {
        printf("%3c", '#');
      } else {
        if(is_visited(maze, x, y, 0)) {
          printf("%3c", '@');
        } else {
          printf("%3c", ' ');
        }
      }
    }
    printf("\n");
  }
}
#endif

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

  for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; ++i) {
    maze_floor_1[i] = 0xFF00u;
    maze_floor_2[i] = 0xFF00u;
  }
  uint16 *maze[] = {maze_floor_1, maze_floor_2};

  //initialise the robot
  Robot *robot = malloc(sizeof(Robot));
  init_robot(robot);

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

  shift_maze_right(maze, robot);
  robot->z = 1;
  shift_maze_right(maze, robot);
  assert(get_east_wall(maze, 2, 2, 1));
  assert(get_west_wall(maze, 11, 10, 0));
  assert(get_north_wall(maze, 19, 9, 0));
  assert(get_south_wall(maze, 10, 18, 1));
  robot->z = 0;
  shift_maze_up(maze, robot);
  robot->z = 1;
  shift_maze_up(maze, robot);
  robot->z = 0;
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

  set_val(maze, 10, 10, 0, 0xFFu);
  assert(get_val(maze, 10, 10, 0) == 0xFFu);

  for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; ++i) {
    maze_floor_1[i] = 0xFF00u;
    maze_floor_2[i] = 0xFF00u;
  }

  init_robot(robot);

  set_south_wall(maze,0,0,0,1);
  set_south_wall(maze,1,0,0,1);
  set_south_wall(maze,2,0,0,1);
  set_south_wall(maze,2,0,0,1);
  set_south_wall(maze,3,0,0,1);
  set_south_wall(maze,4,0,0,1);
  set_south_wall(maze,5,0,0,1);
  set_south_wall(maze,6,0,0,1);
  set_south_wall(maze,7,0,0,1);
  set_south_wall(maze,8,0,0,1);
  set_south_wall(maze,9,0,0,1);
  set_south_wall(maze,10,0,0,1);
  set_south_wall(maze,11,0,0,1);
  set_south_wall(maze,12,0,0,1);
  set_south_wall(maze,12,0,0,1);
  set_south_wall(maze,13,0,0,1);
  set_south_wall(maze,14,0,0,1);
  set_south_wall(maze,15,0,0,1);

  set_south_wall(maze,0, 9,0,1);
  set_south_wall(maze,1, 9,0,1);
  set_south_wall(maze,2, 9,0,1);
  set_south_wall(maze,2, 9,0,1);
  set_south_wall(maze,3, 9,0,1);
  set_south_wall(maze,4, 9,0,1);
  set_south_wall(maze,5, 9,0,1);
  set_south_wall(maze,6, 9,0,1);
  set_south_wall(maze,7, 9,0,1);
  set_south_wall(maze,8, 9,0,1);
  set_south_wall(maze,9, 9,0,1);
  set_south_wall(maze,10,9,0,1);
  set_south_wall(maze,11,9,0,1);
  set_south_wall(maze,12,9,0,1);
  set_south_wall(maze,12,9,0,1);
  set_south_wall(maze,13,9,0,1);
  set_south_wall(maze,14,9,0,1);
  set_south_wall(maze,15,9,0,1);

  set_west_wall(maze,0,0,0,1);
  set_west_wall(maze,0,1,0,1);
  set_west_wall(maze,0,2,0,1);
  set_west_wall(maze,0,3,0,1);
  set_west_wall(maze,0,4,0,1);
  set_west_wall(maze,0,5,0,1);
  set_west_wall(maze,0,6,0,1);
  set_west_wall(maze,0,7,0,1);
  set_west_wall(maze,0,8,0,1);

  set_west_wall(maze,16,0,0,1);
  set_west_wall(maze,16,1,0,1);
  set_west_wall(maze,16,2,0,1);
  set_west_wall(maze,16,3,0,1);
  set_west_wall(maze,16,4,0,1);
  set_west_wall(maze,16,5,0,1);
  set_west_wall(maze,16,6,0,1);
  set_west_wall(maze,16,7,0,1);
  set_west_wall(maze,16,8,0,1);

  set_west_wall(maze,2,2,0,1);
  set_west_wall(maze,2,3,0,1);

  set_west_wall(maze,5,2,0,1);
  set_west_wall(maze,5,3,0,1);

  set_south_wall(maze,7,2,0,1);
  set_south_wall(maze,8,2,0,1);
  set_south_wall(maze,9,2,0,1);

  set_west_wall(maze,5,2,0,1);
  set_west_wall(maze,5,3,0,1);

  set_west_wall(maze,9,2,0,1);
  set_west_wall(maze,9,3,0,1);

  set_south_wall(maze,7,6,0,1);
  set_south_wall(maze,8,6,0,1);
  set_south_wall(maze,9,6,0,1);

  print_test("Testing maze_solver.c");
  for(int i = 0; ; i++){
    printf("%i itt\n", i);
    printf("robot->x:%i y:%i d:%i\n", robot->x, robot->y, robot->d);
    printf("WALL N:%i E:%i S:%i W:%i\n", get_north_wall(maze, robot->x, robot->y, robot->z),
        get_east_wall(maze, robot->x, robot->y, robot->z),
        get_south_wall(maze, robot->x, robot->y, robot->z),
        get_west_wall(maze, robot->x, robot->y, robot->z));
    print_maze(maze, robot);
    for (volatile int i = 0; i < 30000000; ++i) {

    }
    if(maze_solver(maze, robot)) {
      break;
    }
  }
  printf("\n");
  print_maze(maze, robot);
  print_test("Passed All Tests!");
#endif
  return 0;
}
