// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_CAVERN_H
#define MANIC_MINER_CAVERN_H


#include "Headers.h"

#include "Conveyor.h"
#include "Item.h"
#include "Portal.h"
#include "Tile.h"

typedef struct Cavern_ {
    // Current cavern number
    uint8_t SHEET;

    // Specify the cavern name
    char CAVERNNAME[32];

    // Game clock
    //
    // Initialised by the routine at STARTGAME, updated on every pass through
    // the main loop by the routine at Cavern_decreaseAir, and used for timing purposes
    // by the routines at GuardianHorizontal_update, EUGENE and KONGBEAST.
    // Its value (which is always a multiple of 4) is also used by the routine
    // at Cavern_decreaseAir to compute the amount of air to draw in the cell at the right
    // end of the air bar.
    uint8_t CLOCK;

    // Remaining air supply (in practice, always set to 63)
    // Its value ranges from 36 to 63 and is actually the LSB of the display
    // file address for the cell at the right end of the air bar. The amount
    // of air to draw in this cell is determined by the value of the game
    // clock at CLOCK.
    uint8_t AIR;

    // Screen border colour
    uint8_t BORDER;

    Tile BACKGROUND;
    Tile FLOOR;
    Tile CRUMBLING;
    Tile WALL;
    ConveyorTile CONVEYOR;
    Tile NASTY1;
    Tile NASTY2;

    // The extra tile behaves like a floor tile and is used as such in "The Endorian Forest",
    // "Attack of the Mutant Telephones", "Ore Refinery", "Skylab Landing Bay" and "The Bank".
    // It is also used as spider silk in "The Menagerie", and as a switch in
    // "Miner Willy meets the Kong Beast" and "Return of the Alien Kong Beast".
    Tile EXTRA;

    Portal portal;

    Item ITEMS[5];

    // Attributes that define the layout of the cavern. aka CAVERN0.
    uint8_t layout[512];
} Cavern;

bool Cavern_loadData(uint8_t id);

void Cavern_updateGameClock(void);

bool Cavern_isAirDepleted(void);

bool Cavern_decreaseAir(void);

void Cavern_drawAirBar(void);

void Cavern_moveConveyorBelts(void);


#endif //MANIC_MINER_CAVERN_H