#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Servo.h>
#include "defs.h"
#include "pin_def.h"
#include "maze_util.h"

//TPA81
#define TPA81_RIGHT  0x6A
#define TPA81_LEFT   0x68

//variable
#define P_FORWARD 0.5
#define VICTIM_TEMP 40
#define SERVO_START_POS 140
#define SERVO_END_POS 65


//global variable to handle interrupts
int32 encoder_val_l = 0;
int32 encoder_val_r = 0;
motor_dir curr_dir_l   = FORWARD;
motor_dir curr_dir_r   = FORWARD;

//for reset
uint8 reset_signal = 0;
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
  // setup reset button
  attachInterrupt(RESET_BUTTON, reset, FALLING);

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
  int16 mpu_buff[4] = { 0 };

  Robot robot;
  Robot *robot_ptr = &robot;
  init_robot(robot_ptr);
  
  uint16 maze_floor_1[MAZE_SIZE * MAZE_SIZE];
  uint16 maze_floor_2[MAZE_SIZE * MAZE_SIZE];

  for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; ++i) {
    maze_floor_1[i] = 0xFF00u;
    maze_floor_2[i] = 0xFF00u;
  }
  uint16 *maze[] = {maze_floor_1, maze_floor_2};
  
  Servo dropper;
  init_dropper(&dropper);

  while(1) {
    while(!get_left_bumper());
    move_forward_tile(maze, robot_ptr);
  }

  while(!get_right_bumper());
  delay(1000);
  reset_enc();
  for(;;) {
    read_gyro(mpu_buff);
    //read_heat_sensor(&rescue_kit);
    p_sync_forward(255);
    delay(1);
  }
}
