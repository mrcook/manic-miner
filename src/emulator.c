// Manic Miner C Port Copyright 2017 Michael R. Cook

// A simple ZX Spectrum platform wrapper.

#include "headers.h"

// Memory Layout
//   | ROM | Screen File | Attributes  | Printer Buf | System vars | MD, CHANS, PROGS, etc.
//   ^     ^             ^             ^             ^             ^
//   0   16384         22528         23296         23552         23734
//             (6144)         (768)         (256)         (182)
//
// Initialize a 64K block of memory, for general use
// Holds memory for Screen, Attributes, input, sound, etc.
// The emulator may tap into these for IO.
uint8_t MEM[1024 * 64] = {};

uint8_t regA;
uint8_t regB;
uint8_t regC;
uint8_t regD;
uint8_t regE;
uint8_t regH;
uint8_t regL;

uint16_t regBC;
uint16_t regDE;
uint16_t regHL;


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
//
// IN 31    reads from the Kempston joystick
uint8_t IN(uint16_t addr) {
  // get keyboard input values
  return 0;
}

// OUT(254) border/sound output.
void OUT(uint8_t value) {
  // output the sound!
}