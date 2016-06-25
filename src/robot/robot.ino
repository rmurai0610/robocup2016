#define ENC_R 28
#define ENC_L 29

typedef enum {
  FORWARD, BACKWARD
} motor_dir;

//global variable to handle interrupts
uint64_t encoder_val_l = 0;
uint64_t encoder_val_r = 0;
motor_dir curr_dir_l   = FORWARD;
motor_dir curr_dir_r   = FORWARD;

void setup() {

  Serial.begin(9600);

  // setup the sensors
  int outputs[] = { ENC_L, ENC_R };
  int inputs[] = { ENC_L, ENC_R };

  for(int i = 0; i < (int) (sizeof(outputs) / sizeof(int)); i++) {
    pinMode(outputs[i], OUTPUT);
  }
  for(int i = 0; i < (int) (sizeof(inputs) / sizeof(int)); i++) {
    pinMode(inputs[i], INPUT);
  }

  attachInterrupt(ENC_L, enc_l, FALLING);
  attachInterrupt(ENC_R, enc_r, FALLING);

}

void loop() {

  Serial.println("RUNNING");
  delay(1000);

}
