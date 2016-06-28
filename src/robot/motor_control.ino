void move_forward_tile(Maze maze, Robot *robot_ptr){
  return;
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
  if(encoder_val_l > encoder_val_r) {
    diff_encoder = encoder_val_l - encoder_val_r;
    p_pow_l -= (int16) (diff_encoder * P_FORWARD);
  } else {
    diff_encoder = encoder_val_r - encoder_val_l;
    p_pow_r -= (int16) (diff_encoder * P_FORWARD);
  }
  motor_left(p_pow_l);
  motor_right(p_pow_r);
}

void turn_right_90(Robot *robot_ptr) {
  robot_ptr->d += 1;
  robot_ptr->d %= 4;
  int16 mpu_buff[4];
  read_gyro(mpu_buff);

  int16 init_angle = mpu_buff[0];
  //eg 9000, 0 = 9000
  //   9000, 18000 = 9000
  //   -18000,
  int16 target_angle = delta_angle(init_angle, get_target_angle(robot_ptr->d));
  Serial.printf("TARGET_ANGLE:%i\n", target_angle);
  int16 d_angle;
  Serial.println(mpu_buff[0]);
  for(;;) {
    CHECK_RESET;
    read_gyro(mpu_buff);
    d_angle = delta_angle(init_angle, mpu_buff[0]);
    Serial.printf("ANGLE_turned:%i\n", d_angle);
    d_angle = delta_angle(d_angle, target_angle);
    Serial.printf("ANGLE_LEFT:%i\n", d_angle);
    if(d_angle < 100) {
      if(d_angle > -100) {
        motor_off();
        break;
      } else {
        turn_left(255);
      }
    } else {
      turn_right(255);
    }
    delay(10);
  }
}

void turn_left_90(Robot *robot_ptr) {

  if(!robot_ptr->d) {
    //if robot_ptr->d = 0, force it to be West
    robot_ptr->d = W;
  } else {
    robot_ptr->d -= 1;
  }
  int16 target_angle = get_target_angle(robot_ptr->d);
  int16 mpu_buff[4];
  read_gyro(mpu_buff);
  int16 init_angle = mpu_buff[0];
  int16 d_angle;
  for(;;) {
    CHECK_RESET;
    read_gyro(mpu_buff);
    d_angle = delta_angle(init_angle, mpu_buff[0]);
    d_angle = delta_angle(target_angle, d_angle);
    if(-100 <= d_angle && d_angle <= 100) {
      motor_off();
      break;
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

