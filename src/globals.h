// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#pragma once

#include "window.h"
#include "speccy.h"
#include "speccy_display.h"
#include "cavern.h"
#include "game.h"
#include "guardian_horizontal.h"
#include "guardian_vertical.h"
#include "willy.h"

// Setup all game objects
extern Window window;
extern Speccy speccy;
extern SpeccyDisplay speccy_display;
extern Game game;
extern Cavern cavern;
extern GuardianHorizontal HGUARDS[4];
extern GuardianVertical VGUARDS[4];
extern Willy willy;

// Still a few left overs
extern uint8_t EUGDIR;
extern uint8_t EUGHGT;
extern uint8_t GGDATA[256];

extern uint8_t SpeccyDisplay_Font[96][8];
