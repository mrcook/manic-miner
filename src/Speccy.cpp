// Speccy library Copyright 2017 Michael R. Cook

#include "Headers.h"
#include "Colour.h"
#include "Helpers.h"
#include "Globals.h"

void Speccy::initialize(int fps) {
    // The number of millisecond ticks per frame
    frameTick = 1000 / fps;
}

void Speccy::tick() {
    static int sleep_time = 0;
    static int nextFrameTick = 0;

    nextFrameTick += frameTick;

    sleep_time = nextFrameTick - getTickCount();

    if (sleep_time > frameTick) {
        sleep_time = frameTick;
    }

    if (sleep_time >= 0) {
        millisleep(sleep_time);
    }
}


//
// General memory access
//

uint8_t Speccy::readMemory(int address) {
    if (address < 0 && address >= (int) (sizeof(memory) / sizeof(uint8_t))) {
        exit(-1);
    }

    return memory[address];
}

void Speccy::writeMemory(int address, uint8_t byte) {
    if (address < 0 && address >= (int) (sizeof(memory) / sizeof(uint8_t))) {
        exit(-1);
    }

    memory[address] = byte;
}


//
// Core Input/Output functions
//

uint8_t Speccy::IN(uint16_t addr) {
    addr = 0; // prevents compiler error

    // get keyboard input values
    return 0;
}

void Speccy::OUT(uint8_t value) {
    value = 0; // prevents compiler error

    // output the sound, border colour!
}

void Speccy::setBorderColour(uint8_t colour) {
    Speccy::OUT(colour);
}

// The Spectrum uses OUT to make a sound, but here we use a custom function
void Speccy::makeSound(uint8_t pitch, uint8_t duration, uint8_t delay) {
    delay = 0; // prevents compiler error

    // Real speccy does something like this
    for (int d = duration; d > 0; d--) {
        Speccy::OUT(pitch);
        pitch ^= 24;
        // millisleep(delay);
    }
}

// Print a ZX Spectrum font characters to the display file
void Speccy::printCharacter(char ch, uint16_t address) {
    // Get the character bitmap ID (in the ROM)
    uint8_t ch_index_id = (uint8_t) (ch - 32);

    // There are eight pixel rows in a character bitmap
    speccy.drawSprite(&Speccy::Font[ch_index_id], address, 8);
}

// Print a message string to the display file
void Speccy::printString(void *msg, uint16_t address, uint8_t len) {
    uint8_t *ch = (uint8_t *) msg;

    for (int i = 0; i < len; i++, address++) {
        speccy.printCharacter(ch[i], address);
    }
}

// Draw a sprite item to the given screen address
void Speccy::drawSprite(void *character, uint16_t address, uint8_t len) {
    assert(len >= 0 && len <= (sizeof(character) / sizeof(uint8_t)));

    uint8_t *chr = (uint8_t *) character;
    uint8_t msb, lsb;

    // Copy character data to the screen
    for (int i = 0; i < len; i++) {
        speccy.writeMemory(address, chr[i]);

        // increment address to next pixel row
        Speccy::splitAddress(address, &msb, &lsb);
        msb++;
        address = Speccy::buildAddress(msb, lsb);
    }
}


//
// Display/Attribute file manipulation
//

// Clear the entire Spectrum display file
void Speccy::clearDisplayFile() {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    for (int i = 0; i < SCREEN_SIZE; i++) {
        speccy.memory[16384 + i] = 0;
    }
}

// Clears the entire attributes file
void Speccy::clearAttributesFile() {
    for (int i = 0; i < ATTR_SIZE; i++) {
        speccy.memory[22528 + i] = 0;
    }
}

// Clear the top two-thirds of the display file
void Speccy::clearTopTwoThirdsOfDisplayFile() {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    for (int i = 0; i < 4096; i++) {
        speccy.memory[16384 + i] = 0;
    }
}

// Clear the bottom third of the display file.
void Speccy::clearBottomThirdOfDisplayFile() {
    for (int i = 0; i < 2048; i++) {
        Speccy_writeScreen(20480 + i, 0);
    }
}

// Fill the top two thirds of the attribute file with the value given.
void Speccy::fillTopTwoThirdsOfAttributeFileWith(uint8_t byte) {
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(22528 + i, byte);
    }
}


uint8_t Speccy_readScreen(int address) {
    return speccy.readMemory(address);
}

void Speccy_writeScreen(int address, uint8_t byte) {
    speccy.writeMemory(address, byte);
}

