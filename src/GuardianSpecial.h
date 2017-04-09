// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_GUARDIANSPECIAL_H
#define MANIC_MINER_GUARDIANSPECIAL_H


#include "Headers.h"

// Check sheet and update Vertical Guardians Eugene, Skylaps, Kongbeast.
// return `true` if Willy is dead! (used to be `goto LOOP_4`)
bool UpdateSpecialVerticalGuardians(uint8_t sheet);

// Move and draw the light beam in Solar Power Generator.
void LIGHTBEAM(void);

// EUGENE
// Move and draw Eugene in Eugene's Lair.
// First we move Eugene up or down, or change his direction.
void EugeneMove(void);

// After moving Eugene, it's time to draw him.
// return `true` if Willy has died.
bool EugeneDraw(void);

// EUGENE_3
// Sets the colour attributes for a 16x16 pixel sprite.
// SKYLABS:    to set the attributes for a Skylab.
// GuardianVertical::updateAndDraw: to set the attributes for a vertical guardian.
// KONGBEAST:  to set the attributes for the Kong Beast.
void UpdateGuardianColourAttributes(uint16_t addr, uint8_t ink_colour);

// Move and draw the Skylabs in Skylab Landing Bay.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool SKYLABS(void);


#endif //MANIC_MINER_GUARDIANSPECIAL_H
