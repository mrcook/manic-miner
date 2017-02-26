// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "speccy.h"
#include "data.h"
#include "cavern.h"
#include "conveyor.h"
#include "game.h"
#include "guardian_horizontal.h"
#include "guardian_vertical.h"
#include "item.h"
#include "portal.h"
#include "willy.h"

// Setup all game objects
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

// helpers.c
void millisleep(int milliseconds);

int timediff(clock_t t1, clock_t t2);

int getTickCount();
