#include "maze_solver.h"
void turn_to_new_tile(uint8 i, Robot *robot_ptr) {
  if(!i) {
    turn_left_90(robot_ptr);
  } else if(i == 2) {
    turn_right_90(robot_ptr);
  }
}

void update_tiles(Maze maze, Robot *robot_ptr) {
  uint8 x = robot_ptr->x;
  uint8 y = robot_ptr->y;
  uint8 z = robot_ptr->z;

  uint8 curr_val = get_val(maze, x, y, z);
  uint8 min = curr_val;
  uint8 new_min_found = 0;

  if(!get_north_wall(maze, x, y, z)) {
    if(min > get_val(maze, x, (y + 1), z)) {
      min = get_val(maze, x, (y + 1), z);
      new_min_found = 1;
    }
  }

  if(!get_east_wall(maze, x, y, z)) {
    if(min > get_val(maze, (x + 1), y, z)) {
      min = get_val(maze, (x + 1), y, z);
      new_min_found = 1;
    }
  }

  if(!y && !get_south_wall(maze, x, y, z)) {
    if(min > get_val(maze, x, (y - 1), z)) {
      min = get_val(maze, x, (y - 1), z);
      new_min_found = 1;
    }
  }

  if(!x && !get_west_wall(maze, x, y, z)) {
    if(min > get_val(maze, (x - 1), y, z)) {
      min = get_val(maze, (x - 1), y, z);
      new_min_found = 1;
    }
  }

  if(!new_min_found) {
    //this means the current tile should have the smallest number
    min += 1;
  } else {
    //this means the current tile should have the largest number
    set_val(maze, x, y, z, min + 1);
  }

  if(!get_north_wall(maze, x, y, z)) {
    set_val(maze, x, (y + 1), z, min);
  }

  if(!get_east_wall(maze, x, y, z)) {
    set_val(maze, (x + 1), y, z, min);
  }

  if(!y && !get_south_wall(maze, x, y, z)) {
    set_val(maze, x, (y - 1), z, min);
  }

  if(!x && !get_west_wall(maze, x, y, z)) {
    set_val(maze, (x - 1), y, z, min);
  }
}

void maze_solver(Maze maze, Robot *robot_ptr) {
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

  update_tiles(maze, robot_ptr);

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
    d += i;
    d %= 4;
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
  }
  if(!found_new_tile) {
    //need some sort of checking to see if we reached the end or not
    find_unvisited(maze, robot_ptr);
  } else {
    //shift the map is needed
    if(robot_ptr->d == W && !robot_ptr->x) {
      shift_maze_right(maze, robot_ptr);
    }
    if(robot_ptr->d == S && !robot_ptr->y) {
      shift_maze_right(maze, robot_ptr);
    }
    move_forward_tile(robot_ptr);
  }
}

/* Finds the next unvisited tile move the robot to the tile
 * If new tile is not found, it will go to the starting tile or
 * the ramp tile
 * returns
 * 0 -> reached start/ramp tile
 * 1 -> reached new tile
 */
uint8 find_unvisited(Maze maze, Robot *robot_ptr) {
  for(;;) {
    uint8 z = robot_ptr->z;
    uint8 x = robot_ptr->x;
    uint8 y = robot_ptr->y;
    Direction d = robot_ptr->d;

    update_tiles(maze, robot_ptr);

    uint8 curr_val = get_val(maze, x, y, z);
    //found start / ramp tile
    if(!curr_val) {
      return 0;
    }
    curr_val--;

    if(!d) {
      //if d = N, force it to be W
      d = W;
    } else {
      d -= 1;
    }
    uint8 found_new_tile = 0;
    //checks 3 tiles around the current tile for new tile
    for (int i = 0; i < 3; ++i) {
      d += i;
      d %= 4;
      switch(d) {
        case N:
          if(!is_visited(maze, x, y + 1, z)  && !get_north_wall(maze, x, y, z)) {
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
    }
    //goes down the flooded maze need to check 4 Direction
    if(!found_new_tile) {
      uint8 next_val = get_val(maze, robot_ptr->x, robot_ptr->y, robot_ptr->z) - 1;
      d = robot_ptr->d;
      if(!d) {
        //if d = N, force it to be W
        d = W;
      } else {
        d -= 1;
      }
      for (int i = 0; i < 4; ++i) {
        d += i;
        d %= 4;
        switch(d) {
          case N:
            if(get_val(maze, x, y + 1, z) == next_val && !get_north_wall(maze, x, y, z)) {
              turn_to_new_tile(i, robot_ptr);
            }
            break;
          case E:
            if(get_val(maze, x + 1, y, z) == next_val && !get_east_wall(maze, x, y, z)) {
              turn_to_new_tile(i, robot_ptr);
            }
            break;
          case S:
            if(get_val(maze, x, y - 1, z) == next_val && !get_south_wall(maze, x, y, z)) {
              turn_to_new_tile(i, robot_ptr);
            }
            break;
          case W:
            if(get_val(maze, x - 1, y, z) == next_val && !get_west_wall(maze, x, y, z)) {
              turn_to_new_tile(i, robot_ptr);
            }
            break;
        }
      }
    }
    //shift the map is needed
    if(robot_ptr->d == W && !robot_ptr->x) {
      shift_maze_right(maze, robot_ptr);
    }
    if(robot_ptr->d == S && !robot_ptr->y) {
      shift_maze_right(maze, robot_ptr);
    }
    move_forward_tile(robot_ptr);
    if(found_new_tile) {
      return 1;
    }
  }
}
