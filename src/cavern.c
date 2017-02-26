// Manic Miner C Port Copyright 2017 Michael R. Cook

#include "externs.h"

bool Cavern_isAirDepleted() {
    // For some reason 36 means no air left
    return cavern.AIR == 36;
}

// Initialize arrays and copy the cavern definition into the game status buffer.
// We must copy a total of 512 bytes into the buffer address from 32768.
bool Cavern_loadData(uint8_t id) {
    uint8_t msb, lsb;
    uint16_t addr = 32768;

    // Load the Cavern Name
    strcpy(cavern.CAVERNNAME, Data_cavernNames[id]);
    for (int i = 0; i < 32; i++) {
        speccy.memory[addr++] = (uint8_t) cavern.CAVERNNAME[i];
    }

    // Load the cavern tile map layout
    for (int i = 0; i < 512; i++) {
        cavern.layout[i] = Data_cavernLayouts[id][i];
    }

    //
    // Load the cavern tile GFX
    //
    cavern.BACKGROUND.id = Data_tileBackgrounds[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            cavern.BACKGROUND.sprite[i - 1] = Data_tileBackgrounds[id][i];
        }
        speccy.memory[addr++] = Data_tileBackgrounds[id][i];
    }
    cavern.FLOOR.id = Data_tileFloors[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            cavern.FLOOR.sprite[i - 1] = Data_tileFloors[id][i];
        }
        speccy.memory[addr++] = Data_tileFloors[id][i];
    }
    cavern.CRUMBLING.id = Data_tileCrumblingFloors[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            cavern.CRUMBLING.sprite[i - 1] = Data_tileCrumblingFloors[id][i];
        }
        speccy.memory[addr++] = Data_tileCrumblingFloors[id][i];
    }
    cavern.WALL.id = Data_tileWalls[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            cavern.WALL.sprite[i - 1] = Data_tileWalls[id][i];
        }
        speccy.memory[addr++] = Data_tileWalls[id][i];
    }
    cavern.CONVEYOR.id = Data_tileConveyorBelts[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            cavern.CONVEYOR.sprite[i - 1] = Data_tileConveyorBelts[id][i];
        }
        speccy.memory[addr++] = Data_tileConveyorBelts[id][i];
    }
    cavern.NASTY1.id = Data_tileNasties1[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            cavern.NASTY1.sprite[i - 1] = Data_tileNasties1[id][i];
        }
        speccy.memory[addr++] = Data_tileNasties1[id][i];
    }
    cavern.NASTY2.id = Data_tileNasties2[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            cavern.NASTY2.sprite[i - 1] = Data_tileNasties2[id][i];
        }
        speccy.memory[addr++] = Data_tileNasties2[id][i];
    }
    cavern.EXTRA.id = Data_tileExtras[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            cavern.EXTRA.sprite[i - 1] = Data_tileExtras[id][i];
        }
        speccy.memory[addr++] = Data_tileExtras[id][i];
    }

    //
    // The next seven bytes are copied to 32872-32878 and specify
    // Miner Willy's initial location and appearance in the cavern.
    //

    // Pixel y-coordinate * 2 (see PIXEL_Y)
    willy.PIXEL_Y = Data_pixelY[id];
    speccy.memory[addr++] = willy.PIXEL_Y;

    // Animation frame (see FRAME)
    willy.FRAME = Data_frames[id];
    speccy.memory[addr++] = willy.FRAME;

    // Direction and movement flags: facing right (see DMFLAGS)
    willy.DMFLAGS = Data_dmFlags[id];
    speccy.memory[addr++] = willy.DMFLAGS;

    // Airborne status indicator (see AIRBORNE)
    willy.AIRBORNE = Data_airborneStatues[id];
    speccy.memory[addr++] = willy.AIRBORNE;

    // Location in the attribute buffer at 23552: (13,2) (see LOCATION)
    willy.LOCATION = Data_locations[id];
    split_address(willy.LOCATION, &msb, &lsb);
    speccy.memory[addr++] = msb;
    speccy.memory[addr++] = lsb;

    // Jumping animation counter (see JUMPING)
    willy.JUMPING = Data_jumpingStatuses[id];
    speccy.memory[addr++] = willy.JUMPING;

    //
    // The next four bytes are copied to CONVDIR and specify the
    // direction, location and length of the conveyor.
    //

    // Direction in which the conveyor belt is travelling
    cavern.CONVEYOR.CONVDIR = (uint8_t) Data_conveyorBeltsParams[id][0];
    speccy.memory[addr++] = cavern.CONVEYOR.CONVDIR;

    // Location in the screen buffer at 28672
    cavern.CONVEYOR.CONVLOC = Data_conveyorBeltsParams[id][1];
    split_address(cavern.CONVEYOR.CONVLOC, &msb, &lsb);
    speccy.memory[addr++] = msb;
    speccy.memory[addr++] = lsb;

    // Length of the conveyor belt
    cavern.CONVEYOR.CONVLEN = (uint8_t) Data_conveyorBeltsParams[id][2];
    speccy.memory[addr++] = cavern.CONVEYOR.CONVLEN;


    // Specifies the BORDER colour
    cavern.BORDER = Data_borderColours[id];
    speccy.memory[addr++] = cavern.BORDER;

    // The next byte is copied to ITEMATTR, but is not used. // FIXME: why is it here then?
    game.ITEMATTR = Data_itemAttrs[id];
    speccy.memory[addr++] = game.ITEMATTR;

    //
    // The next 25 bytes are copied to ITEMS and specify the
    // location and initial colour of the items in the cavern.
    //
    for (int i = 0; i < 5; i++) {
        // load the item
        cavern.ITEMS[i].attribute = (uint8_t) Data_itemsData[id][i][0];
        cavern.ITEMS[i].address = Data_itemsData[id][i][1];
        cavern.ITEMS[i].addressMSB = (uint8_t) Data_itemsData[id][i][2];
        for (int j = 0; j < 8; j++) {
            cavern.ITEMS[i].tile[j] = Data_itemGraphics[id][j];
        }

        // Add items to the buffer memory
        speccy.memory[addr++] = (uint8_t) Data_itemsData[id][i][0];

        split_address(Data_itemsData[id][i][1], &msb, &lsb);
        speccy.memory[addr++] = msb;
        speccy.memory[addr++] = lsb;

        speccy.memory[addr++] = (uint8_t) Data_itemsData[id][i][2];
        speccy.memory[addr++] = (uint8_t) Data_itemsData[id][i][3];
    }
    // ITEMS terminator byte
    speccy.memory[addr++] = 255;

    //
    // The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
    //
    cavern.portal.PORTAL = Data_portalAttributes[id];
    speccy.memory[addr++] = cavern.portal.PORTAL;

    for (int i = 0; i < 32; i++) {
        cavern.portal.PORTALG[i] = Data_portalGraphics[id][i];
        speccy.memory[addr++] = Data_portalGraphics[id][i];
    }

    // Location in the attribute buffer at 23552
    cavern.portal.PORTALLOC1 = Data_portalAttributeLocations[id];
    // Location in the screen buffer at 24576
    cavern.portal.PORTALLOC2 = Data_portalScreenLocations[id];

    split_address(cavern.portal.PORTALLOC1, &msb, &lsb);
    speccy.memory[addr++] = msb;
    speccy.memory[addr++] = lsb;
    split_address(cavern.portal.PORTALLOC2, &msb, &lsb);
    speccy.memory[addr++] = msb;
    speccy.memory[addr++] = lsb;

    // NOTE: ITEM[8] is also used above to set tile on item struct
    // The next eight bytes are copied to ITEM and define the item graphic.
    for (int i = 0; i < 8; i++) {
        speccy.memory[addr++] = Data_itemGraphics[id][i];
    }

    // The next byte is copied to AIR and specifies the initial air supply in the cavern.
    cavern.AIR = Data_airSupplies[id];
    speccy.memory[addr++] = cavern.AIR;

    // The next byte is copied to CLOCK and initialises the game clock.
    cavern.CLOCK = Data_clockValues[id];
    speccy.memory[addr++] = cavern.CLOCK;

    //
    // The next bytes are copied to HGUARDS and define the horizontal guardians.
    //
    for (int i = 0; i < 4; i++) {
        // Initialise the horizontal guardian structs
        HGUARDS[i].speedColour = (uint8_t) Data_horizontalGuardianLocations[id][i][0];
        HGUARDS[i].attributeAddress = Data_horizontalGuardianLocations[id][i][1];
        HGUARDS[i].addressMSB = (uint8_t) Data_horizontalGuardianLocations[id][i][2];
        HGUARDS[i].frame = (uint8_t) Data_horizontalGuardianLocations[id][i][3];
        HGUARDS[i].addressLeftLSB = (uint8_t) Data_horizontalGuardianLocations[id][i][4];
        HGUARDS[i].addressRightLSB = (uint8_t) Data_horizontalGuardianLocations[id][i][5];

        // While we're here, let's load up the sprites data.
        // We fill the memory later on in the GGDATA section.
        for (int j = 0; j < 256; j++) {
            HGUARDS[i].GGDATA[j] = Data_guardianSprites[id][j];
        }

        // Load guardian data into memory
        speccy.memory[addr++] = (uint8_t) Data_horizontalGuardianLocations[id][i][0];
        split_address(Data_horizontalGuardianLocations[id][i][1], &msb, &lsb);
        speccy.memory[addr++] = msb;
        speccy.memory[addr++] = lsb;
        speccy.memory[addr++] = (uint8_t) Data_horizontalGuardianLocations[id][i][2];
        speccy.memory[addr++] = (uint8_t) Data_horizontalGuardianLocations[id][i][3];
        speccy.memory[addr++] = (uint8_t) Data_horizontalGuardianLocations[id][i][4];
        speccy.memory[addr++] = (uint8_t) Data_horizontalGuardianLocations[id][i][5];
    }
    // HGUARDS terminator byte
    speccy.memory[addr++] = 255;


    // The next two bytes are copied to EUGDIR and EUGHGT but are not used.
    EUGDIR = 0;
    EUGHGT = 0;
    speccy.memory[addr++] = 0;
    speccy.memory[addr++] = 0;

    // FIXME: between here and GGDATA we need to fill 35 bytes of memory
    // FIXME: However, we're now always loading 4 VGUARD (empty or otherwise),
    // FIXME: so we will need to handle the SWORDFISH, BOOT and other gfx differently.

    //
    // The next bytes are copied to VGUARDS for the vertical guardians
    //
    for (int i = 0; i < 4; i++) {
        // Initialise the vertical guardian structs
        VGUARDS[i].attribute = Data_verticalGuardianLocations[id][i][0];
        VGUARDS[i].frame = Data_verticalGuardianLocations[id][i][1];
        VGUARDS[i].yCoord = Data_verticalGuardianLocations[id][i][2];
        VGUARDS[i].xCoord = Data_verticalGuardianLocations[id][i][3];
        VGUARDS[i].yPixelIncrement = Data_verticalGuardianLocations[id][i][4];
        VGUARDS[i].yCoordMinimum = Data_verticalGuardianLocations[id][i][5];
        VGUARDS[i].yCoordMaximum = Data_verticalGuardianLocations[id][i][6];

        // While we're here, let's load up the sprites data.
        // We fill the memory later on in the GGDATA section.
        for (int j = 0; j < 256; j++) {
            VGUARDS[i].GGDATA[j] = Data_guardianSprites[id][j];
        }

        // Load guardian data into memory
        speccy.memory[addr++] = (uint8_t) Data_verticalGuardianLocations[id][i][0];
        split_address(Data_verticalGuardianLocations[id][i][1], &msb, &lsb);
        speccy.memory[addr++] = msb;
        speccy.memory[addr++] = lsb;
        speccy.memory[addr++] = (uint8_t) Data_verticalGuardianLocations[id][i][2];
        speccy.memory[addr++] = (uint8_t) Data_verticalGuardianLocations[id][i][3];
        speccy.memory[addr++] = (uint8_t) Data_verticalGuardianLocations[id][i][4];
        speccy.memory[addr++] = (uint8_t) Data_verticalGuardianLocations[id][i][5];
    }
    // Load the memory here with 7 empty bytes to make up the 35 bytes we need (VGUARDS is only 28)
    for (int i = 0; i < 7; i++) {
        speccy.memory[addr++] = 0;
    }

    //
    // The next 256 bytes are copied to GGDATA and define the guardian graphics.
    //

    // GGDATA is now added to all guardian structs for HGUARDS/VGUARDS and initialised
    // above, except Kong Beast, which is still a standalone GGDATA array.
    // Therefore we need to load the sprites to GGDATA and into the memory space.
    // This is not going to work long-term (for creating custom levels) but is needed for now.
    for (int i = 0; i < 256; i++) {
        // Some places still use GDATA. e.g. Kong Beast
        GGDATA[i] = Data_guardianSprites[id][i];

        // Load our guardian sprite into memory for the game engine
        speccy.memory[addr++] = Data_guardianSprites[id][i];
    }

    //
    // Memory check! Make sure we've loaded the correct number of bytes into the memory space
    //
    int total = addr - 32768;
    if (total != 512) {
        printf("Incorrect number of bytes assigned in cavern init: %d - %d = %d\n", addr, 32768, total);
        return false;
    }

    return true;
}