uint8_t Speccy_readAttribute(int address) {
    return speccy.readMemory(address);
}

void Speccy_writeAttribute(int address, uint8_t byte) {
    speccy.writeMemory(address, byte);
}

// Write a colour pixel to the new screen.
// The colour is taken from the Attributes File, using the given address.
void writeColourPixelToNewScreen(uint8_t pixel, int newScreenAddress) {
    assert(newScreenAddress >= 0 && newScreenAddress < 256 * 192);

    Colour colour;

    uint8_t attribute = getAttrFromAttributesFile(newScreenAddress);

    Speccy::splitColourAttribute(attribute, &colour);

    uint8_t brightness = (uint8_t) (colour.BRIGHT ? 64 : 0);

    // NOTE: bit-7 is normally the FLASH value in the Spectrum attribute,
    // but here we use it to indicate if a pixel is present.
    if (pixel == 1) {
        speccy.newScreen[newScreenAddress] = (uint8_t) (128 | brightness | colour.INK);
    } else {
        speccy.newScreen[newScreenAddress] = (uint8_t) (brightness | colour.PAPER);
    }
}

// Given an address from the new screen array (256*192 pixels),
// calculate the Spectrum Attribute File address
uint8_t getAttrFromAttributesFile(int pixelAddress) {
    // each third of the screen is 16384 pixels.
    // bottom third starts at: 32768

    // Each 32 character rowOffset of 8x8 pixels contains 2048 pixels
    // This will increment one AttributeFile rowOffset at a time
    int rowOffset = (pixelAddress / 2048) * 32;

    // Will calculate the column value from 0-31 regardless of the pixelAddress value
    int column = ((pixelAddress / 8) % 32);

    return speccy.memory[22528 + rowOffset + column];
}


//
// Utility functions to help porting from Z80 to C
//

// Handy function to convert a byte to an array of bits,
// so you can more easily create pixel based graphics.
void Speccy::byteToBits(uint8_t byte, uint8_t *bits) {
    for (int i = 0; i < 8; i++) {
        if (byte & (1 << i)) {
            bits[i] = 1;
        } else {
            bits[i] = 0;
        }
    }
}

// Split a uint16_t memory address into its MSB and LSB values
void Speccy::splitAddress(uint16_t addr, uint8_t *msb, uint8_t *lsb) {
    *lsb = (uint8_t) (addr & 0xFF);
    *msb = (uint8_t) (addr >> 8);
}

// Build a uint16_t memory address from the MSB and LSB values
uint16_t Speccy::buildAddress(uint8_t msb, uint8_t lsb) {
    return (msb << 8) | lsb;
}

// Rotate left n places
uint8_t Speccy::rotL(uint8_t a, uint8_t n) {
    assert (n > 0 && n < 8);
    return (a << n) | (a >> (8 - n));
}

// Rotate right n places
uint8_t Speccy::rotR(uint8_t a, uint8_t n) {
    assert (n > 0 && n < 8);
    return (a >> n) | (a << (8 - n));
}

// Split a Spectrum attribute byte into it's colour parts
// Extract the ink, paper, brightness values from the attribute
void Speccy::splitColourAttribute(uint8_t attribute, Colour *colour) {
    // Flashing uses bit value 128, save as boolean
    colour->flash = (attribute & 128) != 0;

    // Brightness uses bit value 64, save as boolean
    colour->BRIGHT = (attribute & 64) != 0;

    // Ink uses bit values 1,2,4 (0-7)
    colour->INK = (uint8_t) (attribute & 7);

    // Paper uses bit values 8,16,32 (56-63), and shift right to be 0-7
    colour->PAPER = (uint8_t) ((attribute >> 3) & 7);
}











/*
 * NewScreen format functions
 *
 * Converts the original Spectrum screen layout to a more standard linear
 * format; bytes are sequential reading from left-to-right, top-to-bottom.
 */

// Converts the entire spectrum screen format to the NewScreen standard format
// ZX Spectrum to linear screen converter. FIXME: yucky goo!
void Speccy_convertScreenFormat() {
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
                uint8_t bite = speccy.memory[16384 + block_addr_offset + byte_row + b];

                // Convert the Speccy display bytes to pixels and add to the new screen array
                Speccy::byteToBits(bite, pixels);
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

uint8_t Speccy_readNewScreen(int address) {
    assert(address >= 0 && address < (int) (sizeof(speccy.newScreen) / sizeof(uint8_t)));
    return speccy.newScreen[address];
}
