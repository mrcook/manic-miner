// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#include "headers.h"
#include "terminal.h"

bool cursesStarted = false;

void initTerminal(int rows, int columns) {
    if (cursesStarted) {
        refresh();
        return;
    }

    initscr();

    // Make sure our terminal is big enough
    if (rows > LINES || columns > COLS) {
        printf("Your terminal is not big enough!\n");
        exit(-1);
    }

    raw();
    clearScreen();
    noecho();
    cbreak();
    keypad(stdscr, false);
    curs_set(0);

    clearScreen();
    refresh();
    cursesStarted = true;
}

void exitTerminal() {
    // move curses to bottom right corner
    int y = 0;
    int x = 0;
    getyx(stdscr, y, x);
    mvcur(y, x, LINES - 1, 0);

    // exit curses
    endwin();
    fflush(stdout);
}

void clearScreen() {
    clear();
    refresh();
}

void refreshScreen() {
    refresh();
}

chtype readCharAt(Coord pos) {
    return mvinch(pos.Y, pos.X);
}

void printCharAt(char ch, Coord pos) {
    mvaddch(pos.Y, pos.X, ch);
}

void printString(char *str) {
    addstr(str);
}

void printStringAt(char *str, Coord pos) {
    mvaddstr(pos.Y, pos.X, str);
}

void getString(char *str, int bufferSize) {
    getnstr(str, bufferSize);
}

int getKey() {
    return getch();
}

bool keyPressed() {
    return getch() != ERR;
}
