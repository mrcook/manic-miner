// Spectrum Display library Copyright 2017 Michael R. Cook

#include "Headers.h"
#include "Helpers.h"
#include "SpeccyColour.h"
#include "Globals.h"
#include "SpeccyDisplay.h"

uint8_t SpeccyDisplay::read(int address) {
    assert(address >= 0 && address < DISPLAY_PIXELS);

    return screen[address];
}

void SpeccyDisplay::convertSpeccyScreen() {
    int block_addr_offset;
    int address, line, offset;

    uint8_t pixels[8];

    // Toggle the flashing state, before writing the new pixels.
    toggleFlashing();

    // Iterate over each Display File block (top, middle, bottom)
    for (int block = 0; block < 3; block++) {
        block_addr_offset = block * 2048;
        address = 0;
        line = 0;
        offset = 0;

        for (int byte_row = 0; byte_row < 2048; byte_row += 32) {
            for (int b = 0; b < 32; b++) {
                uint8_t bite = speccy.readMemory(16384 + block_addr_offset + byte_row + b);

                // Convert the Speccy display bytes to pixels and add to the new screen array
                byteToBits(bite, pixels);
                // read pixel bits from left-to-right: bit-7 down to bit-0
                for (int pixel = 7; pixel >= 0; pixel--) {
                    // multiply offset by 8 pixels
                    writeColourPixelToNewScreen(pixels[pixel], block_addr_offset * 8 + address);
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
void SpeccyDisplay::writeColourPixelToNewScreen(uint8_t pixel, int newScreenAddress) {
    assert(newScreenAddress >= 0 && newScreenAddress < 256 * 192);

    Colour colour;
    uint8_t attribute = getAttributeByte(newScreenAddress);

    splitColourAttribute(attribute, &colour);

    // FLASH swaps the INK and PAPER colours
    uint8_t paperColour = colour.PAPER;
    uint8_t inkColour = colour.INK;
    if (colour.FLASH && flashState_) {
        uint8_t newINK = paperColour;
        paperColour = inkColour;
        inkColour = newINK;
    }

    // Choose INK/PAPER colour based on presence of a pixel
    uint8_t colourID;
    if (pixel == 1) {
        colourID = inkColour;
    } else {
        colourID = paperColour;
    }

    // If BRIGHT, then add 8 to the colour, given a range of 8-15
    if (colour.BRIGHT) {
        colourID += 8;
    }

    if (screen[newScreenAddress] != colourID) {
        screen[newScreenAddress] = colourID;
    }
}

// Given an address from the new screen array (256*192 pixels),
// calculate the Spectrum Attribute File address
uint8_t SpeccyDisplay::getAttributeByte(int pixelAddress) {
    // each third of the screen is 16384 pixels.
    // bottom third starts at: 32768

    // Each 32 character rowOffset of 8x8 pixels contains 2048 pixels
    // This will increment one AttributeFile rowOffset at a time
    int rowOffset = (pixelAddress / 2048) * 32;

    // Will calculate the column value from 0-31 regardless of the pixelAddress value
    int column = ((pixelAddress / 8) % 32);

    return speccy.readAttribute(22528 + rowOffset + column);
}

void SpeccyDisplay::splitColourAttribute(uint8_t attribute, Colour *colour) {
    // Flashing uses bit flag 7, save as boolean
    colour->FLASH = ((attribute >> 7) & 1) == 1;

    // Brightness uses bit flag 6, save as boolean
    colour->BRIGHT = ((attribute >> 6) & 1) == 1;

    // Clear the FLASH/BRIGHT flags
    attribute &= ~(1 << 7);
    attribute &= ~(1 << 6);

    // Paper uses bit flags 3,4,5 (value 56-63), and shift right to be value 0-7
    colour->PAPER = (uint8_t) (attribute >> 3);

    // Ink uses bit flags 0,1,2 (value 0-7)
    colour->INK = (uint8_t) (attribute & 7);
}

// Handy function to convert a byte to an array of bits,
// so you can more easily create pixel based graphics.
void SpeccyDisplay::byteToBits(uint8_t byte, uint8_t *bits) {
    for (int i = 0; i < 8; i++) {
        if (byte & (1 << i)) {
            bits[i] = 1;
        } else {
            bits[i] = 0;
        }
    }
}

void SpeccyDisplay::toggleFlashing() {
    static int lastTick = 0;

    int currentTick = getTickCount();
    if (currentTick - lastTick > 62) {
        lastTick = currentTick;
        flashState_ = !flashState_;
    }
}
