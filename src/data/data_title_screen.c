// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

// Title screen data: attributes and intro message text.

#include "../headers.h"

// Intro message
char *MESSINTRO = ".  .  .  .  .  .  .  .  .  .  . MANIC MINER . . "
        "(c) BUG-BYTE ltd. 1983 . . By Matthew Smith . . . "
        "Q to P = Left & Right . . Bottom row = Jump . . "
        "A to G = Pause . . H to L = Tune On/Off . . . "
        "Guide Miner Willy through 20 lethal caverns"
        " .  .  .  .  .  .  .  .";

// Title screen graphic data
//
// Used by the routines at START and DRAWSHEET.
//
// The attributes for the top third of the title screen are located at CAVERN19
// (in the cavern data for The Final Barrier).
//
// The attributes for the middle third of the title screen are located at
// LOWERATTRS.
uint8_t TITLESCR1[2048] = {
        5, 0, 0, 0, 0, 0, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 129, 129, 128, 0, 0, 0, 0, 0, 0,
        59, 0, 8, 99, 0, 0, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 7, 255, 224,
        3, 0, 0, 84, 0, 255, 0, 0, 7, 224, 0, 0, 15, 223, 220, 0,
        0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 34, 34, 34, 8, 224, 16,
        0, 255, 159, 148, 243, 0, 63, 192, 31, 248, 3, 252, 0, 0, 0, 0,
        0, 36, 66, 66, 36, 68, 0, 0, 0, 0, 119, 119, 119, 0, 255, 0,
        0, 0, 0, 138, 0, 7, 255, 252, 7, 224, 63, 255, 224, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 74, 0, 0, 0, 1, 255, 255, 128, 0, 0, 224, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 1, 185, 128, 48, 255, 255, 7, 192, 255, 255, 15, 255, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 36, 0, 18, 64, 18, 64, 18, 64, 1, 34, 64, 17, 65, 2, 16,
        36, 16, 33, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 33,
        7, 0, 0, 0, 0, 0, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 3, 66, 66, 192, 0, 0, 0, 0, 0, 0,
        22, 0, 0, 0, 0, 0, 200, 0, 0, 0, 0, 0, 1, 240, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 4, 0, 32,
        5, 0, 0, 85, 0, 255, 0, 0, 127, 254, 0, 0, 15, 239, 120, 0,
        0, 0, 0, 0, 0, 0, 0, 129, 129, 0, 119, 119, 119, 9, 16, 16,
        0, 127, 15, 85, 244, 0, 127, 224, 31, 248, 7, 254, 0, 0, 0, 0,
        0, 36, 66, 68, 34, 66, 0, 0, 0, 0, 119, 119, 119, 49, 255, 140,
        0, 0, 0, 82, 0, 1, 255, 254, 7, 224, 127, 255, 128, 0, 0, 15,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 82, 0, 112, 0, 3, 255, 255, 192, 0, 14, 240, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 0, 3, 16, 0, 48, 127, 255, 1, 240, 255, 252, 1, 255, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 36, 48, 33, 81, 48, 36, 49, 32, 32, 66, 16, 52, 33, 3, 18,
        2, 19, 64, 0, 0, 66, 0, 0, 0, 0, 0, 0, 0, 0, 0, 49,
        3, 0, 0, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 7, 36, 36, 224, 0, 0, 0, 0, 0, 0,
        29, 0, 0, 0, 0, 0, 180, 0, 0, 0, 0, 0, 7, 248, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 129, 129, 0, 0, 0, 0, 4, 24, 32,
        5, 0, 0, 148, 0, 208, 0, 0, 127, 254, 0, 0, 31, 255, 151, 128,
        0, 101, 118, 86, 134, 86, 0, 129, 129, 0, 119, 119, 119, 9, 80, 16,
        0, 62, 7, 85, 192, 0, 255, 224, 31, 248, 7, 255, 0, 0, 0, 0,
        0, 34, 66, 68, 36, 66, 0, 0, 0, 0, 119, 119, 119, 50, 255, 76,
        0, 0, 0, 81, 0, 0, 127, 254, 0, 0, 127, 254, 0, 0, 0, 255,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 6, 82, 48, 127, 0, 3, 255, 255, 192, 0, 254, 248, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        15, 0, 0, 0, 0, 0, 63, 255, 7, 224, 255, 240, 0, 63, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        2, 19, 21, 2, 67, 96, 33, 52, 80, 49, 33, 80, 55, 97, 80, 40,
        18, 3, 70, 0, 0, 36, 0, 0, 0, 0, 0, 0, 0, 0, 0, 39,
        1, 0, 0, 0, 0, 0, 224, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 15, 24, 24, 240, 0, 0, 0, 0, 0, 0,
        31, 0, 0, 0, 0, 0, 246, 0, 0, 0, 0, 0, 15, 252, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 129, 129, 0, 0, 0, 0, 4, 0, 32,
        23, 0, 0, 162, 0, 248, 0, 0, 127, 254, 0, 0, 31, 255, 239, 92,
        112, 133, 151, 84, 104, 103, 0, 129, 129, 0, 255, 255, 255, 63, 255, 252,
        0, 20, 2, 84, 192, 1, 255, 240, 31, 248, 15, 255, 128, 0, 0, 0,
        0, 66, 68, 34, 36, 34, 0, 0, 0, 0, 119, 119, 119, 52, 255, 44,
        0, 0, 0, 149, 0, 0, 31, 255, 0, 0, 255, 248, 0, 0, 15, 255,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 15, 81, 248, 127, 240, 7, 255, 255, 224, 15, 254, 248, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        15, 0, 0, 0, 0, 0, 30, 127, 3, 240, 255, 128, 0, 3, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        33, 48, 84, 48, 103, 40, 145, 32, 52, 81, 144, 36, 49, 84, 97, 32,
        52, 81, 144, 0, 0, 131, 0, 0, 0, 0, 0, 0, 0, 0, 0, 115,
        6, 0, 0, 0, 0, 0, 228, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 31, 24, 24, 248, 0, 0, 0, 0, 0, 0,
        5, 0, 7, 129, 192, 48, 200, 0, 0, 0, 0, 0, 30, 59, 176, 0,
        0, 0, 0, 0, 0, 0, 0, 129, 129, 0, 0, 0, 0, 4, 0, 32,
        29, 0, 0, 170, 0, 192, 0, 0, 63, 252, 0, 0, 14, 127, 238, 222,
        248, 102, 102, 102, 102, 102, 0, 129, 129, 0, 255, 255, 255, 127, 255, 254,
        0, 0, 0, 146, 128, 1, 255, 240, 15, 240, 15, 255, 128, 0, 0, 0,
        0, 66, 36, 66, 66, 68, 0, 0, 0, 0, 119, 119, 119, 63, 255, 252,
        0, 0, 0, 165, 0, 0, 7, 255, 3, 192, 255, 224, 0, 0, 63, 255,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 127, 137, 252, 127, 255, 7, 255, 255, 224, 255, 254, 252, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        63, 0, 0, 0, 0, 0, 0, 31, 1, 128, 254, 0, 0, 0, 193, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        116, 17, 87, 145, 81, 33, 2, 70, 25, 18, 2, 73, 18, 6, 116, 33,
        52, 97, 33, 0, 0, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 67,
        11, 0, 0, 0, 0, 0, 208, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 63, 36, 36, 252, 0, 0, 0, 0, 0, 0,
        3, 0, 2, 195, 160, 0, 208, 0, 0, 0, 0, 0, 29, 215, 216, 0,
        0, 0, 0, 0, 0, 0, 0, 129, 129, 0, 0, 0, 0, 4, 0, 32,
        31, 0, 0, 170, 0, 128, 1, 128, 63, 252, 1, 128, 15, 191, 238, 222,
        248, 102, 102, 102, 102, 102, 0, 129, 129, 0, 119, 119, 119, 255, 255, 255,
        0, 0, 0, 138, 0, 3, 255, 248, 15, 240, 31, 255, 192, 0, 0, 0,
        0, 36, 66, 36, 36, 36, 0, 0, 0, 0, 119, 119, 119, 48, 255, 12,
        0, 0, 0, 169, 0, 0, 1, 254, 31, 248, 127, 128, 0, 0, 255, 255,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 255, 170, 252, 255, 255, 199, 255, 255, 227, 255, 255, 254, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        255, 0, 0, 0, 0, 0, 0, 15, 15, 192, 240, 0, 0, 0, 62, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        248, 16, 47, 70, 33, 113, 21, 70, 49, 38, 21, 66, 19, 21, 3, 36,
        52, 81, 81, 0, 0, 81, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36,
        5, 0, 0, 0, 0, 0, 180, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 127, 66, 66, 254, 0, 0, 0, 0, 0, 0,
        6, 0, 1, 83, 192, 0, 184, 0, 0, 0, 0, 0, 11, 239, 232, 0,
        0, 0, 0, 0, 0, 0, 0, 129, 129, 0, 0, 0, 0, 0, 0, 32,
        10, 0, 0, 170, 0, 0, 7, 128, 63, 252, 1, 224, 7, 223, 207, 111,
        120, 102, 102, 102, 102, 102, 0, 255, 255, 0, 119, 119, 119, 255, 255, 255,
        0, 0, 0, 170, 0, 3, 255, 248, 15, 240, 31, 255, 192, 0, 0, 0,
        0, 34, 66, 68, 34, 66, 0, 0, 0, 0, 119, 119, 119, 48, 60, 12,
        0, 0, 0, 170, 0, 0, 0, 124, 127, 254, 62, 0, 0, 15, 255, 255,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 15, 255, 170, 254, 255, 255, 207, 255, 255, 243, 255, 255, 254, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        255, 0, 0, 0, 0, 0, 0, 7, 3, 240, 224, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 32, 49, 32, 51, 32, 49, 32, 2, 16, 66, 16, 18, 64, 16, 66,
        64, 16, 66, 0, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 64,
        42, 0, 0, 0, 0, 0, 248, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 255, 129, 129, 255, 0, 0, 0, 0, 0, 0,
        3, 0, 0, 163, 0, 0, 100, 0, 0, 0, 0, 0, 7, 223, 236, 0,
        0, 0, 0, 0, 0, 0, 0, 255, 255, 0, 0, 0, 0, 4, 0, 32,
        7, 0, 0, 162, 0, 0, 31, 192, 63, 252, 3, 248, 3, 143, 135, 191,
        240, 102, 102, 102, 102, 102, 0, 255, 255, 0, 119, 119, 119, 255, 255, 255,
        0, 0, 0, 170, 0, 7, 255, 252, 15, 240, 63, 255, 224, 0, 0, 0,
        126, 166, 246, 166, 246, 166, 0, 0, 0, 0, 119, 119, 119, 48, 0, 12,
        0, 0, 0, 138, 0, 0, 0, 24, 255, 255, 24, 0, 0, 255, 255, 255,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 63, 255, 74, 255, 255, 255, 207, 255, 255, 243, 255, 255, 255, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        255, 0, 0, 0, 0, 0, 0, 1, 1, 192, 128, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        252, 189, 254, 188, 253, 190, 203, 223, 235, 207, 205, 239, 207, 191, 254, 205,
        188, 206, 189, 0, 0, 219, 0, 0, 0, 0, 0, 0, 0, 0, 0, 189,
};

