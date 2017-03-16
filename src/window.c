// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#include "headers.h"
#include "globals.h"
#include "window.h"

// keep ncurses functions private to this file
#include <ncurses.h>

#ifdef _WIN32
// this is defined in Windows and also in ncurses
#undef KEY_EVENT
#endif

bool doColour = false;

void Window_init() {
    static bool cursesStarted = false;

    if (cursesStarted) {
        Window_refresh();
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

    Window_clear();
    cursesStarted = true;
}

int Window_getKey() {
    int input;
    int key = getch();

    switch (key) {
        case ERR:
            // No key is currently being pressed
            input = W_KEY_NONE;
            break;
        case ' ':
            input = W_KEY_SPACE;
            break;
        case KEY_LEFT:
            input = W_KEY_LEFT;
            break;
        case KEY_RIGHT:
            input = W_KEY_RIGHT;
            break;
        case KEY_UP:
            input = W_KEY_UP;
            break;
        case KEY_DOWN:
            input = W_KEY_DOWN;
            break;
        case '\n':
        case '\r':
        case KEY_ENTER:
            input = W_KEY_ENTER;
            break;
        default:
            // forward all other key inputs to caller
            input = key;
    }

    return input;
}

void Window_drawPixelAt(int y, int x, int16_t pixel) {
    mvaddch(y, x, pixel);
}

void Window_clear() {
    clear();
}

void Window_refresh() {
    refresh();
}

// Redraws the screen data.
void Window_redraw() {
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

            if (doColour) {
                if (PAIR_NUMBER(character & A_ATTRIBUTES) != colour || (character & A_CHARTEXT) != pixel) {
                    if (bright) {
                        attron(A_BOLD | COLOR_PAIR(colour));
                    } else {
                        attron(COLOR_PAIR(colour));
                    }

                    Window_drawPixelAt(row, col, pixel);

                    if (bright) {
                        attroff(A_BOLD | COLOR_PAIR(colour));
                    } else {
                        attroff(COLOR_PAIR(colour));
                    }
                }
            } else {
                if ((character & A_CHARTEXT) != pixel) {
                    Window_drawPixelAt(row, col, pixel);
                }
            }
        }
        row++;
    }

    refresh();
}

// Puts the terminal in the original mode
void Window_exit() {
    // move curses to bottom right corner
    int y = 0;
    int x = 0;
    getyx(stdscr, y, x);
    mvcur(y, x, LINES - 1, 0);

    // exit curses
    endwin();
    fflush(stdout);
}
