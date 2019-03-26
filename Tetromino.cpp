#include "Tetromino.h"

Tetromino::Tetromino(int xPos, int yPos, Tetromino::Value value) {
  this->xPos = xPos;
  this->yPos = yPos;

  switch (value)
  {
    case I :
      this->width = 1;
      this->fill = 0B1000100010001000;
      this->next = I_90;
      break;
    case I_90:
      this->width = 4 ;
      this->fill = 0B1111000000000000;
      this->next = I;
      break;
    case O :
      this->width = 2;
      this->fill = 0B1100110000000000;
      this->next = O;
      break;
    case L :
      this->width = 2;
      this->fill = 0B1000100011000000;
      this->next = L_90;
      break;
    case L_90 :
      this->width = 3;
      this->fill = 0B0010111000000000;
      this->next = L_180;
      break;
    case L_180 :
      this->width = 2;
      this->fill = 0B1100010001000000;
      this->next = L_270;
      break;
    case L_270 :
      this->width = 3;
      this->fill = 0B1110100000000000;
      this->next = L;
      break;
    case J :
      this->width = 2;
      this->fill = 0B0100010011000000;
      this->next = J_90;
      break;
    case J_90 :
      this->width = 3;
      this->fill = 0B1110001000000000;
      this->next = J_180;
      break;
    case J_180 :
      this->width = 2;
      this->fill = 0B1100100010000000;
      this->next = J_270;
      break;
    case J_270 :
      this->width = 3;
      this->fill = 0B1000111000000000;
      this->next = J;
      break;
    case S :
      this->width = 3;
      this->fill = 0B0110110000000000;
      this->next = S_90;
      break;
    case S_90 :
      this->width = 2;
      this->fill = 0B1000110001000000;
      this->next = S;
      break;
    case Z :
      this->width = 3;
      this->fill = 0B1100011000000000;
      this->next = Z_90;
      break;
    case Z_90 :
      this->width = 2;
      this->fill = 0B0100110010000000;
      this->next = Z;
      break;
    case T :
      this->width = 3;
      this->fill = 0B1110010000000000;
      this->next = T_90;
      break;
    case T_90 :
      this->width = 2;
      this->fill = 0B1000110010000000;
      this->next = T_180;
      break;
    case T_180 :
      this->width = 3;
      this->fill = 0B0100111000000000;
      this->next = T_270;
      break;
    case T_270 :
      this->width = 2;
      this->fill = 0B0100110001000000;
      this->next = T;
      break;
  };
}

void Tetromino::fall() {
  this->yPos += 3;
}

void Tetromino::fallBackwards() {
  this->yPos -= 3;
}

Tetromino *Tetromino::rotated() {
  Tetromino *res = new Tetromino(xPos, yPos, next);
  return res;
}
