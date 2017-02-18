// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "headers.h"

#include "conveyor.h"
#include "guardian_horizontal.h"
#include "guardian_vertical.h"
#include "item.h"
#include "portal.h"
#include "willy.h"

// Setup all game objects
// Cavern cavern;
GuardianHorizontal HGUARDS[4];
GuardianVertical VGUARDS[4];
Item ITEMS[5];
Portal portal;
Willy willy;

// Cavern name
//
// The cavern name is copied here and then used by the routine at STARTGAME.
char *CAVERNNAME; // IMPORTANT: length is always 32.

// Cavern tiles
//
// The cavern tiles are copied here by the routine at STARTGAME and then used to
// draw the cavern by the routine at DRAWSHEET.
//
// The extra tile at EXTRA behaves like a floor tile, and is used as such in The
// Endorian Forest, Attack of the Mutant Telephones, Ore Refinery, Skylab
// Landing Bay and The Bank. It is also used in The Menagerie as spider silk,
// and in Miner Willy meets the Kong Beast and Return of the Alien Kong Beast as
// a switch.
Tile BACKGROUND;       // Background tile (also used by the routines at MOVEWILLY, CRUMBLE, LIGHTBEAM, EUGENE, KONGBEAST and WILLYATTR)
Tile FLOOR;            // Floor tile (also used by the routine at LIGHTBEAM)
Tile CRUMBLING;        // Crumbling floor tile (also used by the routine at MOVEWILLY)
Tile WALL;             // Wall tile (also used by the routines at MOVEWILLY, MOVEWILLY2 and LIGHTBEAM)
ConveyorTile CONVEYOR; // Conveyor tile (also used by the routine at MOVEWILLY2)
Tile NASTY1;           // Nasty tile 1 (also used by the routines at MOVEWILLY and WILLYATTR)
Tile NASTY2;           // Nasty tile 2 (also used by the routines at MOVEWILLY and WILLYATTR)
Tile EXTRA;            // Extra tile (also used by the routine at CHKSWITCH)

// Conveyor definition
//
// The conveyor definition is copied here by the routine at STARTGAME.
uint8_t CONVDIR;  // Direction (0=left, 1=right; used by the routines at MOVEWILLY2 and MVCONVEYOR)
uint16_t CONVLOC; // Address of the conveyor's location in the screen buffer at 28672 (used by the routine at MVCONVEYOR)
uint8_t CONVLEN;  // Length (used by the routine at MVCONVEYOR)

// Border colour
//
// Initialised and used by the routine at STARTGAME, and also used by the
// routines at LOOP, MOVEWILLY and KONGBEAST.
uint8_t BORDER;

// Attribute of the last item drawn
//
// Used by the routines at EUGENE and DRAWITEMS. Holds the attribute byte of the
// last item drawn, or 0 if all the items have been collected.
uint8_t ITEMATTR;

// Remaining air supply
//
// Initialised (always to 63 in practice) and used by the routine at STARTGAME,
// updated by the routine at DECAIR, and also used by the routine at NXSHEET.
// Its value ranges from 36 to 63 and is actually the LSB of the display file
// address for the cell at the right end of the air bar. The amount of air to
// draw in this cell is determined by the value of the game clock at CLOCK.
uint8_t AIR;

// Game clock
//
// Initialised by the routine at STARTGAME, updated on every pass through the
// main loop by the routine at DECAIR, and used for timing purposes by the
// routines at MOVEHG, EUGENE and KONGBEAST. Its value (which is always a
// multiple of 4) is also used by the routine at DECAIR to compute the amount of
// air to draw in the cell at the right end of the air bar.
uint8_t CLOCK;

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




// Current cavern number
//
// Initialised by the routine at START, used by the routines at STARTGAME, LOOP,
// DRAWSHEET and DRAWHG, and updated by the routine at NXSHEET.
uint8_t SHEET;

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
