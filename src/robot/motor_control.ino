extern
void limit_motor_speed(int *pow) {
  if(*pow > 255) {
    *pow = 255;
  }
  if(*pow < -255) {
    *pow = -255;
  }
}

void p_sync_forward(int pow) {
  int p_pow_l = pow;
  int p_pow_r = pow;
  int64_t diff_encoder;
  if(encoder_val_l > encoder_val_r) {
    diff_encoder = encoder_val_l - encoder_val_r;
    p_pow_l -= (int) (diff_encoder * P_FORWARD);
  } else {
    diff_encoder = encoder_val_r - encoder_val_l;
    p_pow_r -= (int) (diff_encoder * P_FORWARD);
  }
  motor_left(p_pow_l);
  motor_right(p_pow_r);
}

void turn_right_90(Robot *robot_ptr) {
  robot_ptr->d += 1;
  robot_ptr->d %= 4;
  int16_t mpu_buff[4];
  read_gyro(mpu_buff);
  int16_t target_angle = get_target_angle(robot_ptr->d);
  int16_t init_angle = mpu_buff[0];
  int16_t d_angle;
  Serial.println(mpu_buff[0]);
  for(;;) {
    CHECK_RESET;
    read_gyro(mpu_buff);
    d_angle = delta_angle(init_angle, mpu_buff[0]);
    d_angle = delta_angle(target_angle, d_angle);
    //allow 1 degree diff
    if(-100 <= d_angle && d_angle <= 100) {
      motor_off();
      break;
    }
    (d_angle < 0) ? turn_right(255) : turn_left(255);
  }
}

void turn_left_90(Robot *robot_ptr) {

  if(!robot_ptr->d) {
    //if robot_ptr->d = 0, force it to be West
    robot_ptr->d = W;
  } else {
    robot_ptr->d -= 1;
  }
  int16_t target_angle = get_target_angle(robot_ptr->d);
  int16_t mpu_buff[4];
  read_gyro(mpu_buff);
  int16_t init_angle = mpu_buff[0];
  int16_t d_angle;
  for(;;) {
    CHECK_RESET;
    read_gyro(mpu_buff);
    d_angle = delta_angle(init_angle, mpu_buff[0]);
    d_angle = delta_angle(target_angle, d_angle);
    if(-100 <= d_angle && d_angle <= 100) {
      motor_off();
      break;
    }
    (d_angle < 0) ? turn_left(255) : turn_right(255);
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

