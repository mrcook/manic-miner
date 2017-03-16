// Speccy library Copyright 2017 Michael R. Cook

#include "Headers.h"
#include "Colour.h"
#include "Helpers.h"
#include "Globals.h"
#include "Window.h"

void printFontCharacterAt(char ch, uint16_t address);

void Speccy_initialize(int fps) {
    // The number of millisecond ticks per frame
    speccy.frameTick = 1000 / fps;
}

void Speccy_tick() {
    static int sleep_time = 0;
    static int nextFrameTick = 0;

    nextFrameTick += speccy.frameTick;

    sleep_time = nextFrameTick - getTickCount();

    if (sleep_time > speccy.frameTick) {
        sleep_time = speccy.frameTick;
    }

    if (sleep_time >= 0) {
        millisleep(sleep_time);
    }
}

uint8_t Speccy_read(int address) {
    if (address < 0 && address >= (int) (sizeof(speccy.memory) / sizeof(uint8_t))) {
        exit(-1);
    }

    return speccy.memory[address];
}

void Speccy_write(int address, uint8_t byte) {
    if (address < 0 && address >= (int) (sizeof(speccy.memory) / sizeof(uint8_t))) {
        exit(-1);
    }

    speccy.memory[address] = byte;
}

void Speccy_clearDisplayFile() {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    for (int i = 0; i < SCREEN_SIZE; i++) {
        speccy.memory[16384 + i] = 0;
    }
}

void Speccy_clearAttributesFile() {
    for (int i = 0; i < ATTR_SIZE; i++) {
        speccy.memory[22528 + i] = 0;
    }
}

void Speccy_clearTopTwoThirdsOfDisplayFile() {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    for (int i = 0; i < 4096; i++) {
        speccy.memory[16384 + i] = 0;
    }
}

void Speccy_clearBottomThirdOfDisplayFile() {
    for (int i = 0; i < 2048; i++) {
        Speccy_writeScreen(20480 + i, 0);
    }
}

void Speccy_fillTopTwoThirdsOfAttributeFileWith(uint8_t byte) {
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(22528 + i, byte);
    }
}

// Extract the ink, paper, brightness values from the attribute
void Speccy_splitColorAttribute(uint8_t attribute, Colour *colour) {
    // Flashing uses bit value 128, save as boolean
    colour->flash = (attribute & 128) != 0;

    // Brightness uses bit value 64, save as boolean
    colour->BRIGHT = (attribute & 64) != 0;

    // Ink uses bit values 1,2,4 (0-7)
    colour->INK = (uint8_t) (attribute & 7);

    // Paper uses bit values 8,16,32 (56-63), and shift right to be 0-7
    colour->PAPER = (uint8_t) ((attribute >> 3) & 7);
}

void Speccy_setBorderColour(uint8_t colour) {
    OUT(colour);
}

void Speccy_makeSound(uint8_t pitch, uint8_t duration, uint8_t delay) {
    delay = 0; // prevents compiler error

    // Real speccy does something like this
    for (int d = duration; d > 0; d--) {
        OUT(pitch);
        pitch ^= 24;
//        millisleep(delay);
    }
}

uint8_t Speccy_readScreen(int address) {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    return Speccy_read(address);

//    if (address < 16384 && address >= 22528) {
//        exit(-1);
//    }
//
//    return speccy.screen[address - 16384];
}

void Speccy_writeScreen(int address, uint8_t byte) {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    Speccy_write(address, byte);
    return;

//    if (address < 16384 && address >= 22528) {
//        exit(-1);
//    }
//
//    speccy.screen[address - 16384] = byte;
}

uint8_t Speccy_readAttribute(int address) {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    return Speccy_read(address);

//    if (address < 22528 && address >= 23296) {
//        exit(-1);
//    }
//
//    return speccy.attributes[address - 22528];
}

void Speccy_writeAttribute(int address, uint8_t byte) {
    // FIXME: some calls go direct to Display File, others to Buffers,
    // so we must use normal memory for now!
    Speccy_write(address, byte);
    return;

//    if (address < 22528 && address >= 23296) {
//        exit(-1);
//    }
//    speccy.attributes[address - 22528] = byte;
}

// Print a ZX Spectrum font characters to the display file
void printFontCharacterAt(char ch, uint16_t address) {
    // Get the character bitmap ID (in the ROM)
    uint8_t ch_index_id = (uint8_t) (ch - 32);

    // There are eight pixel rows in a character bitmap
    Speccy_drawSpriteAt(&Speccy_font[ch_index_id], address, 8);
}

void Speccy_printMessage(void *msg, uint16_t address, uint8_t len) {
    uint8_t *ch = (uint8_t *) msg;

    for (int i = 0; i < len; i++, address++) {
        printFontCharacterAt(ch[i], address);
    }
}

void Speccy_drawSpriteAt(void *character, uint16_t address, uint8_t len) {
    assert(len >= 0 && len <= (sizeof(character) / sizeof(uint8_t)));

    uint8_t *chr = (uint8_t *) character;
    uint8_t msb, lsb;

    // Copy character data to the screen
    for (int i = 0; i < len; i++) {
        Speccy_write(address, chr[i]);

        // increment address to next pixel row
        splitAddress(address, &msb, &lsb);
        msb++;
        address = buildAddress(msb, lsb);
    }
}

void byteToBits(uint8_t byte, uint8_t *bits) {
    for (int i = 0; i < 8; i++) {
        if (byte & (1 << i)) {
            bits[i] = 1;
        } else {
            bits[i] = 0;
        }
    }
}

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

void writeColourPixelToNewScreen(uint8_t pixel, int newScreenAddress) {
    assert(newScreenAddress >= 0 && newScreenAddress < 256 * 192);

    Colour colour;

    uint8_t attribute = getAttrFromAttributesFile(newScreenAddress);

    Speccy_splitColorAttribute(attribute, &colour);

    uint8_t brightness = (uint8_t) (colour.BRIGHT ? 64 : 0);

    // NOTE: bit-7 is normally the FLASH value in the Spectrum attribute,
    // but here we use it to indicate if a pixel is present.
    if (pixel == 1) {
        speccy.newScreen[newScreenAddress] = (uint8_t) (128 | brightness | colour.INK);
    } else {
        speccy.newScreen[newScreenAddress] = (uint8_t) (brightness | colour.PAPER);
    }
}

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

uint8_t Speccy_readNewScreen(int address) {
    assert(address >= 0 && address < (int) (sizeof(speccy.newScreen) / sizeof(uint8_t)));
    return speccy.newScreen[address];
}

void splitAddress(uint16_t addr, uint8_t *msb, uint8_t *lsb) {
    *lsb = (uint8_t) (addr & 0xFF);
    *msb = (uint8_t) (addr >> 8);
}

uint16_t buildAddress(uint8_t msb, uint8_t lsb) {
    return (msb << 8) | lsb;
}

uint8_t rotL(uint8_t a, uint8_t n) {
    assert (n > 0 && n < 8);
    return (a << n) | (a >> (8 - n));
}

uint8_t rotR(uint8_t a, uint8_t n) {
    assert (n > 0 && n < 8);
    return (a >> n) | (a << (8 - n));
}

uint8_t IN(uint16_t addr) {
    addr = 0; // prevents compiler error

    // get keyboard input values
    return 0;
}

void OUT(uint8_t value) {
    value = 0; // prevents compiler error

    // output the sound, border colour!
}
