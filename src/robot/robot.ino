 #include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Servo.h>
#include "defs.h"
#include "pin_def.h"

//TPA81
#define TPA81_RIGHT  0x6A
#define TPA81_LEFT   0x68

//variable
#define P_FORWARD 0.5
#define VICTIM_TEMP 40
#define SERVO_START_POS 140
#define SERVO_END_POS 65


//global variable to handle interrupts
int64_t encoder_val_l = 0;
int64_t encoder_val_r = 0;
motor_dir curr_dir_l   = FORWARD;
motor_dir curr_dir_r   = FORWARD;

//for reset
uint8_t reset_signal = 0;
//To allow any function to access neo pixel easily
Adafruit_NeoPixel neo_pixel = Adafruit_NeoPixel(8, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void init_robot(Robot *robot) {
  robot->x = 0;
  robot->y = 0;
  robot->z = 0;
  robot->d = N;
  robot->start_tile_x = 0;
  robot->start_tile_y = 0;
}

void setup() {

  // start the serial communication
  Serial.begin(9600);
  Serial1.begin(115200);

  // start the i2c communication
  Wire.begin();

  // setup the sensors/motors
  int outputs[] = { PIN_AIN1, PIN_AIN2, PIN_BIN1,
                    PIN_BIN2, PIN_PWMA, PIN_PWMB, STAND_BY,
                    TRIG_PIN_R, TRIG_PIN_L,
                    MPU_RESET_PIN, L_SENSOR_LED_PIN };
  int inputs[]  = { ENC_L, ENC_R,
                    ECHO_PIN_L, ECHO_PIN_R, ECHO_PIN_FL, ECHO_PIN_FR, ECHO_PIN_BL, ECHO_PIN_BR,
                    L_BUMPER_PIN, R_BUMPER_PIN, RESET_BUTTON };

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

  // setup neo_pixel
  neo_pixel.begin();
  neo_pixel.show();
  motor_start();
  //
  neo_pixel.setPixelColor(0, neo_pixel.Color(0, 0, 255));
  neo_pixel.show();
  delay(500);
  neo_pixel.setPixelColor(0, 0);
  neo_pixel.show();
  delay(100);
  Serial.println("Running");
}

void loop() {
  int16_t mpu_buff[4] = { 0 };
  neo_pixel.setPixelColor(0, neo_pixel.Color(0, 0, 255));
  neo_pixel.show();
  delay(500);
  neo_pixel.setPixelColor(0, 0);
  neo_pixel.show();
  /*
  Servo rescue_kit;
  rescue_kit.attach(SERVO_PIN);
  rescue_kit.write(SERVO_START_POS);
  delay(1000);
  rescue_kit.detach();
  */
  /*
  for(;;) {
    Serial.println(digitalRead(20));
    delay(500);
    for(uint16_t i=0; i<neo_pixel.numPixels(); i++) {
      neo_pixel.setPixelColor(i, neo_pixel.Color(0, 0, 255));
      neo_pixel.setPixelColor(neo_pixel.numPixels() - i, neo_pixel.Color(255, 0, 0));
      neo_pixel.show();
      delay(50);
    }
    for(uint16_t i=0; i<neo_pixel.numPixels(); i++) {
      neo_pixel.setPixelColor(i, neo_pixel.Color(255, 0, 0));
      neo_pixel.setPixelColor(neo_pixel.numPixels() - i, neo_pixel.Color(0, 0, 255));
      neo_pixel.show();
      delay(50);
    }

  } */

  Robot robot;
  Robot *robot_ptr = &robot;
  init_robot(robot_ptr);
  /*
  while(1) {
    turn_right_90(robot_ptr);
    delay(500);
    read_gyro(mpu_buff);
    Serial.printf("\nangle: %i\n", mpu_buff[0]);
    Serial.printf("D: %i\n", robot_ptr->d);
    delay(1000);
  }
  */

  for(;;) {

    read_gyro(mpu_buff);

    Serial.printf("\nangle: %i\n", mpu_buff[0]);
    Serial.printf("acc_x: %i\n",   mpu_buff[1]);
    Serial.printf("acc_y: %i\n",   mpu_buff[2]);
    Serial.printf("acc_z: %i\n",   mpu_buff[3]);

    Serial.printf("Light sensor %i\n", get_light_sensor());

    //read_heat_sensor(&rescue_kit);
    //p_sync_forward(255);
    delay(1);
  }

}
