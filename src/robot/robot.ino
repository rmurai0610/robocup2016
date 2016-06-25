//Motor L
#define PIN_AIN1  31 //Direction
#define PIN_AIN2  30 //Direction
#define PIN_PWMA  32 //Speed
//Motor R
#define PIN_BIN1  26 //Direction
#define PIN_BIN2  27 //Direction
#define PIN_PWMB  25 //Speed
//Encoder
#define ENC_R 28
#define ENC_L 29
//Ultrasound Sensor
#define ECHO_PIN_L  11
#define ECHO_PIN_R  15
#define ECHO_PIN_FL 12
#define ECHO_PIN_FR 14
#define TRIG_PIN_L  9
#define TRIG_PIN_FR 9
#define TRIG_PIN_R  10
#define TRIG_PIN_FL 10
//variable
#define P_FORWARD 0.5

typedef enum {
  FORWARD, REVERSE
} motor_dir;

//global variable to handle interrupts
int64_t encoder_val_l = 0;
int64_t encoder_val_r = 0;
motor_dir curr_dir_l   = FORWARD;
motor_dir curr_dir_r   = FORWARD;

void setup() {
  delay(100);
  Serial.begin(9600);

  // setup the sensors/motors
  int outputs[] = { PIN_AIN1, PIN_AIN2, PIN_BIN1,
                    PIN_BIN2, PIN_PWMA, PIN_PWMB,
                    TRIG_PIN_R, TRIG_PIN_L };
  int inputs[]  = { ENC_L, ENC_R,
                    ECHO_PIN_L, ECHO_PIN_R, ECHO_PIN_FL, ECHO_PIN_FR };

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
  delay(100);
  Serial.printf("L: %f\n", read_us(ECHO_PIN_L, TRIG_PIN_L));
  Serial.printf("R: %f\n", read_us(ECHO_PIN_R, TRIG_PIN_R));
  Serial.printf("FL: %f\n", read_us(ECHO_PIN_FL, TRIG_PIN_FL));
  Serial.printf("FR: %f\n", read_us(ECHO_PIN_FR, TRIG_PIN_FR));
 }
