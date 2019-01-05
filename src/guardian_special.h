// Manic Miner C++ port Copyright (c) 2016-2018 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#pragma once

#include "headers.h"

// Check sheet and update Vertical Guardians Eugene, Skylaps, Kongbeast.
// return `true` if Willy is dead! (used to be `goto LOOP_4`)
bool UpdateSpecialVerticalGuardians(uint8_t sheet);

// Move and draw the light beam in Solar Power Generator.
void LIGHTBEAM();

// EUGENE
// Move and GuardianH_draw Eugene in Eugene's Lair.
// First we move Eugene up or down, or change his direction.
void EugeneMove();

// After moving Eugene, it's time to GuardianH_draw him.
// return `true` if Willy has died.
bool EugeneDraw();

// EUGENE_3
// Sets the colour attributes for a 16x16 pixel sprite.
// SKYLABS:    to set the attributes for a Skylab.
// GuardianV_updateAndDraw: to set the attributes for a vertical guardian.
// KONGBEAST:  to set the attributes for the Kong Beast.
void UpdateGuardianColourAttributes(uint16_t addr, uint8_t ink_colour);

// Move and draw the Skylabs in Skylab Landing Bay.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool SKYLABS();
