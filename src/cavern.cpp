// Manic Miner C++ port Copyright (c) 2016-2018 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

#include "globals.h"
#include "data.h"

// Initialize arrays and copy the cavern definition into the game status buffer.
// We must copy a total of 512 bytes into the buffer address from 32768.
bool Cavern::loadData(uint8_t id) {
    uint8_t msb, lsb;
    uint16_t addr = 32768;

    // Load the Cavern Name
    strcpy(CAVERNNAME, Data_cavernNames[id]);
    for (int i = 0; i < 32; i++) {
        speccy.memory[addr++] = (uint8_t) CAVERNNAME[i];
    }

    // Load the cavern tile map layout
    for (int i = 0; i < 512; i++) {
        layout[i] = Data_cavernLayouts[id][i];
    }

    //
    // Load the cavern tile GFX
    //
    BACKGROUND.id = Data_tileBackgrounds[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            BACKGROUND.sprite[i - 1] = Data_tileBackgrounds[id][i];
        }
        speccy.memory[addr++] = Data_tileBackgrounds[id][i];
    }
    FLOOR.id = Data_tileFloors[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            FLOOR.sprite[i - 1] = Data_tileFloors[id][i];
        }
        speccy.memory[addr++] = Data_tileFloors[id][i];
    }
    CRUMBLING.id = Data_tileCrumblingFloors[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            CRUMBLING.sprite[i - 1] = Data_tileCrumblingFloors[id][i];
        }
        speccy.memory[addr++] = Data_tileCrumblingFloors[id][i];
    }
    WALL.id = Data_tileWalls[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            WALL.sprite[i - 1] = Data_tileWalls[id][i];
        }
        speccy.memory[addr++] = Data_tileWalls[id][i];
    }
    CONVEYOR.id = Data_tileConveyorBelts[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            CONVEYOR.sprite[i - 1] = Data_tileConveyorBelts[id][i];
        }
        speccy.memory[addr++] = Data_tileConveyorBelts[id][i];
    }
    NASTY1.id = Data_tileNasties1[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            NASTY1.sprite[i - 1] = Data_tileNasties1[id][i];
        }
        speccy.memory[addr++] = Data_tileNasties1[id][i];
    }
    NASTY2.id = Data_tileNasties2[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            NASTY2.sprite[i - 1] = Data_tileNasties2[id][i];
        }
        speccy.memory[addr++] = Data_tileNasties2[id][i];
    }
    EXTRA.id = Data_tileExtras[id][0]; // First entry is the cavern map ID
    for (int i = 0; i < 9; i++) {
        // Skip the first entry, which is the cavern mapping ID
        if (i > 0) {
            EXTRA.sprite[i - 1] = Data_tileExtras[id][i];
        }
        speccy.memory[addr++] = Data_tileExtras[id][i];
    }

    //
    // The next seven bytes are copied to 32872-32878 and specify
    // Miner Willy's initial location and appearance in the
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
    Speccy::splitAddress(willy.LOCATION, msb, lsb);
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
    CONVEYOR.CONVDIR = (uint8_t) Data_conveyorBeltsParams[id][0];
    speccy.memory[addr++] = CONVEYOR.CONVDIR;

    // Location in the screen buffer at 28672
    CONVEYOR.CONVLOC = Data_conveyorBeltsParams[id][1];
    Speccy::splitAddress(CONVEYOR.CONVLOC, msb, lsb);
    speccy.memory[addr++] = msb;
    speccy.memory[addr++] = lsb;

    // Length of the conveyor belt
    CONVEYOR.CONVLEN = (uint8_t) Data_conveyorBeltsParams[id][2];
    speccy.memory[addr++] = CONVEYOR.CONVLEN;


    // Specifies the BORDER colour
    BORDER = Data_borderColours[id];
    speccy.memory[addr++] = BORDER;

    // The next byte is copied to ITEMATTR, but is not used. // FIXME: why is it here then?
    game.ITEMATTR = Data_itemAttrs[id];
    speccy.memory[addr++] = game.ITEMATTR;

    //
    // The next 25 bytes are copied to ITEMS and specify the
    // location and initial colour of the items in the
    //
    for (int i = 0; i < 5; i++) {
        // load the item
        ITEMS[i].attribute = (uint8_t) Data_itemsData[id][i][0];
        ITEMS[i].address = Data_itemsData[id][i][1];
        ITEMS[i].addressMSB = (uint8_t) Data_itemsData[id][i][2];
        for (int j = 0; j < 8; j++) {
            ITEMS[i].tile[j] = Data_itemGraphics[id][j];
        }

        // Add items to the buffer memory
        speccy.memory[addr++] = (uint8_t) Data_itemsData[id][i][0];

        Speccy::splitAddress(Data_itemsData[id][i][1], msb, lsb);
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
    portal.PORTAL = Data_portalAttributes[id];
    speccy.memory[addr++] = portal.PORTAL;

    for (int i = 0; i < 32; i++) {
        portal.PORTALG[i] = Data_portalGraphics[id][i];
        speccy.memory[addr++] = Data_portalGraphics[id][i];
    }

    // Location in the attribute buffer at 23552
    portal.PORTALLOC1 = Data_portalAttributeLocations[id];
    // Location in the screen buffer at 24576
    portal.PORTALLOC2 = Data_portalScreenLocations[id];

    Speccy::splitAddress(portal.PORTALLOC1, msb, lsb);
    speccy.memory[addr++] = msb;
    speccy.memory[addr++] = lsb;
    Speccy::splitAddress(portal.PORTALLOC2, msb, lsb);
    speccy.memory[addr++] = msb;
    speccy.memory[addr++] = lsb;

    // NOTE: ITEM[8] is also used above to set tile on item struct
    // The next eight bytes are copied to ITEM and define the item graphic.
    for (int i = 0; i < 8; i++) {
        speccy.memory[addr++] = Data_itemGraphics[id][i];
    }

    // The next byte is copied to AIR and specifies the initial air supply in the
    AIR = Data_airSupplies[id];
    speccy.memory[addr++] = AIR;

    // The next byte is copied to CLOCK and initialises the game clock.
    CLOCK = Data_clockValues[id];
    speccy.memory[addr++] = CLOCK;

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
        Speccy::splitAddress(Data_horizontalGuardianLocations[id][i][1], msb, lsb);
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
        Speccy::splitAddress(Data_verticalGuardianLocations[id][i][1], msb, lsb);
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

void Cavern::updateGameClock() {
    CLOCK -= 4;
}

bool Cavern::isAirDepleted() {
    // For some reason 36 means no air left
    return AIR == 36;
}

// Decrease the air remaining in the current cavern, along with the game CLOCK.
bool Cavern::decreaseAir() {
    updateGameClock();

    // Was the game clock just decreased from zero?
    if (CLOCK == 252) {
        // Has the air supply run out?
        if (isAirDepleted()) {
            return true;
        }
        AIR--;
    }

    // A=INT(A/32); this value specifies how many pixels to draw from left to
    // right in the cell at the right end of the air bar.
    uint8_t count = uint8_t(CLOCK & 224);
    count = Speccy::rotL(count, 3);

    // Initialise E to 0 (all bits reset).
    uint8_t pixels = 0;

    // Do we need to draw any pixels in the cell at the right end of the air bar?
    if (count != 0) {
        // Copy the number of pixels to draw (1-7) to B.
        for (int i = count; i > 0; i--) {
            // Set this many bits in E (from bit 7 towards bit 0).
            pixels = Speccy::rotR(pixels, 1);
            pixels |= 1 << 7;
        }
    }

    // Set HL to the display file address at which to draw the top row of
    // pixels in the cell at the right end of the air bar.
    // There are four rows of pixels to draw.
    for (uint8_t msb = 82; msb < 86; msb++) {
        // Draw the four rows of pixels at the right end of the air bar.
        speccy.writeScreen(Speccy::buildAddress(msb, AIR), pixels);
    }

    return false;
}

// Draws the AIR bar graphics to the screen
void Cavern::drawAirBar() {
    // Initialise A to 82 (is 20992); this is the MSB of the display file address
    // at which to start drawing the bar that represents the air supply.
    for (uint8_t a = 82; a < 86; a++) {
        uint16_t addr = Speccy::buildAddress(a, 36);

        // Draw a single row of pixels across C cells.
        for (uint16_t i = 0; i < AIR - 36; i++) {
            speccy.writeScreen(addr + i, 255);
        }
    }
}

// Move the conveyor in the current
void Cavern::moveConveyorBelts() {
    uint8_t h, l, d, e; // MSB/LSB for the address.
    uint8_t pixels_a, pixels_c;

    // Pick up the address of the conveyor's location in the screen buffer at 28672 from CONVLOC.
    uint16_t row_hl = CONVEYOR.CONVLOC;

    // Copy this address to DE.
    uint16_t row_de = CONVEYOR.CONVLOC;

    // Is the conveyor moving left?
    if (CONVEYOR.CONVDIR == 0) {
        // Copy the first pixel row of the conveyor tile to A.
        // Rotate it left twice
        pixels_a = Speccy::rotL(speccy.readMemory(row_hl), 2);

        // Point HL at the third pixel row of the conveyor tile.
        Speccy::splitAddress(row_hl, h, l);
        h += 2;
        row_hl = Speccy::buildAddress(h, l);

        // Copy this pixel row to C
        pixels_c = Speccy::rotR(speccy.readMemory(row_hl), 2);
    } else {
        // The conveyor is moving right.

        // Copy the first pixel row of the conveyor tile to A.
        pixels_a = Speccy::rotR(speccy.readMemory(row_hl), 2);

        // Point HL at the third pixel row of the conveyor tile.
        Speccy::splitAddress(row_hl, h, l);
        h += 2;
        row_hl = Speccy::buildAddress(h, l);

        // Copy this pixel row to C.
        pixels_c = Speccy::rotL(speccy.readMemory(row_hl), 2);
    }

    for (int b = CONVEYOR.CONVLEN; b > 0; b--) {
        // Update the first and third pixel rows of every conveyor tile in the screen buffer at 28672.
        speccy.writeMemory(row_de, pixels_a);
        speccy.writeMemory(row_hl, pixels_c);

        Speccy::splitAddress(row_hl, h, l);
        l++;
        row_hl = Speccy::buildAddress(h, l);

        Speccy::splitAddress(row_de, d, e);
        e++;
        row_de = Speccy::buildAddress(d, e);
    }
}