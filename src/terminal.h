// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#pragma once

#include <string.h>
#include <ncurses.h>

#include "coord.h"

void Terminal_init();

void Terminal_exit(void);

// Output methods
void Terminal_clear(void);

void Terminal_refresh(void);

chtype Terminal_readCharAt(Coord pos);

void Terminal_printCharAt(char ch, Coord pos);

void Terminal_printString(char *str);

void Terminal_printStringAt(char *str, Coord pos);

// Input methods
void Terminal_getString(char *str, int bufferSize);

int Terminal_getKey(void);

bool Terminal_keyAny(void);

bool Terminal_keyEnter(void);

bool Terminal_keyLeft(void);

bool Terminal_keyRight(void);

bool Terminal_keyJump(void);

bool Terminal_keyQuit(void);

bool Terminal_keyExit(void);

bool Terminal_keyPaused(void);

bool Terminal_keyMuteMusic(void);

void Terminal_redraw(void);
