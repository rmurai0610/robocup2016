float read_us(int echo_pin, int trig_pin) {
  
  float duration, distance;
  float speed;

  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  duration = pulseIn(echo_pin, HIGH, 50*58);
  speed = 331.4;
  distance = (duration / 2) * (speed / 10000);
  return distance;
}
