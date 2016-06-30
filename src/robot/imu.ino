byte serial_read(void) {
  while (!Serial1.available()) { /* do nothing! */ }
  return Serial1.read();
}

void read_gyro(int16_t *imu_buff) {
  byte buff[13];
  int check_sum = 0;
  uint8 succ = 0;
  for (int i = 0; i < 3; i++) {
    byte d0 = serial_read();
    byte d1 = serial_read();
    for (; !((d0 == 0xaa) && (d1 == 0x00)); d0 = d1, d1 = serial_read()) { /* do nothing! */}
    for (int counter = 0; counter < 13; buff[counter++] = serial_read()) {/*do nothing! */}
    for (int counter = 0; counter < 12; check_sum += buff[counter++]) {/*do nothing! */}
    Serial1.flush();
    if ((check_sum & 0xff) == buff[12]) {
      //safe to read the value off sensor
      imu_buff[0] = (buff[2]  << 8 | buff[1]);
      imu_buff[1] = (buff[6]  << 8 | buff[5]);
      imu_buff[2] = (buff[8]  << 8 | buff[7]);
      imu_buff[3] = (buff[10] << 8 | buff[9]);
      succ = 1;
    }
  }
  if(!succ) {
    Serial.printf("FAILED!\n");
  }
}

void reset_imu(void) {
  digitalWrite(IMU_RESET_PIN, LOW);
  digitalWrite(IMU_RESET_PIN, HIGH);
  delay(500);
}

//calculate the difference between 2 given angles
int16_t delta_angle(int16_t init_angle, int16_t curr_angle) {
  int16_t d_angle = curr_angle - init_angle;
  if(d_angle > 18000) {
    d_angle -= 36000;
  }
  if(d_angle < -18000) {
    d_angle += 36000;
  }
  return d_angle;
}

//calculate the pitch
float get_pitch(void) {
  const float alpha = 0.5;
  int16 imu_buff[4] = { 0 };
  read_gyro(imu_buff);
  static int16_t fx;
  static int16_t fy;
  static int16_t fz;
  fx = imu_buff[1] * alpha + (fx * (1.0 - alpha));
  fy = imu_buff[2] * alpha + (fy * (1.0 - alpha));
  fz = imu_buff[3] * alpha + (fz * (1.0 - alpha));
  return (atan2(fx, sqrt(fy * fy + fz * fz)) * 180.0) / M_PI;
}
