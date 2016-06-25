extern int64_t encoder_val_l;
extern int64_t encoder_val_r;
extern motor_dir curr_dir_l;
extern motor_dir curr_dir_r;

void enc_l(void) {
  if(curr_dir_l == FORWARD) {
    encoder_val_l++;
  } else {
    encoder_val_l--;
  }
}

void enc_r(void) {
  if(curr_dir_r == FORWARD) {
    encoder_val_r++;
  } else {
    encoder_val_r--;
  }
}

void reset_enc(void) {
  encoder_val_l = 0;
  encoder_val_r = 0;
}