uint8_t TITLESCR2[2048] = {
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 130, 12, 63, 134, 30, 51, 128, 0, 0, 34, 49, 140, 60, 96,
        12, 96, 96, 0, 0, 139, 162, 251, 192, 139, 160, 136, 128, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 252, 252, 254, 124, 124, 0, 254, 198, 254, 254,
        252, 0, 254, 124, 0, 124, 254, 16, 252, 254, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 7, 199,
        193, 7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170,
        170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 65, 0, 12, 96, 198, 14, 49, 129, 0, 0, 32, 49, 140, 28, 96,
        76, 48, 112, 0, 0, 217, 50, 130, 32, 137, 32, 133, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 254, 254, 254, 254, 254, 0, 254, 230, 254, 254,
        254, 0, 254, 254, 0, 254, 254, 56, 254, 254, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 7, 199,
        193, 7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68,
        68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68,
        0, 3, 255, 30, 4, 14, 15, 120, 58, 0, 7, 248, 59, 220, 30, 255,
        159, 240, 0, 0, 0, 1, 255, 136, 243, 206, 137, 255, 128, 0, 0, 0,
        0, 130, 0, 12, 64, 198, 6, 48, 198, 0, 0, 32, 49, 140, 12, 97,
        140, 24, 16, 0, 0, 169, 42, 227, 192, 169, 32, 130, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 198, 198, 192, 194, 194, 0, 192, 246, 48, 192,
        198, 0, 48, 198, 0, 194, 48, 108, 198, 48, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 7, 199,
        193, 7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
        17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
        0, 6, 7, 60, 14, 7, 6, 48, 198, 0, 8, 56, 113, 142, 12, 97,
        140, 56, 0, 0, 0, 2, 4, 20, 138, 36, 202, 0, 0, 0, 0, 0,
        0, 140, 0, 30, 225, 239, 2, 120, 56, 0, 0, 112, 123, 222, 4, 255,
        158, 14, 32, 0, 0, 137, 38, 130, 128, 169, 32, 130, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 254, 254, 240, 248, 248, 0, 240, 246, 48, 240,
        254, 0, 48, 198, 0, 248, 48, 198, 254, 48, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 7, 199,
        193, 7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 9, 139, 108, 14, 7, 134, 49, 131, 0, 16, 44, 113, 143, 12, 96,
        76, 24, 0, 0, 0, 1, 196, 34, 243, 196, 170, 96, 0, 0, 0, 0,
        0, 112, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 7, 192, 0, 0, 139, 162, 250, 96, 83, 190, 250, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 252, 252, 240, 62, 62, 0, 240, 222, 48, 240,
        252, 0, 48, 198, 0, 62, 48, 198, 252, 48, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 7, 199,
        193, 7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 9, 147, 204, 27, 6, 198, 51, 128, 0, 16, 44, 177, 141, 140, 98,
        12, 24, 0, 0, 0, 0, 36, 62, 162, 132, 154, 32, 0, 0, 0, 0,
        255, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 240, 31, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 192, 216, 192, 134, 134, 0, 192, 222, 48, 192,
        216, 0, 48, 198, 0, 134, 48, 254, 216, 48, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 7, 199,
        193, 7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 6, 35, 140, 19, 6, 102, 51, 0, 0, 19, 38, 177, 140, 204, 126,
        15, 240, 0, 0, 0, 255, 196, 34, 154, 110, 137, 192, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 192, 204, 254, 254, 254, 0, 254, 206, 48, 254,
        204, 0, 48, 254, 0, 254, 48, 254, 204, 48, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 7, 199,
        193, 7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 67, 12, 49, 134, 54, 51, 0, 0, 12, 39, 49, 140, 108, 98,
        12, 192, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 192, 198, 254, 124, 124, 0, 254, 198, 48, 254,
        198, 0, 48, 124, 0, 124, 48, 198, 198, 48, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 7, 199,
        193, 7, 199, 199, 193, 7, 199, 193, 7, 199, 199, 193, 7, 199, 193, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

// Attribute data for the bottom two-thirds of the title screen
//
// Used by the routine at START. The graphic data for the middle third of the
// title screen is located at TITLESCR2.
uint8_t LOWERATTRS[512] = {
        22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
        22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22,
        23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
        23, 23, 23, 23, 23, 16, 16, 16, 16, 16, 16, 16, 16, 23, 23, 23,
        23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23,
        23, 23, 23, 23, 23, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23,
        19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
        19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19,
        23, 23, 23, 23, 23, 23, 16, 16, 16, 16, 16, 16, 22, 22, 22, 22,
        22, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
        56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
        56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
        56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
        56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56, 56,
        48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
        48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48,
        87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 103, 103, 103, 103, 103, 103,
        103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103,
        70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,
        70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,
        70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,
        70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,
        70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,
        70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70,
        69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
        69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
        69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
        69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};