//
// Cavern data

CAVERNNAME = "         Central Cavern         ";

// Central Cavern (teleport: 6)
uint8_t CAVERN0[512] = {
  22,0,0,0,0,0,0,0, 0,0,0,5,0,0,0,0, 5,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,68, 0,0,0,68,0,0,0,22,
  22,66,66,66,66,66,66,66, 66,66,66,66,66,66,2,2, 2,2,66,2,2,2,2,66, 66,66,66,66,66,66,66,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,66,66,66,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,22,22,22,0,68,0,0, 0,0,0,0,0,0,0,22,
  22,66,66,66,66,0,0,0, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4, 4,4,4,4,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,66,66,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,68,0,0,0, 0,0,0,0,22,22,22,2, 2,2,2,2,66,66,66,22,
  22,0,0,0,0,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,22,
}

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,2) (see LOCATION)
LOCATION = 23970;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location
// and length of the conveyor.
// Direction (left)
CONVDIR = 0;

// FIXME: are the MSB/LSB stored the right way round. MSB/LSB or LSB/MSB ??
// Location in the screen buffer at 28672: (9,8)
CONVLOC = 30760;

// Length
CONVLEN = 20;

// The next byte is copied to BORDER and specifies the border colour.
// Border colour
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial
// colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (0,9)
  { 3, 23561, 96, 255 },
  // Item 2 at (0,29)
  { 4, 23581, 96, 255 },
  // Item 3 at (1,16)
  { 5, 23600, 96, 255 },
  // Item 4 at (4,24)
  { 6, 23704, 96, 255 },
  // Item 5 at (6,30)
  { 3, 23774, 96, 255 },
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
PORTAL = 14;

// Graphic data
uint8_t PORTALG[32] = {
  255,255,146,73,182,219,255,255,
  146,73,182,219,255,255,146,73,
  182,219,255,255,146,73,182,219,
  255,255,146,73,182,219,255,255,
};

// Location in the attribute buffer at 23552: (13,29)
PORTALLOC1 = 23997;
// Location in the screen buffer at 24576: (13,29)
PORTALLOC2 = 26813;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
uint8_t ITEM[8] = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 252;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=7, initial x=8, 8<=x<=15, speed=normal
  {70, 23784, 96, 0, 232, 239},
  // Horizontal guardian 2 (unused)
  {0,0,0,0,0,0},
  // Horizontal guardian 3 (unused)
  {0,0,0,0,0,0},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next byte is copied to VGUARDS and indicates that there are no vertical guardians in this cavern.
uint8_t VGUARDS[4][7] = {};

// The next two bytes are unused.
// DEFB 0,0;

// Swordfish graphic data
// The next 32 bytes define the swordfish graphic that appears in The Final Barrier when the game is completed.
uint8_t SWORDFISH[32] = {
  2,160,5,67,31,228,115,255,
  242,248,31,63,255,228,63,195,
  0,0,1,0,57,252,111,2,
  81,1,127,254,57,252,1,0,
};

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  31,32,57,224,25,224,15,32, 159,0,95,128,255,192,94,0,
  159,192,31,128,14,0,31,0, 187,160,113,192,32,128,17,0,
  7,196,14,124,6,124,35,196, 23,192,23,224,63,240,23,240,
  23,240,39,224,3,128,3,128, 6,192,6,192,28,112,6,192,
  1,242,3,158,1,158,0,242, 9,240,5,248,15,252,5,224,
  9,252,1,248,0,224,0,224, 0,224,0,224,0,224,1,240,
  0,125,0,231,0,103,0,61, 0,124,0,127,3,252,0,120,
  0,124,0,127,0,56,0,56, 0,108,0,108,1,199,0,108,
  190,0,231,0,230,0,188,0, 62,0,254,0,63,192,30,0,
  62,0,254,0,28,0,28,0, 54,0,54,0,227,128,54,0,
  79,128,121,192,121,128,79,0, 15,144,31,160,63,240,7,160,
  63,144,31,128,7,0,7,0, 7,0,7,0,7,0,15,128,
  35,224,62,112,62,96,35,196, 3,232,7,232,15,252,15,232,
  15,232,7,228,1,192,1,192, 3,96,3,96,14,56,3,96,
  4,248,7,156,7,152,4,240, 0,249,1,250,3,255,0,122,
  3,249,1,248,0,112,0,248, 5,221,3,142,1,4,0,136,
};

// ----------------------------------------------------------------------------

CAVERNNAME = "          The Cold Room         ",

// The Cold Room (teleport: 16)
uint8_t CAVERN1[512] = {
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,22,22,22,22,22, 22,22,22,22,22,22,22,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,13,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,11,11,11, 75,8,8,8,8,8,8,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,22,
  22,75,75,75,75,75,75,75, 75,75,75,75,75,75,75,75, 75,75,75,75,8,8,8,8, 8,8,8,8,22,8,8,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,75,75,75, 75,22,11,11,22,8,8,22,
  22,75,11,11,11,11,11,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,22,8,8,22,8,8,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,22,11,11,22,8,8,22,
  22,8,8,8,8,8,8,8, 8,75,75,75,75,75,75,75, 8,8,8,8,8,8,8,8, 8,22,11,11,22,8,8,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,11,11,11,11,8, 8,22,11,11,22,8,8,22,
  22,8,8,14,14,14,14,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,22,11,11,22,8,8,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,75,75, 75,75,8,8,8,8,8,8, 8,22,11,11,22,8,8,22,
  22,8,8,8,8,8,8,8, 11,11,11,11,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,22,
  22,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,22,
  22,75,75,75,75,75,75,75, 75,75,75,75,75,75,75,75, 75,75,75,75,75,75,75,75, 75,75,75,75,75,75,75,22,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;
// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,2) (see LOCATION)
LOCATION = 23970;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (right)
CONVDIR = 1;

// Location in the screen buffer at 28672: (11,3)
CONVLOC = 30819;

// Length
CONVLEN = 4;


// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (1,7)
  {11, 23591, 96, 255},
  // Item 2 at (1,24)
  {12, 23608, 96, 255},
  // Item 3 at (7,26)
  {13, 23802, 96, 255},
  // Item 4 at (9,3)
  {14, 23843, 104, 255},
  // Item 5 at (12,19)
  {11, 23955, 104, 255},
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
PORTAL = 83;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,146,73,146,73,146,73,
  146,73,146,73,146,73,146,73,
  146,73,146,73,146,73,146,73,
  146,73,146,73,146,73,255,255,
};
// Location in the attribute buffer at 23552: (13,29)
PORTALLOC1 = 23997;
// Location in the screen buffer at 24576: (13,29)
PORTALLOC2 = 26813;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {80,168,84,168,84,44,2,1};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 252;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=3, initial x=18, 1<=x<=18, speed=normal
  {14, 23666, 96, 7, 97, 114},
  // Horizontal guardian 2: y=13, initial x=29, 12<=x<=29, speed=normal
  {13, 23997, 104, 7, 172, 189},
  // Horizontal guardian 3 (unused)
  {255,0,0,0,0,0,0},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next byte is copied to VGUARDS and indicates that there are no vertical guardians in this cavern.
uint8_t VGUARDS[4][7] = {};

// The next two bytes are unused.
//   DEFB 0,0;

// Plinth graphic data
// The next 32 bytes define the plinth graphic that appears on the Game Over screen.
uint8_t PLINTH[32] = {
  255,255,114,78,138,81,170,85,
  74,82,18,72,34,68,42,84,
  42,84,42,84,42,84,42,84,
  42,84,42,84,42,84,42,84,
};

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  12,0,30,0,27,0,30,192, 57,0,50,0,58,0,61,0,
  109,0,105,0,105,0,97,0, 113,0,190,0,8,0,30,0,
  3,0,7,128,6,192,7,176, 14,64,12,128,15,128,13,192,
  27,64,27,64,22,64,24,64, 28,64,47,128,5,64,15,128,
  0,192,1,224,1,176,1,236, 3,144,3,32,3,160,3,208,
  6,208,6,144,6,144,6,16, 7,16,11,232,2,80,7,224,
  0,48,0,120,0,108,0,123, 0,228,0,200,0,232,0,244,
  1,180,1,148,1,148,1,132, 1,196,2,248,0,84,0,248,
  12,0,30,0,54,0,222,0, 39,0,19,0,23,0,47,0,
  45,128,41,128,41,128,33,128, 35,128,31,64,42,0,31,0,
  3,0,7,128,13,128,55,128, 9,192,4,192,5,192,11,192,
  11,96,9,96,9,96,8,96, 8,224,23,208,10,64,7,224,
  0,192,1,224,3,96,13,224, 2,112,1,48,1,240,3,176,
  2,216,2,216,2,104,2,24, 2,56,1,244,2,160,1,240,
  0,48,0,120,0,216,3,120, 0,156,0,76,0,92,0,188,
  0,182,0,150,0,150,0,134, 0,142,0,125,0,16,0,120,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "          The Menagerie         ";

// The Menagerie (teleport: 26)
uint8_t CAVERN2[512] = {
  13,0,0,0,0,0,0,0, 0,0,67,0,0,0,0,0, 0,0,3,0,0,0,0,0, 0,0,0,67,0,0,0,13,
  13,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,67,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,69,69,69,69,5,5,5, 5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,13,
  13,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,69,69,69,69,69,69,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,69,69,69,69,13,
  13,3,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,3,0,0,0,0,2,2, 2,2,2,2,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,3,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,69,69,69,69,69,69,13,
  13,67,0,0,0,0,0,0, 0,0,0,0,0,0,69,69, 69,69,69,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,0,0,0,0,69,69,69, 69,69,69,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,69,69,69, 69,69,69,69,69,69,69,13,
  13,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,13,
  13,69,69,69,69,69,69,69, 69,69,69,69,69,69,69,69, 69,69,69,69,69,69,69,69, 69,69,69,69,69,69,69,13,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,2) (see LOCATION)
LOCATION = 23970;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (9,6)
CONVLOC = 30758;
// Length
CONVLEN = 6;

// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (0,6)
  {3, 23558, 96, 255},
  // Item 2 at (0,15)
  {4, 23567, 96, 255},
  // Item 3 at (0,23)
  {5, 23575, 96, 255},
  // Item 4 at (6,30)
  {6, 23774, 96, 255},
  // Item 5 at (6,21)
  {3, 23765, 96, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 14;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,68,68,153,153,34,34,
  34,34,153,153,68,68,68,68,
  153,153,34,34,34,34,153,153,
  68,68,68,68,153,153,255,255,
};
// Location in the attribute buffer at 23552: (11,29)
PORTALLOC1 = 23933;
// Location in the screen buffer at 24576: (11,29)
PORTALLOC2 = 26749;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=13, initial x=19, 1<=x<=19, speed=normal
  {68, 23987, 104, 7, 161, 179},
  // Horizontal guardian 2: y=3, initial x=16, 1<=x<=16, speed=normal
  {67, 23664, 96, 7, 97, 112},
  // Horizontal guardian 3: y=3, initial x=18, 18<=x<=29, speed=normal
  {66, 23666, 96, 0, 114, 125},
  // Horizontal guardian 4 (unused)
  {255,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next byte is copied to VGUARDS and indicates that there are no vertical guardians in this cavern.
uint8_t VGUARDS[4][7] = {};

// The next two bytes are unused.
//   DEFB 0,0;

// Boot graphic data
// The next 32 bytes define the boot graphic that appears on the Game Over
// screen (see LOOPFT). It also appears at the bottom of the screen next to the
// remaining lives when cheat mode is activated (see LOOP_1).
uint8_t BOOT[32] = {
  42,192,53,64,63,192,9,0,
  9,0,31,128,16,128,16,128,
  17,128,34,64,32,184,89,36,
  68,66,68,2,68,2,255,255,
};

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  3,0,6,128,7,192,3,0, 1,128,0,192,190,192,227,128,
  65,0,171,0,127,0,62,0, 8,0,8,0,8,0,20,0,
  0,192,1,160,1,240,0,192, 0,96,0,48,47,176,56,224,
  24,192,48,192,21,192,10,128, 21,0,2,0,5,0,0,0,
  0,48,0,104,0,124,0,48, 0,24,0,12,11,236,14,56,
  4,16,10,176,7,240,3,224, 0,128,1,64,0,0,0,0,
  0,12,0,26,0,31,0,172, 1,86,0,171,3,91,3,134,
  1,12,3,252,1,252,0,248, 0,32,0,32,0,80,0,0,
  48,0,88,0,248,0,53,0, 106,128,213,0,218,192,97,192,
  48,128,63,192,63,128,31,0, 4,0,4,0,10,0,0,0,
  12,0,22,0,62,0,12,0, 24,0,48,0,55,208,28,112,
  8,32,13,80,15,224,7,192, 1,0,2,128,0,0,0,0,
  3,0,5,128,15,128,3,0, 6,0,12,0,13,244,7,28,
  3,24,3,12,3,168,1,80, 0,168,0,64,0,160,0,0,
  0,192,1,96,3,224,0,192, 1,128,3,0,3,125,1,199,
  0,130,0,213,0,254,0,124, 0,16,0,16,0,16,0,40,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "   Abandoned Uranium Workings   ";

// Abandoned Uranium Workings (teleport: 126)
uint8_t CAVERN3[512] = {
  41,0,0,0,0,0,0,5, 0,0,0,0,0,0,41,41, 41,41,41,41,41,41,41,41, 41,41,41,41,41,41,41,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,70,70,70,70,70, 70,0,0,0,0,0,0,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,70,70,70,70,41,
  41,70,0,0,0,0,0,70, 0,0,0,0,0,0,0,0, 0,70,0,0,0,0,0,0, 0,0,0,0,0,0,0,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,70,70,0,0, 0,0,0,0,0,70,70,70, 0,0,0,0,0,0,0,41,
  41,6,6,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,41,
  41,0,0,0,0,0,0,70, 70,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,70,70,70,0,0,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,70,70,70,0,0,0, 0,0,0,0,0,0,0,41,
  41,3,3,3,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,70,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,70,70,70,0, 0,0,0,0,0,0,70,70, 70,0,0,0,0,0,0,41,
  41,0,0,0,0,0,70,70, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,5, 0,0,0,0,70,70,70,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,70,70,0,0,0,0, 0,0,0,0,0,0,0,41,
  41,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,41,
  41,70,70,70,70,70,70,70, 70,70,70,70,70,70,70,70, 70,70,70,70,70,70,70,70, 70,70,70,70,70,70,70,41,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....


// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing left (see DMFLAGS)
DMFLAGS = 1;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,29) (see LOCATION)
LOCATION = 23997;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (right)
CONVDIR = 1;
// Location in the screen buffer at 28672: (10,1)
CONVLOC = 30785;
// Length
CONVLEN = 3;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (0,1)
  {3, 23553, 96, 255},
  // Item 2 at (1,12)
  {4, 23596, 96, 255},
  // Item 3 at (1,25)
  {5, 23609, 96, 255},
  // Item 4 at (6,16)
  {6, 23760, 96, 255},
  // Item 5 at (6,30)
  {3, 23774, 96, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 14;
// Graphic data
uint8_t PORTALG[32] = {
  34,34,17,17,136,136,68,68,
  34,34,17,17,136,136,68,68,
  34,34,17,17,136,136,68,68,
  34,34,17,17,136,136,68,68,
};
// Location in the attribute buffer at 23552: (1,29)
PORTALLOC1 = 23613;
// Location in the screen buffer at 24576: (1,29)
PORTALLOC2 = 24637;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=13, initial x=1, 1<=x<=10, speed=normal
  {66, 23969, 104, 0, 161, 170},
  // Horizontal guardian 2: y=13, initial x=7, 6<=x<=15, speed=normal
  {68, 23975, 104, 0, 166, 175},
  // Horizontal guardian 3 (unused)
  {255,0,0,0,0,0,0},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 2 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  7,0,11,128,19,192,19,192, 19,192,11,128,7,0,1,0,
  7,0,5,0,7,0,7,128, 79,128,95,192,254,192,60,64,
  1,192,2,224,5,208,5,208, 5,208,2,224,1,192,0,64,
  1,192,1,64,1,192,1,224, 35,224,47,240,127,176,31,16,
  0,112,0,232,1,228,1,228, 1,228,0,232,0,112,0,16,
  0,112,0,80,0,112,0,248, 33,248,39,252,127,236,15,196,
  0,28,0,54,0,99,0,99, 0,99,0,54,0,28,0,4,
  0,28,0,20,0,28,0,30, 4,62,4,255,15,251,3,241,
  56,0,108,0,198,0,198,0, 198,0,108,0,56,0,32,0,
  56,0,40,0,56,0,120,0, 124,32,255,32,223,240,143,192,
  14,0,23,0,39,128,39,128, 39,128,23,0,14,0,8,0,
  14,0,10,0,14,0,31,0, 31,132,63,228,55,254,35,240,
  3,128,7,64,11,160,11,160, 11,160,7,64,3,128,2,0,
  3,128,2,128,3,128,7,128, 7,196,15,244,13,254,8,248,
  0,224,1,208,3,200,3,200, 3,200,1,208,0,224,0,128,
  0,224,0,160,0,224,1,224, 1,242,3,250,3,127,2,60,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "         Eugene's Lair          ";

// Eugene's Lair (teleport: 36)
uint8_t CAVERN4[512] = {
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,19,16,16,16, 16,16,16,16,16,16,16,46,
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,46,
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,46,
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,46,
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 22,16,16,16,16,16,16,46,
  46,21,21,21,21,21,21,21, 21,21,21,21,21,21,16,16, 16,16,20,20,20,20,21,21, 21,21,21,21,16,16,16,46,
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,21,21,46,
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,22,16,16, 16,16,16,16,16,16,16,46,
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,86,86,86,86,86,86, 86,86,86,86,16,16,16,46,
  46,16,16,16,21,21,21,21, 21,21,21,21,21,21,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,46,
  46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,46,
  46,20,20,21,21,21,21,21, 21,21,21,21,21,21,16,16, 16,16,21,21,21,21,21,21, 21,16,16,16,16,16,21,46,
  46,16,16,16,16,16,16,16, 46,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,16, 16,16,16,16,16,16,16,46,
  46,21,21,16,16,16,16,16, 46,16,16,16,16,16,46,16, 16,46,16,16,16,16,16,16, 16,16,16,16,16,16,16,46,
  46,16,16,16,16,22,16,16, 46,16,16,16,16,16,46,16, 16,46,46,46,46,46,46,46, 22,22,16,16,16,16,16,46,
  46,21,21,21,21,21,21,21, 46,46,46,46,46,46,46,46, 46,46,46,46,46,46,46,46, 21,21,21,21,21,21,21,46,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 48;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (3,1) (see LOCATION)
LOCATION = 23649;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (8,18)
CONVLOC = 30738;
// Length
CONVLEN = 10;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 1;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (1,30)
  {19, 23614, 96, 255},
  // Item 2 at (6,10)
  {20, 23754, 96, 255},
  // Item 3 at (7,29)
  {21, 23805, 96, 255},
  // Item 4 at (12,7)
  {22, 23943, 104, 255},
  // Item 5 at (12,9)
  {19, 23945, 104, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 87;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,170,170,170,170,170,170,
  170,170,170,170,170,170,170,170,
  170,170,170,170,170,170,170,170,
  170,170,170,170,170,170,255,255,
};
// Location in the attribute buffer at 23552: (13,15)
PORTALLOC1 = 23983;
// Location in the screen buffer at 24576: (13,15)
PORTALLOC2 = 26799;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {31,35,71,255,143,142,140,248};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=3, initial x=12, 1<=x<=12, speed=normal
  {22 23660 96 7 97 108},
  // Horizontal guardian 2: y=7, initial x=4, 4<=x<=12, speed=normal
  {16 23780 96 0 228 236},
  // Horizontal guardian 3 (unused)
  {255,0,0,0,0,0,0},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0,0},
};


// The next two bytes are copied to EUGDIR and EUGHGT and specify Eugene's initial direction and pixel y-coordinate.
// Initial direction (down)
EUGDIR = 0;
// Initial pixel y-coordinate
EUGHGT = 0;

// The next three bytes are unused.
//   DEFB 0,0,0;

// Eugene graphic data
// The next 32 bytes define the Eugene graphic.
uint8_t EUGENEG[32] = {
  3,192,15,240,31,248,31,248,
  49,140,14,112,111,246,174,117,
  177,141,159,249,155,217,140,49,
  71,226,2,64,2,64,14,112,
};

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  192,0,192,0,192,0,192,0, 192,0,192,0,192,0,223,192,
  223,192,255,192,31,192,15,128, 119,128,255,0,223,0,223,0,
  48,0,48,0,48,0,48,0, 48,32,48,192,51,0,52,0,
  55,240,63,240,7,240,3,224, 29,224,63,192,55,192,55,192,
  12,0,12,0,12,32,12,64, 12,64,12,128,12,128,13,0,
  13,252,15,252,1,252,0,248, 7,120,15,240,13,240,13,240,
  3,0,3,0,3,0,3,0, 3,2,3,12,3,48,3,64,
  3,127,3,255,0,127,0,62, 1,222,3,252,3,124,3,124,
  0,192,0,192,0,192,0,192, 64,192,48,192,12,192,2,192,
  254,192,255,192,254,0,124,0, 123,128,63,192,62,192,62,192,
  0,48,0,48,4,48,2,48, 2,48,1,48,1,48,0,176,
  63,176,63,240,63,128,31,0, 30,224,15,240,15,176,15,176,
  0,12,0,12,0,12,0,12, 4,12,3,12,0,204,0,44,
  15,236,15,252,15,224,7,192, 7,184,3,252,3,236,3,236,
  0,3,0,3,0,3,0,3, 0,3,0,3,0,3,3,251,
  3,251,3,255,3,248,1,240, 1,238,0,255,0,251,0,251,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "       Processing Plant         ";

// Processing Plant (teleport: 136)
uint8_t CAVERN5[512] = {
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,6,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 68,68,68,0,0,0,0,68, 68,0,0,0,0,68,68,68, 68,68,0,0,0,0,0,22,
  22,0,0,68,68,0,0,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,0,0,0, 0,0,0,0,68,68,68,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,68, 68,68,68,68,0,0,0,22,
  22,68,68,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,68, 68,68,68,68,68,68,68,68, 22,68,68,68,68,68,68,68, 68,68,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 22,6,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,67,0,0,0,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,0,0,0, 0,0,0,0,68,68,68,22,
  22,0,0,5,5,5,5,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,68,68, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,68,68,68,68,68,68,68, 68,68,68,68,68,68,68,68, 68,68,68,68,68,68,68,68, 68,68,68,68,68,68,68,22,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 48;

// Animation frame (see FRAME)
FRAME = 3;

// Direction and movement flags: facing left (see DMFLAGS)
DMFLAGS = 1;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (3,15) (see LOCATION)
LOCATION = 23663;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (13,3)
CONVLOC = 30883;
// Length
CONVLEN = 4;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (6,15)
  {3, 23759, 96, 255},
  // Item 2 at (6,17)
  {4, 23761, 96, 255},
  // Item 3 at (7,30)
  {5, 23806, 96, 255},
  // Item 4 at (10,1)
  {6, 23873, 104, 255},
  // Item 5 at (11,13)
  {3, 23917, 104, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 14;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,129,129,191,253,191,253,
  176,13,176,13,176,13,240,15,
  240,15,176,13,176,13,176,13,
  191,253,191,253,129,129,255,255,
};
// Location in the attribute buffer at 23552: (0,29)
PORTALLOC1 = 23581;
// Location in the screen buffer at 24576: (0,29)
PORTALLOC2 = 24605;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=8, initial x=6, 6<=x<=13, speed=normal
  {70, 23814, 104, 0, 6, 13},
  // Horizontal guardian 2: y=8, initial x=14, 14<=x<=21, speed=normal
  {67, 23822, 104, 1, 14, 21},
  // Horizontal guardian 3: y=13, initial x=8, 8<=x<=20, speed=normal
  {69, 23976, 104, 2, 168, 180},
  // Horizontal guardian 4: y=13, initial x=24, 24<=x<=29, speed=normal
  {6, 23992, 104, 3, 184, 189}
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 2 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  31,0,127,192,115,224,243,128, 254,0,248,0,254,0,255,128,
  127,224,127,192,31,0,10,0, 10,0,10,0,10,0,31,0,
  7,192,31,240,30,112,62,120, 63,248,62,0,63,248,63,248,
  31,240,31,240,7,192,2,128, 2,128,7,192,0,0,0,0,
  1,240,7,252,7,62,15,56, 15,224,15,128,15,224,15,248,
  7,254,7,252,1,240,1,240, 0,0,0,0,0,0,0,0,
  0,124,1,207,1,206,3,252, 3,240,3,224,3,240,3,252,
  1,254,1,255,0,124,0,40, 0,40,0,124,0,0,0,0,
  62,0,243,128,115,128,63,192, 15,192,7,192,15,192,63,192,
  127,128,255,128,62,0,20,0, 20,0,62,0,0,0,0,0,
  15,128,63,224,124,224,28,240, 7,240,1,240,7,240,31,240,
  127,224,63,224,15,128,15,128, 0,0,0,0,0,0,0,0,
  3,224,15,248,14,120,30,124, 31,252,0,124,31,252,31,252,
  15,248,15,248,3,224,1,64, 1,64,3,224,0,0,0,0,
  0,248,3,254,7,206,1,207, 0,127,0,31,0,127,1,255,
  7,254,3,254,0,248,0,80, 0,80,0,80,0,80,0,248,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "            The Vat             ";

// The Vat (teleport: 236)
uint8_t CAVERN6[512] = {
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,77,77, 77,77,77,77,77,77,77,77, 77,77,77,77,77,77,77,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,70, 70,77,2,2,2,2,2,2, 2,2,2,2,2,2,0,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,77,2,2,2,2,2,2, 2,2,2,2,2,2,2,77,
  77,0,0,0,0,0,0,4, 4,4,4,4,0,0,70,70, 70,77,2,2,2,2,2,2, 2,2,2,2,22,2,2,77,
  77,70,70,70,0,0,0,0, 0,0,0,0,0,0,0,0, 0,77,2,2,0,2,2,2, 2,2,2,2,2,2,2,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,77,2,2,2,2,2,2, 2,2,2,0,2,2,2,77,
  77,70,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,77,2,2,2,2,2,22, 2,2,2,2,2,2,2,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,70,70, 70,77,2,2,2,2,2,2, 2,2,2,2,2,2,2,77,
  77,70,70,70,70,70,70,70, 70,70,70,70,0,0,0,0, 0,77,2,0,2,2,2,2, 2,2,2,2,22,2,2,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,77,2,2,2,2,2,2, 2,2,2,2,2,2,0,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,77,77, 77,77,2,2,2,2,2,22, 2,2,2,2,2,2,2,77,
  77,0,0,0,0,0,0,0, 0,70,70,70,0,0,77,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,77,
  77,0,0,0,0,0,0,0, 0,0,0,0,0,0,77,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,77,
  77,70,70,70,70,70,70,70, 70,70,70,70,70,70,77,77, 77,77,77,77,77,77,77,77, 77,77,77,77,77,77,77,77,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,2) (see LOCATION)
LOCATION = 23970;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (5,7)
CONVLOC = 28839;
// Length
CONVLEN = 5;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 4;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (3,30)
  {19, 23678, 96, 255},
  // Item 2 at (6,20)
  {20, 23764, 96, 255},
  // Item 3 at (7,27)
  {21, 23803, 96, 255},
  // Item 4 at (10,19)
  {22, 23891, 104, 255},
  // Item 5 at (11,30)
  {19, 23934, 104, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 11;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,129,129,129,129,129,129,
  129,129,129,129,129,129,255,255,
  255,255,129,129,129,129,129,129,
  129,129,129,129,129,129,255,255,
};
// Location in the attribute buffer at 23552: (13,15)
PORTALLOC1 = 23983;
// Location in the screen buffer at 24576: (13,15)
PORTALLOC2 = 26799;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=1, initial x=15, 15<=x<=29, speed=normal
  {69, 23599, 96, 0, 47, 61},
  // Horizontal guardian 2: y=8, initial x=10, 2<=x<=10, speed=normal
  {67, 23818, 104, 7, 2, 10},
  // Horizontal guardian 3: y=13, initial x=17, 17<=x<=29, speed=normal
  {6, 23985, 104, 0, 177, 189},
  // Horizontal guardian 4 (unused)
  {255,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 2 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  24,0,28,0,10,128,15,128, 12,0,28,0,30,0,29,0,
  60,0,62,0,62,0,110,0, 68,0,66,0,129,0,0,0,
  0,0,0,0,6,0,7,0, 2,160,3,224,3,128,7,0,
  7,128,7,64,15,0,15,128, 15,128,27,128,51,0,64,192,
  0,0,0,0,0,0,0,0, 1,128,1,192,0,168,0,248,
  0,224,1,192,1,224,1,208, 3,192,3,224,7,224,62,248,
  0,0,0,0,0,96,0,112, 0,42,0,62,0,56,0,112,
  0,120,0,116,0,240,0,248, 1,248,1,176,3,12,4,0,
  0,0,0,0,6,0,14,0, 84,0,124,0,28,0,14,0,
  30,0,46,0,15,0,31,0, 31,128,13,128,48,192,0,32,
  0,0,0,0,0,0,0,0, 1,128,3,128,21,0,31,0,
  7,0,3,128,7,128,11,128, 3,192,7,192,7,224,31,124,
  0,0,0,0,0,96,0,224, 5,64,7,192,1,192,0,224,
  1,224,2,224,0,240,1,240, 1,240,1,216,0,204,3,2,
  0,24,0,56,1,80,1,240, 0,48,0,56,0,120,0,184,
  0,60,0,124,0,124,0,118, 0,34,0,66,0,129,0,0,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "Miner Willy meets the Kong Beast";

// Miner Willy meets the Kong Beast (teleport: 1236)
uint8_t CAVERN7[512] = {
  114,0,5,0,0,0,6,0, 0,0,5,0,0,0,0,0, 0,114,6,0,114,0,0,0, 0,0,0,0,0,0,0,114,
  114,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,114,0,0,114,0,0,0, 0,0,0,0,0,0,0,114,
  114,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,66, 66,114,0,0,0,0,0,0, 0,0,0,0,0,66,66,114,
  114,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,114,0,0,0,0,0,0, 0,0,0,0,0,0,0,114,
  114,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,114,0,0,0,0,0,0, 0,0,0,0,0,0,0,114,
  114,66,66,66,0,0,0,0, 0,66,66,66,66,66,66,0, 0,114,66,66,0,0,0,0, 0,0,0,0,0,0,0,114,
  114,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,114,0,0,0,66,66,66, 66,0,0,0,0,0,66,114,
  114,0,66,66,66,0,0,0, 0,0,0,0,0,0,0,0, 0,114,0,0,0,0,0,0, 0,0,0,66,0,0,0,114,
  114,0,0,0,0,0,0,0, 66,66,66,0,0,0,0,0, 0,114,0,0,0,0,0,0, 0,0,0,0,0,0,0,114,
  114,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,114,66,66,66,66,66,0, 0,0,0,0,0,0,0,114,
  114,66,0,0,0,0,0,0, 0,0,0,0,66,66,66,0, 0,114,0,0,0,0,0,0, 0,0,0,66,66,66,66,114,
  114,0,0,0,0,0,0,0, 0,66,66,0,0,0,0,0, 0,114,0,0,0,0,0,0, 0,0,0,0,0,0,0,114,
  114,0,0,0,66,66,0,0, 0,0,0,0,0,0,0,0, 0,114,0,0,0,0,66,66, 66,66,66,0,0,0,0,114,
  114,0,0,0,0,0,0,0, 0,0,0,68,68,68,114,0, 0,114,66,66,0,0,0,0, 0,0,0,0,0,0,0,114,
  114,0,0,0,0,0,0,0, 0,0,0,0,0,0,114,0, 0,114,0,0,0,0,0,4, 0,0,0,0,0,0,0,114,
  114,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,114,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,2) (see LOCATION)
LOCATION = 23970;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (right)
CONVDIR = 1;
// Location in the screen buffer at 28672: (13,11)
CONVLOC = 30891;
// Length
CONVLEN = 3;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;
// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (2,13)
  {3, 23629, 96, 255},
  // Item 2 at (6,14)
  {4, 23758, 96, 255},
  // Item 3 at (8,2)
  {5, 23810, 104, 255},
  // Item 4 at (13,29)
  {6, 23997, 104, 255},
  // Item 5 (unused)
  {255,255,255,255,255},
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 14;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,128,1,192,3,160,5,
  144,9,200,19,164,37,146,73,
  201,147,164,37,146,73,201,147,
  164,37,201,147,146,73,255,255,
};
// Location in the attribute buffer at 23552: (13,15)
PORTALLOC1 = 23983;
// Location in the screen buffer at 24576: (13,15)
PORTALLOC2 = 26799;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {128,192,236,114,40,84,138,135};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=13, initial x=9, 1<=x<=9, speed=normal
  {68, 23977, 104, 7, 161, 169},
  // Horizontal guardian 2: y=11, initial x=11, 11<=x<=15, speed=slow
  {195, 23915, 104, 0, 107, 111},
  // Horizontal guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Horizontal guardian 4: y=7, initial x=18, 18<=x<=21, speed=normal
  {5, 23794, 96, 0, 242, 245}
};

// The next two bytes are copied to EUGDIR and EUGHGT; the first byte specifies
// the Kong Beast's initial status, but the second byte is not used.
// Initial status (on the ledge)
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 2 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  19,200,29,184,15,240,6,96, 5,160,2,64,7,224,15,240,
  31,248,51,204,99,198,70,98, 44,52,6,96,2,64,14,112,
  11,208,13,176,15,240,6,96, 5,160,2,64,3,192,31,248,
  127,254,231,231,131,193,199,227, 6,96,12,48,8,16,56,28,
  28,56,6,96,12,48,102,102, 35,196,103,230,55,236,31,248,
  15,240,7,224,2,64,5,160, 6,96,15,240,13,176,11,208,
  112,14,24,24,12,48,6,96, 99,198,39,228,103,230,55,236,
  31,248,15,240,2,64,5,160, 22,104,15,240,13,176,3,192,
  8,0,5,0,8,128,37,0, 72,128,33,0,76,0,51,0,
  68,128,68,128,136,64,132,64, 72,128,72,128,51,0,12,0,
  2,0,17,32,10,64,17,32, 10,64,16,32,3,0,12,192,
  16,32,16,96,34,144,37,16, 24,32,16,32,12,192,3,0,
  0,64,2,32,4,72,2,36, 4,72,2,4,0,200,3,48,
  4,8,4,8,11,68,8,180, 4,8,4,8,3,48,0,192,
  0,68,1,34,2,68,1,34, 2,68,1,2,2,48,0,204,
  1,66,1,34,2,17,2,33, 1,18,1,10,0,204,0,48,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "        Wacky Amoebatrons       ";

// Wacky Amoebatrons (teleport: 46)
uint8_t CAVERN8[512] = {
  22,0,0,22,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,6,6,6,6,0,0,6, 6,6,0,0,6,6,6,6, 6,6,6,6,0,0,6,6, 6,0,0,6,6,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,6,6,22,
  22,0,0,6,6,0,0,6, 6,6,0,0,4,4,4,4, 4,4,4,4,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,6,6, 6,0,0,6,6,0,0,22,
  22,6,6,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,6,0,0,6, 6,6,0,0,6,6,6,6, 6,6,6,6,0,0,6,6, 6,0,0,6,6,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,6,6,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,6,6,6,6,6,6,6, 6,6,6,6,6,6,6,6, 6,6,6,6,6,6,6,6, 6,6,6,6,6,6,6,22,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,1) (see LOCATION)
LOCATION = 23969;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (right)
CONVDIR = 1;
// Location in the screen buffer at 28672: (8,12)
CONVLOC = 30732;
// Length
CONVLEN = 8;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 1;
// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (1,16)
  {3, 23600, 96, 255},
  // Item 2 (unused)
  {255,255,255,255,255},
  // Item 3 (unused)
  {0,255,255,255,255},
  // Item 4 (unused)
  {0,255,255,255,255},
  // Item 5 (unused)
  {0,255,255,255,255},
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 14;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,128,1,129,129,130,65,
  132,33,136,17,144,9,161,133,
  161,133,144,9,136,17,132,33,
  130,65,129,129,128,1,255,255,
};
// Location in the attribute buffer at 23552: (0,1)
PORTALLOC1 = 23553;
// Location in the screen buffer at 24576: (0,1)
PORTALLOC2 = 24577;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=3, initial x=12, 12<=x<=18, speed=normal
  {68, 23660, 96, 0, 108, 114},
  // Horizontal guardian 2: y=10, initial x=16, 12<=x<=18, speed=slow
  {133, 23888, 104, 0, 76, 82},
  // Horizontal guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=5, initial y=8, 5<=y<=100, initial y-increment=1
  {67, 0, 8, 5, 1, 5, 100},
  // Vertical guardian 2: x=10, initial y=8, 5<=y<=100, initial y-increment=2
  {4, 1, 8, 10, 2, 5, 100},
  // Vertical guardian 3: x=20, initial y=8, 5<=y<=100, initial y-increment=1
  {5, 2, 8, 20, 1, 5, 100},
  // Vertical guardian 4: x=25, initial y=8, 5<=y<=100, initial y-increment=2
  {66, 3, 8, 25, 2, 5, 100},
};

