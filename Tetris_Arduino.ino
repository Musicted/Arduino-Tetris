#include <U8g2lib.h>
#include "Pitches.h"
#include "Tetromino.h"
#include "Input.h"

#define SOUND  5 // Buzzer output pin

/* Bitmaps */
const static unsigned char block[] U8X8_PROGMEM = {0x07, 0x05, 0x07};
const static unsigned char blockInv[] U8X8_PROGMEM = {0x00, 0x02, 0x00};

const int NUMLINES = 40;
const int startSlowDown = 150;
static int slowDown = startSlowDown;
static int score = 0;
static short bricks[NUMLINES];
static Tetromino *current;

/* I2C OLED driver */
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R3);

void setup(void) {
  tone(SOUND, NOTE_B4);
  u8g2.begin();
  tone(SOUND, NOTE_C5, 250);
  playIntro();
  tone(SOUND, NOTE_B4, 250);
  delay(250);
  tone(SOUND, NOTE_C5, 200);
  delay(250);
  tone(SOUND, NOTE_C5, 200);
  u8g2.setBitmapMode(1);
  pinMode(READ_Y, INPUT);
  pinMode(READ_X, INPUT);
  
  spawnTetromino();
}

void loop(void) {
  handleInputs();
  doTheLogicStuff();
  drawAll(); 
}

void handleInputs() {
  bool fUp, fLeft, fRight;
  fUp = true;
  fLeft = true;
  fRight = true;
  for (int i = 0; i < slowDown; i++) {
    switch (determine_control(read_x(), read_y()))
    {
      case UP:
      if (fUp) {
        rotate();
        drawAll();
      }
      fUp = false;
      break;
      
      case LEFT:
      if (fLeft) {
        translate(-1);
        drawAll();
      }
      fLeft = false;
      break;
      
      case RIGHT:
      if (fRight) {
        translate(1);
        drawAll();
      }
      fRight = false;
      break;
      
      case DOWN:
      return;
      
      default:
      fUp = true;
      fLeft = true;
      fRight = true;
    }
    delay(1);
  }
}

void doTheLogicStuff() {
  current->fall();
  if (checkCollision()) {
    current->fallBackwards();
    addTetrominoToBricks();
    tone(SOUND, NOTE_C4, 100);
    eliminateFullLines();
    //delay(50);
    spawnTetromino();
  } else {
    //tone(SOUND, NOTE_C3, 50);
  }
}

/* Draw functions */

void drawAll() {
  u8g2.clearBuffer();
  u8g2.firstPage();
  do {
    drawGrid();
    renderCurrent();
    renderBricks();
    drawScore();
    u8g2.drawFrame(0, 0, 31, 127);
  } while (u8g2.nextPage());
}

void renderCurrent() {
  for (int i = 0; i < 16; i++) {
    bool px = (current->fill >> (15 - i)) & 1;
    if (px) {
      u8g2.drawXBMP((current->xPos + i % 4) * 3 + 1, current->yPos + 3 * (i / 4), 3, 3, block);
    }
  }
}

void renderBricks() {
  for (int i = 0; i < NUMLINES; i++) {
    short line = bricks[i];
    for (int j = 0; j < 10; j++) {
      if (line >> (9 - j) & 1) {
        u8g2.drawXBMP(j * 3 + 1, 127 - 3 * (NUMLINES - i), 3, 3, block);
      }
    }
  }
}

void drawGrid() {
  for (int x = current->xPos; x < current->xPos + current->width; x++) {
    for (int y = (current->yPos/3) * 3 + 8; y < 127; y+=3) {
      u8g2.drawBox(3*x+2, y, 1, 1);
    }
  }
}

void drawScore() {
  u8g2.setFont(u8g2_font_trixel_square_tf);
  u8g2.setCursor(4, 10);
  u8g2.drawHLine(0, 14, 31);
  u8g2.print(score); // also doubles as a debug output
}

void playIntro() {
  for (int i = 30; i >= 0; i--)
  {
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_profont29_tf);
      u8g2.drawStr(0, 57 - i, "TE");
      u8g2.drawStr(0, 77, "TR");
      u8g2.drawStr(0, 97 + i, "IS");
    } while (u8g2.nextPage());
    //delay(20);
  }

  //delay(500);

  for (int i = 0; i < 32; i++)
  {
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
      u8g2.drawFrame(16 - i / 2, 0, i, 127);
    } while (u8g2.nextPage());
  }
}

