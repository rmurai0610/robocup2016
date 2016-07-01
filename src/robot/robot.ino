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
#define P_FORWARD          0.5
#define VICTIM_TEMP        35
#define SERVO_START_POS    140
#define SERVO_END_POS      65
#define DIST_WALL          20
#define RAMP_ANGLE         14
#define SILVER             150
#define BLACK              50

//global variable to handle interrupts
int32 encoder_val_l = 0;
int32 encoder_val_r = 0;
motor_dir curr_dir_l   = FORWARD;
motor_dir curr_dir_r   = FORWARD;

//global variable for map / robot backup
uint16 maze_floor_1_copy[MAZE_SIZE * MAZE_SIZE];
uint16 maze_floor_2_copy[MAZE_SIZE * MAZE_SIZE];
uint16 *maze_copy[] = {maze_floor_1_copy, maze_floor_2_copy};
Robot robot_copy;

//for reset
uint8 reset_signal = 0;
//To allow any function to access neo pixel and servo easily
Adafruit_NeoPixel neo_pixel = Adafruit_NeoPixel(8, NEO_PIXEL_PIN, NEO_GRB + NEO_KHZ800);
Servo dropper;
void calibrate_mode() {
  while(1) {
    if(read_heat_sensor(TPA81_LEFT)) {
      flash_all(neo_pixel.Color(0, 0, 255), 500);
    }
    if(read_heat_sensor(TPA81_RIGHT)) {
      flash_all(neo_pixel.Color(0, 0, 255), 500);
    }
  }
}

void init_robot(Robot *robot) {
  robot->x = 1;
  robot->y = 1;
  robot->z = 0;
  robot->d = N;
  robot->start_tile_x = 1;
  robot->start_tile_y = 1;

}

void reset_robot(Maze maze, Robot *robot) {
  robot->x            = robot_copy.x;
  robot->y            = robot_copy.y;
  robot->z            = robot_copy.z;
  robot->d            = robot_copy.d;
  robot->start_tile_x = robot_copy.start_tile_x;
  robot->start_tile_y = robot_copy.start_tile_y;
  //maybe write a function to confirm this fact?
  robot->d = N;
  for(int z = 0; z < 2; z++) {
    for(int y = 0; y < MAZE_Y; y++) {
      for(int x = 0; x < MAZE_X; x++) {
          maze[z][x + y * MAZE_X] = maze_copy[z][x + y * MAZE_X];
      }
    }
  }
  while(!get_left_bumper());
  reset_signal = 0;
  color_wipe(neo_pixel.Color(255, 0, 0), 100);
  delay(100);
  neo_pixel_clear();
  //umm???/////////////////////////////////////////////////////////
  for(int i = 0; i < 5; i++) {
    reset_imu();
  }
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
                    IMU_RESET_PIN, L_SENSOR_LED_PIN };
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
  // setup neo_pixel
  neo_pixel.begin();
  neo_pixel.show();
  motor_start();
  color_wipe(neo_pixel.Color(0, 0,  255), 100);
  delay(100);
  neo_pixel_clear();

  // setup imu sensor
  delay(500);
  reset_imu();
  Serial.println("Running");
}

void loop() {
  int16 imu_buff[4] = { 0 };

  Robot robot;
  Robot *robot_ptr = &robot;
  init_robot(robot_ptr);

  uint16 maze_floor_1[MAZE_SIZE * MAZE_SIZE];
  uint16 maze_floor_2[MAZE_SIZE * MAZE_SIZE];

  for (int i = 0; i < MAZE_SIZE * MAZE_SIZE; ++i) {
    maze_floor_1[i] = 0xFF00u;
    maze_floor_2[i] = 0xFF00u;
  }
  uint16 *maze[]      = {maze_floor_1, maze_floor_2};

  /*
  while(1) {
    Serial.printf("LIGHT: %i\n", get_light_sensor());
    Serial.printf("L:%f\n",read_us_average_l());
    delay(10);
    Serial.printf("R:%f\n",read_us_average_r());
    delay(10);
    Serial.printf("FL:%f\n",read_us_average_fl());
    delay(10);
    Serial.printf("FR:%f\n",read_us_average_fr());
    delay(10);
    Serial.printf("BL:%f\n",read_us_average_bl());
    delay(10);
    Serial.printf("BR:%f\n",read_us_average_br());
    delay(1000);
  }    */
  
  init_dropper();
  reset_enc();
  flash_all(neo_pixel.Color(0, 0, 255), 500);
  while(!get_left_bumper());
  delay(1000);
  reset_imu();
  reset_enc();
   
  for(;;) {
    //Serial.printf("robot x:%i y:%i z:%i\n", robot_ptr->x, robot_ptr->y, robot_ptr->z);
    /*
   for(int y = 0; y < 10; y++) {
      for(int x = 0; x < 10; x++) {
        Serial.printf("%i ", (uint8) maze[robot_ptr->z][robot_ptr->x + robot_ptr->y * MAZE_X]);
      }
      Serial.printf("\n");
    }  */
    Serial.printf("ROBOT: x->%i, y->%i, z->%i, d->%i\n", robot_ptr->x, robot_ptr->y, robot_ptr->z, robot_ptr->d);
    update_wall(maze, robot_ptr);
    if(maze_solver(maze, robot_ptr)  && !reset_signal) {
      //finished maze
      for(;;) {
        rainbow(100);
      }
    }

    if(reset_signal) {
      reset_robot(maze, robot_ptr);
    }
    flash_all(neo_pixel.Color(255, 0, 255), 500);
  }
}