// The next 6 bytes are unused.
//   DEFB 0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  10,32,22,104,1,80,57,98, 101,206,3,208,255,238,135,241,
  119,228,199,255,139,241,50,172, 100,166,73,162,18,144,54,152,
  0,0,5,32,3,64,49,116, 29,204,3,208,63,236,7,244,
  63,224,103,252,11,242,50,172, 36,164,11,144,26,216,0,192,
  0,0,0,0,2,32,9,96, 5,200,3,208,31,224,7,248,
  31,224,23,248,15,240,18,168, 5,160,10,176,0,192,0,0,
  0,0,5,32,3,64,49,116, 29,204,3,208,63,236,7,244,
  63,224,103,252,11,242,50,172, 36,164,11,144,26,216,0,192,
  12,0,12,0,12,0,12,0, 12,0,12,0,12,0,12,0,
  12,0,12,0,255,192,12,0, 97,128,210,192,179,64,97,128,
  3,0,3,0,3,0,3,0, 3,0,3,0,3,0,3,0,
  3,0,3,0,63,240,3,0, 24,96,36,208,60,208,24,96,
  0,192,0,192,0,192,0,192, 0,192,0,192,0,192,0,192,
  0,192,0,192,15,252,0,192, 6,24,11,52,13,44,6,24,
  0,48,0,48,0,48,0,48, 0,48,0,48,0,48,0,48,
  0,48,0,48,3,255,0,48, 1,134,2,77,3,205,1,134,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "       The Endorian Forest      ";

