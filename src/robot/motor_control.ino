#define DIST_TO_WALL_FRONT 8.5
#define DIST_TO_WALL_BACK  7
#define DIST_TO_WALL_SIDE  6
#define P_ALIGN            30
#define P_ALIGN_SIDE       100
#define P_GYRO             0.5

void move_forward_tile(Maze maze, Robot *robot_ptr) {
  float us_fl;
  float us_fr;
  float us_l;
  float us_r;
  uint16 light_sensor;
  int8 acc_val = 0;
  int8 black_count = 0;
  uint8 x = robot_ptr->x;
  uint8 y = robot_ptr->y;
  uint8 z = robot_ptr->z;
  uint8 d = robot_ptr->d;

  set_visited(maze, x, y, z, 1);
  switch (d) {
    case N:
      if (robot_ptr->y < MAZE_Y) {
        robot_ptr->y++;
      }
      break;
    case E:
      if (robot_ptr->x < MAZE_Y) {
        robot_ptr->x++;
      }
      break;
    case S:
      if (!get_south_wall(maze, x, y, z) && y == 1) {
        shift_maze_up(maze, robot_ptr);
      }
      if (robot_ptr->y) {
        robot_ptr->y--;
      }
      break;
    case W:
      if (x == 1 && !get_west_wall(maze, x, y, z)) {
        shift_maze_right(maze, robot_ptr);
      }
      if (robot_ptr->x) {
        robot_ptr->x--;
      }
      break;
  }
  reset_enc();
  for (;;) {
    CHECK_RESET;
    us_fl = read_us_fl();
    us_fr = read_us_fr();
    light_sensor = get_light_sensor();
    if (encoder_val_l > 230 && encoder_val_r > 230) {
      motor_off();
      break;
    }
    if (us_fl > 1.0 && us_fl <= DIST_TO_WALL_FRONT && us_fr > 1.0 && us_fr <= DIST_TO_WALL_FRONT) {
      motor_off();
      us_fl = read_us_average_fl();
      us_fr = read_us_average_fr();
      if (us_fl <= DIST_TO_WALL_FRONT && us_fr <= DIST_TO_WALL_FRONT) {
        motor_off();
        break;
      }
    }
    if (get_left_bumper() && get_right_bumper()) {
      mark_tile_unreachable(maze, robot_ptr);
      break;
    }
    if (get_left_bumper()) {
      avoid_left();
    }
    if (get_right_bumper()) {
      avoid_right();
    }

    if (get_pitch() > RAMP_ANGLE) {
      acc_val++;
    }
    if (get_pitch() < -RAMP_ANGLE) {
      acc_val--;
    }
    if (light_sensor < BLACK) {
      black_count++;
    }
    if (black_count > 6) {
      mark_tile_unreachable(maze, robot_ptr);
      break;
    }
    check_for_victim(maze, robot_ptr);
    p_sync_forward(255);
  }
  motor_off();
  if (acc_val > 1) {
    us_l = read_us_l();
    delay(10);
    us_r = read_us_r();
    //going up the ramp
    motor_off();
    if (get_pitch() > RAMP_ANGLE) {
      ramp(maze, robot_ptr, 1);
      return;
    }
  }
  if (acc_val < -1) {
    us_l = read_us_l();
    delay(10);
    us_r = read_us_r();
    //going down the ramp
    if (us_l < 20 && us_r < 20) {
      motor_off();
      if (get_pitch() < -RAMP_ANGLE) {
        ramp(maze, robot_ptr, 0);
        return;
      }
    }
  }
  light_sensor = get_light_sensor();
  if (light_sensor > SILVER) {
    reached_checkpoint(maze, robot_ptr);
    flash_all(neo_pixel.Color(255, 255, 0), 500);
  }
  motor_off();
  align_robot();
  return;
}

uint8 acceptable_val(uint8 target, float curr) {
  const float epsilon = 0.8;
  return ((target - epsilon) < curr && curr < (target + epsilon));
}

