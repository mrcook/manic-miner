// Speccy library Copyright 2017 Michael R. Cook

#ifndef MANIC_MINER_SPECCY_H
#define MANIC_MINER_SPECCY_H


#include "Headers.h"
#include "Colour.h"

// Simple ZX Spectrum platform wrapper to aid developers porting Z80 software.

// Memory Layout
//   | ROM | Screen File | Attributes  | Printer Buf | System vars | MD, CHANS, PROGS, etc.
//   ^     ^             ^             ^             ^             ^
//   0   16384         22528         23296         23552         23734
//             (6144)         (768)         (256)         (182)

// Colours:
//        FL BR P3 P2 P1 I3 I2 I1 (FLASH, BRIGHT, PAPER, INK)
//  bits:  7  6  5  4  3  2  1  0
//
// INK and PAPER have values 0-7, corresponding to the table below.
//
// Colour number, name, binary value, with their RGB (hex) equivalent
//   #  Name     Binary   RGB: NORMAL   BRIGHT
//   0  BLACK    000           #000000  #000000c
//   1  BLUE     001           #0000D7  #0000FFc
//   2  RED      010           #D70000  #FF0000c
//   3  MAGENTA  011           #D700D7  #FF00FFc  (purple)
//   4  GREEN    100           #00D700  #00FF00c
//   5  CYAN     101           #00D7D7  #00FFFFc  (pale blue)
//   6  YELLOW   110           #D7D700  #FFFF00c
//   7  WHITE    111           #D7D7D7  #FFFFFFc

// 23672: Interrupt counter (stored in the system variables).
// The interrupt service routine in the ROM updates the Spectrum’s 24-bit
// frames counter 50 times per second, as well as doing other things.
// The Spectrum generates 50 screen updates per second (frames/interrupts)
// and dictates the speed at which your game runs at.
// Usually 50, 25, or 17. E.g. Manic Miner runs at 17 FPS

// NOTE: the user can use the address space from 23552 onwards!

static const int TOTAL_MEMORY = 1024 * 64;
static const int SCREEN_SIZE = 6144;
static const int SCREEN_PIXELS_SIZE = SCREEN_SIZE * 8;
static const int ATTR_SIZE = 768;

class Speccy {
public:
    // Used to calculate the correct frame rate, during play
    int frameTick;

    // Initialize a 64K block of memory, for general use.
    // Mostly you'll use this as a buffer for user memory
    uint8_t memory[TOTAL_MEMORY];

    // Display buffers in a standard (linear) format.
    // Useful for sending to ncurses/SDL/etc.
    // contains data for each pixel, not byte character
    uint8_t newScreen[SCREEN_SIZE * 8];


    // IN from Keyboard and Joystick
    // IN 65278 reads the half row CAPS SHIFT to V
    // IN 65022 reads the half row A to G
    // IN 64510 reads the half row Q to T
    // IN 63486 reads the half row 1 to 5
    // IN 61438 reads the half row O to 6
    // IN 57342 reads the half row P to 7
    // IN 49150 reads the half row ENTER to H
    // IN 32766 reads the half row SPACE to B
    //
    // IN 254   reads every row of keys
    static uint8_t IN(uint16_t addr) {
        addr = 0; // prevents compiler error

        // get keyboard input values
        return 0;
    }

    // OUT(254) border/sound output.
    static void OUT(uint8_t value) {
        value = 0; // prevents compiler error

        // output the sound, border colour!
    }


    // Handy function to convert a byte to an array of bits,
    // so you can more easily create pixel based graphics.
    static void byteToBits(uint8_t byte, uint8_t *bits) {
        for (int i = 0; i < 8; i++) {
            if (byte & (1 << i)) {
                bits[i] = 1;
            } else {
                bits[i] = 0;
            }
        }
    }

    // Split a uint16_t memory address into its MSB and LSB values
    static void splitAddress(uint16_t addr, uint8_t *msb, uint8_t *lsb) {
        *lsb = (uint8_t) (addr & 0xFF);
        *msb = (uint8_t) (addr >> 8);
    }

    // Build a uint16_t memory address from the MSB and LSB values
    static uint16_t buildAddress(uint8_t msb, uint8_t lsb) {
        return (msb << 8) | lsb;
    }

    // Rotate left n places
    static uint8_t rotL(uint8_t a, uint8_t n) {
        assert (n > 0 && n < 8);
        return (a << n) | (a >> (8 - n));
    }

    // Rotate right n places
    static uint8_t rotR(uint8_t a, uint8_t n) {
        assert (n > 0 && n < 8);
        return (a >> n) | (a << (8 - n));
    }
};

// Initialize the speccy framework (FPS, etc.)
void Speccy_initialize(int fps);

// Tick the world over.
// Call this whenever the display needs updating or FPS syncing.
void Speccy_tick(void);

/*
 * NewScreen format functions
 *
 * Converts the original Spectrum screen layout to a more standard linear
 * format; bytes are sequential reading from left-to-right, top-to-bottom.
 */

// Converts the entire spectrum screen format to the NewScreen standard format
void Speccy_convertScreenFormat();

// Write a colour pixel to the new screen.
// The colour is taken from the Attributes File, using the given address.
void writeColourPixelToNewScreen(uint8_t pixel, int newScreenAddress);

// Given an address from the new screen array (256*192 pixels),
// calculate the Spectrum Attribute File address
uint8_t getAttrFromAttributesFile(int pixelAddress);

// Read a byte from the NewScreen format
uint8_t Speccy_readNewScreen(int address);

//
// General memory access
//

// General memory read/write. Use as needed.
uint8_t Speccy_read(int address);

void Speccy_write(int address, uint8_t byte);


//
// Screen/Attribute access
//

// Clear the entire Spectrum display file
void Speccy_clearDisplayFile(void);

// Clears the entire attributes file
void Speccy_clearAttributesFile(void);

// Clear the top two-thirds of the display file
void Speccy_clearTopTwoThirdsOfDisplayFile(void);

// Clear the bottom third of the display file.
void Speccy_clearBottomThirdOfDisplayFile(void);

// Fill the top two thirds of the attribute file with the value given.
void Speccy_fillTopTwoThirdsOfAttributeFileWith(uint8_t byte);

uint8_t Speccy_readScreen(int address);

void Speccy_writeScreen(int address, uint8_t byte);

uint8_t Speccy_readAttribute(int address);

void Speccy_writeAttribute(int address, uint8_t byte);

// Print a message string to the display file
void Speccy_printMessage(void *msg, uint16_t address, uint8_t len);

// Draw a sprite item to the given screen address
void Speccy_drawSpriteAt(void *character, uint16_t address, uint8_t len);

//
// Sound and border functions
//

// Split a Spectrum attribute byte into it's colour parts
void Speccy_splitColorAttribute(uint8_t attribute, Colour *colour);

void Speccy_setBorderColour(uint8_t colour);

// The Spectrum uses OUT to make a sound, but here we use a custom function
void Speccy_makeSound(uint8_t pitch, uint8_t duration, uint8_t delay);


//
// Utility functions
//

void splitColourAttribute(uint8_t attribute, Colour *colour);


void printFontCharacterAt(char ch, uint16_t address);


#endif //MANIC_MINER_SPECCY_H
