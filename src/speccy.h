// Speccy library Copyright 2017 Michael R. Cook

// Simple ZX Spectrum platform wrapper to aid developers porting Z80 software.

// Memory Layout
//   | ROM | Screen File | Attributes  | Printer Buf | System vars | MD, CHANS, PROGS, etc.
//   ^     ^             ^             ^             ^             ^
//   0   16384         22528         23296         23552         23734
//             (6144)         (768)         (256)         (182)

// 23672: Interrupt counter (stored in the system variables).
// The interrupt service routine in the ROM updates the Spectrum’s 24-bit
// frames counter 50 times per second, as well as doing other things.

// NOTE: the user can use the address space from 23552 onwards!

#pragma once

#include "headers.h"

static const int TOTAL_MEMORY = 1024 * 64;
static const int SCREEN_SIZE = 6144;
static const int ATTR_SIZE = 768;

typedef struct Speccy_ {
    // The Spectrum generates 50 frames per second (screen updates/interrupts)
    int nextFrameTick;

    // Initialize a 64K block of memory, for general use.
    // Mostly you'll use this as a buffer for user memory
    uint8_t memory[TOTAL_MEMORY];

    // Spectrum display file screen/attribute memory.
    // NOTE: this will be the Spectrum display format
    uint8_t screen[SCREEN_SIZE];
    uint8_t attributes[ATTR_SIZE];

    // Display buffers in a standard (linear) format.
    // Useful for sending to ncurses/SDL/etc.
    uint8_t convertedScreen[SCREEN_SIZE];
} Speccy;

// Tick the world over.
// Call this whenever the display needs updating or FPS syncing.
void Speccy_tick(void);

// General memory read/write. Use as needed.
uint8_t Speccy_read(int address);

void Speccy_write(int address, uint8_t byte);

// Display specific memory. Addresses will be adjusted accordingly,
// so just use whatever was given in the assembly code.

void Speccy_clearScreen();

uint8_t Speccy_readScreen(int address);

void Speccy_writeScreen(int address, uint8_t byte);

uint8_t Speccy_readAttribute(int address);

void Speccy_writeAttribute(int address, uint8_t byte);

// Handy function to convert a byte to an array of bits,
// so you can more easily create pixel based graphics.
void Speccy_byteToBits(uint8_t byte, uint8_t *bits);

// Converts the entire spectrum screen format
// to a regular (linear) screen buffer.
void Speccy_convertScreenFormat();

// Read a byte from the converted screen buffer
uint8_t Speccy_readScreenBuffer(int address);

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
uint8_t IN(uint16_t addr);

// OUT(254) border/sound output.
void OUT(uint8_t value);

// Split a uint16_t memory address into its MSB and LSB values
void split_address(uint16_t addr, uint8_t *msb, uint8_t *lsb);

// Build a uint16_t memory address from the MSB and LSB values
uint16_t build_address(uint8_t msb, uint8_t lsb);

// Rotate left n places
uint8_t rotl(uint8_t a, uint8_t n);

// Rotate right n places
uint8_t rotr(uint8_t a, uint8_t n);

