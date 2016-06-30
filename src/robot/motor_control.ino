#define DIST_TO_WALL_FRONT 9
#define DIST_TO_WALL_SIDE  7
#define P_ALIGN            30
#define P_GYRO             0.5

void move_forward_tile(Maze maze, Robot *robot_ptr){
  uint8_t d = robot_ptr->d;
  set_visited(maze, robot_ptr->x, robot_ptr->y, robot_ptr->z, 1);
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
  reset_enc();
  for(;;) {
    CHECK_RESET;
    if(encoder_val_l > 230 && encoder_val_r > 230) {
      motor_off();
      break;
    }
    if(get_left_bumper()) {
      avoid_left();
    }
    if(get_right_bumper()) {
      avoid_right();
    }
    p_sync_forward(255);
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
  for(;;) {

    if(curr_fl > 20 || curr_fr > 20 || curr_fr < 1 || curr_fl < 1) {
      motor_off();
      return;
    }
    if(acceptable_val(DIST_TO_WALL_FRONT, curr_fl) && acceptable_val(DIST_TO_WALL_FRONT, curr_fr)) {
      motor_off();
      return;
    }
    if(curr_fl > DIST_TO_WALL_FRONT) {
      p_pow_l = 150;
    } else {
      p_pow_l = -150;
    }
    if(curr_fr > DIST_TO_WALL_FRONT) {
      p_pow_r = 150;
    } else {
      p_pow_r = -150;
    }
    if(!acceptable_val(DIST_TO_WALL_FRONT, curr_fl)) {
      p_pow_l += (int32) ((curr_fl - DIST_TO_WALL_FRONT) * P_ALIGN);
    } else {
      p_pow_l = 0;
    }
    if(!acceptable_val(DIST_TO_WALL_FRONT, curr_fr)) {
      p_pow_r += (int32) ((curr_fr - DIST_TO_WALL_FRONT) * P_ALIGN);
    } else {
      p_pow_r = 0;
    }

    if(p_pow_l > -180 && p_pow_l < 0) {
      p_pow_l = -180;
    }
    if(p_pow_r > -180 && p_pow_r < 0) {
      p_pow_r = -180;
    }
    if(p_pow_l < 180 && p_pow_l > 0) {
      p_pow_l = 180;
    }
    if(p_pow_r < 180 && p_pow_r > 0) {
      p_pow_r = 180;
    }
    limit_motor_speed(&p_pow_l);
    limit_motor_speed(&p_pow_r);
    
    motor_left(p_pow_l);
    motor_right(p_pow_r);
    Serial.printf("L:%i R:%i\n", p_pow_l, p_pow_r);
    curr_fl = read_us_fl();
    delay(10);
    curr_fr = read_us_fr();
    delay(10);
  }
}

void limit_motor_speed(int16 *pow) {
  if(*pow > 255) {
    *pow = 255;
  }
  if(*pow < -255) {
    *pow = -255;
  }
}

void p_sync_forward(int16 pow) {
  int16 p_pow_l = pow;
  int16 p_pow_r = pow;
  int32 diff_encoder;
  float us_l;
  float us_r;
  if(encoder_val_l > encoder_val_r) {
    diff_encoder = encoder_val_l - encoder_val_r;
    p_pow_l -= (int16) (diff_encoder * P_FORWARD);
  } else {
    diff_encoder = encoder_val_r - encoder_val_l;
    p_pow_r -= (int16) (diff_encoder * P_FORWARD);
  }
  us_l = read_us_l();
  delay(10);
  us_r = read_us_r();
  Serial.printf("L:%f , R:%f\n", us_l, us_r);
  neo_pixel.setPixelColor(0, neo_pixel.Color(0, 0, 0));
  neo_pixel.setPixelColor(7, neo_pixel.Color(0, 0, 0));
  if(us_l > 1.0 && us_l < 10) {
    if(us_l > DIST_TO_WALL_SIDE + 1) {
      p_pow_l -= (int16) ((us_l - (DIST_TO_WALL_SIDE + 1)) * P_ALIGN);
      neo_pixel.setPixelColor(0, neo_pixel.Color(0, 0, 255));
    } 
    if(us_l < DIST_TO_WALL_SIDE - 1) {
      p_pow_r -= (int16) ((us_l - (DIST_TO_WALL_SIDE - 1)) * P_ALIGN);
      neo_pixel.setPixelColor(0, neo_pixel.Color(0, 255, 0));
    }
  }
  if(us_r > 1.0 && us_r < 10) {
    if(us_r > DIST_TO_WALL_SIDE + 1) {
      p_pow_r -= (int16) ((us_r - (DIST_TO_WALL_SIDE + 1)) * P_ALIGN);
      neo_pixel.setPixelColor(7, neo_pixel.Color(0, 0, 255));
    } 
    if(us_r < DIST_TO_WALL_SIDE - 1) {
      p_pow_l -= (int16) ((us_r - (DIST_TO_WALL_SIDE - 1)) * P_ALIGN);
      neo_pixel.setPixelColor(7, neo_pixel.Color(0, 255, 0));
    }    
  } 

  if(p_pow_l < 50) {
    p_pow_l = 50;
  }
  if(p_pow_r < 50) {
    p_pow_r = 50;
  }
  limit_motor_speed(&p_pow_l);
  limit_motor_speed(&p_pow_r);
  motor_left(p_pow_l);
  motor_right(p_pow_r);
  neo_pixel.show();
  Serial.printf("pow L:%i R:%i\n", p_pow_l, p_pow_r);
}

void turn_right_90(Robot *robot_ptr) {
  robot_ptr->d += 1;
  robot_ptr->d %= 4;
  int16 imu_buff[4];
  read_gyro(imu_buff);

  int16 init_angle = imu_buff[0];
  int16 target_angle = delta_angle(init_angle, get_target_angle(robot_ptr->d));
  Serial.printf("TARGET_ANGLE:%i\n", target_angle);
  int16 d_angle;
  Serial.println(imu_buff[0]);
  for(;;) {
    CHECK_RESET;
    read_gyro(imu_buff);
    d_angle = delta_angle(init_angle, imu_buff[0]);
    Serial.printf("ANGLE_turned:%i\n", d_angle);
    d_angle = delta_angle(d_angle, target_angle);
    Serial.printf("d_angle:%i\n", d_angle);
    if(d_angle < 100) {
      if(d_angle > -100) {
        motor_off();
        read_gyro(imu_buff);
        d_angle = delta_angle(init_angle, imu_buff[0]);
        d_angle = delta_angle(d_angle, target_angle);
        if(-100 < d_angle && d_angle < 100) {
          break;
        }
      } else {
        if(d_angle > -1000) {
          turn_left(200);
        } else {
          turn_left(255);
        }
      }
    } else {
      if(d_angle < 1000) {
        turn_right(200);
      } else {
        turn_right(255);
      }
    }
  }
}

void turn_left_90(Robot *robot_ptr) {

  if(!robot_ptr->d) {
    //if robot_ptr->d = 0, force it to be West
    robot_ptr->d = W;
  } else {
    robot_ptr->d -= 1;
  }
  int16 imu_buff[4];
  read_gyro(imu_buff);

  int16 init_angle = imu_buff[0];
  int16 target_angle = -delta_angle(init_angle, get_target_angle(robot_ptr->d));
  Serial.printf("TARGET_ANGLE:%i\n", target_angle);
  int16 d_angle;
  Serial.println(imu_buff[0]);
  for(;;) {
    CHECK_RESET;
    read_gyro(imu_buff);
    d_angle = -delta_angle(init_angle, imu_buff[0]);
    Serial.printf("ANGLE_turned:%i\n", d_angle);
    d_angle = delta_angle(d_angle, target_angle);
    Serial.printf("ANGLE_LEFT:%i\n", d_angle);
    if(d_angle < 100) {
      if(d_angle > -100) {
        motor_off();
        break;
      } else {
        if(d_angle > -1000) {
          turn_right(200);
        } else {
          turn_right(255);
        }
      }
    } else {
      if(d_angle < 1000) {
        turn_left(200);
      } else {
        turn_left(255);
      }
      
    }
  }
}

int16_t get_target_angle(uint8_t d) {
  switch (d) {
    case N:
      Serial.printf("TARGET_ANGLE:  0\n");
      return 0;
    case E:
      Serial.printf("TARGET_ANGLE:  9000\n");
      return 9000;
    case S:
      Serial.printf("TARGET_ANGLE:  18000\n");
      return 18000;
    case W:
      Serial.printf("TARGET_ANGLE:  -9000\n");
      return -9000;
  }
}

void avoid_left(void) {
  int32 enc_curr_l = encoder_val_l;
  int32 enc_curr_r = encoder_val_r;
  while(enc_curr_l - encoder_val_l < 80) {
    motor_left(-255);
    motor_right(0);
  }
  motor_off();
  while(enc_curr_r - encoder_val_r < 80) {
    motor_left(0);
    motor_right(-255);
  }
  motor_off();
  delay(100);
  while(enc_curr_l - encoder_val_l > 0) {
    motor_left(255);
    motor_right(0);
  }
  motor_off();
  while(enc_curr_r - encoder_val_r > 0) {
    motor_left(0);
    motor_right(255);
  }
  motor_off();
}

void avoid_right(void) {
  int32 enc_curr_l = encoder_val_l;
  int32 enc_curr_r = encoder_val_r;
  while(enc_curr_r - encoder_val_r < 80) {
    motor_left(0);
    motor_right(-255);
  } 
  motor_off();
  while(enc_curr_l - encoder_val_l < 80) {
    motor_left(-255);
    motor_right(0);
  }
  motor_off();
  delay(100);
  while(enc_curr_r - encoder_val_r > 0) {
    motor_left(0);
    motor_right(255);
  }
  motor_off();
  while(enc_curr_l - encoder_val_l > 0) {
    motor_left(255);
    motor_right(0);
  }
  motor_off();
}