/* Logic functions */

void translate(int dir) {
  current->xPos += dir;
  if (checkCollision() || current->xPos < 0 || current->xPos + current->width > 10) {
    tone(SOUND, NOTE_B2, 50);
    current->xPos -= dir;
  } else {
    tone(SOUND, NOTE_B3, 50);
  }
}

void rotate() {
  Tetromino *bak = current;
  current = current->rotated();
  int overhang = current->xPos + current->width - 10;
  if (overhang > 0) {
    current->xPos -= overhang;
  }
  if (checkCollision()) {
    free(current);
    current = bak;
    tone(SOUND, NOTE_B2, 50);
  } else {
    tone(SOUND, NOTE_B3, 50);
  }
  free(bak);
  return;
}

bool checkCollision() {
  short lines[] = {(current->fill >> 12) & 15,
                   (current->fill >> 8) & 15,
                   (current->fill >> 4) & 15,
                   current->fill & 15
                  };
  
  int height = 0;
  for (int i = 0; i < 4; i++) {
    if (lines[i] != 0) {
      height++;
    }
  }
  
  if (current->yPos + 3 * height > 127) {
    return true;
  }
  
  int topLine =  NUMLINES - ((127 - current->yPos) / 3);
  if (topLine < 0) {
    return false;
  }
  bool res = false;
  for (int i = 0; i < height; i++) {
    if (current-> xPos <= 6) {
      res = res || (bricks[topLine + i] & (lines[i] << (6 - current->xPos))) > 0;
    } else {
      res = res || (bricks[topLine + i] & (lines[i] >> (current->xPos - 6))) > 0;
    }
  }
  return res;
}

void gameReset() {
  //delay(500);
  for (int i = 0; i < NUMLINES; i++) {
    slowDown = 1;
    u8g2.clearBuffer();
    u8g2.firstPage();
    do {
      drawScore();
      u8g2.drawFrame(0, 0, 31, 127);u8g2.drawFrame(0, 0, 31, 127);
      eliminate(NUMLINES - 1);
      renderBricks();
      //delay(20);
    } while (u8g2.nextPage());
  }
  score = 0;
  slowDown = startSlowDown;
  spawnTetromino();
}

void eliminateFullLines() {
  int cld = 0;
  for (int i = 0; i < NUMLINES; i++) {
    if (bricks[i] == 1023) {
      tone(SOUND, NOTE_B4);
      delay(200);
      for (int j = 0; j <= cld; j++) {
        tone(SOUND, NOTE_C5, 175);
        delay(200);
      }
      delay(200 * (3 - cld));
      score++;
      cld++;
      eliminate(i);
      u8g2.clearBuffer();
      u8g2.firstPage();
      do {
        u8g2.drawFrame(0, 0, 31, 127);
        renderBricks();
        drawScore();
      } while (u8g2.nextPage());
      delay(1200);
    }
  }
}

void eliminate(int line) {
  for (int i = line; i > 0; i--) {
    bricks[i] = bricks[i - 1];
  }
  bricks[0] = 0;
  if (slowDown > 0) {
    slowDown--;
  }
}

void addTetrominoToBricks() {
  short lines[] = {(current->fill >> 12) & 15,
                   (current->fill >> 8) & 15,
                   (current->fill >> 4) & 15,
                   current->fill & 15
                  };
  int height = 0;
  for (int i = 0; i < 4; i++) {
    if (lines[i] != 0) {
      height++;
    }
  }
  int topLine =  NUMLINES - ((127 - current->yPos) / 3);
  for (int i = 0; i < height; i++) {
    if (current-> xPos <= 6) {
      bricks[topLine + i] = bricks[topLine + i] | (lines[i] << (6 - current->xPos));
    } else {
      bricks[topLine + i] = bricks[topLine + i] | (lines[i] >> (current->xPos - 6));
    }
  }

  if (bricks[2] > 0) {
    gameReset();
  }
}

void spawnTetromino() {
  int tetType = random(19);
  free(current);
  current = new Tetromino(0, 1, (Tetromino::Value)tetType);
  current->xPos = random(11 - current->width);
}
