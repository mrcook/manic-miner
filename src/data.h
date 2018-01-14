// Manic Miner C++ port Copyright (c) 2016-2018 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#pragma once

// Data arrays from which we read cavern data into the game

#include "headers.h"

// data_cavern.c
extern char Data_cavernNames[20][33];
extern uint8_t Data_pixelY[20];
extern uint8_t Data_frames[20];
extern uint8_t Data_dmFlags[20];
extern uint8_t Data_airborneStatues[20];
extern uint16_t Data_locations[20];
extern uint8_t Data_jumpingStatuses[20];
extern uint16_t Data_conveyorBeltsParams[20][3];
extern uint8_t Data_borderColours[20];
extern uint8_t Data_itemAttrs[20];
extern uint16_t Data_itemsData[20][5][5];
extern uint8_t Data_portalAttributes[20];
extern uint8_t Data_portalGraphics[20][32];
extern uint16_t Data_portalAttributeLocations[20];
extern uint16_t Data_portalScreenLocations[20];
extern uint8_t Data_itemGraphics[20][8];
extern uint8_t Data_airSupplies[20];
extern uint8_t Data_clockValues[20];
extern uint8_t Data_cavernLayouts[20][512];
extern uint16_t SBUFADDRS[128];

// data_guardian_sprites.c
extern uint16_t Data_horizontalGuardianLocations[20][4][6];
extern uint8_t Data_verticalGuardianLocations[20][4][7];
extern uint8_t Data_guardianSprites[20][256];

// data_music.c
extern uint8_t THEMETUNE[95][3];
extern uint8_t GAMETUNE[64];

// data_tiles.c
extern uint8_t SWORDFISH[32];
extern uint8_t PLINTH[32];
extern uint8_t BOOT[32];
extern uint8_t EUGENEG[32];
extern uint8_t Data_tileBackgrounds[20][9];
extern uint8_t Data_tileFloors[20][9];
extern uint8_t Data_tileCrumblingFloors[20][9];
extern uint8_t Data_tileWalls[20][9];
extern uint8_t Data_tileConveyorBelts[20][9];
extern uint8_t Data_tileNasties1[20][9];
extern uint8_t Data_tileNasties2[20][9];
extern uint8_t Data_tileExtras[20][9];

// data_title_screen.c
extern const char *MESSINTRO;
extern uint8_t TITLESCR1[2048];
extern uint8_t TITLESCR2[2048];
extern uint8_t LOWERATTRS[512];

// data_willy_sprites.c
extern uint8_t WILLYDATA[256];
