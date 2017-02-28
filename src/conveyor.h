// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_CONVEYOR_H
#define MANIC_MINER_CONVEYOR_H


#include "headers.h"
#include "tile.h"

typedef struct ConveyorTile_ {
    // ID used to match the appropriate location in the cavern mapping.
    int id;

    // Direction (0=left, 1=right)
    uint8_t CONVDIR;

    // Address of the conveyor's location in the screen buffer at 28672
    uint16_t CONVLOC;

    uint8_t CONVLEN;

    // The actual tile sprite
    uint8_t sprite[8];

} ConveyorTile;


#endif //MANIC_MINER_CONVEYOR_H
