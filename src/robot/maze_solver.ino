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

  uint8 d = robot_ptr->d;
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
  //standard LHR but avoids going back on to previously visited tiles
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
      } else {
        //on the ramp tile!
        set_key_tile(maze, x - 1, y, z, 0);
        set_key_tile(maze, x + 1, y, z, 0);
        set_key_tile(maze, x, y - 1, z, 0);
        set_key_tile(maze, x, y + 1, z, 0);
        return 0;
      }
    }
  } else {
    move_forward_tile(maze, robot_ptr);
    if(reset_signal) {
      return 0;
    }
  }
  return 0;
}

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
  flash_all(neo_pixel.Color(100, 255, 30), 500);
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
  int min_x = 0, min_y = 0, min_val = 255;
  //flood the maze and find the closest
  for(;;) {
    end_search = 1;
    for (y = 0; y < MAZE_Y; ++y) {
      for (x = 0; x < MAZE_X; ++x) {
        uint8 lowest_val = lowest_neighbour(maze, x, y, z);

        if(lowest_val && get_val(maze, x, y, z) == 255) {
          set_val(maze, x, y, z, lowest_val + 1);
          if(!is_visited(maze, x, y, z)) {
            //go_to_next_unvisited(maze, robot_ptr, x, y); 
            //return 1;
            
            if(min_val > lowest_val + 1) {
              min_x = x;
              min_y = y;
              min_val = lowest_val + 1;
            }     
          }
          end_search = 0;
        }
      }
    }
    if(end_search && min_val == 255) {
      //on the non starting floor
      if(robot_ptr->z) {
        flash_all(neo_pixel.Color(0, 255, 0), 500);
        //go_to_next_unvisited(maze, robot_ptr, robot_ptr->ramp_end_x, robot_ptr->ramp_end_y);
      } else {
        //otherwise go back to start tile
        flash_all(neo_pixel.Color(0, 255, 0), 500);
        go_to_next_unvisited(maze, robot_ptr, robot_ptr->start_tile_x, robot_ptr->start_tile_y);
      }
      return 0;
    } 
    if(end_search) {
      go_to_next_unvisited(maze, robot_ptr, min_x, min_y);
      return 1;
    }
  }
}

void go_to_next_unvisited(Maze maze, Robot *robot_ptr, uint8 nx, uint8 ny) {
  flash_all(neo_pixel.Color(30, 100, 30), 500);
  uint8 z = robot_ptr->z;
  for (int y = 0; y < MAZE_Y; ++y) {
    for (int x = 0; x < MAZE_X; ++x) {
      set_val(maze, x, y, z, 0xFF);
    }
  }
  set_val(maze, nx, ny, z, 1);
  uint8 end_search = 0;

  // make flooded table
  for(;;) {
    for (int y = 0; y < MAZE_Y; ++y) {
      for (int x = 0; x < MAZE_X; ++x) {
        uint8 lowest_val = lowest_neighbour(maze, x, y, z);
        if(lowest_val && get_val(maze, x, y, z) == 255) {
          set_val(maze, x, y, z, lowest_val + 1);
          if(x == robot_ptr->x && y == robot_ptr->y) {
            end_search = 1;
          }
        }
      }
    }
    if(end_search) {
      break;
    }
  }
  uint8 d;
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
    move_forward_tile(maze, robot_ptr);
    if(reset_signal) {
      break;
    }
  } while(curr_val > 1);
}

