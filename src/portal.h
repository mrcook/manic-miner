// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#pragma once

#include "headers.h"

typedef struct portal_ {
    // Attribute byte
    uint8_t PORTAL;

    // Address of the portal's location in the attribute buffer at 23552
    uint16_t PORTALLOC1;

    // Address of the portal's location in the screen buffer at 24576
    uint16_t PORTALLOC2;

    // Graphic data
    uint8_t PORTALG[32];
} Portal;