void align_robot(void) {
  float curr_fl = read_us_average_fl();
  float curr_fr = read_us_average_fr();
  int16 p_pow_r = 0;
  int16 p_pow_l = 0;
  for (;;) {
    if (curr_fl > 20 || curr_fr > 20 || curr_fr < 1 || curr_fl < 1) {
      motor_off();
      break;
    }
    if (acceptable_val(DIST_TO_WALL_FRONT, curr_fl) && acceptable_val(DIST_TO_WALL_FRONT, curr_fr)) {
      motor_off();
      break;
    }
    if (curr_fl > DIST_TO_WALL_FRONT) {
      p_pow_l = 150;
    } else {
      p_pow_l = -150;
    }
    if (curr_fr > DIST_TO_WALL_FRONT) {
      p_pow_r = 150;
    } else {
      p_pow_r = -150;
    }
    if (!acceptable_val(DIST_TO_WALL_FRONT, curr_fl)) {
      p_pow_l += (int32) ((curr_fl - DIST_TO_WALL_FRONT) * P_ALIGN);
    } else {
      p_pow_l = 0;
    }
    if (!acceptable_val(DIST_TO_WALL_FRONT, curr_fr)) {
      p_pow_r += (int32) ((curr_fr - DIST_TO_WALL_FRONT) * P_ALIGN);
    } else {
      p_pow_r = 0;
    }

    if (p_pow_l > -180 && p_pow_l < 0) {
      p_pow_l = -180;
    }
    if (p_pow_r > -180 && p_pow_r < 0) {
      p_pow_r = -180;
    }
    if (p_pow_l < 180 && p_pow_l > 0) {
      p_pow_l = 180;
    }
    if (p_pow_r < 180 && p_pow_r > 0) {
      p_pow_r = 180;
    }
    limit_motor_speed(&p_pow_l);
    limit_motor_speed(&p_pow_r);

    motor_left(p_pow_l);
    motor_right(p_pow_r);
    curr_fl = read_us_fl();
    delay(10);
    curr_fr = read_us_fr();
    delay(10);
  }

  float curr_bl = read_us_average_bl();
  float curr_br = read_us_average_br();
  p_pow_r = 0;
  p_pow_l = 0;
  for (;;) {
    if (curr_bl > 20 || curr_br > 20 || curr_br < 1 || curr_bl < 1) {
      motor_off();
      break;
    }
    if (acceptable_val(DIST_TO_WALL_BACK, curr_bl) && acceptable_val(DIST_TO_WALL_BACK, curr_br)) {
      motor_off();
      break;
    }
    if (curr_bl > DIST_TO_WALL_BACK) {
      p_pow_l = 150;
    } else {
      p_pow_l = -150;
    }
    if (curr_br > DIST_TO_WALL_BACK) {
      p_pow_r = 150;
    } else {
      p_pow_r = -150;
    }
    if (!acceptable_val(DIST_TO_WALL_BACK, curr_bl)) {
      p_pow_l += (int32) ((curr_bl - DIST_TO_WALL_BACK) * P_ALIGN);
    } else {
      p_pow_l = 0;
    }
    if (!acceptable_val(DIST_TO_WALL_BACK, curr_br)) {
      p_pow_r += (int32) ((curr_br - DIST_TO_WALL_BACK) * P_ALIGN);
    } else {
      p_pow_r = 0;
    }

    if (p_pow_l > -180 && p_pow_l < 0) {
      p_pow_l = -180;
    }
    if (p_pow_r > -180 && p_pow_r < 0) {
      p_pow_r = -180;
    }
    if (p_pow_l < 180 && p_pow_l > 0) {
      p_pow_l = 180;
    }
    if (p_pow_r < 180 && p_pow_r > 0) {
      p_pow_r = 180;
    }
    limit_motor_speed(&p_pow_l);
    limit_motor_speed(&p_pow_r);

    motor_left(-p_pow_l);
    motor_right(-p_pow_r);
    curr_bl = read_us_bl();
    delay(10);
    curr_br = read_us_br();
    delay(10);
  }
}

