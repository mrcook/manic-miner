// Curses Terminal Wrapper Copyright 2017 Michael R. Cook

#ifndef MANIC_MINER_WINDOW_H
#define MANIC_MINER_WINDOW_H

#include "Headers.h"
#include "Colour.h"
#include "Speccy.h"
#include "Display.h"

enum Input {
    W_KEY_NONE,
    W_KEY_SPACE,
    W_KEY_LEFT,
    W_KEY_RIGHT,
    W_KEY_UP,
    W_KEY_DOWN,
    W_KEY_ENTER,
};

class Window {
public:
    static Window &instance() {
        static Window *instance = new Window();
        return *instance;
    }

    void initialize(Display *display);

    int getKey();

    void drawPixelAt(int y, int x, int16_t pixel);

    void clearWindow();

    void refreshWindow();

    // Redraws the screen data.
    void redraw();

    // Puts the terminal in the original mode
    void exit();

    void toggleColour();

private:
    bool doColour_, cursesStarted_;

    Display *display_;

    Window() : doColour_(false), cursesStarted_(false) {}
};


#endif //MANIC_MINER_WINDOW_H
