// Spectrum Display library Copyright (c) 2016-2018 Michael R. Cook

// Converts the original Spectrum screen layout to a more standard linear
// format; bytes are sequential reading from left-to-right, top-to-bottom.

#pragma once

#include "headers.h"
#include "speccy_colour.h"

class SpeccyDisplay {
public:
    static const int SPECCY_SCREEN = 6144;
    static const int DISPLAY_PIXELS = SPECCY_SCREEN * 8;

    static SpeccyDisplay &instance() {
        static SpeccyDisplay *instance = new SpeccyDisplay();
        return *instance;
    }

    // Read a byte from the NewScreen format
    uint8_t read(int address);

    // Converts the entire spectrum screen format to the NewScreen standard format
    // ZX Spectrum to linear screen converter. FIXME: yucky goo!
    void convertSpeccyScreen();

    // Write a colour pixel to the new screen.
    // The colour is taken from the Attributes File, using the given address.
    void writeColourPixelToNewScreen(uint8_t pixel, int newScreenAddress);

    // Given an address from the new screen array (256*192 pixels),
    // calculate the Spectrum Attribute File address
    uint8_t getAttributeByte(int pixelAddress);

    // Split a Spectrum attribute byte into it's colour parts
    // Extract the ink, paper, brightness values from the attribute
    Colour colourFromAttribute(uint8_t attribute);

    void toggleFlashing();

private:
    // Handy function to convert a byte to an array of bits,
    // so you can more easily create pixel based graphics.
    void byteToBits(uint8_t byte, uint8_t *bits);

    // Display in a standard (linear) format.
    // Useful for sending to ncurses/SDL/etc.
    // contains data for each pixel, not byte character
    uint8_t screen[DISPLAY_PIXELS];

    bool flashState_;
};
