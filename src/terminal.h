// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#pragma once

#include <ncurses.h>
#include <string.h>

#include "coord.h"

// Output methods
void clearScreen(void);

void refreshScreen(void);

chtype readCharAt(Coord pos);

void printCharAt(char ch, Coord pos);

void printString(char *str);

void printStringAt(char *str, Coord pos);

// Input methods
void getString(char *str, int bufferSize);

int getKey(void);

bool keyPressed(void);
