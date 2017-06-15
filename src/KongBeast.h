// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_KONG_BEAST_H
#define MANIC_MINER_KONG_BEAST_H


#include "Headers.h"

bool KONGBEAST(void);
bool animateKongBeast(void); // was KONGBEAST_8

void Kong_openWall(void);
void Kong_removeBeastFloor(void);
void Kong_beastFalls(void);

bool CHKSWITCH(uint16_t switchAddress);

// Has the switch already been flipped?
bool Kong_switchFlipped(uint16_t switchAddress);

void Kong_flipSwitch(uint16_t switchAddress);

#endif //MANIC_MINER_KONG_BEAST_H