// The Endorian Forest (teleport: 146)
uint8_t CAVERN9[512] = {
  22,0,0,0,0,0,0,0, 0,0,0,4,0,68,68,68, 22,0,4,0,4,68,68,68, 68,68,68,68,68,68,68,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,4,0,0, 0,0,0,0,0,0,0,22,
  22,68,68,68,68,68,68,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,0,0,0, 0,0,0,68,68,68,68,22,
  22,0,0,4,0,0,0,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 22,68,68,68,68,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 68,2,2,2,2,2,2,2, 22,0,0,0,0,0,0,68, 68,68,68,68,68,68,68,22,
  22,68,68,68,68,0,0,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 22,68,68,68,68,68,68,68, 2,2,2,0,0,0,0,22,
  22,68,68,68,68,68,0,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,4,0,0,0,0,0,0, 0,68,68,68,68,68,68,68, 22,0,0,0,0,0,0,0, 0,0,0,0,0,68,68,22,
  22,68,68,68,68,2,2,0, 0,0,0,0,0,0,0,0, 22,68,68,68,68,68,68,68, 0,0,0,0,0,0,4,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 22,0,0,0,0,0,0,4, 2,2,2,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 5,5,5,5,5,5,5,5, 5,5,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,68,68,68,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,68,68,68,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 64;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (4,1) (see LOCATION)
LOCATION = 23681;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the (unused) conveyor.

// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (0,19)
CONVLOC = 28691;
// Length
CONVLEN = 1;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (2,21)
  {3, 23637, 96, 255},
  // Item 2 at (1,14)
  {4, 23598, 96, 255},
  // Item 3 at (6,12)
  {5, 23756, 96, 255},
  // Item 4 at (8,18)
  {6, 23826, 104, 255},
  // Item 5 at (1,30)
  {3, 23614, 96, 255},
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 30;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,248,143,136,145,170,145,
  170,149,138,133,144,145,213,185,
  213,85,209,69,137,57,137,3,
  168,171,170,171,138,137,255,255,
};
// Location in the attribute buffer at 23552: (13,12)
PORTALLOC1 = 23980;
// Location in the screen buffer at 24576: (13,12)
PORTALLOC2 = 26796;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {8,8,62,95,95,71,97,62};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 248;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=7, initial x=9, 9<=x<=14, speed=normal
  {70, 23785, 96, 0, 233, 238},
  // Horizontal guardian 2: y=10, initial x=12, 8<=x<=14, speed=slow
  {194, 23884, 104, 0, 72, 78},
  // Horizontal guardian 3: y=13, initial x=8, 4<=x<=26, speed=normal
  {67, 23976, 104, 0, 164, 186},
  // Horizontal guardian 4: y=5, initial x=18, 17<=x<=21, speed=normal
  {5, 23730, 96, 0, 177, 181}
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 2 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  112,0,80,0,124,0,52,0, 62,0,62,0,24,0,60,0,
  126,0,126,0,247,0,251,0, 60,0,118,0,110,0,119,0,
  28,0,20,0,31,0,13,0, 15,128,15,128,6,0,15,0,
  27,128,27,128,27,128,29,128, 15,0,6,0,6,0,7,0,
  7,0,5,0,7,192,3,64, 3,224,3,224,1,128,3,192,
  7,224,7,224,15,112,15,176, 3,192,7,96,6,224,7,112,
  1,192,1,64,1,240,0,208, 0,248,0,248,0,96,0,240,
  1,248,3,252,7,254,6,246, 0,248,1,218,3,14,3,132,
  3,128,6,128,15,128,11,0, 31,0,31,0,6,0,15,0,
  31,128,63,192,127,224,111,96, 31,0,91,128,112,192,33,192,
  0,224,1,160,3,224,2,192, 7,192,7,192,1,128,3,192,
  7,224,7,224,14,240,13,240, 3,192,6,224,7,96,14,224,
  0,56,0,104,0,248,0,176, 1,240,1,240,0,96,0,240,
  1,248,1,216,1,216,1,184, 0,240,0,96,0,96,0,224,
  0,14,0,26,0,62,0,44, 0,124,0,124,0,24,0,60,
  0,126,0,126,0,239,0,223, 0,60,0,110,0,118,0,238,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "Attack of the Mutant Telephones ";

