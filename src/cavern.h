// Manic Miner C Port Copyright 2017 Michael R. Cook

#pragma once

#include "headers.h"
#include "conveyor.h"
#include "portal.h"
#include "tile.h"
#include "item.h"

typedef struct Cavern_ {
    int SHEET;

    char CAVERNNAME[32];

    int CLOCK;

    int AIR;

    uint8_t BORDER;

    Tile BACKGROUND;
    Tile FLOOR;
    Tile CRUMBLING;
    Tile WALL;
    Conveyor CONVEYOR;
    Tile NASTY1;
    Tile NASTY2;

    // The extra tile behaves like a floor tile and is used as such in "The Endorian Forest",
    // "Attack of the Mutant Telephones", "Ore Refinery", "Skylab Landing Bay" and "The Bank".
    // It is also used as spider silk in "The Menagerie", and as a switch in
    // "Miner Willy meets the Kong Beast" and "Return of the Alien Kong Beast".
    Tile EXTRA;

    Portal portal;

    Item ITEMS[5];

    uint8_t map[512];
} Cavern;
