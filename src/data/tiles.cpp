// Manic Miner C/C++ port Copyright (c) 2016-2021 Michael R. Cook
// Manic Miner Copyright (c) 1983 Bug-Byte Ltd.

// Tile GFX: walls, floor, boot, Eugene, etc.

#include "../headers.h"

// Swordfish graphic that appears in The Final Barrier when the game is completed.
uint8_t SWORDFISH[32] = {
        2, 160, 5, 67, 31, 228, 115, 255,
        242, 248, 31, 63, 255, 228, 63, 195,
        0, 0, 1, 0, 57, 252, 111, 2,
        81, 1, 127, 254, 57, 252, 1, 0,
};

// Plinth graphic that appears on the Game Over screen.
uint8_t PLINTH[32] = {
        255, 255, 114, 78, 138, 81, 170, 85,
        74, 82, 18, 72, 34, 68, 42, 84,
        42, 84, 42, 84, 42, 84, 42, 84,
        42, 84, 42, 84, 42, 84, 42, 84,
};

// Boot graphic that appears on the Game Over screen (see LOOPFT).
// It also appears at the bottom of the screen next to the
// remaining lives when cheat mode is activated (see LOOP_1).
uint8_t BOOT[32] = {
        42, 192, 53, 64, 63, 192, 9, 0,
        9, 0, 31, 128, 16, 128, 16, 128,
        17, 128, 34, 64, 32, 184, 89, 36,
        68, 66, 68, 2, 68, 2, 255, 255,
};

// Eugene graphic data
uint8_t EUGENEG[32] = {
        3, 192, 15, 240, 31, 248, 31, 248,
        49, 140, 14, 112, 111, 246, 174, 117,
        177, 141, 159, 249, 155, 217, 140, 49,
        71, 226, 2, 64, 2, 64, 14, 112,
};


//
// Cavern Tiles data (Caverns 0 to 19)
//

// MRC:
// Having IDs based only on individual caverns produces a fair bit of
// duplication, so it would be better to have tile "sets" where a
// particular GFX has a unique ID. When generating caverns the array is
// searched for that specific ID.
// As a bonus, this could be a nice tool for generating more "advanced"
// caverns: e.g. multiple floor/wall/conveyor styles in a single cavern.

// Note: with these new unique IDs, these separate arrays can be joined
// into one. The cavern map data will also need updating

