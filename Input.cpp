#include "Input.h"

dir determine_control(int x, int y) {
  x = (x - CALIBRATE_X) * MULT_X;
  y = (y - CALIBRATE_Y) * MULT_Y;
  if (abs(x) + abs(y) > 300) { // joystick is sufficiently off-center
    if (abs(x) > abs(y)) {
      if (x > 0) {
        return DOWN;
      } else {
        return UP;
      }
    } else {
      if (y > 0) {
        return LEFT;
      } else {
        return RIGHT;
      }
    }
  }
  return NONE;
}

int read_y() {
  return analogRead(READ_Y);
}

int read_x() {
  return analogRead(READ_X);
}
