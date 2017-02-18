// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "headers.h"

#include "speccy.h"
#include "cavern.h"
#include "conveyor.h"
#include "game.h"
#include "guardian_horizontal.h"
#include "guardian_vertical.h"
#include "item.h"
#include "portal.h"
#include "willy.h"

// Setup all game objects
Speccy speccy;
Cavern cavern;
Game game;
GuardianHorizontal HGUARDS[4];
GuardianVertical VGUARDS[4];
Willy willy;

// Now just the Kong Beast Guardian graphic data -- MRC
uint8_t GGDATA[256];

// IMPORTANT: not initialized anywhere, so let's do that here -MRC-
// Swordfish graphic data
uint8_t SWORDFISH[32];
