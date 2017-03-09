// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#ifndef MANIC_MINER_WINDOW_H
#define MANIC_MINER_WINDOW_H


enum KeyboardInput {
    W_KEY_NONE,
    W_KEY_SPACE,
    W_KEY_LEFT,
    W_KEY_RIGHT,
    W_KEY_UP,
    W_KEY_DOWN,
    W_KEY_ENTER,
};

void Window_init();

int Window_getKey(void);

void Window_drawPixelAt(int y, int x, int16_t pixel);

void Window_clear(void);

void Window_refresh(void);

void Window_redraw(void);

void Window_exit(void);


#endif //MANIC_MINER_WINDOW_H
