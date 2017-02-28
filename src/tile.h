// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_TILE_H
#define MANIC_MINER_TILE_H


#include "headers.h"

typedef struct Tile_ {
    // Attribute byte is used both to match the appropriate location in the cavern mapping,
    // and set the sprite Attributes (colours)....I think!
    uint8_t id;

    // The actual tile sprite
    uint8_t sprite[8];
} Tile;


#endif //MANIC_MINER_TILE_H