// Attack of the Mutant Telephones (teleport: 246)
uint8_t CAVERN10[512] = {
  14,14,14,14,14,14,14,0, 0,0,0,0,0,0,0,0, 0,0,0,66,0,0,0,0, 0,0,0,0,0,0,0,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,70,0,0,0,0, 0,0,0,0,0,0,0,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,14,
  14,65,65,65,65,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,14,
  14,0,0,0,0,65,65,65, 65,65,65,0,0,0,0,65, 65,69,69,69,69,69,69,69, 65,65,0,0,0,0,0,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 66,0,0,0,0,65,65,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 66,0,0,0,0,0,0,14,
  14,65,65,0,0,6,6,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 66,0,0,0,0,65,65,14,
  14,0,0,0,0,0,0,0, 0,0,0,65,65,65,65,65, 65,65,65,65,0,0,0,0, 70,0,0,0,0,0,0,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,66,0,0,0, 0,0,0,66,0,0,0,0, 0,0,0,0,65,0,0,14,
  14,0,0,0,0,0,1,1, 1,65,0,0,66,0,0,0, 0,0,0,70,0,0,0,0, 0,0,0,0,0,0,0,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,70,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,65,65,65,14,
  14,65,65,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,65, 65,65,0,0,0,0,0,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,14,
  14,65,65,65,65,65,65,65, 65,65,65,65,65,65,65,65, 65,65,65,65,65,65,65,65, 65,65,65,65,65,65,65,14,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 16;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (1,3) (see LOCATION)
LOCATION = 23587;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (8,5)
CONVLOC = 30725;
// Length
CONVLEN = 2;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (0,24)
  {3, 23576, 96, 255},
  // Item 2 at (1,30)
  {4, 23614, 96, 255},
  // Item 3 at (4,1)
  {5, 23681, 96, 255},
  // Item 4 at (6,19)
  {6, 23763, 96, 255},
  // Item 5 at (13,30)
  {3, 23998, 104, 255},
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 86;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,218,171,234,107,255,255,
  144,9,144,9,255,255,144,9,
  144,9,255,255,144,9,144,9,
  255,255,144,9,144,9,255,255,
};
// Location in the attribute buffer at 23552: (1,1)
PORTALLOC1 = 23585;
// Location in the screen buffer at 24576: (1,1)
PORTALLOC2 = 24609;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {60,90,149,213,213,213,90,60};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=3, initial x=15, 15<=x<=24, speed=normal
  {70, 23663, 96, 0, 111, 120},
  // Horizontal guardian 2: y=7, initial x=14, 14<=x<=18, speed=slow
  {196, 23790, 96, 0, 238, 242},
  // Horizontal guardian 3: y=13, initial x=15, 5<=x<=19, speed=normal
  {66, 23983, 104, 7, 165, 179},
  // Horizontal guardian 4 (unused)
  {255,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=12, initial y=8, 2<=y<=56, initial y-increment=2
  {67, 0, 8, 12, 2, 2, 56},
  // Vertical guardian 2: x=3, initial y=32, 32<=y<=100, initial y-increment=1
  {4, 1, 32, 3, 1, 32, 100},
  // Vertical guardian 3: x=21, initial y=48, 48<=y<=100, initial y-increment=1
  {6, 2, 48, 21, 1, 48, 100},
  // Vertical guardian 4: x=26, initial y=48, 4<=y<=100, initial y-increment=-3
  {66, 3, 48, 26, 253, 4, 100},
};

// The next 6 bytes are unused.
//   DEFB 0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  0,0,0,0,63,252,99,198, 235,215,232,23,15,240,7,224,
  12,48,11,208,27,216,28,56, 63,252,63,252,63,252,63,252,
  60,0,127,192,127,248,99,198, 8,87,8,23,15,247,7,224,
  12,48,11,208,27,216,28,56, 63,252,63,252,63,252,63,252,
  0,0,0,0,63,252,99,198, 235,215,232,23,15,240,7,224,
  12,48,11,208,27,216,28,56, 63,252,63,252,63,252,63,252,
  0,60,3,254,31,254,99,198, 234,16,232,16,239,240,7,224,
  12,48,11,208,27,216,28,56, 63,252,63,252,63,252,63,252,
  12,0,22,0,45,0,76,128, 140,64,140,64,76,128,45,0,
  22,0,12,0,55,0,76,0, 127,192,255,192,64,128,46,0,
  3,0,3,0,5,128,7,128, 11,64,11,64,7,128,5,128,
  3,0,3,0,14,192,3,32, 63,224,63,240,16,32,7,64,
  0,192,0,192,0,192,0,192, 0,128,0,128,0,192,0,192,
  0,192,0,192,1,208,4,200, 15,252,15,248,0,8,3,176,
  0,48,0,104,0,180,0,180, 1,50,1,50,0,180,0,180,
  0,104,0,48,0,184,1,50, 3,255,1,255,1,0,0,220,
}

// ----------------------------------------------------------------------------

CAVERNNAME = " Return of the Alien Kong Beast ";

// Return of the Alien Kong Beast (teleport: 1246)
uint8_t CAVERN11[512] = {
  101,0,5,0,0,0,6,0, 0,0,5,0,0,0,0,0, 0,101,6,0,0,101,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,3, 3,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,67,67,67,0,0,0,0, 0,3,3,3,3,3,101,0, 0,101,3,3,3,3,3,3, 67,67,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,101,0, 0,101,0,0,0,0,0,0, 0,0,0,0,0,0,67,101,
  101,0,0,0,0,0,67,67, 0,0,0,0,0,0,101,0, 0,101,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,67,0,0,0,0, 0,0,0,0,0,0,101,0, 0,101,0,0,0,0,0,0, 0,67,67,67,67,67,67,101,
  101,0,0,0,0,0,0,0, 0,0,67,67,67,67,101,0, 0,101,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,67,0, 0,0,0,0,0,0,0,0, 0,101,67,67,67,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,101,0,0,0,0,0,0, 0,67,67,0,0,0,0,101,
  101,67,67,67,67,67,67,0, 0,0,0,0,0,0,0,0, 0,101,0,0,0,0,0,4, 0,0,0,0,4,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,67,67,67,101,0, 0,101,70,70,70,70,70,70, 70,70,70,70,70,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,101,0, 0,101,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,67,67,67,67,67,67,67, 67,67,67,67,67,67,101,101, 101,101,67,67,67,67,67,67, 67,67,67,67,67,67,67,101,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,2) (see LOCATION)
LOCATION = 23970;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (right)
CONVDIR = 1;
// Location in the screen buffer at 28672: (13,18)
CONVLOC = 30898;
// Length
CONVLEN = 11;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (3,15)
  {3, 23663, 96, 255},
  // Item 2 at (7,16)
  {4, 23792, 96, 255},
  // Item 3 at (6,2)
  {5, 23746, 96, 255},
  // Item 4 at (13,29)
  {6, 23997, 104, 255},
  // Item 5 at (5,26)
  {3, 23738, 96, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 94;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,128,1,143,241,143,241,
  143,241,143,241,143,241,140,49,
  140,49,143,241,143,241,143,241,
  143,241,143,241,128,1,255,255,
};
// Location in the attribute buffer at 23552: (13,15)
PORTALLOC1 = 23983;
// Location in the screen buffer at 24576: (13,15)
PORTALLOC2 = 26799;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {128,192,236,114,40,84,138,135};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=13, initial x=9, 1<=x<=9, speed=normal
  {68, 23977, 104, 7, 161, 169},
  // Horizontal guardian 2: y=11, initial x=11, 11<=x<=15, speed=slow
  {198, 23915, 104, 0, 107, 111},
  // Horizontal guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Horizontal guardian 4: y=6, initial x=25, 25<=x<=28, speed=normal
  {5, 23769, 96, 0, 217, 220}
};

// The next two bytes are copied to EUGDIR and EUGHGT; the first byte specifies
// the Kong Beast's initial status, but the second byte is not used.
// Initial status (on the ledge)
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 2 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  19,200,29,184,15,240,6,96, 5,160,2,64,7,224,15,240,
  31,248,51,204,99,198,70,98, 44,52,6,96,2,64,14,112,
  11,208,13,176,15,240,6,96, 5,160,2,64,3,192,31,248,
  127,254,231,231,131,193,199,227, 6,96,12,48,8,16,56,28,
  28,56,6,96,12,48,102,102, 35,196,103,230,55,236,31,248,
  15,240,7,224,2,64,5,160, 6,96,15,240,13,176,11,208,
  112,14,24,24,12,48,6,96, 99,198,39,228,103,230,55,236,
  31,248,15,240,2,64,5,160, 22,104,15,240,13,176,3,192,
  8,0,5,0,8,128,37,0, 72,128,33,0,76,0,51,0,
  68,128,68,128,136,64,132,64, 72,128,72,128,51,0,12,0,
  2,0,17,32,10,64,17,32, 10,64,16,32,3,0,12,192,
  16,32,16,96,34,144,37,16, 24,32,16,32,12,192,3,0,
  0,64,2,32,4,72,2,36, 4,72,2,4,0,200,3,48,
  4,8,4,8,11,68,8,180, 4,8,4,8,3,48,0,192,
  0,68,1,34,2,68,1,34, 2,68,1,2,2,48,0,204,
  1,66,1,34,2,17,2,33, 1,18,1,10,0,204,0,48,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "          Ore Refinery          ";

// Ore Refinery (teleport: 346)
uint8_t CAVERN12[512] = {
  22,22,22,22,22,22,22,22, 22,22,22,22,22,22,22,22, 22,22,22,22,22,22,22,22, 22,22,22,22,22,22,22,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,5, 5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5, 5,0,0,5,5,5,5,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,5, 5,0,0,5,5,5,5,0, 0,5,5,5,5,5,0,0, 5,5,5,5,0,0,5,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,5, 5,5,5,5,0,0,5,5, 5,0,0,0,5,5,5,5, 5,0,0,5,5,5,5,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,5, 5,5,0,0,5,5,5,0, 0,5,5,5,5,0,0,5, 5,5,5,0,0,5,5,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,6,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,5,5,4,4,4,4,4, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4, 4,4,4,4,4,5,5,22,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,29) (see LOCATION)
