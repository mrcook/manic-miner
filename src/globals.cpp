// Manic Miner C++ port Copyright (c) 2016-2018 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#include "headers.h"
#include "globals.h"
#include "speccy_display.h"

// Setup all game objects
Speccy speccy = {};
SpeccyDisplay speccy_display = {};
Cavern cavern = {};
Game game = {};
GuardianHorizontal HGUARDS[4] = {};
GuardianVertical VGUARDS[4] = {};
Willy willy = {};

// Still a few left over globals

// EUGENE    - to hold Eugene's direction: 0=down, 1=up
// KONGBEAST - to hold the Kong Beast's status: 0=on the ledge, 1=falling, 2=dead
uint8_t EUGDIR = 0;

// EUGENE    - to hold Eugene's pixel y-coordinate
// KONGBEAST - to hold the Kong Beast's pixel y-coordinate
uint8_t EUGHGT = 0;

// Kong Beast sprites (duplicate from Data_guardianSprites[8] and [12])
// was previously used for others, but they now use the struct
uint8_t GGDATA[256] = {};
