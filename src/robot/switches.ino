//returns 1 when bumper hits something
uint8_t get_left_bumper(void) {
  return (1 - digitalRead(L_BUMPER_PIN));
}

uint8_t get_right_bumper(void) {
  return (1 - digitalRead(R_BUMPER_PIN));
}

void reset(void) {
  int8 val = 0;
  for(int8 i = 0; i < 50; i++) {
    if(!digitalRead(RESET_BUTTON)) {
      val++;
    }
  }
  if(val > 45) {
    reset_signal = 1;
  }
}