LOCATION = 23997;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (right)
CONVDIR = 1;
// Location in the screen buffer at 28672: (15,3)
CONVLOC = 30947;
// Length
CONVLEN = 26;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 1;
// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (3,26)
  {3, 23674, 96, 255},
  // Item 2 at (6,10)
  {4, 23754, 96, 255},
  // Item 3 at (9,19)
  {5, 23859, 104, 255},
  // Item 4 at (9,26)
  {6, 23866, 104, 255},
  // Item 5 at (12,11)
  {3, 23947, 104, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 79;
// Graphic data
uint8_t PORTALG[32] = {
  3,192,7,224,15,240,9,144,
  9,144,7,224,5,160,2,64,
  97,134,248,31,254,127,5,224,
  7,160,254,127,248,31,96,6,
};
// Location in the attribute buffer at 23552: (13,1)
PORTALLOC1 = 23969;
// Location in the screen buffer at 24576: (13,1)
PORTALLOC2 = 26785;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {24,110,66,219,201,98,126,24};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 252;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=1, initial x=7, 7<=x<=29, speed=normal
  {67, 23591, 96, 0, 39, 61},
  // Horizontal guardian 2: y=4, initial x=16, 7<=x<=29, speed=slow
  {196, 23696, 96, 0, 135, 157},
  // Horizontal guardian 3: y=7, initial x=20, 10<=x<=26, speed=normal
  {70, 23796, 96, 7, 234, 250},
  // Horizontal guardian 4: y=10, initial x=18, 7<=x<=29, speed=slow
  {194, 23890, 104, 0, 71, 93}
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=5, initial y=8, 8<=y<=100, initial y-increment=2
  {71, 0, 8, 5, 2, 8, 100},
  // Vertical guardian 2 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  0,0,0,0,0,0,3,192, 12,48,16,8,32,4,64,2,
  128,1,64,2,32,4,208,11, 44,52,75,210,18,72,2,64,
  0,0,0,0,0,0,3,192, 12,48,16,8,32,4,64,2,
  248,31,87,234,43,212,18,72, 12,48,3,192,0,0,0,0,
  4,32,4,32,18,72,75,210, 44,52,147,201,167,229,70,98,
  134,97,71,226,35,196,16,8, 12,48,3,192,0,0,0,0,
  0,0,0,0,0,0,3,192, 12,48,18,72,42,84,95,250,
  246,127,71,226,35,196,16,8, 12,48,3,192,0,0,0,0,
  97,128,178,64,179,192,97,128, 12,0,255,192,82,128,18,0,
  18,0,30,0,12,0,12,0, 12,0,12,0,30,0,63,0,
  24,96,36,208,60,208,24,96, 3,0,63,240,20,160,4,128,
  4,128,7,128,3,0,3,0, 7,128,15,192,0,0,0,0,
  6,24,13,60,13,36,6,24, 0,192,15,252,5,40,1,32,
  1,32,1,224,1,224,3,240, 0,0,0,0,0,0,0,0,
  1,134,3,203,2,75,1,134, 0,48,3,255,1,74,0,72,
  0,72,0,120,0,48,0,48, 0,120,0,252,0,0,0,0,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "       Skylab Landing Bay       ";

// Skylab Landing Bay (teleport: 1346)
uint8_t CAVERN13[512] = {
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,76, 12,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,76,12,8,8,8, 8,8,8,76,12,8,8,8, 8,8,8,76,12,8,8,8, 8,8,8,76,12,8,8,104,
  104,8,8,8,8,8,8,76, 12,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,76, 12,8,8,8,8,8,8,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,8,8,76,12,8, 8,8,8,8,8,76,12,8, 8,8,8,8,8,76,12,8, 8,8,8,8,8,76,12,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,76,12,8,8,8,8,8, 8,76,12,8,8,8,8,75, 75,75,75,75,75,8,8,8, 8,76,12,8,8,8,8,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,8,8,8,8,76, 12,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,8, 8,8,8,8,8,8,8,104,
  104,104,104,104,104,104,104,104, 104,104,104,104,104,104,104,104, 104,104,104,104,104,104,104,104, 104,104,104,104,104,104,104,104,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,29) (see LOCATION)
LOCATION = 23997;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (11,15)
CONVLOC = 30831;
// Length
CONVLEN = 6;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 6;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (2,23)
  {11, 23639, 96, 255},
  // Item 2 at (8,3)
  {12, 23811, 104, 255},
  // Item 3 at (7,27)
  {13, 23803, 96, 255},
  // Item 4 at (7,16)
  {14, 23792, 96, 255},
  // Item 5 (unused)
  {0,255,255,255,255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 30;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,255,255,252,63,248,31,
  240,15,224,7,193,131,194,67,
  194,67,193,131,224,7,240,15,
  248,31,252,63,255,255,255,255,
};
// Location in the attribute buffer at 23552: (0,15)
PORTALLOC1 = 23567;
// Location in the screen buffer at 24576: (0,15)
PORTALLOC2 = 24591;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {170,170,254,254,254,254,170,170};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 248;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=7, initial x=9, 9<=x<=14, speed=slow (unused)
  {255, 23785, 96, 0, 233, 238},
  // Horizontal guardian 2: y=10, initial x=12, 8<=x<=14, speed=slow (unused)
  {194, 23884, 104, 0, 72, 78},
  // Horizontal guardian 3: y=13, initial x=8, 4<=x<=26, speed=normal (unused)
  {67, 23976, 104, 0, 164, 186},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=1, initial y=0, 0<=y<=72, y-increment=4
  {15, 0, 0, 1, 4, 0, 72},
  // Vertical guardian 2: x=11, initial y=0, 0<=y<=32, y-increment=1
  {13, 0, 0, 11, 1, 0, 32},
  // Vertical guardian 3: x=21, initial y=2, 2<=y<=56, y-increment=3
  {14, 0, 2, 21, 3, 2, 56},
  // Vertical guardian 4 (unused)
  {255,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  3,192,255,255,171,213,255,255, 19,200,41,148,21,168,11,208,
  5,160,3,192,3,192,5,160, 10,80,20,40,40,20,16,8,
  0,0,0,0,3,192,255,255, 171,213,255,255,19,200,41,148,
  21,168,11,208,5,160,3,192, 3,192,37,160,74,84,20,42,
  0,0,0,0,0,0,0,7, 3,253,255,215,171,248,255,192,
  3,192,1,128,21,164,75,210, 5,164,35,194,11,208,37,168,
  0,0,0,0,0,0,0,32, 2,2,0,21,3,206,15,212,
  203,200,183,194,227,200,49,129, 7,228,195,200,23,194,35,252,
  0,0,1,0,0,0,8,32, 0,0,0,0,33,2,0,17,
  3,138,14,144,75,192,55,2, 98,192,49,1,5,226,195,68,
  0,0,0,0,0,0,0,0, 0,0,2,0,0,0,0,32,
  16,8,10,132,0,32,101,0, 34,104,8,160,3,208,23,224,
  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
  2,0,0,32,16,0,0,0, 5,16,0,104,34,160,13,208,
  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,128, 0,32,8,0,2,192,7,96,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "            The Bank            ";

// The Bank (teleport: 2346)
uint8_t CAVERN14[512] = {
  14,0,0,0,0,0,14,14, 14,14,14,14,14,14,14,14, 14,14,14,14,14,14,14,14, 14,14,14,14,14,14,14,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,6,6,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,6,6,14,
  14,0,0,0,0,0,0,0, 69,69,69,69,69,69,69,69, 69,69,69,69,69,69,69,69, 65,65,65,65,65,6,6,14,
  14,0,0,0,0,0,0,0, 66,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,66,6,6,14,
  14,65,65,65,65,65,0,0, 70,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,66,6,6,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 65,65,0,0,66,6,6,14,
  14,0,0,0,0,0,0,1, 0,0,0,0,65,65,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,66,6,6,14,
  14,0,0,65,65,0,0,0, 0,0,0,0,0,0,0,0, 0,0,65,65,0,0,0,0, 0,0,0,0,66,6,6,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,65,65,0,66,6,6,14,
  14,65,65,0,0,0,0,0, 0,0,0,0,65,65,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,70,6,6,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,65,65,0,0,0,0, 0,0,0,0,0,6,6,14,
  14,0,0,0,0,65,65,65, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,65, 65,0,0,0,0,6,6,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,65,65,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,6,6,14,
  14,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,6,6,14,
  14,65,65,65,65,65,65,65, 65,65,65,65,65,65,65,65, 65,65,65,65,65,65,65,65, 65,65,65,65,65,65,65,14,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,2) (see LOCATION)
LOCATION = 23970;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (3,8)
CONVLOC = 28776;
// Length
CONVLEN = 16;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (2,25)
  {3, 23641, 96, 255},
  // Item 2 at (6,12)
  {4, 23756, 96, 255},
  // Item 3 at (14,26)
  {5, 24026, 104, 255},
  // Item 4 at (6,19) (unused)
  {255, 23763, 96, 255},
  // Item 5 at (13,30) (unused)
  {3, 23998, 104, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 86;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,128,1,128,1,128,1,
  128,1,136,1,170,1,156,61,
  255,71,156,1,170,1,136,1,
  128,1,128,1,128,1,255,255,
};
// Location in the attribute buffer at 23552: (3,1)
PORTALLOC1 = 23649;
// Location in the screen buffer at 24576: (3,1)
PORTALLOC2 = 24673;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {124,56,100,222,142,222,130,124};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 252;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=13, initial x=17, 17<=x<=19, speed=normal
  {69, 23985, 104, 0, 177, 179},
  // Horizontal guardian 2 (unused)
  {0,0,0,0,0,0,0},
  // Horizontal guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=9, initial y=40, 36<=y<=102, initial y-increment=2
  {6, 0, 40, 9, 2, 36, 102},
  // Vertical guardian 2: x=15, initial y=64, 36<=y<=102, initial y-increment=1
  {7, 1, 64, 15, 1, 36, 102},
  // Vertical guardian 3: x=21, initial y=80, 32<=y<=104, initial y-increment=-3
  {68, 3, 80, 21, 253, 32, 104},
  // Vertical guardian 4 (unused)
  {255,0,0,0,0,0,0},
};

// The next 6 bytes are unused.
//   DEFB 0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  97,134,159,249,159,249,97,134, 3,192,255,255,128,1,170,169,
  159,253,181,89,144,13,181,89, 159,253,170,169,128,1,255,255,
  29,184,34,244,34,244,29,184, 3,192,255,255,213,85,191,255,
  234,173,176,7,229,77,176,7, 234,173,191,255,213,85,255,255,
  7,224,8,16,8,16,7,224, 3,192,255,255,255,255,213,87,
  224,3,202,167,231,243,202,167, 224,3,213,87,255,255,255,255,
  29,184,47,68,47,68,29,184, 3,192,255,255,170,171,192,1,
  149,83,207,249,154,179,207,249, 149,83,192,1,170,171,255,255,
  0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
  0,0,0,0,255,192,129,192, 255,192,130,64,254,64,255,192,
  0,0,0,0,0,0,0,0, 0,0,0,0,63,240,32,112,
  63,240,32,144,63,144,63,240, 0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0, 15,252,8,28,15,252,8,36,
  15,228,15,252,0,0,0,0, 0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0, 0,0,0,0,3,255,2,7,
  3,255,2,9,3,249,3,255, 0,0,0,0,0,0,0,0,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "      The Sixteenth Cavern      ";

// The Sixteenth Cavern (teleport: 12346)
uint8_t CAVERN15[512] = {
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,66,0,0,0,0,66,0, 0,0,0,101,0,0,101,0, 0,0,0,0,0,0,66,66, 66,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,101,0,0,101,101, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,66,0,0,0, 0,0,0,101,0,0,101,101, 101,0,0,0,0,0,0,0, 0,66,66,66,66,66,66,101,
  101,0,0,0,0,0,0,0, 0,0,0,101,0,0,101,101, 101,101,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,2,2,70,70,70,70,70, 70,70,70,70,70,70,70,70, 70,70,70,70,70,70,70,70, 70,70,70,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,101,101,66,66,0,0, 0,0,0,0,0,0,0,0, 0,0,66,0,0,0,0,101,
  101,66,66,66,66,66,66,66, 66,66,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,66,0,0,0, 0,0,66,0,0,0,0,101,
  101,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,4, 4,4,0,0,0,0,0,101,
  101,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,101,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,2) (see LOCATION)
LOCATION = 23970;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (9,3)
CONVLOC = 30755;
// Length
CONVLEN = 24;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (2,30)
  {3, 23646, 96, 255},
  // Item 2 at (7,13)
  {4, 23789, 96, 255},
  // Item 3 at (0,1)
  {5, 23553, 96, 255},
  // Item 4 at (10,17)
  {6, 23889, 104, 255},
  // Item 5 at (5,26) (unused)
  {255, 23738, 96, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 94;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,129,129,129,129,255,255,
  129,129,129,129,255,255,129,129,
  129,129,255,255,129,129,129,129,
  255,255,129,129,129,129,255,255,
};
// Location in the attribute buffer at 23552: (5,12)
PORTALLOC1 = 23724;
// Location in the screen buffer at 24576: (5,12)
PORTALLOC2 = 24748;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {15,9,61,39,244,156,144,240};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 248;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=13, initial x=9, 1<=x<=18, speed=normal
  {68, 23977, 104, 0, 161, 178},
  // Horizontal guardian 2: y=10, initial x=1, 1<=x<=7, speed=normal
  {6, 23873, 104, 0, 65, 71},
  // Horizontal guardian 3: y=7, initial x=18, 18<=x<=23, speed=normal
  {67, 23794, 96, 0, 242, 247},
  // Horizontal guardian 4: y=5, initial x=26, 25<=x<=29, speed=slow
  {133, 23738, 96, 0, 185, 189}
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 2 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  0,0,8,0,20,0,42,0, 85,0,74,0,132,0,128,192,
  128,192,65,0,127,128,63,192, 31,128,15,0,10,128,18,64,
  42,0,21,0,42,0,21,0, 32,0,32,0,32,0,32,48,
  32,48,16,64,31,224,15,240, 7,224,3,192,2,160,4,144,
  0,0,16,0,40,0,84,0, 170,0,81,0,33,0,1,12,
  2,12,2,16,3,248,3,252, 1,248,0,240,0,168,1,36,
  5,64,10,128,5,64,10,128, 0,64,0,64,0,64,0,67,
  0,131,0,132,0,254,0,255, 0,126,0,60,0,42,0,73,
  2,160,1,80,2,160,1,80, 2,0,2,0,2,0,194,0,
  193,0,33,0,127,0,255,0, 126,0,60,0,84,0,146,0,
  0,0,0,8,0,20,0,42, 0,85,0,138,0,132,48,128,
  48,64,8,64,31,192,63,192, 31,128,15,0,21,0,36,128,
  0,84,0,168,0,84,0,168, 0,4,0,4,0,4,12,4,
  12,4,2,8,7,248,15,240, 7,224,3,192,5,64,9,32,
  0,0,0,16,0,40,0,84, 0,170,0,82,0,33,3,1,
  3,1,0,130,1,254,3,252, 1,248,0,240,1,80,2,72,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "         The Warehouse          ";

// The Warehouse (teleport: 56)
uint8_t CAVERN16[512] = {
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,22,22,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,6,0, 0,6,0,0,0,6,0,0, 6,0,0,0,0,0,6,0, 6,0,0,0,0,0,0,22,
  22,4,4,68,68,68,68,68, 68,68,0,0,68,68,68,68, 68,68,68,0,0,68,68,68, 0,68,68,0,0,4,4,22,
  22,68,68,33,68,68,68,68, 68,68,0,0,68,68,68,68, 68,68,68,0,0,68,68,68, 68,68,68,0,0,68,68,22,
  22,68,68,68,68,68,68,68, 68,68,0,0,68,68,68,0, 68,68,68,0,0,68,68,68, 68,68,33,0,0,68,68,22,
  22,68,68,0,0,68,68,68, 68,68,0,0,68,68,32,32, 32,32,32,0,0,68,68,68, 68,68,68,0,0,68,68,22,
  22,0,68,0,0,68,68,68, 68,68,0,0,68,68,68,68, 68,68,68,0,0,68,68,68, 68,68,68,0,0,68,68,22,
  22,68,68,0,0,68,68,68, 68,68,0,0,68,68,68,68, 68,68,68,0,68,68,33,68, 68,68,68,0,0,68,68,22,
  22,68,68,0,0,68,68,68, 68,33,0,0,68,68,68,68, 68,68,68,68,68,68,68,68, 68,68,0,0,0,68,68,22,
  22,68,68,0,0,68,68,68, 68,68,0,0,68,68,68,68, 68,68,68,68,68,68,68,68, 68,68,68,0,0,68,68,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,4,4,4,4,22,
  22,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,22,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 48;

// Animation frame (see FRAME)
FRAME = 3;

// Direction and movement flags: facing left (see DMFLAGS)
DMFLAGS = 1;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (3,1) (see LOCATION)
LOCATION = 23649;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (right)
CONVDIR = 1;
// Location in the screen buffer at 28672: (8,14)
CONVLOC = 30734;
// Length
CONVLEN = 5;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (5,24)
  {35, 23736, 96, 255},
  // Item 2 at (7,15)
  {36, 23791, 96, 255},
  // Item 3 at (9,1)
  {37, 23841, 104, 255},
  // Item 4 at (10,19)
  {38, 23891, 104, 255},
  // Item 5 at (11,26)
  {35, 23930, 104, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 76;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,128,1,191,253,160,5,
  165,165,165,165,165,165,165,165,
  165,165,165,165,175,245,165,165,
  165,165,165,165,165,165,255,255,
};
// Location in the attribute buffer at 23552: (1,29)
PORTALLOC1 = 23613;
// Location in the screen buffer at 24576: (1,29)
PORTALLOC2 = 24637;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=13, initial x=5, 5<=x<=8, speed=slow
  {194, 23973, 104, 0, 165, 168},
  // Horizontal guardian 2: y=13, initial x=12, 12<=x<=25, speed=normal
  {5, 23980, 104, 0, 172, 185},
  // Horizontal guardian 3 (unused)
  {255,0,0,0,0,0,0},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=3, initial y=64, 64<=y<=102, initial y-increment=2
  {65, 0, 64, 3, 2, 64, 102},
  // Vertical guardian 2: x=10, initial y=64, 3<=y<=96, initial y-increment=-3
  {6, 1, 64, 10, 253, 3, 96},
  // Vertical guardian 3: x=19, initial y=48, 0<=y<=64, initial y-increment=1
  {71, 2, 48, 19, 1, 0, 64},
  // Vertical guardian 4: x=27, initial y=0, 4<=y<=96, initial y-increment=4
  {67, 3, 0, 27, 4, 4, 96},
};

// The next 6 bytes are unused.
//   DEFB 0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  85,85,255,255,255,255,8,16, 8,16,8,16,248,31,85,85,
  255,255,255,255,8,16,8,16, 8,16,88,21,255,255,255,255,
  0,0,85,85,255,255,255,255, 8,16,248,31,8,16,63,254,
  56,30,8,16,95,245,255,255, 255,255,0,0,255,255,0,0,
  0,0,0,0,255,255,85,85, 255,255,255,255,8,16,56,30,
  63,254,8,16,248,31,95,245, 255,255,255,255,0,0,0,0,
  0,0,85,85,255,255,248,31, 8,16,85,85,255,255,255,255,
  120,29,248,31,248,31,8,16, 85,85,255,255,255,255,0,0,
  126,0,153,0,255,0,219,0, 231,0,126,0,36,0,36,0,
  36,0,66,0,66,0,66,0, 129,0,129,0,195,0,195,0,
  0,0,31,128,38,64,63,192, 54,192,57,192,31,128,16,128,
  32,64,32,64,64,32,64,32, 128,16,128,48,192,48,192,0,
  0,0,0,0,0,0,7,224, 9,144,15,240,13,176,14,112,
  7,224,8,16,16,8,32,4, 64,2,128,1,192,3,192,3,
  0,0,1,248,2,100,3,252, 3,108,3,156,1,248,1,8,
  2,4,2,4,4,2,4,2, 8,1,12,1,12,3,0,3,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "      Amoebatrons' Revenge      ";

// Amoebatrons' Revenge (teleport: 156)
uint8_t CAVERN17[512] = {
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,22,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,66,66,0,0,66, 66,66,0,0,66,66,66,66, 66,66,66,66,0,0,66,66, 66,0,0,66,66,66,66,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,66,66,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,66,66,66,66, 66,66,66,66,0,0,66,66, 66,0,0,66,66,0,0,22,
  22,0,0,66,66,0,0,66, 66,66,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,66,66,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,66,66,0,0,66, 66,66,0,0,66,66,66,66, 66,66,66,66,0,0,66,66, 66,0,0,66,66,0,0,22,
  22,66,66,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  22,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,22,
  66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 3;

// Direction and movement flags: facing left (see DMFLAGS)
DMFLAGS = 1;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,29) (see LOCATION)
LOCATION = 23997;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the (unused) conveyor.

// Direction (right)
CONVDIR = 1;
// Location in the screen buffer at 28672: (9,7)
CONVLOC = 30759;
// Length
CONVLEN = 3;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 1;
// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (1,16)
  {3, 23600, 96, 255},
  // Item 2 (unused)
  {255,255,255,255,255},
  // Item 3 (unused)
  {0,255,255,255,255},
  // Item 4 (unused)
  {0,255,255,255,255},
  // Item 5 (unused)
  {0,255,255,255,255},
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 14;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,128,1,176,13,160,5,
  170,85,170,85,170,85,170,85,
  170,85,170,85,170,85,170,85,
  160,5,176,13,128,1,255,255,
};
// Location in the attribute buffer at 23552: (0,29)
PORTALLOC1 = 23581;
// Location in the screen buffer at 24576: (0,29)
PORTALLOC2 = 24605;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 128;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=3, initial x=12, 12<=x<=18, speed=slow
  {196, 23660, 96, 0, 108, 114},
  // Horizontal guardian 2: y=10, initial x=16, 12<=x<=17, speed=slow
  {133, 23888, 104, 0, 76, 81},
  // Horizontal guardian 3: y=6, initial x=16, 12<=x<=17, speed=normal
  {67, 23760, 96, 0, 204, 209},
  // Horizontal guardian 4: y=13, initial x=16, 12<=x<=18, speed=normal
  {6, 23984, 104, 7, 172, 178}
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=5, initial y=8, 5<=y<=104, initial y-increment=3
  {67, 0, 8, 5, 3, 5, 104},
  // Vertical guardian 2: x=10, initial y=8, 5<=y<=104, initial y-increment=2
  {4, 1, 8, 10, 2, 5, 104},
  // Vertical guardian 3: x=20, initial y=8, 5<=y<=104, initial y-increment=4
  {5, 2, 8, 20, 4, 5, 104},
  // Vertical guardian 4: x=25, initial y=8, 5<=y<=104, initial y-increment=1
  {6, 3, 8, 25, 1, 5, 104},
};

