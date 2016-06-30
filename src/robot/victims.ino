uint8 read_heat_sensor(int address)
{
  byte heat_sensor_buff[8];
  for (int i = 0; i < 8; i++) {
    Wire.beginTransmission(address);
    Wire.write(i + 2);
    Wire.endTransmission();
    Wire.requestFrom(address, 1);
    while (Wire.available() > 1) { /* do nothing */}
    byte b = Wire.read();
    //read TPA81 and store data onto array
    heat_sensor_buff[i] = b;
    if(b > VICTIM_TEMP) {
      return 1;
    }
  }
  return 0;
}

void check_for_victim(Maze maze, Robot *robot_ptr) {
  uint8 x = robot_ptr->x;
  uint8 y = robot_ptr->y;
  uint8 z = robot_ptr->z;
  uint8 d = robot_ptr->d;
  uint8 vic_l_found = 0;
  uint8 vic_r_found = 0;
  if(read_heat_sensor(TPA81_LEFT)) {
    d += 2;
    d %= 4;
    switch(d) {
      case N:
        if(get_south_victim(maze, x, y, z)) {
          vic_l_found = 1;
        } else {
          set_south_victim(maze, x, y, z, 1);
        }
        break;
      case E:
        if(get_west_victim(maze, x, y, z)) {
          vic_l_found = 1;
        } else {
          set_west_victim(maze, x, y, z, 1);
        }
        break;
      case S:
        if(get_north_victim(maze, x, y, z)) {
          vic_l_found = 1;
        } else {
          set_north_victim(maze, x, y, z, 1);
        }
        break;
      case W:
        if(get_east_victim(maze, x, y, z)) {
          vic_l_found = 1;
        } else {
          set_east_victim(maze, x, y, z, 1);
        }
        break;
    }
    if(!vic_l_found) {
      should_look_for_victim_left = 0;
      should_look_for_victim_right = 0;
      turn_right_90(robot_ptr);
      flash_all(neo_pixel.Color(255, 255, 255), 500);
      drop_rescue_kit();
      for(int i = 0; i < 4; i++) {
        flash_all(neo_pixel.Color(255, 255, 255), 500);
        delay(500);
      }
      turn_left_90(robot_ptr);
      should_look_for_victim_left = 1;
      should_look_for_victim_right = 1;
    }
  }
  if(read_heat_sensor(TPA81_RIGHT)) {
    d += 2;
    d %= 4;
    switch(d) {
      case N:
        if(get_south_victim(maze, x, y, z)) {
          vic_r_found = 1;
        } else {
          set_south_victim(maze, x, y, z, 1);
        }
        break;
      case E:
        if(get_west_victim(maze, x, y, z)) {
          vic_r_found = 1;
        } else {
          set_west_victim(maze, x, y, z, 1);
        }
        break;
      case S:
        if(get_north_victim(maze, x, y, z)) {
          vic_r_found = 1;
        } else {
          set_north_victim(maze, x, y, z, 1);
        }
        break;
      case W:
        if(get_east_victim(maze, x, y, z)) {
          vic_r_found = 1;
        } else {
          set_east_victim(maze, x, y, z, 1);
        }
        break;
    }
    if(!vic_r_found) {
      should_look_for_victim_left = 0;
      should_look_for_victim_right = 0;
      turn_left_90(robot_ptr);
      flash_all(neo_pixel.Color(255, 255, 255), 500);
      drop_rescue_kit();
      for(int i = 0; i < 4; i++) {
        flash_all(neo_pixel.Color(255, 255, 255), 500);
        delay(500);
      }
      turn_right_90(robot_ptr);
      should_look_for_victim_left = 1;
      should_look_for_victim_right = 1;
    }
  }
}

void drop_rescue_kit(void) {
  delay(100);
  for(int i = SERVO_START_POS; i >= SERVO_END_POS; i-- ) {
    dropper.write(i);
    delay(10);
  }
  for(int i = SERVO_END_POS; i <= SERVO_START_POS; i++) {
    dropper.write(i);
    delay(10);
  }
  delay(100);
}

void init_dropper(void) {
  dropper.attach(SERVO_PIN);
  dropper.write(SERVO_START_POS);
  delay(500);
}

