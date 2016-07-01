void mark_tile_unreachable(Maze maze, Robot *robot_ptr) {
  uint8 d = robot_ptr->d;
  uint8 x = robot_ptr->x;
  uint8 y = robot_ptr->y;
  uint8 z = robot_ptr->z;
  set_north_wall(maze, x, y, z, 1);
  set_east_wall(maze, x, y, z, 1);
  set_south_wall(maze, x, y, z, 1);
  set_east_wall(maze, x, y, z, 1);
  set_visited(maze, x, y, z, 1);
  set_key_tile(maze, x, y, z, 1);
  switch(d) {
   case N:
     if(y < MAZE_Y) {
       robot_ptr->y--;
     }
     break;
   case E:
     if(x < MAZE_Y) {
       robot_ptr->x--;
     }
     break;
   case S:
     if(y) {
       robot_ptr->y++;
     }
     break;
   case W:
     if(x) {
       robot_ptr->x++;
     }
     break;
  }
  while(encoder_val_l > 0 || encoder_val_r > 0) {
    move_backward(255);
  }
  motor_off();
}

void reached_checkpoint(Maze maze, Robot *robot) {
  robot_copy.x            = robot->x;
  robot_copy.y            = robot->y;
  robot_copy.z            = robot->z;
  robot_copy.d            = robot->d;
  robot_copy.start_tile_x = robot->start_tile_x;
  robot_copy.start_tile_y = robot->start_tile_y;
  robot_copy.ramp_begin_x = robot->ramp_begin_x;
  robot_copy.ramp_begin_y = robot->ramp_begin_y;
  robot_copy.ramp_end_x   = robot->ramp_end_x;
  robot_copy.ramp_end_y   = robot->ramp_end_y;
  for(int z = 0; z < 2; z++) {
    for(int y = 0; y < MAZE_Y; y++) {
      for(int x = 0; x < MAZE_X; x++) {
          maze_copy[z][x + y * MAZE_X] = maze[z][x + y * MAZE_X];
      }
    }
  }
}

void update_wall(Maze maze, Robot *robot_ptr) {
  uint8 d = robot_ptr->d;
  int x = robot_ptr->x;
  int y = robot_ptr->y;
  int z = robot_ptr->z;
  delay(10);
  float us_l  = read_us_average_l();
  delay(10);
  float us_r  = read_us_average_r();
  delay(10);
  float us_fl = read_us_average_fl();
  delay(10);
  float us_fr = read_us_average_fr();
  delay(10);
  float us_bl = read_us_average_bl();
  delay(10);
  float us_br = read_us_average_br();
  delay(10);

  switch(d) {
    case N:
      set_west_wall(maze, x, y, z, sees_wall(us_l));
      set_north_wall(maze, x, y, z, sees_wall(us_fl) && sees_wall(us_fr));
      set_east_wall(maze, x, y, z, sees_wall(us_r));
      set_south_wall(maze, x, y, z, sees_wall(us_bl) && sees_wall(us_br));
      break;
    case E:
      set_north_wall(maze, x, y, z, sees_wall(us_l));
      set_east_wall(maze, x, y, z, sees_wall(us_fl) && sees_wall(us_fr));
      set_south_wall(maze, x, y, z, sees_wall(us_r));
      set_west_wall(maze, x, y, z, sees_wall(us_bl) && sees_wall(us_br));
      break;
    case S:
      set_east_wall(maze, x, y, z, sees_wall(us_l));
      set_south_wall(maze, x, y, z, sees_wall(us_fl) && sees_wall(us_fr));
      set_west_wall(maze, x, y, z, sees_wall(us_r));
      set_north_wall(maze, x, y, z, sees_wall(us_bl) && sees_wall(us_br));
      break;
    case W:
      set_south_wall(maze, x, y, z, sees_wall(us_l));
      set_west_wall(maze, x, y, z, sees_wall(us_fl) && sees_wall(us_fr));
      set_north_wall(maze, x, y, z, sees_wall(us_r));
      set_east_wall(maze, x, y, z, sees_wall(us_bl) && sees_wall(us_br));
      break;
  }
}

uint8 sees_wall(float val) {
  return (val > 1.0 && val < DIST_WALL);
}

/*make space on the right hand side of the array*/
void shift_maze_right(Maze maze, Robot *robot_ptr) {
  uint8 z = robot_ptr->z;
  for (int y = 0; y < MAZE_Y; ++y) {
    int x = MAZE_X - 1;
    while(x --> 0) {
      maze[z][(x + 1) + y * MAZE_X] = maze[z][x + y * MAZE_X];
    }
    maze[z][0 + y * MAZE_X] = 0;
  }
  if(!z) {
    robot_ptr->start_tile_x++;
    robot_ptr->ramp_begin_x++;
  } else {
    robot_ptr->ramp_end_x++;
  }
  robot_ptr->x++;
}

