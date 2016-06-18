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
    N, E, S, W
} Direction;

typedef struct {
  uint8 x;
  uint8 y;
  uint8 z;
  Direction d;
} Robot;

#endif
