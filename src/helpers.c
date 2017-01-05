// Manic Miner C Port Copyright 2017 Michael R. Cook

// Cross Platform Helper Functions for Manic Miner

#include "headers.h"

// Sleep for the given number of milliseconds
void millisleep(int milliseconds) {
  #ifdef _WIN32
    Sleep(milliseconds);
  #elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
  #else
    usleep((useconds_t)milliseconds * 1000);
  #endif
}

int timediff(clock_t t1, clock_t t2) {
    return (int)(((double)t2 - t1) / CLOCKS_PER_SEC * 1000);
}

// Split a uint16_t memory address into its MSB and LSB values
void split_address(uint16_t addr, uint8_t *msb, uint8_t *lsb) {
  *lsb = (uint8_t)(addr & 0xFF);
  *msb = (uint8_t)(addr >> 8);
}

// Build a uint16_t memory address from the MSB and LSB values
uint16_t build_address(uint8_t msb, uint8_t lsb) {
  return (msb << 8) | lsb;
}