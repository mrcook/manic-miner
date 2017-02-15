// Manic Miner C Port Copyright 2017 Michael R. Cook

#pragma once

#include "headers.h"

typedef struct Portal_ {
    uint8_t PORTAL;

    // Address of the portal's location in the attribute buffer at 23552
    uint16_t PORTALLOC1;

    // Address of the portal's location in the screen buffer at 24576
    uint16_t PORTALLOC2;

    uint8_t PORTALG[32];
} Portal;
