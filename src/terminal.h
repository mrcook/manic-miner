// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#ifndef MANIC_MINER_TERMINAL_H
#define MANIC_MINER_TERMINAL_H


#include "coord.h"

enum KeyboardInput {
    T_KEY_NONE,
    T_KEY_SPACE,
    T_KEY_LEFT,
    T_KEY_RIGHT,
    T_KEY_UP,
    T_KEY_DOWN,
    T_KEY_ENTER,
};

void Terminal_init();

void Terminal_exit(void);

// Output methods
void Terminal_clear(void);

void Terminal_refresh(void);

char Terminal_readCharAt(Coord pos);

void Terminal_printCharAt(char ch, Coord pos);

void Terminal_printString(char *str);

void Terminal_printStringAt(char *str, Coord pos);

// Input methods
void Terminal_getString(char *str, int bufferSize);

int Terminal_getKey(void);

void Terminal_redraw(void);


#endif //MANIC_MINER_TERMINAL_H
