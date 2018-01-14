// SDL Wrapper Copyright (c) 2016-2018 Michael R. Cook

#include "headers.h"
#include "window.h"

bool Window::initialize(const std::string gameName, SpeccyDisplay *display) {
    display_ = display;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // FIXME: is this needed?
    // Set scaling interpolation algorithm: 0 = pixel sampling
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

    // 1 = 256 x 192
    // 2 = 512 x 384
    // 4 = 1024 x 768
    int scalingFactor = 3;

    window_ = SDL_CreateWindow(
            gameName.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH * scalingFactor,
            SCREEN_HEIGHT * scalingFactor,
            SDL_WINDOW_SHOWN
    );

    if (window_ == nullptr) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, -1, 0);
    if (renderer_ == nullptr) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    texture_ = SDL_CreateTexture(
            renderer_,
            pixelFormat->format,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH,
            SCREEN_HEIGHT
    );
    if (texture_ == nullptr) {
        printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    pixels_ = new uint32_t[SCREEN_WIDTH * SCREEN_HEIGHT];
    memset(pixels_, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));

    audio = new AudioSystem();

    return true;
}

int Window::getKey() {
    int input;

    SDL_PumpEvents();
    sdlKeyState = SDL_GetKeyboardState(nullptr);

    if (sdlKeyState[SDL_SCANCODE_M]) {
        input = WindowKeys::INPUT_KEY_M;
    } else if (sdlKeyState[SDL_SCANCODE_P]) {
        input = WindowKeys::INPUT_KEY_P;
    } else if (sdlKeyState[SDL_SCANCODE_Q]) {
        input = WindowKeys::INPUT_KEY_Q;
    } else if (sdlKeyState[SDL_SCANCODE_RETURN] || sdlKeyState[SDL_SCANCODE_RETURN2] || sdlKeyState[SDL_SCANCODE_KP_ENTER]) {
        input = WindowKeys::INPUT_KEY_RETURN;
    } else if (sdlKeyState[SDL_SCANCODE_SPACE] && sdlKeyState[SDL_SCANCODE_LEFT]) {
        input = WindowKeys::INPUT_KEY_LEFT_SPACE;
    } else if (sdlKeyState[SDL_SCANCODE_SPACE] && sdlKeyState[SDL_SCANCODE_RIGHT]) {
        input = WindowKeys::INPUT_KEY_RIGHT_SPACE;
    } else if (sdlKeyState[SDL_SCANCODE_SPACE]) {
        input = WindowKeys::INPUT_KEY_SPACE;
    } else if (sdlKeyState[SDL_SCANCODE_LEFT]) {
        input = WindowKeys::INPUT_KEY_LEFT;
    } else if (sdlKeyState[SDL_SCANCODE_RIGHT]) {
        input = WindowKeys::INPUT_KEY_RIGHT;
    } else if (sdlKeyState[SDL_QUIT]) {
        quit();
        exit(0);
    } else {
        input = WindowKeys::INPUT_KEY_NONE;
    }

    sdlKeyState = nullptr;

    return input;
}

void Window::redraw() {
    // Update new screen format with latest display information
    display_->convertSpeccyScreen();

    for (int y = 0; y < display_->DISPLAY_PIXELS; y++) {
        uint32_t pixel = GenerateSDLPixelColour(display_->read(y));
        if (pixels_[y] != pixel) {
            pixels_[y] = pixel;
        }
    }

    SDL_UpdateTexture(texture_, nullptr, pixels_, SCREEN_WIDTH * sizeof(uint32_t));

    SDL_RenderClear(renderer_);
    SDL_RenderCopy(renderer_, texture_, nullptr, nullptr);
    SDL_RenderPresent(renderer_);
}

void Window::quit() {
    delete[] pixels_;
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
}

uint32_t Window::GenerateSDLPixelColour(uint8_t colour_id) {
    const SDL_Color &colour = spectrumColourPalette[colour_id];
    return SDL_MapRGB(pixelFormat, colour.r, colour.g, colour.b);
}