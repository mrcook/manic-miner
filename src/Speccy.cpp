// Speccy library Copyright 2017 Michael R. Cook

#include "Headers.h"
#include "Helpers.h"
#include "Globals.h"
#include "Window.h"

bool Speccy::initialize(const std::string gameName, int fps) {
    if (!Window::instance().initialize(gameName, &SpeccyDisplay::instance())) {
        return false;
    }

    Window::instance().audio->frame_rate = fps;

    // The number of millisecond ticks per frame
    frameTick = 1000 / fps;

    return true;
}

void Speccy::quit(void) {
    Window::instance().quit();
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
    assert(address >= 16384 && address < TOTAL_MEMORY);

    return memory[address];
}

void Speccy::writeMemory(int address, uint8_t byte) {
    assert(address >= 16384 && address < TOTAL_MEMORY);

    memory[address] = byte;
}


//
// Core Input/Output functions
//

int Speccy::getKey() {
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

void Speccy::OUT(uint8_t value) {
    (void) value; // prevents compiler error
    // output the sound, border colour!
}

void Speccy::setBorderColour(uint8_t colour) {
    Speccy::OUT(colour);
}

// The Spectrum uses OUT to make a sound, but here we use a custom function
void Speccy::beep(int pitch, uint8_t duration, uint8_t volume) {
    Window::instance().audio->playNote(pitch, duration, volume);
}

// Print a ZX Spectrum font characters to the display file
void Speccy::printCharacter(char ch, uint16_t address) {
    // Get the character bitmap ID (in the ROM)
    uint8_t ch_index_id = (uint8_t) (ch - 32);

    // There are eight pixel rows in a character bitmap
    drawSprite(&Speccy::Font[ch_index_id], address, 8);
}

// Print a message string to the display file
void Speccy::printString(void *msg, uint16_t address, uint8_t len) {
    uint8_t *ch = (uint8_t *) msg;

    for (int i = 0; i < len; i++, address++) {
        printCharacter(ch[i], address);
    }
}

// Draw a sprite item to the given screen address
void Speccy::drawSprite(void *character, uint16_t address, uint8_t len) {
    assert(len <= (sizeof(character) / sizeof(uint8_t)));

    uint8_t *chr = (uint8_t *) character;
    uint8_t msb, lsb;


    // Copy character data to the screen
    for (int i = 0; i < len; i++) {
        writeMemory(address, chr[i]);

        // increment address to next pixel row
        Speccy::splitAddress(address, msb, lsb);
        msb++;
        address = Speccy::buildAddress(msb, lsb);
    }
}


//
// Display/Attribute file manipulation
//

// Clear the entire Spectrum display file
void Speccy::clearDisplayFile() {
    for (int i = 0; i < SPECCY_SCREEN; i++) {
        writeScreen(16384 + i, 0);
    }
}

// Clears the entire attributes file
void Speccy::clearAttributesFile() {
    for (int i = 0; i < ATTR_SIZE; i++) {
        writeAttribute(22528 + i, 0);
    }
}

// Clear the top two-thirds of the display file
void Speccy::clearTopTwoThirdsOfDisplayFile() {
    for (int i = 0; i < 4096; i++) {
        writeScreen(16384 + i, 0);
    }
}

// Clear the bottom third of the display file.
void Speccy::clearBottomThirdOfDisplayFile() {
    for (int i = 0; i < 2048; i++) {
        writeScreen(20480 + i, 0);
    }
}

// Fill the top two thirds of the attribute file with the value given.
void Speccy::fillTopTwoThirdsOfAttributeFileWith(uint8_t byte) {
    for (int i = 0; i < 512; i++) {
        writeAttribute(22528 + i, byte);
    }
}

void Speccy::writeScreen(int address, uint8_t byte) {
    assert(address >= 16384 && address < 22528);

    writeMemory(address, byte);
}

uint8_t Speccy::readAttribute(int address) {
    assert(address >= 22528 && address < 23296);

    return readMemory(address);
}

void Speccy::writeAttribute(int address, uint8_t byte) {
    assert(address >= 22528 && address < 23296);

    writeMemory(address, byte);
}

void Speccy::redrawWindow() {
    Window::instance().redraw();
}

//
// Utility functions to help porting from Z80 to C
//

// Split a uint16_t memory address into its MSB and LSB values
void Speccy::splitAddress(uint16_t addr, uint8_t &msb, uint8_t &lsb) {
    lsb = (uint8_t) (addr & 0xFF);
    msb = (uint8_t) (addr >> 8);
}

// Build a uint16_t memory address from the MSB and LSB values
uint16_t Speccy::buildAddress(uint8_t msb, uint8_t lsb) {
    return (msb << 8) | lsb;
}

uint8_t Speccy::getAddressMSB(uint16_t addr) {
    return (uint8_t) (addr >> 8);
}

uint8_t Speccy::getAddressLSB(uint16_t addr) {
    return (uint8_t) (addr & 0xFF);
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
