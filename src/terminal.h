// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#pragma once

#include "coord.h"

enum KeyboardInput {
    MM_KEY_NONE,
    MM_KEY_LEFT,
    MM_KEY_RIGHT,
    MM_KEY_JUMP,
    MM_KEY_LEFT_JUMP,
    MM_KEY_RIGHT_JUMP,
    MM_KEY_ENTER,
    MM_KEY_PAUSE,
    MM_KEY_QUIT,
    MM_KEY_MUTE,
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
