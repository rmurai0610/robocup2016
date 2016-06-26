float read_us(int echo_pin, int trig_pin) {

  float duration, distance;
  float speed;

  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  //reads up to 50 cm
  duration = pulseIn(echo_pin, HIGH, 50*58);
  speed = 331.4;
  distance = (duration / 2) * (speed / 10000);
  return distance;
}


float read_us_l(void) {
  return read_us(ECHO_PIN_L, TRIG_PIN_L);
}

float read_us_r(void) {
  return read_us(ECHO_PIN_R, TRIG_PIN_R);
}

float read_us_fl(void) {
  return read_us(ECHO_PIN_FL, TRIG_PIN_FL);
}

float read_us_r(void) {
  return read_us(ECHO_PIN_FR, TRIG_PIN_FR);
}
