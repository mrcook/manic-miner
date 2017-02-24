// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#include "headers.h"
#include "externs.h"
#include "terminal.h"

// keep ncurses functions private to this file
#include <ncurses.h>

#ifdef _WIN32
// this is defined in Windows and also in ncurses
#undef KEY_EVENT
#endif


void Terminal_init() {
    static bool cursesStarted = false;

    if (cursesStarted) {
        Terminal_refresh();
        return;
    }

    initscr();

    // Make sure our terminal is big enough
//    if (rows > LINES || columns > COLS) {
//        printf("Your terminal is not big enough!\n");
//        exit(-1);
//    }

    raw();
    Terminal_clear();
    noecho();
    cbreak();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    curs_set(0);

    Terminal_clear();
    cursesStarted = true;
}

// Puts the terminal in the original mode
void Terminal_exit() {
    // move curses to bottom right corner
    int y = 0;
    int x = 0;
    getyx(stdscr, y, x);
    mvcur(y, x, LINES - 1, 0);

    // exit curses
    endwin();
    fflush(stdout);
}

void Terminal_clear() {
    clear();
    Terminal_refresh();
}

void Terminal_refresh() {
    refresh();
}

char Terminal_readCharAt(Coord pos) {
    return (char) mvinch(pos.Y, pos.X);
}

void Terminal_printCharAt(char ch, Coord pos) {
    mvaddch(pos.Y, pos.X, ch);
}

void Terminal_printString(char *str) {
    addstr(str);
}

void Terminal_printStringAt(char *str, Coord pos) {
    mvaddstr(pos.Y, pos.X, str);
}

void Terminal_getString(char *str, int bufferSize) {
    getnstr(str, bufferSize);
}

int Terminal_getKey() {
    switch (getch()) {
        case ERR:
            return MM_KEY_NONE;
        case ' ':
        case KEY_UP:
            return MM_KEY_JUMP;
        case KEY_LEFT:
            return MM_KEY_LEFT;
        case KEY_RIGHT:
            return MM_KEY_RIGHT;
        case 'p':
        case '\n': case '\r': case KEY_ENTER:
            return MM_KEY_ENTER;
        case 'P':
            return MM_KEY_PAUSE;
        case 'q':
        case 'Q':
            return MM_KEY_QUIT;
        case 'm':
        case 'M':
            return MM_KEY_MUTE;
        default:
            return MM_KEY_NONE;
    }
}

// Refresh redraws the screen data.
void Terminal_redraw() {
    int row = 0;
    int col = 0;
    char tile;
    uint8_t bits[8] = {0};

    // Dump spectrum speccy.memory to our buffer
    Speccy_convertScreenFormat();

    for (int r = 0; r < 6144; r += 32) {
        for (int c = 0; c < 32; c++) {
            Speccy_byteToBits(Speccy_readScreenBuffer(r + c), bits);

            for (int pixel = 7; pixel >= 0; pixel--) {
                if (bits[pixel] == 0) {
                    tile = ' ';
                } else {
                    tile = '#';
                }

                // double up the width to get a better aspect ratio in curses.
                if (Terminal_readCharAt((Coord) {row, col}) != tile) {
                    Terminal_printCharAt(tile, (Coord) {row, col});
                    col++;
                    Terminal_printCharAt(tile, (Coord) {row, col});
                } else {
                    col++;
                }

                if (col < 511) {
                    col++;
                } else {
                    row++;
                    col = 0;
                }
            }
        }
    }

    refresh();
}
