// Manic Miner C Port Copyright 2017 Michael R. Cook

#ifndef MANIC_MINER_COLOUR_H
#define MANIC_MINER_COLOUR_H


#include "headers.h"

// The fixed colours within the game
// A= 39 (INK 7: PAPER 4: BRIGHT 0)  =  INK: WHITE  / PAPER: GREEN
// A= 40 (INK 0: PAPER 5: BRIGHT 0)  =  INK: BLACK  / PAPER: CYAN
// A= 47 (INK 7: PAPER 5: BRIGHT 0)  =  INK: WHITE  / PAPER: CYAN
// A= 56 (INK 0: PAPER 7: BRIGHT 0)  =  INK: BLACK  / PAPER: WHITE
// A= 63 (INK 7: PAPER 7: BRIGHT 0)  =  INK: WHITE  / PAPER: WHITE

// A= 68 (INK 4: PAPER 0: BRIGHT 1)  =  INK: GREEN  / PAPER: BLACK
// A= 69 (INK 5: PAPER 0: BRIGHT 1)  =  INK: CYAN   / PAPER: BLACK
// A= 70 (INK 6: PAPER 0: BRIGHT 1)  =  INK: YELLOW / PAPER: BLACK
// A= 71 (INK 7: PAPER 0: BRIGHT 1)  =  INK: WHITE  / PAPER: BLACK
// A= 80 (INK 0: PAPER 2: BRIGHT 1)  =  INK: WHITE  / PAPER: RED
// A=119 (INK 7: PAPER 6: BRIGHT 1)  =  INK: WHITE  / PAPER: YELLOW

typedef struct Colour_ {
    bool flash;
    bool BRIGHT;
    uint8_t PAPER;
    uint8_t INK;
} Colour;

enum GameColours {
  //              NCURSES       #
  BLACK   = 0, // COLOR_BLACK   0
  BLUE    = 1, // COLOR_BLUE    4
  RED     = 2, // COLOR_RED     1
  MAGENTA = 3, // COLOR_MAGENTA 5
  GREEN   = 4, // COLOR_GREEN   2
  CYAN    = 5, // COLOR_CYAN    6
  YELLOW  = 6, // COLOR_YELLOW  3
  WHITE   = 7, // COLOR_WHITE   7
};


#endif //MANIC_MINER_COLOUR_H