/*make space on the bottom of the array*/
void shift_maze_up(Maze maze, Robot *robot_ptr) {
  uint8 z = robot_ptr->z;
  for (int x = 0; x < MAZE_X; ++x) {
    int y = MAZE_Y - 1;
    while(y --> 0) {
      maze[z][x + (y + 1) * MAZE_X] = maze[z][x + y * MAZE_X];
    }
    maze[z][x] = 0;
  }
  if(!z) {
    robot_ptr->start_tile_y++;
    robot_ptr->ramp_begin_y++;
  } else {
    robot_ptr->ramp_end_y++;
  }
  robot_ptr->y++;
}

/*==========Getters==========*/
/*return 0 when the wall doesn't exist
 *otherwise return an uint8 value
 *DOESNT ALWAYS RETURN 1
 */
uint8 get_west_wall(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & WEST_WALL_MASK;
}
uint8 get_south_wall(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & SOUTH_WALL_MASK;
}

uint8 get_east_wall(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][(x + 1) + y * MAZE_X];
  return curr_tile & WEST_WALL_MASK;
}

uint8 get_north_wall(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][x + (y + 1) * MAZE_X];
  return curr_tile & SOUTH_WALL_MASK;
}

uint8 get_west_victim(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & WEST_WALL_VIC_MASK;
}

uint8 get_south_victim(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & SOUTH_WALL_VIC_MASK;
}

uint8 get_east_victim(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & EAST_WALL_VIC_MASK;
}

uint8 get_north_victim(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & NORTH_WALL_VIC_MASK;
}

uint8 is_visited(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 1;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & VISITED_MASK;
}

uint8 get_key_tile(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 1;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & KEY_TILE_MASK;
}

uint8 get_val(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return 1;
  }
  uint16 curr_tile = maze[z][x + y * MAZE_X];
  //return first 8 bits
  return (uint8) (curr_tile >> 8);
}


/*==========Setters==========*/
void set_west_wall(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(get_key_tile(maze, x - 1, y, z) || get_key_tile(maze, x, y, z)) {
    return;
  }
  if(value) {
    /*set the wall to 1*/
    maze[z][x + y * MAZE_X] |= WEST_WALL_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][x + y * MAZE_X] &= ~WEST_WALL_MASK;
  }
}

void set_south_wall(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(get_key_tile(maze, x, y - 1, z) || get_key_tile(maze, x, y, z)) {
    return;
  }
  if(value) {
    /*set the wall to 1*/
    maze[z][x + y * MAZE_X] |= SOUTH_WALL_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][x + y * MAZE_X] &= ~SOUTH_WALL_MASK;
  }
}

void set_east_wall(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(get_key_tile(maze, x + 1, y, z) || get_key_tile(maze, x, y, z)) {
    return;
  }
  if(value) {
    /*set the wall to 1*/
    maze[z][(x + 1) + y * MAZE_X] |= WEST_WALL_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][(x + 1) + y * MAZE_X] &= ~WEST_WALL_MASK;
  }

}

void set_north_wall(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > (MAZE_Y - 1) || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(get_key_tile(maze, x, y + 1, z) || get_key_tile(maze, x, y, z)) {
    return;
  }
  if(value) {
    /*set the wall to 1*/
    maze[z][x + (y + 1) * MAZE_X] |= SOUTH_WALL_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][x + (y + 1) * MAZE_X] &= ~SOUTH_WALL_MASK;
  }
}

void set_west_victim(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    /*set the wall to 1*/
    maze[z][x + y * MAZE_X] |= WEST_WALL_VIC_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][x + y * MAZE_X] &= ~WEST_WALL_VIC_MASK;
  }
}

void set_south_victim(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    /*set the wall to 1*/
    maze[z][x + y * MAZE_X] |= SOUTH_WALL_VIC_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][x + y * MAZE_X] &= ~SOUTH_WALL_VIC_MASK;
  }
}

void set_east_victim(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > (MAZE_X - 1) || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    /*set the wall to 1*/
    maze[z][x + y * MAZE_X] |= EAST_WALL_VIC_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][x + y * MAZE_X] &= ~EAST_WALL_VIC_MASK;
  }

}

void set_north_victim(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    /*set the wall to 1*/
    maze[z][x + y * MAZE_X] |= NORTH_WALL_VIC_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][x + y * MAZE_X] &= ~NORTH_WALL_VIC_MASK;
  }
}

void set_visited(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    maze[z][x + y * MAZE_X] |= VISITED_MASK;
  } else {
    maze[z][x + y * MAZE_X] &= ~VISITED_MASK;
  }
}

void set_key_tile(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    maze[z][x + y * MAZE_X] |= KEY_TILE_MASK;
  } else {
    maze[z][x + y * MAZE_X] &= ~KEY_TILE_MASK;
  }
}

void set_val(Maze maze, uint8 x, uint8 y, uint8 z, uint8 val) {
  if(x > MAZE_Y || y > MAZE_Y || z > MAZE_Z) {
    //out of bound request
    return;
  }
  //set val to the first 8 bits
  maze[z][x + y * MAZE_X] &= 0x00FFu;
  maze[z][x + y * MAZE_X] |= (val << 8);
}