void limit_motor_speed(int16 *pow) {
  if (*pow > 255) {
    *pow = 255;
  }
  if (*pow < -255) {
    *pow = -255;
  }
}

void p_sync_forward(int16 pow) {
  int16 p_pow_l = pow;
  int16 p_pow_r = pow;
  int32 diff_encoder;
  float us_l;
  float us_r;
  if (encoder_val_l > encoder_val_r) {
    diff_encoder = encoder_val_l - encoder_val_r;
    p_pow_l -= (int16) (diff_encoder * P_FORWARD);
  } else {
    diff_encoder = encoder_val_r - encoder_val_l;
    p_pow_r -= (int16) (diff_encoder * P_FORWARD);
  }
  us_l = read_us_l();
  delay(10);
  us_r = read_us_r();
  delay(10);

  if (us_l > 1.0 && us_l < 10) {
    if (us_l > DIST_TO_WALL_SIDE + 1) {
      p_pow_l -= (int16) ((us_l - (DIST_TO_WALL_SIDE + 1)) * P_ALIGN_SIDE);
      p_pow_r = pow;
    }
    if (us_l < DIST_TO_WALL_SIDE - 1) {
      p_pow_r += (int16) ((us_l - (DIST_TO_WALL_SIDE - 1)) * P_ALIGN_SIDE);
      p_pow_l = pow;
    }
  }
  if (us_r > 1.0 && us_r < 10) {
    if (us_r > DIST_TO_WALL_SIDE + 1) {
      p_pow_r -= (int16) ((us_r - (DIST_TO_WALL_SIDE + 1)) * P_ALIGN_SIDE);
      p_pow_l = pow;
    }
    if (us_r < DIST_TO_WALL_SIDE - 1) {
      p_pow_l += (int16) ((us_r - (DIST_TO_WALL_SIDE - 1)) * P_ALIGN_SIDE);
      p_pow_r = pow;
    }
  }

  if (p_pow_l < 50) {
    p_pow_l = 50;
  }
  if (p_pow_r < 50) {
    p_pow_r = 50;
  }
  limit_motor_speed(&p_pow_l);
  limit_motor_speed(&p_pow_r);
  motor_left(p_pow_l);
  motor_right(p_pow_r);
}
//////////////////////////////////////////////////////////////////////////////////
uint8 should_look_for_victim_right = 1;
uint8 should_look_for_victim_left = 1;
//////////////////////////////////////////////////////////////////////////////////
void turn_right_90(Robot *robot_ptr) {
  align_robot();
  robot_ptr->d += 1;
  robot_ptr->d %= 4;

  uint8 x = robot_ptr->x;
  uint8 y = robot_ptr->y;
  uint8 z = robot_ptr->z;
  uint8 d = robot_ptr->d;

  uint8 vic_found = 0;
  int16 vic_angle = 0;

  int16 imu_buff[4];
  read_gyro(imu_buff);
  int16 init_angle = imu_buff[0];
  int16 target_angle = delta_angle(init_angle, get_target_angle(robot_ptr->d));
  int16 d_angle;
  for (;;) {
    CHECK_RESET;
    read_gyro(imu_buff);
    d_angle = delta_angle(init_angle, imu_buff[0]);
    d_angle = delta_angle(d_angle, target_angle);
    if (!vic_found) {
      if (read_heat_sensor(TPA81_LEFT)) {
        vic_angle = d_angle;
        vic_found = 1;
      }
    }
    if (d_angle < 100) {
      if (d_angle > -100) {
        motor_off();
        read_gyro(imu_buff);
        d_angle = delta_angle(init_angle, imu_buff[0]);
        d_angle = delta_angle(d_angle, target_angle);
        if (-100 < d_angle && d_angle < 100) {
          break;
        }
      } else {
        if (d_angle > -1000) {
          turn_left(180);
        } else {
          turn_left(255);
        }
      }
    } else {
      if (d_angle < 1000) {
        turn_right(180);
      } else {
        turn_right(255);
      }
    }
  }
  if (vic_found && should_look_for_victim_left) {
    should_look_for_victim_left = 0;
    should_look_for_victim_right = 0;
    if (vic_angle > 4500) {
      flash_all(neo_pixel.Color(0, 255, 100), 500);
      drop_rescue_kit();
      for (int i = 0; i < 4; i++) {
        flash_all(neo_pixel.Color(255, 255, 255), 500);
        delay(500);
      }
    } else {
      turn_right_90(robot_ptr);
      flash_all(neo_pixel.Color(0, 255, 100), 500);
      drop_rescue_kit();
      for (int i = 0; i < 4; i++) {
        flash_all(neo_pixel.Color(255, 255, 255), 500);
        delay(500);
      }
      turn_left_90(robot_ptr);
    }
    should_look_for_victim_left = 1;
    should_look_for_victim_right = 1;
  }
  align_robot();
}