// The next 6 bytes are unused.
//   DEFB 0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  3,192,14,112,19,200,49,140, 57,156,95,250,141,178,132,164,
  73,36,41,18,36,137,66,73, 130,82,4,144,8,136,0,64,
  3,192,14,112,19,200,49,140, 57,156,95,250,77,177,133,17,
  132,146,72,164,41,36,41,18, 68,137,2,72,2,80,4,0,
  3,192,14,112,19,200,49,140, 57,156,95,250,77,177,68,145,
  130,73,130,74,68,148,37,36, 41,34,8,144,4,72,0,64,
  3,192,14,112,19,200,49,140, 57,156,95,250,77,178,41,18,
  36,145,66,73,130,74,132,74, 72,145,9,32,9,0,0,128,
  12,0,12,0,12,0,12,0, 12,0,12,0,12,0,12,0,
  12,0,12,0,255,192,12,0, 97,128,210,192,179,64,97,128,
  3,0,3,0,3,0,3,0, 3,0,3,0,3,0,3,0,
  3,0,3,0,63,240,3,0, 24,96,36,208,60,208,24,96,
  0,192,0,192,0,192,0,192, 0,192,0,192,0,192,0,192,
  0,192,0,192,15,252,0,192, 6,24,11,52,13,44,6,24,
  0,48,0,48,0,48,0,48, 0,48,0,48,0,48,0,48,
  0,48,0,48,3,255,0,48, 1,134,2,77,3,205,1,134,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "     Solar Power Generator      ";

