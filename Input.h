#ifndef Input
#define Input

#include <Arduino.h>

/* Pin definitions */
#define READ_Y A0
#define READ_X A1

/* calibration */
#define CALIBRATE_Y 518 // Y axis center point
#define CALIBRATE_X 480 // X axis center point
#define MULT_Y 1        // Y axis multiplier (useful for inverting)
#define MULT_X 1        // X axis multiplier

enum dir 
{
  NONE,
  UP,
  DOWN,
  LEFT,
  RIGHT
};
dir determine_control(int x, int y);
int read_y();
int read_x();

#endif