void turn_left_90(Robot *robot_ptr) {
  align_robot();
  if (!robot_ptr->d) {
    //if robot_ptr->d = 0, force it to be West
    robot_ptr->d = W;
  } else {
    robot_ptr->d -= 1;
  }

  uint8 x = robot_ptr->x;
  uint8 y = robot_ptr->y;
  uint8 z = robot_ptr->z;
  uint8 d = robot_ptr->d;

  uint8 vic_found = 0;
  int16 vic_angle = 0;

  int16 imu_buff[4];
  read_gyro(imu_buff);

  int16 init_angle = imu_buff[0];
  int16 target_angle = -delta_angle(init_angle, get_target_angle(robot_ptr->d));
  int16 d_angle;
  for (;;) {
    CHECK_RESET;
    read_gyro(imu_buff);
    d_angle = -delta_angle(init_angle, imu_buff[0]);
    d_angle = delta_angle(d_angle, target_angle);
    if (!vic_found) {
      if (read_heat_sensor(TPA81_RIGHT)) {
        vic_angle = d_angle;
        vic_found = 1;
      }
    }
    if (d_angle < 100) {
      if (d_angle > -100) {
        motor_off();
        break;
      } else {
        if (d_angle > -1000) {
          turn_right(180);
        } else {
          turn_right(255);
        }
      }
    } else {
      if (d_angle < 1000) {
        turn_left(180);
      } else {
        turn_left(255);
      }
    }
  }
  if (vic_found && should_look_for_victim_right) {
    should_look_for_victim_left = 0;
    should_look_for_victim_right = 0;
    if (vic_angle > 4500) {
      flash_all(neo_pixel.Color(0, 255, 100), 500);
      drop_rescue_kit();
      for (int i = 0; i < 4; i++) {
        flash_all(neo_pixel.Color(255, 255, 255), 500);
        delay(500);
      }
    } else {
      turn_left_90(robot_ptr);
      flash_all(neo_pixel.Color(0, 255, 100), 500);
      drop_rescue_kit();
      for (int i = 0; i < 4; i++) {
        flash_all(neo_pixel.Color(255, 255, 255), 500);
        delay(500);
      }
      turn_right_90(robot_ptr);
    }
    should_look_for_victim_left = 1;
    should_look_for_victim_right = 1;
  }
  align_robot();
}

int16_t get_target_angle(uint8_t d) {
  switch (d) {
    case N:
      return 0;
    case E:
      return 9000;
    case S:
      return 18000;
    case W:
      return -9000;
  }
}

void avoid_left(void) {
  int32 enc_curr_l = encoder_val_l;
  int32 enc_curr_r = encoder_val_r;
  while (enc_curr_l - encoder_val_l < 80) {
    motor_left(-255);
    motor_right(0);
  }
  motor_off();
  while (enc_curr_r - encoder_val_r < 80) {
    motor_left(0);
    motor_right(-255);
  }
  motor_off();
  delay(100);
  while (enc_curr_l - encoder_val_l > 0) {
    motor_left(255);
    motor_right(0);
  }
  motor_off();
  while (enc_curr_r - encoder_val_r > 0) {
    motor_left(0);
    motor_right(255);
  }
  motor_off();
}

