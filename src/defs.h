#ifndef DEF_H
#define DEF_H
#include <stdint.h>

typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t  uint8;

typedef int32_t int32;
typedef int16_t int16;
typedef int8_t  int8;

typedef uint16_t** Maze;

typedef enum {
    N = 0, E = 1, S = 2, W = 3
} Direction;

typedef struct {
  uint8 x;
  uint8 y;
  uint8 z;
  Direction d;
  uint8 start_tile_x;
  uint8 start_tile_y;
} Robot;

#endif
