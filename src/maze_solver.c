#include "maze_solver.h"
#include <stdio.h>

void turn_to_new_tile(uint8 i, Robot *robot_ptr) {
  if(!i) {
    turn_left_90(robot_ptr);
  } else if(i == 2) {
    turn_right_90(robot_ptr);
  } else if(i == 3) {
    turn_right_90(robot_ptr);
    turn_right_90(robot_ptr);
  }
}
//returns 1 when it finishes solving maze
uint8 maze_solver(Maze maze, Robot *robot_ptr) {
  //check for all 4 surrounding tile
  //if all of the tiles are visited or is wall use
  //find next unvisited to go to the next unvisited tile
  //      N=0
  //       |
  //       |
  // W=3--------E=1
  //       |
  //       |
  //      S=2

  Direction d = robot_ptr->d;
  uint8 z = robot_ptr->z;
  uint8 x = robot_ptr->x;
  uint8 y = robot_ptr->y;

  //As enum in C is contigious I think (hope)...
  if(!d) {
    //if d = 0, force it to be West
    d = W;
  } else {
    d -= 1;
  }
  uint8 found_new_tile = 0;
  //checks 3 tiles around the current tile
  for (int i = 0; i < 3; ++i) {
    switch(d) {
      case N:
        if(!is_visited(maze, x, y + 1, z) && !get_north_wall(maze, x, y, z)) {
          found_new_tile = 1;
          turn_to_new_tile(i, robot_ptr);
        }
        break;
      case E:
        if(!is_visited(maze, x + 1, y, z) && !get_east_wall(maze, x, y, z)) {
          found_new_tile = 1;
          turn_to_new_tile(i, robot_ptr);
        }
        break;
      case S:
        if(!is_visited(maze, x, y - 1, z) && !get_south_wall(maze, x, y, z)) {
          found_new_tile = 1;
          turn_to_new_tile(i, robot_ptr);
        }
        break;
      case W:
        if(!is_visited(maze, x - 1, y, z) && !get_west_wall(maze, x, y, z)) {
          found_new_tile = 1;
          turn_to_new_tile(i, robot_ptr);
        }
        break;
    }
    if(found_new_tile) {
      break;
    }
    d++;
    d %= 4;
  }
  if(!found_new_tile) {
    //need some sort of checking to see if we reached the end or not
    if(find_unvisited(maze, robot_ptr)) {
      return 0;
    } else {
      //TODO add check for the non starting floor
      if(!robot_ptr->z) {
        return 1;
      }

    }
  } else {
    //shift the map is needed
    if(robot_ptr->d == W && !robot_ptr->x) {
      shift_maze_right(maze, robot_ptr);
    }
    if(robot_ptr->d == S && !robot_ptr->y) {
      shift_maze_up(maze, robot_ptr);
    }
    move_forward_tile(maze, robot_ptr);
  }
  return 0;
}

uint8 lowest_neighbour(Maze maze, uint8 x, uint8 y, uint8 z) {
  uint8 lowest_val = 0;
  if(y != MAZE_Y - 1 && get_val(maze, x, y + 1, z) != 255 && !get_north_wall(maze, x, y, z)) {
    lowest_val = get_val(maze, x, y + 1, z);
  }
  if(x != MAZE_X - 1 && get_val(maze, x + 1, y, z) != 255 && !get_east_wall(maze, x, y, z)) {
    if(lowest_val > get_val(maze, x + 1, y, z) || !lowest_val) {
      lowest_val = get_val(maze, x + 1, y, z);
    }
  }
  if(y && get_val(maze, x, y - 1, z) != 255 && !get_south_wall(maze, x, y, z)) {
    if(lowest_val > get_val(maze, x, y - 1, z) || !lowest_val) {
      lowest_val = get_val(maze, x, y - 1, z);
    }
  }
  if(x && get_val(maze, x - 1, y, z) != 255 && !get_west_wall(maze, x, y, z)) {
    if(lowest_val > get_val(maze, x - 1, y, z) || !lowest_val) {
      lowest_val = get_val(maze, x - 1, y, z);
    }
  }
  return lowest_val;
}

