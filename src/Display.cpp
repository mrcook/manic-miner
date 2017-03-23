// Display library Copyright 2017 Michael R. Cook

#include "Headers.h"
#include "Colour.h"
#include "Globals.h"
#include "Display.h"

uint8_t Display::read(int address) {
    assert(address >= 0 && address < DISPLAY_PIXELS);

    return screen[address];
}

void Display::convertSpeccyScreen() {
    int block_addr_offset;
    int address, line, offset;

    uint8_t pixels[8];

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

// Write a colour pixel to the new screen.
// The colour is taken from the Attributes File, using the given address.
void Display::writeColourPixelToNewScreen(uint8_t pixel, int newScreenAddress) {
    assert(newScreenAddress >= 0 && newScreenAddress < 256 * 192);

    Colour colour;

    uint8_t attribute = getAttributeByte(newScreenAddress);

    splitColourAttribute(attribute, &colour);

    uint8_t brightness = (uint8_t) (colour.BRIGHT ? 64 : 0);

    // NOTE: bit-7 is normally the FLASH value in the Spectrum attribute,
    // but here we use it to indicate if a pixel is present.
    if (pixel == 1) {
        screen[newScreenAddress] = (uint8_t) (128 | brightness | colour.INK);
    } else {
        screen[newScreenAddress] = (uint8_t) (brightness | colour.PAPER);
    }
}

// Given an address from the new screen array (256*192 pixels),
// calculate the Spectrum Attribute File address
uint8_t Display::getAttributeByte(int pixelAddress) {
    // each third of the screen is 16384 pixels.
    // bottom third starts at: 32768

    // Each 32 character rowOffset of 8x8 pixels contains 2048 pixels
    // This will increment one AttributeFile rowOffset at a time
    int rowOffset = (pixelAddress / 2048) * 32;

    // Will calculate the column value from 0-31 regardless of the pixelAddress value
    int column = ((pixelAddress / 8) % 32);

    return speccy.readAttribute(22528 + rowOffset + column);
}

void Display::splitColourAttribute(uint8_t attribute, Colour *colour) {
    // Flashing uses bit value 128, save as boolean
    colour->flash = (attribute & 128) != 0;

    // Brightness uses bit value 64, save as boolean
    colour->BRIGHT = (attribute & 64) != 0;

    // Ink uses bit values 1,2,4 (0-7)
    colour->INK = (uint8_t) (attribute & 7);

    // Paper uses bit values 8,16,32 (56-63), and shift right to be 0-7
    colour->PAPER = (uint8_t) ((attribute >> 3) & 7);
}

// Handy function to convert a byte to an array of bits,
// so you can more easily create pixel based graphics.
void Display::byteToBits(uint8_t byte, uint8_t *bits) {
    for (int i = 0; i < 8; i++) {
        if (byte & (1 << i)) {
            bits[i] = 1;
        } else {
            bits[i] = 0;
        }
    }
}
