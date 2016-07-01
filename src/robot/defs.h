#ifndef DEFS_H
#define DEFS_H
#include <WString.h>
//Reset macro
#define CHECK_RESET \
  if(reset_signal) {\
    motor_off();    \
    break;          \
  }                 \

//type defs

#define N 0
#define E 1
#define S 2
#define W 3

typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t  uint8;

typedef int32_t int32;
typedef int16_t int16;
typedef int8_t  int8;

typedef struct {
  uint8 x;
  uint8 y;
  uint8 z;
  uint8 d;
  uint8 start_tile_x;
  uint8 start_tile_y;
  uint8 ramp_begin_x;
  uint8 ramp_begin_y;
  uint8 ramp_end_x;
  uint8 ramp_end_y;
} Robot;

typedef uint16_t** Maze;

typedef enum {
  FORWARD, REVERSE
} motor_dir;

#endif
