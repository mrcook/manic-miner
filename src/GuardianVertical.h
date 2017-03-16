// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_GUARDIAN_VERTICAL_H
#define MANIC_MINER_GUARDIAN_VERTICAL_H


#include "Headers.h"

/*
    Vertical guardians

    There are four slots, each one seven bytes long, used to hold
    the state of the vertical guardians in the current cavern.

    For each vertical guardian, the seven bytes are used as follows:
    +------+------------------------------+
    | Byte | Contents                     |
    +------+------------------------------+
    | 0    | Attribute                    |
    | 1    | Animation frame              |
    | 2    | Pixel y-coordinate           |
    | 3    | x-coordinate                 |
    | 4    | Pixel y-coordinate increment |
    | 5    | Minimum pixel y-coordinate   |
    | 6    | Maximum pixel y-coordinate   |
    +------+------------------------------+
*/

typedef struct GuardianVertical_ {
    uint8_t attribute;
    uint8_t frame;
    uint8_t yCoord;
    uint8_t xCoord;
    uint8_t yPixelIncrement;
    uint8_t yCoordMinimum;
    uint8_t yCoordMaximum;

    // Guardian graphic data
    uint8_t GGDATA[256];
} GuardianVertical;

// VGUARDIANS
bool GuardianVertical_updateDraw(void);


#endif //MANIC_MINER_GUARDIAN_VERTICAL_H