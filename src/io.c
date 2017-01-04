// Manic Miner C Port Copyright 2017 Michael R. Cook

// IO wrapper for interfacing Spectrum with ncurses.

#include "headers.h"
#include "externs.h"

#ifdef _WIN32
// this is defined in Windows and also in ncurses
    #undef KEY_EVENT
#endif

#include <ncurses.h>

// Curses screen memory buffer
uint8_t mem_buf[6144];

int8_t last_refresh;

// initializes curses routines
void initialize_curses() {
  initscr();

  // Check we have enough screen. -CJS-
//  if (LINES < 192 || COLS < 256) {
//    printf("Screen too small to play Curses Miner.\n");
//    exit(1);
//  }

  cbreak();
  nodelay(stdscr, TRUE);
  noecho();

  nonl();
  intrflush(stdscr, false);
  keypad(stdscr, false);

  #ifdef __APPLE__
    // Default delay on macOS is 1 second, let's fix that!
    set_escdelay(50);
  #endif

  clear();
  refresh();
}

// Puts the terminal in the original mode
void restore_terminal() {
  // move curses to bottom right corner
  int y = 0;
  int x = 0;
  getyx(stdscr, y, x);
  mvcur(y, x, LINES - 1, 0);

  // exit curses
  endwin();
  fflush(stdout);
}

void clear_screen() {
  clear();
}

void clear_to_bottom_from(int row) {
  move(row, 0);
  clrtobot();
}

// Refresh redraws the screen data.
void redraw_screen() {
  int row = 0;
  int col = 0;
  char tile;
  uint8_t bits[8] = {0};

  // Dump spectrum MEM to our buffer
  memory_dump_to_buffer();

  for (int r = 0; r < 6144; r += 32) {
    for (int c = 0; c < 32; c++) {
      byte_to_bits(mem_buf[r + c], bits);

      for (int pixel = 7; pixel >= 0; pixel--) {
        if (bits[pixel] == 0) {
          tile = ' ';
        } else {
          tile = '#';
        }

        // double up the width to get a better aspect ratio in curses.
        put_tile(tile, row, col);
        col++;
        put_tile(tile, row, col);

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

// Outputs a char to a given y, x position
// sign bit of a character used to indicate standout mode. -CJS
void put_tile(char ch, int row, int col) {
  mvaddch(row, col, ch);
}

bool check_enter_keypress() {
  int input = getch();
  if (input == KEY_ENTER || input == 10 || input == 13) {
    return true;
  }
  return false;
}

void byte_to_bits(uint8_t byte, uint8_t *bits) {
  for (int i = 0 ; i < 8 ; i++) {
    if (byte & (1 << i)) {
      bits[i] = 1;
    } else {
      bits[i] = 0;
    }
  }
}

void memory_dump_to_buffer() {
  int block_addr_offset;
  int addr, line, offset;

  for (int bid = 0; bid < 3; bid++) {
    block_addr_offset = bid * 2048;
    addr = 0;
    line = 0;
    offset = 0;

    for (int byte_row = 0; byte_row < 2048; byte_row += 32) {
      for (int b = 0; b < 32; b++) {
        mem_buf[block_addr_offset + addr] = MEM[16384 + block_addr_offset + byte_row + b];
        addr += 1;
      }

      addr += 32 * 7;
      line += 1;

      if (line == 8) {
        line = 0;

        offset += 1;
        addr = offset * 32;
      }
    }
  }
}