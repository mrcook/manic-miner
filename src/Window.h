// SDL Wrapper Copyright 2017 Michael R. Cook

#ifndef MANIC_MINER_WINDOW_H
#define MANIC_MINER_WINDOW_H

#include "Headers.h"
#include "SpeccyColour.h"
#include "Speccy.h"
#include "SpeccyDisplay.h"
#include "AudioSystem.h"
#include <SDL2/SDL.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 256;
const int SCREEN_HEIGHT = 192;

enum WindowKeys {
    INPUT_KEY_NONE,

    INPUT_KEY_M,
    INPUT_KEY_P,
    INPUT_KEY_Q,

    INPUT_KEY_RETURN,
    INPUT_KEY_SPACE,

    INPUT_KEY_LEFT,
    INPUT_KEY_RIGHT,

    INPUT_KEY_LEFT_SPACE,
    INPUT_KEY_RIGHT_SPACE,
};

class Window {
public:
    static Window &instance() {
        static Window *instance = new Window();
        return *instance;
    }

    bool initialize(const std::string gameName, SpeccyDisplay *display);

    int getKey();

    // Redraws the screen data.
    void redraw();

    // Destroys SDL window and quits SDL subsystems
    void quit();

    AudioSystem *audio;

private:
    Window() {}

    // The window we'll be rendering to
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;

    // The texture on which the pixels will be drawn
    SDL_Texture *texture_ = nullptr;

    // Pixels array used to render to the screen
    uint32_t *pixels_ = nullptr;

    // Pointer to the SpeccyDisplay, where we create a usable display array
    SpeccyDisplay *display_ = nullptr;

    uint32_t GenerateSDLPixelColour(uint8_t colour_id);

    SDL_PixelFormat *pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);

    const uint8_t *sdlKeyState = nullptr;
};


// RIPPED from YAKC
//uint32_t palette[8] = {
//        0xFF000000,     // black
//        0xFFFF0000,     // blue
//        0xFF0000FF,     // red
//        0xFFFF00FF,     // magenta
//        0xFF00FF00,     // green
//        0xFFFFFF00,     // cyan
//        0xFF00FFFF,     // yellow
//        0xFFFFFFFF,     // white
//};

constexpr SDL_Color spectrumColourPalette[] = {
    {0,   0,   0,   0},
    {0,   0,   215, 0},
    {215, 0,   0,   0},
    {215, 0,   215, 0},
    {0,   215, 0,   0},
    {0,   215, 215, 0},
    {215, 215, 0,   0},
    {215, 215, 215, 0},
    {0,   0,   0,   0},
    {0,   0,   255, 0},
    {255, 0,   0,   0},
    {255, 0,   255, 0},
    {0,   255, 0,   0},
    {0,   255, 255, 0},
    {255, 255, 0,   0},
    {255, 255, 255, 0}
};


#endif //MANIC_MINER_WINDOW_H
