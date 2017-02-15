// Manic Miner C Port Copyright 2017 Michael R. Cook

#pragma once

#include "headers.h"

typedef struct Tile_ {
    // ID used to match the appropriate location in the cavern mapping.
    int id;

    // The actual tile sprite
    uint8_t sprite[8];
} Tile;
