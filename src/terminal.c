// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#include "headers.h"
#include "globals.h"
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

    // cbreak();           // <curses.h> not needed when using raw()...does not disable Ctrl chars
    raw();                 // <curses.h> disable control characters. I.e. Ctrl-C does not work!
    nonl();                // <curses.h> disable translation return/ newline for detection of return key
    noecho();              // <curses.h> do not echo typed characters
    nodelay(stdscr, true); // <curses.h> non-blocking input
    keypad(stdscr, true);  // <curses.h> enable keypad for input
    curs_set(0);           // <curses.h> sets the appearance of the cursor based on the value of visibility

    start_color();   // <curses.h> use colors
    init_pair(BLACK, COLOR_BLACK, COLOR_BLACK);       // <curses.h> define color-pair
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);          // <curses.h> define color-pair
    init_pair(RED, COLOR_RED, COLOR_RED);             // <curses.h> define color-pair
    init_pair(MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA); // <curses.h> define color-pair
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);       // <curses.h> define color-pair
    init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);          // <curses.h> define color-pair
    init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);    // <curses.h> define color-pair
    init_pair(WHITE, COLOR_WHITE, COLOR_WHITE);       // <curses.h> define color-pair

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
    return (char) (mvinch(pos.Y, pos.X) & A_CHARTEXT);
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
    int key = getch();

    switch (key) {
        case ERR:
            // No key is currently being pressed
            input = T_KEY_NONE;
            break;
        case ' ':
            input = T_KEY_SPACE;
            break;
        case KEY_LEFT:
            input = T_KEY_LEFT;
            break;
        case KEY_RIGHT:
            input = T_KEY_RIGHT;
            break;
        case KEY_UP:
            input = T_KEY_UP;
            break;
        case KEY_DOWN:
            input = T_KEY_DOWN;
            break;
        case '\n':
        case '\r':
        case KEY_ENTER:
            input = T_KEY_ENTER;
            break;
        default:
            // forward all other key inputs to caller
            input = key;
    }

    return input;
}

// Refresh redraws the screen data.
void Terminal_redraw() {
    // Update new screen format with latest display information
    Speccy_convertScreenFormat();

    uint8_t bufferAttribute;
    char pixel;
    bool bright;
    uint8_t colour;
    chtype character;

    // Iterate over the new screen pixels, apply the colour.
    int row = 0;
    for (int r = 0; r < SCREEN_PIXELS_SIZE; r += 256) {
        for (int col = 0; col < 256; col++) {
            bufferAttribute = Speccy_readNewScreen(r + col);

            // pick up the brightness value
            bright = (bool) (bufferAttribute & 64);

            // ignore bit-7 (pixel) so we only get the colour values (0-7)
            colour = (uint8_t) (bufferAttribute & 7);

            // pickup bit-7 (value 128) for the pixel information
            if (bufferAttribute & 128) {
                pixel = '#';
            } else {
                pixel = ' ';
            }

            // Try to speed things up a little by first checking the current
            // attributes, and only writing if they have changed.
            character = mvinch(row, col);
            if (PAIR_NUMBER(character & A_ATTRIBUTES) != colour || (character & A_CHARTEXT) != pixel) {
                if (bright) {
                    attron(A_BOLD | COLOR_PAIR(colour));
                } else {
                    attron(COLOR_PAIR(colour));
                }

                Terminal_printCharAt(pixel, (Coord) {row, col});

                if (bright) {
                    attroff(A_BOLD | COLOR_PAIR(colour));
                } else {
                    attroff(COLOR_PAIR(colour));
                }
            }
        }
        row++;
    }

    refresh();
}
