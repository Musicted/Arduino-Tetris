#ifndef Tetromino
class Tetromino {
  public:
    enum Value
    {
      I,
      I_90,
      O,
      L,
      L_90,
      L_180,
      L_270,
      J,
      J_90,
      J_180,
      J_270,
      Z,
      Z_90,
      S,
      S_90,
      T,
      T_90,
      T_180,
      T_270
    };

    int xPos, yPos, width;
    short fill;
    Tetromino::Value next;

    Tetromino(int, int, Tetromino::Value);
    void fall();
    void fallBackwards();
    Tetromino *rotated();
};
#endif