void avoid_right(void) {
  int32 enc_curr_l = encoder_val_l;
  int32 enc_curr_r = encoder_val_r;
  while (enc_curr_r - encoder_val_r < 80) {
    motor_left(0);
    motor_right(-255);
  }
  motor_off();
  while (enc_curr_l - encoder_val_l < 80) {
    motor_left(-255);
    motor_right(0);
  }
  motor_off();
  delay(100);
  while (enc_curr_r - encoder_val_r > 0) {
    motor_left(0);
    motor_right(255);
  }
  motor_off();
  while (enc_curr_l - encoder_val_l > 0) {
    motor_left(255);
    motor_right(0);
  }
  motor_off();
}

void ramp(Maze maze, Robot *robot_ptr, uint8 up) {
  int16 speed_setting = 255;
  if(!up) {
    speed_setting = 150;
  }
  motor_off();
  if (robot_ptr->z) {
    robot_ptr->x = robot_ptr->ramp_begin_x;
    robot_ptr->y = robot_ptr->ramp_begin_y;
    robot_ptr->z = 0;
  } else {
    uint8_t d = robot_ptr->d;
    switch (d) {
      case N:
        if (robot_ptr->y < MAZE_Y) {
          robot_ptr->y --;
        }
        break;
      case E:
        if (robot_ptr->x < MAZE_Y) {
          robot_ptr->x --;
        }
        break;
      case S:
        if (robot_ptr->y) {
          robot_ptr->y++;
        }
        break;
      case W:
        if (robot_ptr->x) {
          robot_ptr->x++;
        }
        break;
    }
    //set_ramp_tile(maze, robot_ptr->x, robot_ptr->y, robot_ptr->z, 1);
    robot_ptr->ramp_begin_x = robot_ptr->x;
    robot_ptr->ramp_begin_y = robot_ptr->y;
    robot_ptr->x = 1;
    robot_ptr->y = 1;
    robot_ptr->z = 1;
    //set_ramp_tile(maze, robot_ptr->x, robot_ptr->y, robot_ptr->z, 1);
    robot_ptr->ramp_end_x = robot_ptr->x;
    robot_ptr->ramp_end_y = robot_ptr->y;
  }

  //go out of the room and set the back to be wall
  flash_all(neo_pixel.Color(255, 0, 0), 500);
  reset_enc();
  float us_l, us_r, us_fl, us_fr;
  for (;;) {
    //go up/down the ramp
    us_l = read_us_average_l();
    delay(10);
    us_r = read_us_average_r();
    delay(10);
    us_fl = read_us_fl();
    delay(10);
    us_fr = read_us_fr();
    if (get_right_bumper() && get_left_bumper()) {
      move_backward(500);
      motor_off();
      align_robot();
      break;
    }
    p_sync_forward(speed_setting);
  }

  us_l = read_us_average_l();
  delay(10);
  us_r = read_us_average_r();
  delay(10);

  uint8 x = robot_ptr->x;
  uint8 y = robot_ptr->y;
  uint8 z = robot_ptr->z;
  uint8 d = robot_ptr->d;

  switch (d) {
    case N:
      set_south_wall(maze, x, y, z, 1);
      set_key_tile(maze, x, y - 1, z, 1);
      break;
    case E:
      set_west_wall(maze, x, y, z, 1);
      set_key_tile(maze, x - 1, y, z, 1);
      break;
    case S:
      set_north_wall(maze, x, y, z, 1);
      set_key_tile(maze, x, y + 1, z, 1);
      break;
    case W:
      set_east_wall(maze, x, y, z, 1);
      set_key_tile(maze, x + 1, y, z, 1);
      break;
  }
  if (us_r < 20) {
    turn_left_90(robot_ptr);
  } else {
    turn_right_90(robot_ptr);
  }

  move_forward_tile(maze, robot_ptr);
}
