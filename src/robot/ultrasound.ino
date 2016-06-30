float read_us(int echo_pin, int trig_pin) {
  float duration, distance;
  float speed;

  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  //reads up to 50 cm
  duration = pulseIn(echo_pin, HIGH, 50 * 58);
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

float read_us_fr(void) {
  return read_us(ECHO_PIN_FR, TRIG_PIN_FR);
}

float read_us_bl(void) {
  return read_us(ECHO_PIN_BL, TRIG_PIN_BL);
}

float read_us_br(void) {
  return read_us(ECHO_PIN_BR, TRIG_PIN_BR);
}

float read_us_average_l(void) {
  float acc_data = 0.0;
  uint8 valid_data = 0;
  for(int i = 0; i < 5; i++) {
    float temp_val = read_us_l();
    delay(10);
    if(temp_val > 1.0) {
      acc_data += temp_val;
      valid_data++;
    } else {
      return 0;
    }
  }
  // avoid div by 0
  if(valid_data) {
    return acc_data / valid_data;
  }
  return 0;
}

float read_us_average_r(void) {
  float acc_data = 0.0;
  uint8 valid_data = 0;
  for(int i = 0; i < 5; i++) {
    float temp_val = read_us_r();
    delay(10);
    if(temp_val > 1.0) {
      acc_data += temp_val;
      valid_data++;
    } else {
      return 0;
    }
  }
  // avoid div by 0
  if(valid_data) {
    return acc_data / valid_data;
  }
  return 0;
}

float read_us_average_fl(void) {
  float acc_data = 0.0;
  uint8 valid_data = 0;
  for(int i = 0; i < 5; i++) {
    float temp_val = read_us_fl();
    delay(10);
    if(temp_val > 1.0) {
      acc_data += temp_val;
      valid_data++;
    } else {
      return 0;
    }
  }
  // avoid div by 0
  if(valid_data) {
    return acc_data / valid_data;
  }
  return 0;
}

float read_us_average_fr(void) {
  float acc_data = 0.0;
  uint8 valid_data = 0;
  for(int i = 0; i < 5; i++) {
    float temp_val = read_us_fr();
    delay(10);
    if(temp_val > 1.0) {
      acc_data += temp_val;
      valid_data++;
    } else {
      return 0;
    }
  }
  // avoid div by 0
  if(valid_data) {
    return acc_data / valid_data;
  }
  return 0;
}

float read_us_average_bl(void) {
  float acc_data = 0.0;
  uint8 valid_data = 0;
  for(int i = 0; i < 5; i++) {
    float temp_val = read_us_bl();
    delay(10);
    if(temp_val > 1.0) {
      acc_data += temp_val;
      valid_data++;
    } else {
      return 0;
    }
  }
  // avoid div by 0
  if(valid_data) {
    return acc_data / valid_data;
  }
  return 0;
}


float read_us_average_br(void) {
  float acc_data = 0.0;
  uint8 valid_data = 0;
  for(int i = 0; i < 5; i++) {
    float temp_val = read_us_br();
    delay(10);
    if(temp_val > 1.0) {
      acc_data += temp_val;
      valid_data++;
    } else {
      return 0;
    }
  }
  // avoid div by 0
  if(valid_data) {
    return acc_data / valid_data;
  }
  return 0;
}
