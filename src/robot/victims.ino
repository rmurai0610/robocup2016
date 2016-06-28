void read_heat_sensor(Servo *rescue_kit)
{
  byte heat_sensor_buff[8];
  for (int i = 0; i < 8; i++) {
    Wire.beginTransmission(TPA81_LEFT);
    Wire.write(i + 2);
    Wire.endTransmission();
    Wire.requestFrom(TPA81_LEFT, 1);
    while (Wire.available() > 1) { /* do nothing */}
    byte b = Wire.read();
    //read TPA81 and store data onto array
    heat_sensor_buff[i] = b;
    Serial.printf("temp: %i\n", heat_sensor_buff[i]);
    if(b > VICTIM_TEMP) {
      drop_rescue_kit(rescue_kit);
    }
  }
}

void drop_rescue_kit(Servo *rescue_kit) {
  rescue_kit->attach(SERVO_PIN);
  delay(100);
  for(int i = SERVO_START_POS; i >= SERVO_END_POS; i-- ) {
    rescue_kit->write(i);
    delay(10);
  }
  for(int i = SERVO_END_POS; i <= SERVO_START_POS; i++) {
    rescue_kit->write(i);
    delay(10);
  }
  delay(100);
  rescue_kit->detach();
  delay(1000);
}
