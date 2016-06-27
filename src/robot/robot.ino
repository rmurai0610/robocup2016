#include <Wire.h>
#include <Servo.h> 

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
#define ECHO_PIN_BR 33
#define ECHO_PIN_BL 24
#define TRIG_PIN_L  9
#define TRIG_PIN_FR 9
#define TRIG_PIN_BR 9
#define TRIG_PIN_R  10
#define TRIG_PIN_FL 10
#define TRIG_PIN_BL 10

//MPU
#define MPU_RESET_PIN 2
//TPA81
#define TPA81_LEFT  0x6A
#define TPA81_RIGHT 0x68
//Servo
#define SERVO_PIN 3
#define SERVO_START_POS 140
#define SERVO_END_POS 65
//variable
#define P_FORWARD 0.5
#define VICTIM_TEMP 40

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
  // start the serial communication
  Serial.begin(115200);
  Serial1.begin(115200);
  // start the i2c communication
  Wire.begin();

  // setup the sensors/motors
  int outputs[] = { PIN_AIN1, PIN_AIN2, PIN_BIN1,
                    PIN_BIN2, PIN_PWMA, PIN_PWMB,
                    TRIG_PIN_R, TRIG_PIN_L,
                    MPU_RESET_PIN };
  int inputs[]  = { ENC_L, ENC_R,
                    ECHO_PIN_L, ECHO_PIN_R, ECHO_PIN_FL, ECHO_PIN_FR, ECHO_PIN_BL, ECHO_PIN_BR };
                    
  for(int i = 0; i < (int) (sizeof(outputs) / sizeof(int)); i++) {
    pinMode(outputs[i], OUTPUT);
  }
  for(int i = 0; i < (int) (sizeof(inputs) / sizeof(int)); i++) {
    pinMode(inputs[i], INPUT);
  }
  
  // setup encoders
  attachInterrupt(ENC_L, enc_l, FALLING);
  attachInterrupt(ENC_R, enc_r, FALLING);
  
  // setup mpu sensor
  delay(500);
  reset_mpu();
}

void loop() {
  int16_t mpu_buff[4] = { 0 };
  Servo rescue_kit;
  rescue_kit.attach(SERVO_PIN);
  rescue_kit.write(SERVO_START_POS);
  
  for(;;) {
    /*
    read_gyro(mpu_buff);
    turn_right(255);
    
    Serial.printf("\nangle: %i\n", mpu_buff[0]);
    
    Serial.printf("acc_x: %i\n",   mpu_buff[1]);
    Serial.printf("acc_y: %i\n",   mpu_buff[2]);
    Serial.printf("acc_z: %i\n",   mpu_buff[3]);
    */
    /*
    Serial.printf("L: %f\n",  read_us(ECHO_PIN_L, TRIG_PIN_L));
    Serial.printf("R: %f\n",  read_us(ECHO_PIN_R, TRIG_PIN_R));
    Serial.printf("FL: %f\n", read_us(ECHO_PIN_FL, TRIG_PIN_FL));
    Serial.printf("FR: %f\n", read_us(ECHO_PIN_FR, TRIG_PIN_FR)); 
    */
    Serial.printf("BL: %f\n", read_us(ECHO_PIN_BL, TRIG_PIN_BL));
    Serial.printf("BR: %f\n", read_us(ECHO_PIN_BR, TRIG_PIN_BR)); 
    /*
    //read_heat_sensor();
    if(mpu_buff[0] > 9000) {
      motor_off();
    }
    delay(100);
    */
    read_heat_sensor(&rescue_kit);
    delay(1000);
  }
  
}
