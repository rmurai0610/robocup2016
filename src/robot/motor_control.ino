void limit_motor_speed(int *pow) {
  if(*pow > 255) {
    *pow = 255;
  }
  if(*pow < -255) {
    *pow = -255;
  }
}

void p_sync_forward(int pow) {
  int p_pow_l = pow;
  int p_pow_r = pow;
  int64_t diff_encoder;
  if(encoder_val_l > encoder_val_r) {
    diff_encoder = encoder_val_l - encoder_val_r;
    p_pow_l -= (int) (diff_encoder * P_FORWARD);
  } else {
    diff_encoder = encoder_val_r - encoder_val_l;
    p_pow_r -= (int) (diff_encoder * P_FORWARD);
  }
  motor_left(p_pow_l);
  motor_right(p_pow_r);
}

