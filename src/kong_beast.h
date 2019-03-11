// Manic Miner C/C++ port Copyright (c) 2016-2019 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#pragma once

#include "headers.h"

bool KONGBEAST();
bool animateKongBeast(); // was KONGBEAST_8

void Kong_openWall();
void Kong_removeBeastFloor();
void Kong_beastFalls();

bool CHKSWITCH(uint16_t switchAddress);

// Has the switch already been flipped?
bool Kong_switchFlipped(uint16_t switchAddress);

void Kong_flipSwitch(uint16_t switchAddress);
