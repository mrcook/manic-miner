// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "headers.h"

#include "cavern.h"
#include "conveyor.h"
#include "guardian_horizontal.h"
#include "guardian_vertical.h"
#include "item.h"
#include "portal.h"
#include "willy.h"

// Setup all game objects
Cavern cavern;
GuardianHorizontal HGUARDS[4];
GuardianVertical VGUARDS[4];
Willy willy;

// Attribute of the last item drawn
//
// Used by the routines at EUGENE and DRAWITEMS. Holds the attribute byte of the
// last item drawn, or 0 if all the items have been collected.
uint8_t ITEMATTR;

// Eugene's direction or the Kong Beast's status
//
// Initialised by the routine at STARTGAME, and used by the routines at EUGENE
// (to hold Eugene's direction: 0=down, 1=up) and KONGBEAST (to hold the Kong
// Beast's status: 0=on the ledge, 1=falling, 2=dead).
uint8_t EUGDIR;

// Eugene's or the Kong Beast's pixel y-coordinate
//
// Initialised by the routine at STARTGAME, and used by the routines at START
// (to hold the index into the message scrolled across the screen after the
// theme tune has finished playing), ENDGAM (to hold the distance of the boot
// from the top of the screen as it descends onto Willy), EUGENE (to hold
// Eugene's pixel y-coordinate) and KONGBEAST (to hold the Kong Beast's pixel
// y-coordinate).
uint8_t EUGHGT;

// Kong Beast Guardian graphic data
uint8_t GGDATA[256];

// IMPORTANT: not initialized anywhere, so let's do that here -MRC-
// Swordfish graphic data
uint8_t SWORDFISH[32];

// 'AIR'
//
// Used by the routine at STARTGAME.
char MESSAIR[] = "AIR";

// Unused
// DEFM "0000"

// High score
//
// Used by the routine at LOOP and updated by the routine at ENDGAM.
char HGHSCOR[] = "000000";

// Score
//
// Initialised by the routine at STARTGAME, and used by the routines at LOOP,
// ENDGAM, NXSHEET and INCSCORE.
char SCORE[] = "0000";             // Overflow digits (these may be updated, but are never printed)
char SCORBUF[] = "000000";

// 'High Score 000000   Score 000000'
//
// Used by the routine at STARTGAME.
char MESSHSSC[] = "High Score 000000   Score 000000";

// ----------------------------
//  IMPORTANT: custom scoring
int current_score;
int highscore;
// ----------------------------

// 'Game'
//
// Used by the routine at ENDGAM.
char MESSG[] = "Game";

// 'Over'
//
// Used by the routine at ENDGAM.
char MESSO[] = "Over";

// Screen flash counter
//
// Initialised by the routine at START, and used by the routines at LOOP and INCSCORE.
uint8_t FLASH;

// Kempston joystick indicator
//
// Initialised by the routine at START, and used by the routines at LOOP,
// MOVEWILLY2 and CHECKENTER. Holds 1 if a joystick is present, 0 otherwise.
uint8_t KEMP;

// Game mode indicator
//
// Initialised by the routine at START, and used by the routines at STARTGAME,
// LOOP and NXSHEET. Holds 0 when a game is in progress, or a value from 1 to 64
// when in demo mode.
uint8_t DEMO;

// In-game music note index
//
// Initialised by the routine at START, and used and updated by the routine at LOOP.
uint8_t NOTEINDEX;

// Music flags
//
// The keypress flag in bit 0 is initialised by the routine at START; bits 0 and
// 1 are checked and updated by the routine at LOOP.
//
// +--------+-----------------------------------------------------------------+
// | Bit(s) | Meaning                                                         |
// +--------+-----------------------------------------------------------------+
// | 0      | Keypress flag (set=H-ENTER being pressed, reset=no key pressed) |
// | 1      | In-game music flag (set=music off, reset=music on)              |
// | 2-7    | Unused                                                          |
// +--------+-----------------------------------------------------------------+
uint8_t MUSICFLAGS;

// 6031769 key counter
//
// Used by the routines at LOOP and NXSHEET.
uint8_t CHEAT;
