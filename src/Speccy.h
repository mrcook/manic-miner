// Speccy library Copyright 2017 Michael R. Cook

#ifndef MANIC_MINER_SPECCY_H
#define MANIC_MINER_SPECCY_H


#include "Headers.h"
#include "SpeccyColour.h"

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
//   0  BLACK    000           #000000  #000000
//   1  BLUE     001           #0000D7  #0000FF
//   2  RED      010           #D70000  #FF0000
//   3  MAGENTA  011           #D700D7  #FF00FF  (purple)
//   4  GREEN    100           #00D700  #00FF00
//   5  CYAN     101           #00D7D7  #00FFFF  (pale blue)
//   6  YELLOW   110           #D7D700  #FFFF00
//   7  WHITE    111           #D7D7D7  #FFFFFF

// FLASHing swaps the INK and PAPER colours, and changes every 16 frames.

// 23672: Interrupt counter (stored in the system variables).
// The interrupt service routine in the ROM updates the Spectrum’s 24-bit
// frames counter 50 times per second, as well as doing other things.
// The Spectrum generates 50 screen updates per second (frames/interrupts)
// and dictates the speed at which your game runs at.
// Usually 50, 25, or 17. E.g. Manic Miner runs at 17 FPS

// NOTE: the user can use the address space from 23552 onwards!

static const int TOTAL_MEMORY = 1024 * 64;
static const int ATTR_SIZE = 768;
static const int SPECCY_SCREEN = 6144;

class Speccy {
public:
    // Used to calculate the correct frame rate, during play
    int frameTick;

    // Initialize a 64K block of memory, for general use.
    // Mostly you'll use this as a buffer for user memory
    uint8_t memory[TOTAL_MEMORY];

    // Initialize the speccy framework (FPS, etc.)
    void initialize(int fps);

    // Tick the world over.
    // Call this whenever the display needs updating or FPS syncing.
    void tick();

    // General memory read.
    uint8_t readMemory(int address);

    // General memory write.
    void writeMemory(int address, uint8_t byte);

    //
    // Core Input/Output functions
    //

    void setBorderColour(uint8_t colour);

    // The Spectrum uses OUT to make a sound, but here we use a custom function
    void makeSound(int pitch, uint8_t duration, uint8_t volume);

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
    static uint8_t IN(uint16_t addr);

    // OUT(254) border/sound output.
    static void OUT(uint8_t value);

    static uint8_t Font[96][8];

    void printCharacter(char ch, uint16_t address);

    // Print a message string to the display file
    void printString(void *msg, uint16_t address, uint8_t len);

    // Draw a sprite item to the given screen address
    void drawSprite(void *character, uint16_t address, uint8_t len);

    //
    // Display/Attribute file manipulation
    //

    // Clear the entire Spectrum display file
    void clearDisplayFile(void);

    // Clears the entire attributes file
    void clearAttributesFile(void);

    // Clear the top two-thirds of the display file
    void clearTopTwoThirdsOfDisplayFile(void);

    // Clear the bottom third of the display file.
    void clearBottomThirdOfDisplayFile(void);

    // Fill the top two thirds of the attribute file with the value given.
    void fillTopTwoThirdsOfAttributeFileWith(uint8_t byte);

    uint8_t readScreen(int address);

    void writeScreen(int address, uint8_t byte);

    uint8_t readAttribute(int address);

    void writeAttribute(int address, uint8_t byte);


    /*
     * Utility functions to help porting from Z80 to C
     *
     *   - Manipulating Address values (split/join MSB and LSB)
     *   - Bit shift/rotate uint8_t values
     *   - Manipulate Attribute bytes (split out colours)
     */

    // Split a uint16_t memory address into its MSB and LSB values
    static void splitAddress(uint16_t addr, uint8_t *msb, uint8_t *lsb);

    // Build a uint16_t memory address from the MSB and LSB values
    static uint16_t buildAddress(uint8_t msb, uint8_t lsb);

    // Rotate left n places
    static uint8_t rotL(uint8_t a, uint8_t n);

    // Rotate right n places
    static uint8_t rotR(uint8_t a, uint8_t n);
};


#endif //MANIC_MINER_SPECCY_H