// Note: these ID are also used as colour attributes, so we'll need to
// include that data in the new arrays.
uint8_t Data_tileBackgrounds[20][9] = {
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {8,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {16, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {8,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
        {36, 0, 0, 0, 0, 0, 0, 0, 0},
        {0,  0, 0, 0, 0, 0, 0, 0, 0},
};

// Possible new ID: 100 - 119
uint8_t Data_tileFloors[20][9] = {
        {66, 255, 255, 219, 110, 197, 64,  0,   0},
        {75, 255, 255, 219, 110, 197, 64,  0,   0},
        {69, 255, 255, 102, 153, 102, 153, 255, 0},
        {70, 255, 255, 219, 110, 197, 64,  0,   0},
        {21, 255, 255, 219, 110, 197, 64,  0,   0},
        {68, 255, 255, 153, 153, 255, 153, 102, 0},
        {70, 255, 255, 219, 110, 197, 64,  0,   0},
        {66, 255, 255, 219, 110, 197, 64,  0,   0},
        {6,  255, 255, 219, 110, 197, 64,  0,   0},
        {68, 124, 255, 239, 30,  12,  8,   8,   8},
        {65, 255, 255, 219, 110, 197, 64,  0,   0},
        {67, 255, 255, 219, 110, 197, 64,  0,   0},
        {5,  255, 255, 17,  34,  68,  136, 255, 255},
        {76, 255, 255, 98,  100, 120, 112, 96,  96},
        {65, 255, 255, 221, 119, 170, 85,  34,  0},
        {66, 255, 255, 219, 110, 197, 64,  0,   0},
        {4,  255, 255, 219, 110, 197, 64,  0,   0},
        {66, 255, 255, 219, 110, 197, 64,  0,   0},
        {32, 255, 255, 219, 110, 197, 64,  0,   0},
        {66, 255, 255, 219, 110, 197, 64,  0,   0},
};

// Possible new ID: 120 - 139
uint8_t Data_tileCrumblingFloors[20][9] = {
        {2,  255, 219, 165, 36,  82,  32,  8,   0},
        {11, 255, 219, 165, 36,  82,  32,  8,   0},
        {5,  255, 255, 102, 153, 66,  24,  234, 0},
        {6,  255, 219, 165, 36,  82,  32,  8,   0},
        {20, 255, 219, 165, 36,  82,  32,  8,   0},
        {4,  255, 219, 165, 36,  82,  32,  8,   0}, // (unused)
        {2,  255, 170, 85,  170, 85,  170, 85,  170},
        {2,  255, 219, 165, 36,  82,  32,  8,   0}, // (unused)
        {66, 255, 219, 165, 36,  82,  32,  8,   0}, // (unused)
        {2,  252, 255, 135, 12,  8,   8,   8,   0},
        {1,  255, 219, 165, 36,  82,  32,  8,   0},
        {3,  255, 219, 165, 36,  82,  32,  8,   0},
        {66, 255, 219, 165, 36,  82,  32,  8,   0}, // (unused)
        {2,  252, 255, 255, 135, 255, 8,   8,   0}, // (unused)
        {1,  255, 219, 165, 36,  82,  32,  8,   0},
        {2,  255, 219, 165, 36,  82,  32,  8,   0},
        {68, 255, 170, 85,  170, 85,  170, 85,  170},
        {2,  255, 219, 165, 36,  82,  32,  8,   0}, // (unused)
        {2,  255, 219, 165, 36,  82,  32,  8,   0}, // (unused)
        {2,  255, 219, 165, 36,  82,  32,  8,   0},
};

// Possible new ID: 140 - 159
uint8_t Data_tileWalls[20][9] = {
        {22,  34,  255, 136, 255, 34,  255, 136, 255},
        {22,  34,  255, 136, 255, 34,  255, 136, 255},
        {13,  129, 195, 165, 153, 153, 165, 195, 129},
        {41,  34,  255, 136, 255, 34,  255, 136, 255},
        {46,  34,  255, 136, 255, 34,  255, 136, 255},
        {22,  255, 153, 255, 102, 255, 153, 255, 102},
        {77,  34,  255, 136, 255, 34,  255, 136, 255},
        {114, 34,  255, 136, 255, 34,  255, 136, 255},
        {22,  90,  90,  90,  90,  90,  90,  90,  90},
        {22,  74,  74,  74,  82,  84,  74,  42,  42},
        {14,  170, 85,  170, 85,  170, 85,  170, 85},
        {101, 34,  255, 136, 255, 34,  255, 136, 255},
        {22,  90,  90,  90,  90,  90,  90,  90,  90},
        {104, 1,   130, 196, 232, 224, 216, 188, 126},
        {14,  170, 85,  170, 85,  170, 85,  170, 85},
        {101, 73,  249, 79,  73,  255, 72,  120, 207},
        {22,  255, 153, 187, 255, 255, 153, 187, 255},
        {22,  255, 129, 129, 255, 255, 129, 129, 255},
        {22,  34,  255, 136, 255, 34,  255, 136, 255},
        {38,  34,  255, 136, 255, 34,  255, 136, 255},
};

// Possible new ID: 160 - 179
uint8_t Data_tileConveyorBelts[20][9] = {
        {4,  240, 102, 240, 102, 0,   153, 255, 0},
        {14, 240, 102, 240, 102, 0,   153, 255, 0},
        {2,  240, 170, 240, 102, 102, 0,   0,   0},
        {3,  240, 102, 240, 102, 0,   153, 255, 0},
        {86, 252, 102, 252, 102, 0,   0,   0,   0},
        {5,  240, 102, 240, 102, 0,   153, 255, 0},
        {4,  244, 102, 244, 0,   0,   0,   0,   0},
        {68, 240, 102, 240, 170, 0,   0,   0,   0},
        {4,  240, 102, 240, 102, 0,   0,   0,   0},
        {67, 240, 102, 240, 102, 0,   0,   0,   0}, // (unused)
        {6,  254, 102, 254, 0,   0,   0,   0,   0},
        {70, 240, 102, 240, 170, 0,   0,   0,   0},
        {4,  240, 102, 240, 102, 0,   0,   0,   0},
        {75, 240, 102, 240, 102, 0,   0,   0,   0},
        {69, 254, 102, 254, 0,   0,   0,   0,   0},
        {70, 240, 102, 240, 170, 0,   0,   0,   0},
        {32, 240, 102, 240, 102, 0,   0,   0,   0},
        {4,  240, 102, 240, 102, 0,   153, 255, 0}, // (unused)
        {38, 240, 102, 240, 102, 0,   153, 255, 0},
        {5,  240, 102, 240, 102, 0,   153, 255, 0},
};

// Possible new ID: 180 - 199
uint8_t Data_tileNasties1[20][9] = {
        {68, 68, 40, 148, 81, 53,  214, 88,  16},
        {12, 68, 40, 148, 81, 53,  214, 88,  16},
        {6,  68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {4,  68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {22, 68, 40, 148, 81, 53,  214, 88,  16},
        {67, 68, 40, 148, 81, 53,  214, 88,  16},
        {21, 68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {4,  68, 40, 148, 81, 53,  214, 88,  16},
        {68, 68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {69, 68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {70, 16, 16, 214, 56, 214, 56,  84,  146},
        {4,  68, 40, 148, 81, 53,  214, 88,  16},
        {68, 68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {0,  68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {70, 16, 16, 214, 56, 214, 56,  84,  146},
        {4,  68, 68, 68,  68, 102, 238, 238, 255},
        {6,  68, 40, 148, 81, 53,  214, 88,  16},
        {68, 68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {68, 68, 40, 148, 81, 53,  214, 88,  16}, // (unused)
        {68, 16, 16, 214, 56, 214, 56,  84,  146},
};

// Possible new ID: 200 - 219
uint8_t Data_tileNasties2[20][9] = {
        {5,  255, 254, 126, 124, 76,  76,  8,   8},
        {13, 255, 254, 94,  108, 76,  76,  8,   8},
        {67, 16,  214, 56,  214, 56,  68,  198, 40},
        {5,  16,  16,  16,  84,  56,  214, 56,  84},
        {19, 126, 60,  28,  24,  24,  8,   8,   8},
        {6,  60,  24,  189, 231, 231, 189, 24,  60},
        {22, 165, 66,  60,  219, 60,  126, 165, 36},
        {5,  126, 60,  28,  24,  24,  8,   8,   8},
        {5,  126, 60,  28,  24,  24,  8,   8,   8}, // (unused)
        {4,  72,  178, 93,  18,  112, 174, 169, 71},
        {66, 16,  16,  16,  16,  16,  16,  16,  16},
        {5,  126, 60,  28,  24,  24,  8,   8,   8},
        {69, 126, 60,  28,  24,  24,  8,   8,   8}, // (unused)
        {0,  72,  178, 93,  18,  112, 174, 169, 71}, // (unused)
        {66, 16,  16,  16,  16,  16,  16,  16,  16},
        {5,  126, 60,  28,  24,  24,  8,   8,   8}, // (unused)
        {33, 66,  215, 254, 101, 166, 125, 238, 215},
        {5,  126, 60,  28,  24,  24,  8,   8,   8}, // (unused)
        {5,  126, 60,  28,  24,  24,  8,   8,   8}, // (unused)
        {10, 126, 60,  28,  24,  24,  8,   8,   8}, // (unused)
};

// Possible new ID: 220 - 239
uint8_t Data_tileExtras[20][9] = {
        {0,  0,   0,   0,   0,   0,   0,   0,   0},
        {0,  0,   0,   0,   0,   0,   0,   0,   0},
        {3,  16,  16,  16,  16,  16,  16,  16,  16},
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
        {6,  255, 129, 129, 66,  60,  16,  96,  96},
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
        {5,  255, 255, 202, 101, 146, 40,  130, 0},
        {69, 255, 255, 255, 255, 170, 0,   0,   0},
        {6,  255, 129, 129, 66,  60,  16,  96,  96},
        {6,  255, 129, 129, 129, 129, 129, 129, 129},
        {12, 255, 255, 70,  38,  30,  14,  6,   6},
        {6,  255, 255, 24,  24,  24,  24,  24,  24},
        {6,  255, 129, 129, 66,  60,  16,  96,  96}, // (unused)
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
        {0,  0,   0,   0,   0,   0,   0,   0,   0}, // (unused)
};
