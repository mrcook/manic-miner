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

// Eugene's direction or the Kong Beast's status
//
// Initialised by the routine at STARTGAME, and used by the routines at EUGENE
// (to hold Eugene's direction: 0=down, 1=up) and KONGBEAST (to hold the Kong
// Beast's status: 0=on the ledge, 1=falling, 2=dead).
uint8_t EUGDIR;

// Eugene's or the Kong Beast's pixel y-coordinate
//
// Initialised by the routine at STARTGAME, and used by the routines at START
// (to hold the index into the message scrolled across the screen after the
// theme tune has finished playing), ENDGAM (to hold the distance of the boot
// from the top of the screen as it descends onto Willy), EUGENE (to hold
// Eugene's pixel y-coordinate) and KONGBEAST (to hold the Kong Beast's pixel
// y-coordinate).
uint8_t EUGHGT;

// Now just the Kong Beast Guardian graphic data -- MRC
uint8_t GGDATA[256];

// IMPORTANT: not initialized anywhere, so let's do that here -MRC-
// Swordfish graphic data
uint8_t SWORDFISH[32];