// Solar Power Generator (teleport: 256)
uint8_t CAVERN18[512] = {
  22,22,22,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,36,36,32,32,36,36,36, 36,32,32,32,32,32,32,36, 36,36,36,36,36,36,36,36, 32,32,32,32,32,32,32,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,32,32,32,36,36, 36,36,36,36,36,36,36,22,
  22,32,32,36,36,36,36,36, 36,36,36,36,36,32,32,32, 36,36,36,36,36,36,36,36, 32,32,32,32,32,32,32,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,32,32,36,36,36, 36,36,36,36,36,36,36,22,
  22,32,32,32,32,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 32,32,32,32,32,32,32,22,
  22,36,36,36,36,36,36,38, 38,38,38,36,36,36,32,32, 32,32,32,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,22,22,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,36, 36,36,36,36,36,36,36,22,
  22,22,22,32,32,32,32,32, 32,32,32,32,32,32,32,32, 32,32,32,32,32,32,32,22, 32,32,32,32,32,32,32,22,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 160;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing right (see DMFLAGS)
DMFLAGS = 0;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (10,14) (see LOCATION)
LOCATION = 23886;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (left)
CONVDIR = 0;
// Location in the screen buffer at 28672: (12,7)
CONVLOC = 30855;
// Length
CONVLEN = 4;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 3;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (1,30)
  {35, 23614, 96, 255},
  // Item 2 at (5,1)
  {36, 23713, 96, 255},
  // Item 3 at (12,30)
  {37, 23966, 104, 255},
  // Item 4 (unused)
  {255,255,255,255,255},
  // Item 5 (unused)
  {0,255,255,255,255},
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 78;
// Graphic data
uint8_t PORTALG[32] = {
  255,255,128,1,191,253,160,5,
  175,245,168,21,171,213,170,85,
  170,85,171,213,168,21,175,245,
  160,5,191,253,128,1,255,255,
};
// Location in the attribute buffer at 23552: (1,1)
PORTALLOC1 = 23585;
// Location in the screen buffer at 24576: (1,1)
PORTALLOC2 = 24609;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 240;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=3, initial x=24, 23<=x<=29, speed=normal
  {38, 23672, 96, 0, 119, 125},
  // Horizontal guardian 2: y=6, initial x=28, 22<=x<=29, speed=normal
  {33, 23772, 96, 0, 214, 221},
  // Horizontal guardian 3: y=9, initial x=29, 23<=x<=29, speed=slow
  {162, 23869, 104, 7, 55, 61},
  // Horizontal guardian 4: y=13, initial x=16, 13<=x<=29, speed=normal
  {38, 23984, 104, 0, 173, 189}
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=5, initial y=64, 2<=y<=102, initial y-increment=3
  {38, 0, 64, 5, 3, 2, 102},
  // Vertical guardian 2: x=11, initial y=56, 48<=y<=102, initial y-increment=-2
  {34, 1, 56, 11, 254, 48, 102},
  // Vertical guardian 3: x=16, initial y=80, 4<=y<=80, initial y-increment=1
  {33, 2, 80, 16, 1, 4, 80},
  // Vertical guardian 4 (unused)
  {255,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  3,64,15,112,63,60,63,76, 95,102,95,118,159,127,0,127,
  142,0,142,255,70,254,64,242, 32,4,48,12,12,48,2,192,
  3,192,15,240,63,172,63,204, 95,198,71,182,153,191,158,127,
  142,127,141,159,69,230,65,242, 32,4,48,12,12,48,3,192,
  3,192,15,240,47,188,55,200, 91,230,93,230,158,223,158,63,
  140,127,139,127,71,190,65,194, 0,4,48,4,12,48,3,192,
  3,192,11,240,61,188,61,204, 93,230,94,244,158,227,158,31,
  136,127,135,127,7,126,65,178, 32,4,48,12,12,16,3,192,
  6,0,12,0,24,0,56,0, 116,0,202,128,133,192,3,192,
  6,64,206,192,216,64,255,192, 226,0,200,128,213,64,8,128,
  1,128,3,0,6,0,14,0, 29,0,50,160,33,112,0,240,
  1,144,99,176,102,16,127,240, 120,128,98,32,101,80,2,32,
  0,96,0,192,1,128,3,128, 7,64,12,168,8,92,0,60,
  0,100,48,236,49,132,63,252, 62,32,48,136,49,84,0,136,
  0,24,0,48,0,96,0,224, 1,208,3,42,2,23,0,15,
  0,25,6,59,6,97,7,255, 7,136,6,34,6,85,0,34,
}

// ----------------------------------------------------------------------------

CAVERNNAME = "        The Final Barrier       ";

// The Final Barrier (teleport: 1256)
uint8_t CAVERN19[512] = {
  44,34,34,34,34,34,44,40, 40,40,40,40,47,47,47,47, 47,40,40,40,40,40,46,50, 50,46,40,40,40,40,40,40,
  44,34,34,34,34,34,44,40, 40,47,40,40,47,47,47,47, 47,40,40,40,40,40,58,56, 56,58,40,40,40,42,42,42,
  44,34,34,22,34,44,46,46, 46,46,46,46,47,47,47,47, 47,46,43,46,43,46,58,56, 56,58,47,47,47,42,42,42,
  40,44,44,22,44,46,46,46, 46,46,46,46,46,40,40,40, 44,44,44,44,44,44,58,58, 58,58,47,47,47,40,42,40,
  40,47,40,22,40,46,46,46, 46,46,46,46,46,44,44,44, 38,38,38,38,38,38,38,38, 38,38,38,38,38,38,38,38,
  40,44,44,22,44,46,46,46, 46,46,46,46,46,39,38,38, 38,38,38,0,0,38,0,0, 0,0,0,0,0,0,0,38,
  12,38,38,38,38,33,33,33, 14,14,33,33,33,39,38,38, 38,38,38,0,0,38,0,0, 0,0,0,0,0,0,0,38,
  38,38,38,38,38,38,38,38, 38,38,38,38,38,38,38,38, 38,38,38,0,0,38,0,0, 0,0,0,0,0,0,0,38,
  38,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,66,66,38,
  38,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,38,
  38,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,0, 0,0,2,0,0,0,0,38,
  38,0,0,0,0,0,0,0, 0,68,0,0,68,0,0,0, 0,68,0,0,0,68,0,0, 0,0,0,0,66,0,0,38,
  38,66,66,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,38,
  38,0,0,0,0,66,66,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,38,
  38,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,38,
  38,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,66, 66,66,66,66,66,66,66,38,
};

// The next 72 bytes are copied to BACKGROUND and contain the attributes and graphic data for the tiles used to build the cavern.
// .... extracted ....

// The next seven bytes are copied to 32872-32878 and specify Miner Willy's initial location and appearance in the cavern.

// Pixel y-coordinate * 2 (see PIXEL_Y)
PIXEL_Y = 208;

// Animation frame (see FRAME)
FRAME = 0;

// Direction and movement flags: facing left (see DMFLAGS)
DMFLAGS = 1;

// Airborne status indicator (see AIRBORNE)
AIRBORNE = 0;

// Location in the attribute buffer at 23552: (13,27) (see LOCATION)
LOCATION = 23995;

// Jumping animation counter (see JUMPING)
JUMPING = 0;

// The next four bytes are copied to CONVDIR and specify the direction, location and length of the conveyor.
// Direction (right)
CONVDIR = 1;
// Location in the screen buffer at 28672: (10,1)
CONVLOC = 30785;
// Length
CONVLEN = 22;

// Border colour
// The next byte is copied to BORDER and specifies the border colour.
BORDER = 2;

// The next byte is copied to ITEMATTR, but is not used.
ITEMATTR = 0;

// The next 25 bytes are copied to ITEMS and specify the location and initial colour of the items in the cavern.
uint16_t ITEMS[5][5] = {
  // Item 1 at (5,23)
  {3, 23735, 96, 255},
  // Item 2 at (6,30)
  {4, 23774, 96, 255},
  // Item 3 at (11,10)
  {5, 23914, 104, 255},
  // Item 4 at (11,14)
  {6, 23918, 104, 255},
  // Item 5 at (11,19)
  {3, 23923, 104, 255}
};

// The next 37 bytes are copied to PORTAL and define the portal graphic and its location.
// Attribute
PORTAL = 30;
// Graphic data
uint8_t PORTALG[32] = {
  0,0,7,224,24,24,35,196,
  68,34,72,18,72,18,72,18,
  68,34,34,68,26,88,74,82,
  122,94,66,66,126,126,0,0,
};
// Location in the attribute buffer at 23552: (5,19)
PORTALLOC1 = 23731;
// Location in the screen buffer at 24576: (5,19)
PORTALLOC2 = 24755;

// Item graphic data
// The next eight bytes are copied to ITEM and define the item graphic.
ITEM = {48,72,136,144,104,4,10,4};

// The next byte is copied to AIR and specifies the initial air supply in the cavern.
AIR = 63;

// The next byte is copied to CLOCK and initialises the game clock.
CLOCK = 252;

// The next 28 bytes are copied to HGUARDS and define the horizontal guardians.
uint16_t HGUARDS[4][6] = {
  // Horizontal guardian 1: y=13, initial x=7, 7<=x<=22, speed=normal
  {70, 23975, 104, 0, 167, 182},
  // Horizontal guardian 2 (unused)
  {255,0,0,0,0,0,0},
  // Horizontal guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Horizontal guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next two bytes are copied to EUGDIR and EUGHGT but are not used.
EUGDIR = 0;
EUGHGT = 0;

// The next 28 bytes are copied to VGUARDS and define the vertical guardians.
uint8_t VGUARDS[4][7] = {
  // Vertical guardian 1: x=24, initial y=48, 40<=y<=103, initial y-increment=1
  {7, 0, 48, 24, 1, 40, 103},
  // Vertical guardian 2 (unused)
  {255,0,0,0,0,0,0},
  // Vertical guardian 3 (unused)
  {0,0,0,0,0,0,0},
  // Vertical guardian 4 (unused)
  {0,0,0,0,0,0,0},
};

// The next 7 bytes are unused.
//   DEFB 0,0,0,0,0,0,0;

// The next 256 bytes are copied to GGDATA and define the guardian graphics.
uint8_t GGDATA[256] = {
  0,0,0,0,0,0,3,192, 12,48,16,8,32,4,64,2,
  128,1,64,2,32,4,208,11, 44,52,75,210,18,72,2,64,
  0,0,0,0,0,0,3,192, 12,48,16,8,32,4,64,2,
  248,31,87,234,43,212,18,72, 12,48,3,192,0,0,0,0,
  4,32,4,32,18,72,75,210, 44,52,147,201,167,229,70,98,
  134,97,71,226,35,196,16,8, 12,48,3,192,0,0,0,0,
  0,0,0,0,0,0,3,192, 12,48,18,72,42,84,95,250,
  246,127,71,226,35,196,16,8, 12,48,3,192,0,0,0,0,
  18,0,12,0,30,0,191,64, 115,128,115,128,191,64,94,128,
  76,128,82,128,127,128,12,0, 97,128,146,192,178,64,97,128,
  3,0,7,128,7,128,28,224, 59,112,59,112,28,224,23,160,
  23,160,19,32,31,224,3,0, 24,96,36,144,52,176,24,96,
  1,224,1,224,1,32,14,220, 13,236,13,236,14,220,5,40,
  5,232,5,232,7,248,0,192, 6,24,13,36,9,52,6,24,
  0,120,0,72,0,48,3,123, 2,253,2,253,3,123,1,50,
  1,74,1,122,1,254,0,48, 1,134,2,205,2,73,1,134,
}