/*
 * 0 -> no new tile found return to start/ramp tile
 * 1 -> found new tile and got to it
 */
uint8 find_unvisited(Maze maze, Robot *robot_ptr) {
  // set the start point
  uint8 z = robot_ptr->z;
  for (int y = 0; y < MAZE_Y; ++y) {
    for (int x = 0; x < MAZE_X; ++x) {
      set_val(maze, x, y, z, 0xFF);
    }
  }
  set_val(maze, robot_ptr->x, robot_ptr->y, z, 1);
  uint8 end_search = 1;
  int x = 0, y = 0;
  //flood the maze and find the closest
  for(;;) {
    end_search = 1;
    for (y = 0; y < MAZE_Y; ++y) {
      for (x = 0; x < MAZE_X; ++x) {
        uint8 lowest_val = lowest_neighbour(maze, x, y, z);

        if(lowest_val && get_val(maze, x, y, z) == 255) {
          set_val(maze, x, y, z, lowest_val + 1);
          if(!is_visited(maze, x, y, z)) {
            go_to_next_unvisited(maze, robot_ptr, x, y);
            return 1;
          }
          end_search = 0;
        }
      }
    }
    if(end_search) {
      //on the non starting floor
      if(robot_ptr->z) {
      } else {
        //otherwise go back to start tile
        go_to_next_unvisited(maze, robot_ptr, robot_ptr->start_tile_x, robot_ptr->start_tile_y);
      }
      return 0;
    }
  }
}

void go_to_next_unvisited(Maze maze, Robot *robot_ptr, uint8 nx, uint8 ny) {
  uint8 z = robot_ptr->z;
  for (int y = 0; y < MAZE_Y; ++y) {
    for (int x = 0; x < MAZE_X; ++x) {
      set_val(maze, x, y, z, 0xFF);
    }
  }
  set_val(maze, nx, ny, z, 1);
  uint8 end_search = 1;

  // make flooded table
  for(;;) {
    end_search = 1;
    for (int y = 0; y < MAZE_Y; ++y) {
      for (int x = 0; x < MAZE_X; ++x) {
        uint8 lowest_val = lowest_neighbour(maze, x, y, z);
        if(lowest_val && get_val(maze, x, y, z) == 255) {
          set_val(maze, x, y, z, lowest_val + 1);
          end_search = 0;
        }
      }
    }
    if(end_search) {
      break;
    }
  }
  Direction d;
  uint8 x;
  uint8 y;
  uint8 curr_val;

  do {
    d = robot_ptr->d;
    x = robot_ptr->x;
    y = robot_ptr->y;
    curr_val = get_val(maze, x, y, z) - 1;

    if(!d) {
      //if d = 0, force it to be West
      d = W;
    } else {
      d -= 1;
    }
    //checks 3 tiles around the current tile
    for (int i = 0; i < 4; ++i) {
      switch(d) {
        case N:
          if(get_val(maze, x, y + 1, z) == curr_val && !get_north_wall(maze, x, y, z)) {
            turn_to_new_tile(i, robot_ptr);
          }
          break;
        case E:
          if(get_val(maze, x + 1, y, z) == curr_val && !get_east_wall(maze, x, y, z)) {
            turn_to_new_tile(i, robot_ptr);
          }
          break;
        case S:
          if(get_val(maze, x, y - 1, z) == curr_val && !get_south_wall(maze, x, y, z)) {
            turn_to_new_tile(i, robot_ptr);
          }
          break;
        case W:
          if(get_val(maze, x - 1, y, z) == curr_val && !get_west_wall(maze, x, y, z)) {
            turn_to_new_tile(i, robot_ptr);
          }
          break;
      }
      d++;
      d %= 4;
    }
    //shift the map is needed
    if(robot_ptr->d == W && !robot_ptr->x) {
      shift_maze_right(maze, robot_ptr);
    }
    if(robot_ptr->d == S && !robot_ptr->y) {
      shift_maze_up(maze, robot_ptr);
    }
    move_forward_tile(maze, robot_ptr);
  } while(curr_val > 1);
}

