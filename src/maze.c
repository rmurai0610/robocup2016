#include "maze.h"

/*make space on the right hand side of the array*/
void shift_maze_right(Maze maze, uint8 z) {
  for (int y = 0; y < MAZE_Y; ++y) {
    int x = MAZE_X - 1;
    while(x --> 0) {
      maze[z][(x + 1) + y * MAZE_X] = maze[z][x + y * MAZE_X];
    }
    maze[z][0 + y * MAZE_X] = 0;
  }
}
/*make space on the bottom of the array*/
void shift_maze_up(Maze maze, uint8 z) {
  for (int x = 0; x < MAZE_X; ++x) {
    int y = MAZE_Y - 1;
    while(y --> 0) {
      maze[z][x + (y + 1) * MAZE_X] = maze[z][x + y * MAZE_X];
    }
    maze[z][x] = 0;
  }
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
  if(x > MAZE_Y || y > (MAZE_Y - 1) || z > MAZE_Z) {
    /*out of bound request*/
    return 0;
  }
  uint8 curr_tile = maze[z][x + (y + 1) * MAZE_X];
  return curr_tile & SOUTH_WALL_MASK;
}

uint8 is_visited(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > MAZE_Y || y > (MAZE_Y - 1) || z > MAZE_Z) {
    /*out of bound request*/
    /*return 1 as I don't want to target these cells*/
    return 1;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & VISITED_MASK;
}

uint8 is_checkpoint(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > MAZE_Y || y > (MAZE_Y - 1) || z > MAZE_Z) {
    /*out of bound request*/
    /*return 1 as I don't want to target these cells*/
    return 1;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & CHECKPOINT_MASK;
}

uint8 is_start_tile(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > MAZE_Y || y > (MAZE_Y - 1) || z > MAZE_Z) {
    /*out of bound request*/
    /*return 1 as I don't want to target these cells*/
    return 1;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & START_TILE_MASK;
}

uint8 is_ramp_tile(Maze maze, uint8 x, uint8 y, uint8 z) {
  if(x > MAZE_Y || y > (MAZE_Y - 1) || z > MAZE_Z) {
    /*out of bound request*/
    /*return 1 as I don't want to target these cells*/
    return 1;
  }
  uint8 curr_tile = maze[z][x + y * MAZE_X];
  return curr_tile & RAMP_TILE_MASK;
}
/*==========Setters==========*/
void set_west_wall(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
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
  if(value) {
    /*set the wall to 1*/
    maze[z][x + (y + 1) * MAZE_X] |= SOUTH_WALL_MASK;
  } else {
    /*set the wall to 0*/
    maze[z][x + (y + 1) * MAZE_X] &= ~SOUTH_WALL_MASK;
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

void set_checkpoint(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    maze[z][x + y * MAZE_X] |= CHECKPOINT_MASK;
  } else {
    maze[z][x + y * MAZE_X] &= ~CHECKPOINT_MASK;
  }
}

void set_start_tile(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    maze[z][x + y * MAZE_X] |= START_TILE_MASK;
  } else {
    maze[z][x + y * MAZE_X] &= ~START_TILE_MASK;
  }
}

void set_ramp_tile(Maze maze, uint8 x, uint8 y, uint8 z, uint8 value) {
  if(x > MAZE_X || y > MAZE_Y || z > MAZE_Z) {
    /*out of bound request*/
    return;
  }
  if(value) {
    maze[z][x + y * MAZE_X] |= RAMP_TILE_MASK;
  } else {
    maze[z][x + y * MAZE_X] &= ~RAMP_TILE_MASK;
  }
}
