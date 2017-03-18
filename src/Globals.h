// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_GLOBALS_H
#define MANIC_MINER_GLOBALS_H


#include "Window.h"
#include "Speccy.h"
#include "Cavern.h"
#include "Game.h"
#include "GuardianHorizontal.h"
#include "GuardianVertical.h"
#include "Willy.h"

// Setup all game objects
extern Window window;
extern Speccy speccy;
extern Game game;
extern Cavern cavern;
extern GuardianHorizontal HGUARDS[4];
extern GuardianVertical VGUARDS[4];
extern Willy willy;

// Still a few left overs
extern uint8_t EUGDIR;
extern uint8_t EUGHGT;
extern uint8_t GGDATA[256];


#endif //MANIC_MINER_GLOBALS_H
