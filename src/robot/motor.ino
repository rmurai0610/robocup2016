void motor_left(int pow) {
  if(pow < 0) {
    digitalWrite(PIN_AIN1, HIGH);
    digitalWrite(PIN_AIN2, LOW);
    analogWrite(PIN_PWMA, -pow);
    curr_dir_l = REVERSE;
  } else {
    digitalWrite(PIN_AIN2, HIGH);
    digitalWrite(PIN_AIN1, LOW);
    analogWrite(PIN_PWMA, pow);
    curr_dir_l = FORWARD;
  }
}

void motor_right(int pow) {
  if(pow < 0) {
    digitalWrite(PIN_BIN2, HIGH);
    digitalWrite(PIN_BIN1, LOW);
    analogWrite(PIN_PWMB, -pow);
    curr_dir_r = REVERSE;
  } else {
    digitalWrite(PIN_BIN1, HIGH);
    digitalWrite(PIN_BIN2, LOW);
    analogWrite(PIN_PWMB, pow);
    curr_dir_r = FORWARD;
  }
}

void turn_left(int pow) {
  motor_right(pow);
  motor_left(-pow);
}

void turn_right(int pow) {
  motor_right(-pow);
  motor_left(pow);
}

void move_forward(int pow) {
  motor_right(pow);
  motor_left(pow);
}

void move_backward(int pow) {
  motor_right(-pow);
  motor_left(-pow);
}

void motor_left_off(void) {
  digitalWrite(PIN_AIN1, LOW);
  digitalWrite(PIN_AIN2, LOW);
}


void motor_right_off(void) {
  digitalWrite(PIN_BIN1, LOW);
  digitalWrite(PIN_BIN2, LOW);
}


void motor_off(void) {
  motor_right_off();
  motor_left_off();
}

void motor_start(void) {
  digitalWrite(STAND_BY, HIGH);
}

