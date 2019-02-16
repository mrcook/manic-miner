// SDL Wrapper Copyright (c) 2016-2018 Michael R. Cook

#pragma once

#include "headers.h"
#include "speccy_colour.h"
#include "speccy.h"
#include "speccy_display.h"
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

typedef struct window_ {
    // AudioSystem *audio = nullptr;

    // The window we'll be rendering to
    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;

    // The texture on which the pixels will be drawn
    SDL_Texture *texture_ = nullptr;

    // Pixels array used to render to the screen
    uint32_t *pixels_ = nullptr;

    SDL_PixelFormat *pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);

    const uint8_t *sdlKeyState = nullptr;
} Window;

bool Window_initialize(std::string gameName, int zoom);

int Window_getKey();

// Redraws the screen data.
void Window_redraw();

// Destroys SDL window and quits SDL subsystems
void Window_quit();

uint32_t Window_generateSDLPixelColour(uint8_t colour_id);


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
