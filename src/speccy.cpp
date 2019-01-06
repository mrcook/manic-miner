// Speccy library Copyright (c) 2016-2018 Michael R. Cook

#include "headers.h"
#include "helpers.h"
#include "globals.h"
#include "window.h"

bool Speccy_initialize(std::string gameName, int fps, int zoom) {
    if (!Window::instance().initialize(gameName, zoom)) {
        return false;
    }

    // Window::instance().audio->frame_rate = fps;

    // The number of millisecond ticks per frame
    speccy.frameTick = 1000 / fps;

    return true;
}

void Speccy_quit() {
    Window::instance().quit();
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


//
// General memory access
//

uint8_t Speccy_readMemory(int address) {
    assert(address >= 16384 && address < TOTAL_MEMORY);

    return speccy.memory[address];
}

void Speccy_writeMemory(int address, uint8_t byte) {
    assert(address >= 16384 && address < TOTAL_MEMORY);

    speccy.memory[address] = byte;
}


//
// Core Input/Output functions
//

int Speccy_getKey() {
    switch (Window::instance().getKey()) {
        case WindowKeys::INPUT_KEY_M:
            return KEY_M;
        case WindowKeys::INPUT_KEY_P:
            return KEY_P;
        case WindowKeys::INPUT_KEY_Q:
            return KEY_Q;
        case WindowKeys::INPUT_KEY_RETURN:
            return KEY_RETURN;
        case WindowKeys::INPUT_KEY_LEFT_SPACE:
            return KEY_LEFT_SPACE;
        case WindowKeys::INPUT_KEY_RIGHT_SPACE:
            return KEY_RIGHT_SPACE;
        case WindowKeys::INPUT_KEY_SPACE:
            return KEY_SPACE;
        case WindowKeys::INPUT_KEY_LEFT:
            return KEY_LEFT;
        case WindowKeys::INPUT_KEY_RIGHT:
            return KEY_RIGHT;
        default:
            return KEY_NONE;
    }
}

void Speccy_OUT(uint8_t value) {
    (void) value; // prevents compiler error
    // output the sound, border colour!
}

void Speccy_setBorderColour(uint8_t colour) {
    Speccy_OUT(colour);
}

// The Spectrum uses OUT to make a sound, but here we use a custom function
void Speccy_beep(int pitch, uint8_t duration, uint8_t volume) {
    (void) pitch; // prevents compiler error
    (void) duration; // prevents compiler error
    (void) volume; // prevents compiler error
    // Window::instance().audio->playNote(pitch, duration, volume);
}

// Print a ZX Spectrum font characters to the display file
void Speccy_printCharacter(char ch, uint16_t address) {
    // Get the character bitmap ID (in the ROM)
    auto ch_index_id = (uint8_t) (ch - 32);

    // There are eight pixel rows in a character bitmap
    Speccy_drawSprite(&SpeccyDisplay_Font[ch_index_id], address, 8);
}

// Print a message string to the display file
void Speccy_printString(void *msg, uint16_t address, uint8_t len) {
    uint8_t *ch = (uint8_t *) msg;

    for (int i = 0; i < len; i++, address++) {
        Speccy_printCharacter(ch[i], address);
    }
}

// Draw a sprite item to the given screen address
void Speccy_drawSprite(void *character, uint16_t address, uint8_t len) {
    assert(len <= (sizeof(character) / sizeof(uint8_t)));

    uint8_t *chr = (uint8_t *) character;
    uint8_t msb, lsb;


    // Copy character data to the screen
    for (int i = 0; i < len; i++) {
        Speccy_writeMemory(address, chr[i]);

        // increment address to next pixel row
        Speccy_splitAddress(address, msb, lsb);
        msb++;
        address = Speccy_buildAddress(msb, lsb);
    }
}


//
// Display/Attribute file manipulation
//

// Clear the entire Spectrum display file
void Speccy_clearDisplayFile() {
    for (int i = 0; i < SPECCY_SCREEN; i++) {
        Speccy_writeScreen(16384 + i, 0);
    }
}

// Clears the entire attributes file
void Speccy_clearAttributesFile() {
    for (int i = 0; i < ATTR_SIZE; i++) {
        Speccy_writeAttribute(22528 + i, 0);
    }
}

// Clear the top two-thirds of the display file
void Speccy_clearTopTwoThirdsOfDisplayFile() {
    for (int i = 0; i < 4096; i++) {
        Speccy_writeScreen(16384 + i, 0);
    }
}

// Clear the bottom third of the display file.
void Speccy_clearBottomThirdOfDisplayFile() {
    for (int i = 0; i < 2048; i++) {
        Speccy_writeScreen(20480 + i, 0);
    }
}

// Fill the top two thirds of the attribute file with the value given.
void Speccy_fillTopTwoThirdsOfAttributeFileWith(uint8_t byte) {
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(22528 + i, byte);
    }
}

void Speccy_writeScreen(int address, uint8_t byte) {
    assert(address >= 16384 && address < 22528);

    Speccy_writeMemory(address, byte);
}

uint8_t Speccy_readAttribute(int address) {
    assert(address >= 22528 && address < 23296);

    return Speccy_readMemory(address);
}

void Speccy_writeAttribute(int address, uint8_t byte) {
    assert(address >= 22528 && address < 23296);

    Speccy_writeMemory(address, byte);
}

void Speccy_redrawWindow() {
    Window::instance().redraw();
}

//
// Utility functions to help porting from Z80 to C
//

// Split a uint16_t memory address into its MSB and LSB values
void Speccy_splitAddress(uint16_t addr, uint8_t &msb, uint8_t &lsb) {
    lsb = (uint8_t) (addr & 0xFF);
    msb = (uint8_t) (addr >> 8);
}

// Build a uint16_t memory address from the MSB and LSB values
uint16_t Speccy_buildAddress(uint8_t msb, uint8_t lsb) {
    return (msb << 8) | lsb;
}

uint8_t Speccy_getAddressMSB(uint16_t addr) {
    return (uint8_t) (addr >> 8);
}

uint8_t Speccy_getAddressLSB(uint16_t addr) {
    return (uint8_t) (addr & 0xFF);
}

// Rotate left n places
uint8_t Speccy_rotL(uint8_t a, uint8_t n) {
    assert (n > 0 && n < 8);

    return (a << n) | (a >> (8 - n));
}

// Rotate right n places
uint8_t Speccy_rotR(uint8_t a, uint8_t n) {
    assert (n > 0 && n < 8);

    return (a >> n) | (a << (8 - n));
}
