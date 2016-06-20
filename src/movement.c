#include "movement.h"
void move_forward_tile(Robot *robot_ptr) {
  Direction d = robot_ptr->d;
  switch(d) {
   case N:
     if(robot_ptr->y < MAZE_Y) {
       robot_ptr->y++;
     }
     break;
   case E:
     if(robot_ptr->x < MAZE_Y) {
       robot_ptr->x++;
     }
     break;
   case S:
     if(robot_ptr->y) {
       robot_ptr->y--;
     }
     break;
   case W:
     if(robot_ptr->x) {
       robot_ptr->x--;
     }
     break;
  }
}

void turn_left_90(Robot *robot_ptr) {
  if(!robot_ptr->d) {
    //if robot_ptr->d = 0, force it to be West
    robot_ptr->d = W;
  } else {
    robot_ptr->d -= 1;
  }
}

void turn_right_90(Robot *robot_ptr) {
  robot_ptr->d += 1;
  robot_ptr->d %= 4;
}

