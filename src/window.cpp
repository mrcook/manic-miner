// SDL Wrapper Copyright (c) 2016-2021 Michael R. Cook

#include "headers.h"
#include "globals.h"
#include "window.h"
#include "speccy_display.h"

bool Window_initialize(std::string gameName, int zoom) {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // FIXME: is this needed?
    // Set scaling interpolation algorithm: 0 = pixel sampling
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

    // 1 = 256 x 192
    // 2 = 512 x 384 (default)
    // 4 = 1024 x 768
    int scalingFactor = zoom;

    window.window_ = SDL_CreateWindow(
            gameName.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH * scalingFactor,
            SCREEN_HEIGHT * scalingFactor,
            SDL_WINDOW_SHOWN
    );

    if (window.window_ == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window.renderer_ = SDL_CreateRenderer(window.window_, -1, 0);
    if (window.renderer_ == nullptr) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window.texture_ = SDL_CreateTexture(
            window.renderer_,
            window.pixelFormat->format,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH,
            SCREEN_HEIGHT
    );
    if (window.texture_ == nullptr) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    window.pixels_ = new uint32_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    memset(window.pixels_, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));

    // audio = new AudioSystem();

    return true;
}

int Window_getKey() {
    int input;

    SDL_PumpEvents();
    window.sdlKeyState = SDL_GetKeyboardState(nullptr);

    if (window.sdlKeyState[SDL_SCANCODE_M]) {
        input = WindowKeys::INPUT_KEY_M;
    } else if (window.sdlKeyState[SDL_SCANCODE_P]) {
        input = WindowKeys::INPUT_KEY_P;
    } else if (window.sdlKeyState[SDL_SCANCODE_Q]) {
        input = WindowKeys::INPUT_KEY_Q;
    } else if (window.sdlKeyState[SDL_SCANCODE_RETURN] || window.sdlKeyState[SDL_SCANCODE_RETURN2] || window.sdlKeyState[SDL_SCANCODE_KP_ENTER]) {
        input = WindowKeys::INPUT_KEY_RETURN;
    } else if (window.sdlKeyState[SDL_SCANCODE_SPACE] && window.sdlKeyState[SDL_SCANCODE_LEFT]) {
        input = WindowKeys::INPUT_KEY_LEFT_SPACE;
    } else if (window.sdlKeyState[SDL_SCANCODE_SPACE] && window.sdlKeyState[SDL_SCANCODE_RIGHT]) {
        input = WindowKeys::INPUT_KEY_RIGHT_SPACE;
    } else if (window.sdlKeyState[SDL_SCANCODE_SPACE]) {
        input = WindowKeys::INPUT_KEY_SPACE;
    } else if (window.sdlKeyState[SDL_SCANCODE_LEFT]) {
        input = WindowKeys::INPUT_KEY_LEFT;
    } else if (window.sdlKeyState[SDL_SCANCODE_RIGHT]) {
        input = WindowKeys::INPUT_KEY_RIGHT;
    } else if (window.sdlKeyState[SDL_QUIT]) {
        Window_quit();
        exit(0);
    } else {
        input = WindowKeys::INPUT_KEY_NONE;
    }

    window.sdlKeyState = nullptr;

    return input;
}

void Window_redraw() {
    // Update new screen format with latest display information
    SpeccyDisplay_convertScreen();

    for (int y = 0; y < DISPLAY_PIXELS; y++) {
        uint32_t pixel = Window_generateSDLPixelColour(SpeccyDisplay_read(y));
        if (window.pixels_[y] != pixel) {
            window.pixels_[y] = pixel;
        }
    }

    SDL_UpdateTexture(window.texture_, nullptr, window.pixels_, SCREEN_WIDTH * sizeof(uint32_t));

    SDL_RenderClear(window.renderer_);
    SDL_RenderCopy(window.renderer_, window.texture_, nullptr, nullptr);
    SDL_RenderPresent(window.renderer_);
}

void Window_quit() {
    delete[] window.pixels_;
    SDL_DestroyTexture(window.texture_);
    SDL_DestroyRenderer(window.renderer_);
    SDL_DestroyWindow(window.window_);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

uint32_t Window_generateSDLPixelColour(uint8_t colour_id) {
    const SDL_Color &colour = spectrumColourPalette[colour_id];
    return SDL_MapRGB(window.pixelFormat, colour.r, colour.g, colour.b);
}
