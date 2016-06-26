byte serial_read(void) {
  while(!Serial1.available()) { /* do nothing! */ }
  return Serial1.read();
}

void read_gyro(int16_t *mpu_buff) {
  byte buff[13];
  int check_sum = 0;
  byte d0 = serial_read();
  byte d1 = serial_read();
  for (;!((d0 == 0xaa) && (d1 ==0x00)); d0 = d1, d1 = serial_read()) { /* do nothing! */}
  for(int counter = 0; counter < 13; buff[counter++] = serial_read()) { /*do nothing! */}
  for(int counter = 0; counter < 12; check_sum += buff[counter++]) { /*do nothing! */ }
  if((check_sum & 0xff) == buff[12]) {
    //safe to read the value off sensor
    mpu_buff[0] = (buff[2]  << 8 | buff[1]);
    mpu_buff[1] = (buff[6]  << 8 | buff[5]);
    mpu_buff[2] = (buff[8]  << 8 | buff[7]);
    mpu_buff[3] = (buff[10] << 8 | buff[9]);
  }
}

void reset_mpu(void) {
  digitalWrite(MPU_RESET_PIN, HIGH);
  delay(500);
}
