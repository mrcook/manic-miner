// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

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
extern Speccy speccy;
extern Game game;
extern Cavern cavern;
extern GuardianHorizontal HGUARDS[4];
extern GuardianVertical VGUARDS[4];
extern Willy willy;

// Text and graphics
extern uint8_t EUGDIR;
extern uint8_t EUGHGT;
extern uint8_t GGDATA[256]; // now only used for Kong Beast
extern uint8_t SWORDFISH[32];

extern uint8_t LRMOVEMENT[16];

// data.c
extern uint8_t WILLYDATA[256];
extern uint16_t SBUFADDRS[128];

extern int CHEATDT[7];

extern uint8_t THEMETUNE[95][3];
extern uint8_t GAMETUNE[64];

extern uint8_t LOWERATTRS[512];
extern uint8_t TITLESCR1[2048];
extern uint8_t TITLESCR2[2048];

extern uint8_t PLINTH[32];
extern uint8_t BOOT[32];
extern uint8_t EUGENEG[32];
extern uint8_t CAVERN19[512];

void initialize_cavern0();

// helpers.c
void millisleep(int milliseconds);

int timediff(clock_t t1, clock_t t2);

int getTickCount();