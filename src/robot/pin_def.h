#ifndef PIN_DEF
#define PIN_DEF
//MOTOR
#define STAND_BY  23
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
//IMU
#define IMU_RESET_PIN 2
//Servo
#define SERVO_PIN 3
//NeoPixle
#define NEO_PIXEL_PIN 4
//Light sensor
#define L_SENSOR_LED_PIN 17
#define L_SENSOR         16
//Switch / Bumper
#define R_BUMPER_PIN 22
#define L_BUMPER_PIN 21
#define RESET_BUTTON 20
#endif
