// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_ITEM_H
#define MANIC_MINER_ITEM_H


#include "Headers.h"

/*
    Item definition

    +---------+-----------------------------------------------------------------+
    | Byte(s) | Contents                                                        |
    +---------+-----------------------------------------------------------------+
    | 0       | Current attribute                                               |
    | 1,2     | Address of the item's location in the attribute buffer at 23552 |
    | 3       | MSB of the address of the item's location in the screen buffer  |
    |         | at 24576                                                        |
    | 4       | Unused (always 255)                                             |
    +---------+-----------------------------------------------------------------+
*/

typedef struct Item_ {
    uint8_t attribute;
    uint16_t address;
    uint8_t addressMSB;

    uint8_t tile[8];
} Item;


#endif //MANIC_MINER_ITEM_H
