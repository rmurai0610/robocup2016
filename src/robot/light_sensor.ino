int get_light_sensor(void) {
  int val   = 0;
  led_on();
  val = analogRead(L_SENSOR);
  return val; 
}

void led_off(void) {
  digitalWrite(L_SENSOR_LED_PIN, LOW);
}

void led_on(void) {
  digitalWrite(L_SENSOR_LED_PIN, HIGH);
}

