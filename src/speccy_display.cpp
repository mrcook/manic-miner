// Spectrum Display library Copyright (c) 2016-2021 Michael R. Cook

#include "headers.h"
#include "helpers.h"
#include "speccy_colour.h"
#include "globals.h"
#include "speccy_display.h"

uint8_t SpeccyDisplay_read(int address) {
    assert(address >= 0 && address < DISPLAY_PIXELS);

    return speccy_display.screen[address];
}

void SpeccyDisplay_convertScreen() {
    int block_addr_offset;
    int address, line, offset;

    uint8_t pixels[8];


    // Toggle the flashing state, before writing the new pixels.
    SpeccyDisplay_toggleFlashing();

    // Iterate over each Display File block (top, middle, bottom)
    for (int block = 0; block < 3; block++) {
        block_addr_offset = block * 2048;
        address = 0;
        line = 0;
        offset = 0;

        for (int byte_row = 0; byte_row < 2048; byte_row += 32) {
            for (int b = 0; b < 32; b++) {
                uint8_t bite = Speccy_readMemory(16384 + block_addr_offset + byte_row + b);

                // Convert the Speccy display bytes to pixels and add to the new screen array
                SpeccyDisplay_byteToBits(bite, pixels);
                // read pixel bits from left-to-right: bit-7 down to bit-0
                for (int pixel = 7; pixel >= 0; pixel--) {
                    // multiply offset by 8 pixels
                    SpeccyDisplay_writeColourPixelToNewScreen(pixels[pixel], block_addr_offset * 8 + address);
                    address++;
                }
            }

            // advance the address to the next "character": 8 pixel rows
            address += 256 * 7;

            // increment the pixel "row" we are currently working on
            line += 1;

            // If we've process the last line of pixels, we start on the next byte
            if (line == 8) {
                line = 0;

                // The "offset" s for starting the next "character" worth of pixels
                offset += 1;
                address = offset * 256;
            }
        }
    }
}

// Write a colour ID for the pixel to the new screen.
// The colour is taken from the Attributes File, using the given address.
// Normal colours are values 0-7, brights colour 8-15.
void SpeccyDisplay_writeColourPixelToNewScreen(uint8_t pixel, int newScreenAddress) {
    assert(newScreenAddress >= 0 && newScreenAddress < 256 * 192);

    Colour colour = SpeccyDisplay_colourFromAttribute(SpeccyDisplay_getAttributeByte(newScreenAddress));

    // FLASH swaps the INK and PAPER colours
    uint8_t paperColour = colour.PAPER;
    uint8_t inkColour = colour.INK;
    if (colour.FLASH && speccy_display.flashState_) {
        uint8_t newINK = paperColour;
        paperColour = inkColour;
        inkColour = newINK;
    }

    // Choose INK/PAPER colour based on presence of a pixel
    uint8_t colourID = paperColour;
    if (pixel == 1) {
        colourID = inkColour;
    }

    // If BRIGHT, then add 8 to the colour, given a range of 8-15
    if (colour.BRIGHT) {
        colourID += 8;
    }

    if (speccy_display.screen[newScreenAddress] != colourID) {
        speccy_display.screen[newScreenAddress] = colourID;
    }
}

// Given an address from the new screen array (256*192 pixels),
// calculate the Spectrum Attribute File address
uint8_t SpeccyDisplay_getAttributeByte(int pixelAddress) {
    // each third of the screen is 16384 pixels.
    // bottom third starts at: 32768

    // Each 32 character rowOffset of 8x8 pixels contains 2048 pixels
    // This will increment one AttributeFile rowOffset at a time
    int rowOffset = (pixelAddress / 2048) * 32;

    // Will calculate the column value from 0-31 regardless of the pixelAddress value
    int column = ((pixelAddress / 8) % 32);

    return Speccy_readAttribute(22528 + rowOffset + column);
}

Colour SpeccyDisplay_colourFromAttribute(uint8_t attribute) {
    Colour colour = {};

    // Flashing uses bit flag 7, save as boolean
    colour.FLASH = ((attribute >> 7) & 1) == 1;

    // Brightness uses bit flag 6, save as boolean
    colour.BRIGHT = ((attribute >> 6) & 1) == 1;

    // Clear the FLASH/BRIGHT flags
    attribute &= ~(1 << 7);
    attribute &= ~(1 << 6);

    // Paper uses bit flags 3,4,5 (value 56-63), and shift right to be value 0-7
    colour.PAPER = (uint8_t) (attribute >> 3);

    // Ink uses bit flags 0,1,2 (value 0-7)
    colour.INK = (uint8_t) (attribute & 7);

    return colour;
}

// Handy function to convert a byte to an array of bits,
// so you can more easily create pixel based graphics.
void SpeccyDisplay_byteToBits(uint8_t byte, uint8_t *bits) {
    for (int i = 0; i < 8; i++) {
        if (byte & (1 << i)) {
            bits[i] = 1;
        } else {
            bits[i] = 0;
        }
    }
}

void SpeccyDisplay_toggleFlashing() {
    static int lastTick = 0;

    int currentTick = getTickCount();
    if (currentTick - lastTick > 62) {
        lastTick = currentTick;
        speccy_display.flashState_ = !speccy_display.flashState_;
    }
}
