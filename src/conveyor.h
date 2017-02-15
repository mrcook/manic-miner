// Manic Miner C Port Copyright 2017 Michael R. Cook

#pragma once

#include "headers.h"
#include "tile.h"

enum ConveyorDirection {
    Left,
    Right,
};

typedef struct Conveyor_ {
    // ID used to match the appropriate location in the cavern mapping.
    int id;

    // The actual tile sprite
    uint8_t sprite[8];

    // Direction (0=left, 1=right)
    enum ConveyorDirection CONVDIR;

    // Address of the conveyor's location in the screen buffer at 28672
    uint16_t CONVLOC;

    uint8_t CONVLEN;
} Conveyor;
