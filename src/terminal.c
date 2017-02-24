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

    initscr(); // <curses.h> do initialization work 

    // Make sure our terminal is big enough
//    if (rows > LINES || columns > COLS) {
//        printf("Your terminal is not big enough!\n");
//        exit(-1);
//    }

    // cbreak();              // <curses.h> not needed when using raw()...does not disable Ctrl chars
    raw();                 // <curses.h> disable control characters. I.e. Ctrl-C does not work!
    nonl();                // <curses.h> disable translation return/ newline for detection of return key
    noecho();              // <curses.h> do not echo typed characters
    nodelay(stdscr, true); // <curses.h> non-blocking input
    keypad(stdscr, true);  // <curses.h> enable keypad for input
    curs_set(0);           // <curses.h> sets the appearance of the cursor based on the value of visibility

    // start_color();   // <curses.h> use colors
    // init_pair(RED, COLOR_RED, COLOR_BLACK);         // <curses.h> define color-pair
    // init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);     // <curses.h> define color-pair
    // init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);   // <curses.h> define color-pair
    // init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);       // <curses.h> define color-pair
    // init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);       // <curses.h> define color-pair
    // init_pair(MAGENTA, COLOR_MAGENTA, COLOR_BLACK); // <curses.h> define color-pair
    // init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);     // <curses.h> define color-pair

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
    int input;

    switch (getch()) {
        case ERR:
            input = MM_KEY_NONE;
            break;
        case ' ':
        case KEY_UP:
            input = MM_KEY_JUMP;
            break;
        case KEY_LEFT:
            input = MM_KEY_LEFT;
            break;
        case KEY_RIGHT:
            input = MM_KEY_RIGHT;
            break;
        case '\n': case '\r': case KEY_ENTER:
            input = MM_KEY_ENTER;
            break;
        case 'p':
        case 'P':
            input = MM_KEY_PAUSE;
            break;
        case 'q':
        case 'Q':
            input = MM_KEY_QUIT;
            break;
        case 'm':
        case 'M':
            input = MM_KEY_MUTE;
            break;
        default:
            input = MM_KEY_NONE;
    }

    return input;
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
