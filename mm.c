// Manic Miner disassembly
// http://skoolkit.ca/
//
// Copyright 1983 Bug-Byte Ltd (Manic Miner)
// Copyright 2010, 2012-2016 Richard Dymond (this disassembly)

#include "headers.h"
#include "externs.h"

#include "character_set.c"
#include "helpers.c"
#include "globals.c"
#include "data.c"


// ---------------------------SPECCY EMULATOR--------------------------------

// Memory Layout
//   | ROM | Screen File | Attributes  | Printer Buf | System vars | MD, CHANS, PROGS, etc.
//   ^     ^             ^             ^             ^             ^
//   0   16384         22528         23296         23552         23734
//             (6144)         (768)         (256)         (182)
//
// Keyboard
// IN 65278 reads the half row CAPS SHIFT to V
// IN 65022 reads the half row A to G
// IN 64510 reads the half row Q to T
// IN 63486 reads the half row 1 to 5
// IN 61438 reads the half row O to 6
// IN 57342 reads the half row P to 7
// IN 49150 reads the half row ENTER to H
// IN 32766 reads the half row SPACE to B

// Initialize a 64K block of memory, for general use
// Holds memory for Screen, Attributes, input, sound, etc.
// The emulator may tap into these for IO.
uint8_t MEM[1024 * 64] = {};

uint8_t regA;
uint8_t regB;
uint8_t regC;
uint8_t regD;
uint8_t regE;
uint8_t regH;
uint8_t regL;

uint16_t regBC;
uint16_t regDE;
uint16_t regHL;

// ==========================================================================


// ORG 32765
// JP BEGIN


// ------------------------------------------------------
//   NOTE: variable declarations moved to the globals.c
//   NOTE: Willy sprites and buff addr moved data.c
// ------------------------------------------------------


// The game has just loaded
// BEGIN:
//   DI                      // Disable interrupts
//   LD SP,40190             // Place the stack somewhere safe (near the end of the source code remnants at SOURCE)
//   JP START                // Display the title screen and play the theme tune


// -----------------------------------------------------
//   NOTE: variable declarations moved to the globals.c
//   NOTE: Cheat and Music data moved data.c
// -----------------------------------------------------


// IMPORTANT: this function closes before the "source code remnants" and "cavern data" starts -MRC-
int main() {

// Display the title screen and play the theme tune
//
// Used by the routines at BEGIN, LOOP and ENDGAM.
//
// The first thing this routine does is initialise some game status buffer
// variables in preparation for the next game.
START:
  // XOR A                   // A=0
  // LD (SHEET),A            // Initialise the current cavern number at SHEET
  SHEET = 0;

  // LD (KEMP),A             // Initialise the Kempston joystick indicator at KEMP
  KEMP = 0;

  // LD (DEMO),A             // Initialise the game mode indicator at DEMO
  DEMO = 0;

  // LD (NOTEINDEX),A        // Initialise the in-game music note index at NOTEINDEX
  NOTEINDEX = 0;

  // LD (FLASH),A            // Initialise the screen flash counter at FLASH
  FLASH = 0;

  // LD A,2                  // Initialise the number of lives remaining at NOMEN
  // LD (NOMEN),A
  NOMEN = 2;

  // LD HL,MUSICFLAGS        // Initialise the keypress flag in bit 0 at MUSICFLAGS
  // SET 0,(HL)
  MUSICFLAGS |= 1 << 0;

// Next, prepare the screen.

  // Clear the entire display file
  // LD HL,16384
  // LD DE,16385
  // LD BC,6143
  // LD (HL),0
  // LDIR
  for (int i = 0; i <= 6143; i++) {
    MEM[16384 + i] = 0;
  }

  // Copy the graphic data at TITLESCR1 to the top two-thirds of the display file
  // LD HL,TITLESCR1
  // LD DE,16384
  // LD BC,4096
  // LDIR
  for (int i = 0; i < 4096; i++) {
    MEM[16384 + i] = TITLESCR1[i];
  }

  // LD HL,18493             // Draw Willy at (9,29)
  // LD DE,WILLYR2
  // LD C,0
  // CALL DRWFIX
  DRWFIX(&WILLYR2, 18493, 0);

  // LD HL,CAVERN19          // Copy the attribute bytes from CAVERN19 to the top
  // LD DE,22528             // third of the attribute file
  // LD BC,256
  // LDIR
  for (int i = 0; i < 256; i++) {
    MEM[22528 + i] = CAVERN19[i];
  }

  // Copy the attribute bytes from LOWERATTRS to the bottom two-thirds of the attribute file
  // LD HL,LOWERATTRS
  // LD BC,512
  // LDIR
  for (int i = 0; i < 512; i++) {
    MEM[22528 + 256 + i] = LOWERATTRS[i];
  }


  // Now check whether there is a joystick connected.
  //   LD BC,31                // B=0, C=31 (joystick port)
  //   DI                      // Disable interrupts (which are already disabled)
  //   XOR A                   // A=0
  // START_0:
  //   IN E,(C)                // Combine 256 readings of the joystick port in A; if
  //   OR E                    // no joystick is connected, some of these readings
  //   DJNZ START_0            // will have bit 5 set
  //   AND 32                  // Is a joystick connected (bit 5 reset)?
  //   JR NZ,START_1           // Jump if not
  //   LD A,1                  // Set the Kempston joystick indicator at KEMP to 1
  //   LD (KEMP),A
  KEMP = 0; // IMPORTANT: no joystick support just yet! -MRC-

  // And finally, play the theme tune and check for keypresses.

  // START_1: // label only used in the KEMP detection routine above.
  // LD IY,THEMETUNE         // Point IY at the theme tune data at THEMETUNE
  // CALL PLAYTUNE           // Play the theme tune
  // JP NZ,STARTGAME         // Start the game if ENTER or the fire button was pressed
  // IMPORTANT: there is only one THEMETUNE, so PLAYTUNE can just access it directly -MRC-
  if ( PLAYTUNE() ) {
    goto STARTGAME;
  }

  // Initialise the game status buffer variable at EUGHGT;
  // this will be used as an index for the message scrolled across the screen
  // XOR A
  // LD (EUGHGT),A
  EUGHGT = 0;

  for (EUGHGT = 0; EUGHGT < 224; EUGHGT++) {
    // LD A,(EUGHGT)           // Pick up the message index from EUGHGT

    // Point IX at the corresponding location in the message at MESSINTRO
    // Print 32 characters of the message at (19,0)
    // LD IX,MESSINTRO
    // LD IXl,A
    // LD DE,20576
    // LD C,32
    // CALL PMESS
    PMESS(&MESSINTRO[EUGHGT], 20576, 32);

    // Pick up the message index from EUGHGT
    // Keep only bits 1 and 2, and move them into bits 6 and 7, so that A holds 0, 64, 128 or 192;
    // this value determines the animation frame to use for Willy
    // LD A,(EUGHGT)
    // AND 6
    // RRCA
    // RRCA
    // RRCA
    uint8_t anim_frame = (EUGHGT & 6) >> 3;

    // Point DE at the graphic data for Willy's sprite (MANDAT+A)
    // LD E,A
    // LD D,130
    uint8_t *mandat_sprite_ptr = &MANDAT[anim_frame];

    // Draw Willy at (9,29)
    // LD HL,18493
    // LD C,0
    // CALL DRWFIX
    DRWFIX(mandat_sprite_ptr, 18493, 0);

    // Pause for about 0.1s
    //   LD BC,100
    // START_3:
    //   DJNZ START_3
    //   DEC C
    //   JR NZ,START_3
    millisleep(100);

    // LD BC,49150             // Read keys H-J-K-L-ENTER
    // IN A,(C)
    // AND 1                   // Keep only bit 0 of the result (ENTER)
    // CP 1                    // Is ENTER being pressed?
    // JR NZ,STARTGAME         // If so, start the game
    if (((MEM[49150] & 0xFF) & 1) == 1) {
      goto STARTGAME;
    }

    // LD A,(EUGHGT)           // Pick up the message index from EUGHGT
    // INC A                   // Increment it
    // CP 224                  // Set the zero flag if we've reached the end of the message
    // LD (EUGHGT),A           // Store the new message index at EUGHGT
    // JR NZ,START_2           // Jump back unless we've finished scrolling the message across the screen
  }

  // Initialise the game mode indicator at DEMO to 64: demo mode
  // LD A,64
  // LD (DEMO),A
  DEMO = 64;
// This routine continues into the one at STARTGAME.


// Start the game (or demo mode)
//
// Used by the routine at START.
STARTGAME:
  // IMPORTANT: Probably better to have custom SCORE/SCORBUF/HGHSCOR updating and printing. -MRC-
  // Initialise the score at SCORE
  // LD HL,SCORE
  // LD DE,33830
  // LD BC,9
  // LD (HL),48
  // LDIR
  current_score = 0;
  print_score(current_score);

// This entry point is used by the routines at LOOP (when teleporting into a
// cavern or reinitialising the current cavern after Willy has lost a life) and
// NXSHEET.
NEWSHT:
  // IMPORTANT: only using the first cavern, CAVERN0, while porting -MRC-
  // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
  // SLA A                   // Point HL at the first byte of the cavern definition
  // SLA A
  // ADD A,176
  // LD H,A
  // LD L,0
  // LD DE,24064             // Copy the cavern's attribute bytes into the buffer
  // LD BC,512               // at 24064
  // LDIR
  for (int i = 0; i < 512; i++) {
    MEM[24064 + i] = CAVERN0[i];
  }

  // IMPORTANT: initialize_cavern0() doesn't currently copy data to: game status buffer at 32768 -MRC-
  // LD DE,CAVERNNAME        // Copy the rest of the cavern definition into the
  // LD BC,512               // game status buffer at 32768
  // LDIR
  initialize_cavern0();

  // Draw the current cavern to the screen buffer at 28672
  // CALL DRAWSHEET
  DRAWSHEET();

  // LD HL,20480             // Clear the bottom third of the display file
  // LD DE,20481
  // LD BC,2047
  // LD (HL),0
  // LDIR
  for (int i = 0; i < 2048; i++) {
    MEM[20481 + i] = 0;
  }

  // Print the cavern name (see CAVERNNAME) at (16,0)
  // LD IX,CAVERNNAME
  // LD C,32
  // LD DE,20480
  // CALL PMESS
  PMESS(&CAVERNNAME, 20480, 32);

  // Print 'AIR' (see MESSAIR) at (17,0)
  // LD IX,MESSAIR
  // LD C,3
  // LD DE,20512
  // CALL PMESS
  PMESS(&MESSAIR, 20512, 3);

  // Initialise A to 82; this is the MSB of the display file address at which to start drawing the bar that represents the air supply
  // LD A,82
  // STARTGAME_0:
  for (uint8_t a = 82; a < 86; a++) {
    // LD H,A                  // Prepare HL and DE for drawing a row of pixels in
    // LD D,A                  // the air bar
    // LD L,36
    // LD E,37
    uint16_t addr = build_address(a, 37);

    // LD B,A                  // Save the display file address MSB in B briefly
    // LD A,(AIR)              // Pick up the value of the initial air supply from AIR
    // SUB 36                  // Now C determines the length of the air bar (in cell
    // LD C,A                  // widths)
    // LD A,B                  // Restore the display file address MSB to A
    // LD B,0                  // Now BC determines the length of the air bar (in cell widths)
    // LD (HL),255             // Draw a single row of pixels across C cells
    // LDIR
    for (uint16_t i = 0; i < AIR - 36; i++) {
      MEM[addr + i] = 255;
    }

    // INC A                   // Increment the display file address MSB in A (moving down to the next row of pixels)
    // CP 86                   // Have we drawn all four rows of pixels in the air bar yet?
    // JR NZ,STARTGAME_0       // If not, jump back to draw the next one
  }

  // Print 'High Score 000000   Score 000000' (see MESSHSSC) at (19,0)
  // LD IX,MESSHSSC
  // LD DE,20576
  // LD C,32
  // CALL PMESS
  PMESS(&MESSHSSC, 20576, 32);

  // LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
  // LD C,254                // Set the border colour
  // OUT (C),A
  MEM[254] = BORDER;

  // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  // OR A                    // Are we in demo mode?
  // JR Z,LOOP               // If not, enter the main loop now
  if (DEMO == 0) {
    // Reset the game mode indicator at DEMO to 64 (we're in demo mode)
    // LD A,64
    // LD (DEMO),A
    DEMO = 64;
  }
// This routine continues into the main loop at LOOP.


// Main loop
//
// The routine at STARTGAME continues here.
//
// The first thing to do is check whether there are any remaining lives to draw
// at the bottom of the screen.
LOOP:
  // LD A,(NOMEN)            // Pick up the number of lives remaining from NOMEN
  // LD HL,20640             // Set HL to the display file address at which to draw the first Willy sprite
  regHL = 20640;
  // OR A                    // Are there any lives remaining?
  // JR Z,LOOP_1             // Jump if not
  // LD B,A                  // Initialise B to the number of lives remaining
  // The following loop draws the remaining lives at the bottom of the screen.
  // LOOP_0:
  for (int i = 0; i < NOMEN; i++) {
    // LD C,0                  // C=0; this tells the sprite-drawing routine at DRWFIX to overwrite any existing graphics
    // PUSH HL                 // Save HL and BC briefly
    // PUSH BC
    // LD A,(NOTEINDEX)        // Pick up the in-game music note index from NOTEINDEX; this will determine the animation frame for the Willy sprites
    // RLCA                    // Now A=0 (frame 0), 32 (frame 1), 64 (frame 2) or 96
    // RLCA                    // (frame 3)
    // RLCA
    // AND 96
    uint8_t anim_frame = (NOTEINDEX << 3) & 96;
    // LD E,A                  // Point DE at the corresponding Willy sprite (at
    // LD D,130                // MANDAT+A)
    uint8_t *mandat_sprite_ptr = &MANDAT[anim_frame];
    // CALL DRWFIX             // Draw the Willy sprite on the screen
    DRWFIX(mandat_sprite_ptr, regHL, 0);

    // POP BC                  // Restore HL and BC
    // POP HL
    // INC HL                  // Move HL along to the location at which to draw the
    // INC HL                  // next Willy sprite
    regHL += 2;
    // DJNZ LOOP_0             // Jump back to draw any remaining sprites
  }

  // Now draw a boot if cheat mode has been activated.
  // LOOP_1:
  // LD A,(CHEAT)            // Pick up the 6031769 key counter from CHEAT
  // CP 7                    // Has 6031769 been keyed in yet?
  // JR NZ,LOOP_2            // Jump if not
  if (CHEAT == 7) {
    // LD DE,BOOT              // Point DE at the graphic data for the boot (at BOOT)
    // LD C,0                  // C=0 (overwrite mode)
    // CALL DRWFIX             // Draw the boot at the bottom of the screen next to the remaining lives
    DRWFIX(&BOOT, regHL, 0);
  }

  // Next, prepare the screen and attribute buffers for drawing to the screen.
  // LOOP_2:
  // LD HL,24064             // Copy the contents of the attribute buffer at 24064
  // LD DE,23552             // (the attributes for the empty cavern) into the
  // LD BC,512               // attribute buffer at 23552
  // LDIR
  for (int i = 0; i < 512; i++) {
    MEM[23552 + i] = MEM[24064 + i];
  }

  // LD HL,28672             // Copy the contents of the screen buffer at 28672
  // LD DE,24576             // (the tiles for the empty cavern) into the screen
  // LD BC,4096              // buffer at 24576
  // LDIR
  for (int i = 0; i < 4096; i++) {
    MEM[24576 + i] = MEM[28672 + i];
  }

  CALL MOVEHG             // Move the horizontal guardians in the current cavern

  // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  // OR A                    // Are we in demo mode?
  if (DEMO == 0) {
    CALL Z,MOVEWILLY        // If not, move Willy
  }
  // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  // OR A                    // Are we in demo mode?
  if (DEMO == 0) {
    CALL Z,WILLYATTRS       // If not, check and set the attribute bytes for Willy's sprite in the buffer at 23552, and draw Willy to the screen buffer at 24576
  }

  CALL DRAWHG             // Draw the horizontal guardians in the current cavern
  CALL MVCONVEYOR         // Move the conveyor in the current cavern
  CALL DRAWITEMS          // Draw the items in the current cavern and collect any that Willy is touching

  switch (SHEET) {
  case 4:
    // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
    // CP 4                    // Are we in Eugene's Lair?
    CALL Z,EUGENE           // If so, move and draw Eugene
  case 13:
    // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
    // CP 13                   // Are we in Skylab Landing Bay?
    JP Z,SKYLABS            // If so, move and draw the Skylabs
  case 8:
    // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
    // CP 8                    // Are we in Wacky Amoebatrons or beyond?
    CALL NC,VGUARDIANS      // If so, move and draw the vertical guardians
  case 7:
    // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
    // CP 7                    // Are we in Miner Willy meets the Kong Beast?
    CALL Z,KONGBEAST        // If so, move and draw the Kong Beast
  case 11:
    // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
    // CP 11                   // Are we in Return of the Alien Kong Beast?
    CALL Z,KONGBEAST        // If so, move and draw the Kong Beast
  case 18:
    // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
    // CP 18                   // Are we in Solar Power Generator?
    CALL Z,LIGHTBEAM        // If so, move and draw the light beam
  }

// This entry point is used by the routine at SKYLABS.
LOOP_3:
  CALL CHKPORTAL          // Draw the portal, or move to the next cavern if Willy has entered it

// This entry point is used by the routine at KILLWILLY.
LOOP_4:
  // LD HL,24576             // Copy the contents of the screen buffer at 24576 to
  // LD DE,16384             // the display file
  // LD BC,4096
  // LDIR
  for (int i = 0; i < 4096; i++) {
    MEM[16384 + i] = MEM[24576 + i];
  }

  // LD A,(FLASH)            // Pick up the screen flash counter from FLASH
  // OR A                    // Is it zero?
  // JR Z,LOOP_5             // Jump if so
  if (FLASH != 0) {
    // Decrement the screen flash counter at FLASH
    // DEC A
    // LD (FLASH),A
    FLASH--;

    // RLCA                    // Move bits 0-2 into bits 3-5 and clear all the other
    // RLCA                    // bits
    // RLCA
    // AND 56
    uint8_t new_flash_value = (FLASH << 3) & 56;
    // LD HL,23552             // Set every attribute byte in the buffer at 23552 to
    // LD DE,23553             // this value
    // LD BC,511
    // LD (HL),A
    // LDIR
    for (int i = 0; i < 512; i++) {
      MEM[23552 + i] = new_flash_value;
    }
  }

  // LOOP_5:
  // LD HL,23552             // Copy the contents of the attribute buffer at 23552
  // LD DE,22528             // to the attribute file
  // LD BC,512
  // LDIR
  for (int i = 0; i < 512; i++) {
    MEM[22528 + i] = MEM[23552 + i];
  }

  // Print the score (see SCORBUF) at (19,26)
  // LD IX,SCORBUF
  // LD DE,20602
  // LD C,6
  // CALL PMESS
  PMESS(&SCORBUF, 20602, 6);

  // LD IX,HGHSCOR           // Print the high score (see HGHSCOR) at (19,11)
  // LD DE,20587
  // LD C,6
  // CALL PMESS
  PMESS(&HGHSCOR, 20587, 6);

  // Decrease the air remaining in the current cavern
  // Jump if there's no air left
  // CALL DECAIR
  // JP Z,MANDEAD
  if ( !DECAIR() ) {
    goto MANDEAD;
  }

  // Now check whether SHIFT and SPACE are being pressed.
  // LD BC,65278             // Read keys SHIFT-Z-X-C-V
  // IN A,(C)
  // LD E,A                  // Save the result in E
  regE = MEM[65278] & 0xFF;

  regC = 65278 & 0xFF;
  // LD B,127                // Read keys B-N-M-SS-SPACE
  regBC = (127 << 8) | regC;
  // IN A,(C)
  regA = (MEM[regBC] & 0xFF);
  // OR E                    // Combine the results
  regA = regA | regE;
  // AND 1                   // Are SHIFT and SPACE being pressed?
  // JP Z,START              // If so, quit the game
  if ((regA & 1) == 0) {
    goto START;
  }

  // Now read the keys A, S, D, F and G (which pause the game).
  // LD B,253                // Read keys A-S-D-F-G
  regBC = (253 << 8) | regC;
  // IN A,(C)
  regA = (MEM[regBC] & 0xFF);
  // AND 31                  // Are any of these keys being pressed?
  // CP 31
  // JR Z,LOOP_7             // Jump if not
  if ((regA & 31) == 31) {
    // LOOP_6:
    do {
      // LD B,2                  // Read every half-row of keys except A-S-D-F-G
      regBC = (2 << 8) | regC;
      // IN A,(C)
      regA = (MEM[regBC] & 0xFF);
      // AND 31                  // Are any of these keys being pressed?
      // CP 31
      // JR Z,LOOP_6             // Jump back if not (the game is still paused)
    } while ((regA & 31) != 31);
  }

  // Here we check whether Willy has had a fatal accident.
  // LOOP_7:
  // LD A,(AIRBORNE)         // Pick up the airborne status indicator from AIRBORNE
  // CP 255                  // Has Willy landed after falling from too great a height, or collided with a nasty or a guardian?
  // JP Z,MANDEAD            // Jump if so
  if (AIRBORNE == 255) {
    goto MANDEAD;
  }

  // Now read the keys H, J, K, L and ENTER (which toggle the in-game music).
  // LD B,191                // Prepare B for reading keys H-J-K-L-ENTER
  // LD HL,MUSICFLAGS        // Point HL at the music flags at MUSICFLAGS
  // IN A,(C)                // Read keys H-J-K-L-ENTER
  // AND 31                  // Are any of these keys being pressed?
  // CP 31
  // JR Z,LOOP_8             // Jump if not
  if (((MEM[49150] & 0xFF) & 31) == 31) {
    // BIT 0,(HL)              // Were any of these keys being pressed the last time we checked?
    // JR NZ,LOOP_9            // Jump if so
    if ((MUSICFLAGS >> 0) & 1 == 0) {
      // LD A,(HL)               // Set bit 0 (the keypress flag) and flip bit 1 (the
      // XOR 3                   // in-game music flag) at MUSICFLAGS
      // LD (HL),A
      MUSICFLAGS ^= 3;
      // JR LOOP_9
    }
  } else {
    // LOOP_8:
    // RES 0,(HL)              // Reset bit 0 (the keypress flag) at MUSICFLAGS
    MUSICFLAGS &= ~(1 << 0);
  }

  // LOOP_9:
  // BIT 1,(HL)              // Has the in-game music been switched off?
  // JR NZ,NONOTE4           // Jump if so
  if ((MUSICFLAGS >> 1) & 1 == 0) {
    // The next section of code plays a note of the in-game music.

    // Increment the in-game music note index at NOTEINDEX
    // LD A,(NOTEINDEX)
    // INC A
    // LD (NOTEINDEX),A
    NOTEINDEX++;

    AND 126                 // Point HL at the appropriate entry in the tune data
    RRCA                    // table at GAMETUNE
    LD E,A
    LD D,0
    LD HL,GAMETUNE
    ADD HL,DE
    LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
    LD E,(HL)               // Initialise the pitch delay counter in E
    LD BC,3                 // Initialise the duration delay counters in B (0) and C (3)

    // TM51:
    for (;;) {
      OUT (254),A             // Produce a note of the in-game music

      // SEE37708:
      DEC E
      JR NZ,NOFLP6
      LD E,(HL)
      XOR 24

      NOFLP6:
      DJNZ TM51
      DEC C
      JR NZ,TM51
    }
  }

  // If we're in demo mode, check the keyboard and joystick and return to the
  // title screen if there's any input.
  // NONOTE4:
  // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  // OR A                    // Are we in demo mode?
  // JR Z,NODEM1             // Jump if not
  if (DEMO != 0) {
    // We're in demo mode; is it time to show the next cavern?

    // DEC A
    // JP Z,MANDEAD            // Jump if so
    if (DEMO - 1 == 0) {
      goto MANDEAD;
    }
    // LD (DEMO),A             // Update the game mode indicator at DEMO
    DEMO--;

    // LD BC,254               // Read every row of keys on the keyboard
    // IN A,(C)
    // AND 31                  // Are any keys being pressed?
    // CP 31
    // JP NZ,START             // If so, return to the title screen
    if (((MEM[254] & 0xFF) & 31) == 31) {
      goto START;
    }

    // LD A,(KEMP)             // Pick up the Kempston joystick indicator from KEMP
    // OR A                    // Is there a joystick connected?
    // JR Z,NODEM1             // Jump if not
    if (KEMP != 0) {
      /* TODO
        IN A,(31)               // Collect input from the joystick
        OR A                    // Is the joystick being moved or the fire button being pressed?
        JP NZ,START             // If so, return to the title screen
      */
    }
  }

// IMPORTANT: not handling cheat codes just yet -MRC-
/* TODO
  // Here we check the teleport keys.
  // NODEM1:
  LD BC,61438             // Read keys 6-7-8-9-0
  IN A,(C)
  BIT 4,A                 // Is '6' (the activator key) being pressed?
  JP NZ,CKCHEAT           // Jump if not
  LD A,(CHEAT)            // Pick up the 6031769 key counter from CHEAT
  CP 7                    // Has 6031769 been keyed in yet?
  JP NZ,CKCHEAT           // Jump if not
  LD B,247                // Read keys 1-2-3-4-5
  IN A,(C)
  CPL                     // Keep only bits 0-4 and flip them
  AND 31
  CP 20                   // Is the result 20 or greater?
  JP NC,CKCHEAT           // Jump if so (this is not a cavern number)
  LD (SHEET),A            // Store the cavern number at SHEET
  JP NEWSHT               // Teleport into the cavern

// Now check the 6031769 keys.
CKCHEAT:
  LD A,(CHEAT)            // Pick up the 6031769 key counter from CHEAT
  CP 7                    // Has 6031769 been keyed in yet?
  JP Z,LOOP               // If so, jump back to the start of the main loop
  RLCA                    // Point IX at the corresponding entry in the 6031769
  LD E,A                  // table at CHEATDT
  LD D,0
  LD IX,CHEATDT
  ADD IX,DE
  LD BC,63486             // Read keys 1-2-3-4-5
  IN A,(C)
  AND 31                  // Keep only bits 0-4
  CP (IX+0)               // Does this match the first byte of the entry in the 6031769 table?
  JR Z,CKNXCHT            // Jump if so
  CP 31                   // Are any of the keys 1-2-3-4-5 being pressed?
  JP Z,LOOP               // If not, jump back to the start of the main loop
  CP (IX-2)               // Does the keyboard reading match the first byte of the previous entry in the 6031769 table?
  JP Z,LOOP               // If so, jump back to the start of the main loop
  XOR A                   // Reset the 6031769 key counter at CHEAT to 0 (an
  LD (CHEAT),A            // incorrect key is being pressed)
  JP LOOP                 // Jump back to the start of the main loop
CKNXCHT:
  LD B,239                // Read keys 6-7-8-9-0
  IN A,(C)
  AND 31                  // Keep only bits 0-4
  CP (IX+1)               // Does this match the second byte of the entry in the 6031769 table?
  JR Z,INCCHT             // If so, jump to increment the 6031769 key counter
  CP 31                   // Are any of the keys 6-7-8-9-0 being pressed?
  JP Z,LOOP               // If not, jump back to the start of the main loop
  CP (IX-1)               // Does the keyboard reading match the second byte of the previous entry in the 6031769 table?
  JP Z,LOOP               // If so, jump back to the start of the main loop
  XOR A                   // Reset the 6031769 key counter at CHEAT to 0 (an
  LD (CHEAT),A            // incorrect key is being pressed)
  JP LOOP                 // Jump back to the start of the main loop
INCCHT:
  LD A,(CHEAT)            // Increment the 6031769 key counter at CHEAT (the
  INC A                   // next key in the sequence is being pressed)
  LD (CHEAT),A
  JP LOOP                 // Jump back to the start of the main loop
*/


// The air in the cavern has run out, or Willy has had a fatal accident, or it's
// demo mode and it's time to show the next cavern.
MANDEAD:
  // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  // OR A                    // Is it demo mode?
  // JP NZ,NXSHEET           // If so, move to the next cavern
  if (DEMO != 0) {
    goto NXSHEET;
  }

  // LD A,71                 // A=71 (INK 7: PAPER 0: BRIGHT 1)

  // The following loop fills the top two thirds of the attribute file with a
  // single value (71 TO 64 STEP -1) and makes a sound effect.
  // LPDEAD1:
  for (int attr = 71; attr > 64; attr--) {
    // LD HL,22528             // Fill the top two thirds of the attribute file with
    // LD DE,22529             // the value in A
    // LD BC,511
    // LD (HL),A
    // LDIR
    for (int i = 0; i < 512; i++) {
      MEM[22528 + i] = attr;
    }

    LD E,A                  // Save the attribute byte (64-71) in E for later retrieval

    CPL                     // D=63-8*(E AND 7); this value determines the pitch
    AND 7                   // of the short note that will be played
    RLCA
    RLCA
    RLCA
    OR 7
    LD D,A
    LD C,E                  // C=8+32*(E AND 7); this value determines the
    RRC C                   // duration of the short note that will be played
    RRC C
    RRC C
    OR 16                   // Set bit 4 of A (for no apparent reason)
    XOR A                   // Set A=0 (this will make the border black)
  TM21:
    OUT (254),A             // Produce a short note whose pitch is determined by D
    XOR 24                  // and whose duration is determined by C
    LD B,D
  TM22:
    DJNZ TM22
    DEC C
    JR NZ,TM21

    LD A,E                  // Restore the attribute byte (originally 71) to A

    // DEC A                   // Decrement it (effectively decrementing the INK colour)
    // CP 63                   // Have we used attribute value 64 (INK 0) yet?
    // JR NZ,LPDEAD1           // If not, jump back to update the INK colour in the top two thirds of the screen and make another sound effect
  }

// Finally, check whether any lives remain.
  // LD HL,NOMEN             // Pick up the number of lives remaining from NOMEN
  // LD A,(HL)
  // OR A
  // Are there any lives remaining?
  if (NOMEN == 0) {
    // If not, display the game over sequence
    // JP Z,ENDGAM
    goto ENDGAM;
  } else {
    // Decrease the number of lives remaining by one
    // DEC (HL)
    NOMEN--;
  }

  // Jump back to reinitialise the current cavern
  // JP NEWSHT
  goto NEWSHT;



// Display the game over sequence
//
// Used by the routine at LOOP. First check whether we have a new high score.
ENDGAM:
  LD HL,HGHSCOR           // Point HL at the high score at HGHSCOR
  LD DE,SCORBUF           // Point DE at the current score at SCORBUF
  LD B,6                  // There are 6 digits to compare
LPHGH:
  LD A,(DE)               // Pick up a digit of the current score
  CP (HL)                 // Compare it with the corresponding digit of the high score
  JP C,FEET               // Jump if it's less than the corresponding digit of the high score
  JP NZ,NEWHGH            // Jump if it's greater than the corresponding digit of the high score
  INC HL                  // Point HL at the next digit of the high score
  INC DE                  // Point DE at the next digit of the current score
  DJNZ LPHGH              // Jump back to compare the next pair of digits
NEWHGH:
  LD HL,SCORBUF           // Replace the high score with the current score
  LD DE,HGHSCOR
  LD BC,6
  LDIR
// Now prepare the screen for the game over sequence.
FEET:
  LD HL,16384             // Clear the top two-thirds of the display file
  LD DE,16385
  LD BC,4095
  LD (HL),0
  LDIR
  XOR A                   // Initialise the game status buffer variable at
  LD (EUGHGT),A           // EUGHGT; this variable will determine the distance of the boot from the top of the screen
  LD DE,WILLYR2           // Draw Willy at (12,15)
  LD HL,18575
  LD C,0
  CALL DRWFIX
  LD DE,PLINTH            // Draw the plinth (see PLINTH) underneath Willy at
  LD HL,18639             // (14,15)
  LD C,0
  CALL DRWFIX
// The following loop draws the boot's descent onto the plinth that supports
// Willy.
LOOPFT:
  LD A,(EUGHGT)           // Pick up the distance variable from EUGHGT
  LD C,A                  // Point BC at the corresponding entry in the screen
  LD B,131                // buffer address lookup table at SBUFADDRS
  LD A,(BC)               // Point HL at the corresponding location in the
  OR 15                   // display file
  LD L,A
  INC BC
  LD A,(BC)
  SUB 32
  LD H,A
  LD DE,BOOT              // Draw the boot (see BOOT) at this location, without
  LD C,0                  // erasing the boot at the previous location; this
  CALL DRWFIX             // leaves the portion of the boot sprite that's above the ankle in place, and makes the boot appear as if it's at the end of a long, extending trouser leg
  LD A,(EUGHGT)           // Pick up the distance variable from EUGHGT
  CPL                     // A=255-A
  LD E,A                  // Store this value (63-255) in E; it determines the (rising) pitch of the sound effect that will be made
  XOR A                   // A=0 (black border)
  LD BC,64                // C=64; this value determines the duration of the sound effect
TM111:
  OUT (254),A             // Produce a short note whose pitch is determined by E
  XOR 24
  LD B,E
TM112:
  DJNZ TM112
  DEC C
  JR NZ,TM111
  LD HL,22528             // Prepare BC, DE and HL for setting the attribute
  LD DE,22529             // bytes in the top two-thirds of the screen
  LD BC,511
  LD A,(EUGHGT)           // Pick up the distance variable from EUGHGT
  AND 12                  // Keep only bits 2 and 3
  RLCA                    // Shift bits 2 and 3 into bits 3 and 4; these bits determine the PAPER colour: 0, 1, 2 or 3
  OR 71                   // Set bits 0-2 (INK 7) and 6 (BRIGHT 1)
  LD (HL),A               // Copy this attribute value into the top two-thirds
  LDIR                    // of the screen
  LD A,(EUGHGT)           // Add 4 to the distance variable at EUGHGT; this will
  ADD A,4                 // move the boot sprite down two pixel rows
  LD (EUGHGT),A
  CP 196                  // Has the boot met the plinth yet?
  JR NZ,LOOPFT            // Jump back if not
// Now print the "Game Over" message, just to drive the point home.
  LD IX,MESSG             // Print "Game" (see MESSG) at (6,10)
  LD C,4
  LD DE,16586
  CALL PMESS
  LD IX,MESSO             // Print "Over" (see MESSO) at (6,18)
  LD C,4
  LD DE,16594
  CALL PMESS
  LD BC,0                 // Prepare the delay counters for the following loop;
  LD D,6                  // the counter in C will also determine the INK colours to use for the "Game Over" message
// The following loop makes the "Game Over" message glisten for about 1.57s.
TM91:
  DJNZ TM91               // Delay for about a millisecond
  LD A,C                  // Change the INK colour of the "G" in "Game" at
  AND 7                   // (6,10)
  OR 64
  LD (22730),A
  INC A                   // Change the INK colour of the "a" in "Game" at
  AND 7                   // (6,11)
  OR 64
  LD (22731),A
  INC A                   // Change the INK colour of the "m" in "Game" at
  AND 7                   // (6,12)
  OR 64
  LD (22732),A
  INC A                   // Change the INK colour of the "e" in "Game" at
  AND 7                   // (6,13)
  OR 64
  LD (22733),A
  INC A                   // Change the INK colour of the "O" in "Over" at
  AND 7                   // (6,18)
  OR 64
  LD (22738),A
  INC A                   // Change the INK colour of the "v" in "Over" at
  AND 7                   // (6,19)
  OR 64
  LD (22739),A
  INC A                   // Change the INK colour of the "e" in "Over" at
  AND 7                   // (6,20)
  OR 64
  LD (22740),A
  INC A                   // Change the INK colour of the "r" in "Over" at
  AND 7                   // (6,21)
  OR 64
  LD (22741),A
  DEC C                   // Decrement the counter in C
  JR NZ,TM91              // Jump back unless it's zero
  DEC D                   // Decrement the counter in D (initially 6)
  JR NZ,TM91              // Jump back unless it's zero
  JP START                // Display the title screen and play the theme tune


// Decrease the air remaining in the current cavern
//
// Used by the routines at LOOP, LIGHTBEAM and NXSHEET. Returns with the zero
// flag set if there is no air remaining.
DECAIR:
  LD A,(CLOCK)            // Update the game clock at CLOCK
  SUB 4
  LD (CLOCK),A
  CP 252                  // Was it just decreased from zero?
  JR NZ,DECAIR_0          // Jump if not
  LD A,(AIR)              // Pick up the value of the remaining air supply from AIR
  CP 36                   // Has the air supply run out?
  RET Z                   // Return (with the zero flag set) if so
  DEC A                   // Decrement the air supply at AIR
  LD (AIR),A
  LD A,(CLOCK)            // Pick up the value of the game clock at CLOCK
DECAIR_0:
  AND 224                 // A=INT(A/32); this value specifies how many pixels
  RLCA                    // to draw from left to right in the cell at the right
  RLCA                    // end of the air bar
  RLCA
  LD E,0                  // Initialise E to 0 (all bits reset)
  OR A                    // Do we need to draw any pixels in the cell at the right end of the air bar?
  JR Z,DECAIR_2           // Jump if not
  LD B,A                  // Copy the number of pixels to draw (1-7) to B
DECAIR_1:
  RRC E                   // Set this many bits in E (from bit 7 towards bit 0)
  SET 7,E
  DJNZ DECAIR_1
DECAIR_2:
  LD A,(AIR)              // Pick up the value of the remaining air supply from AIR
  LD L,A                  // Set HL to the display file address at which to draw
  LD H,82                 // the top row of pixels in the cell at the right end of the air bar
  LD B,4                  // There are four rows of pixels to draw
DECAIR_3:
  LD (HL),E               // Draw the four rows of pixels at the right end of
  INC H                   // the air bar
  DJNZ DECAIR_3
  XOR A                   // Reset the zero flag to indicate that there is still
  INC A                   // some air remaining; these instructions are redundant, since the zero flag is already reset at this point
  RET

// Draw the current cavern to the screen buffer at 28672
//
// Used by the routine at STARTGAME.
DRAWSHEET:
  LD IX,24064             // Point IX at the first byte of the attribute buffer at 24064
  LD A,112                // Set the operand of the 'LD D,n' instruction at
  LD (35484),A            // SBMSB (below) to 112
  CALL DRAWSHEET_0        // Draw the tiles for the top half of the cavern to the screen buffer at 28672
  LD IX,24320             // Point IX at the 256th byte of the attribute buffer at 24064 in preparation for drawing the bottom half of the cavern; this instruction is redundant, since IX already holds 24320
  LD A,120                // Set the operand of the 'LD D,n' instruction at
  LD (35484),A            // SBMSB (below) to 120
DRAWSHEET_0:
  LD C,0                  // C will count 256 tiles
// The following loop draws 256 tiles (for either the top half or the bottom
// half of the cavern) to the screen buffer at 28672.
DRAWSHEET_1:
  LD E,C                  // E holds the LSB of the screen buffer address
  LD A,(IX+0)             // Pick up an attribute byte from the buffer at 24064; this identifies the type of tile to draw
  LD HL,BACKGROUND        // Move HL through the attribute bytes and graphic
  LD BC,72                // data of the background, floor, crumbling floor,
  CPIR                    // wall, conveyor and nasty tiles starting at BACKGROUND until we find a byte that matches the attribute byte of the tile to be drawn
  LD C,E                  // Restore the value of the tile counter in C
  LD B,8                  // There are eight bytes in the tile
SBMSB:
  LD D,0                  // This instruction is set to either 'LD D,112' or 'LD D,120' above; now DE holds the appropriate address in the screen buffer at 28672
DRAWSHEET_2:
  LD A,(HL)               // Copy the tile graphic data to the screen buffer at
  LD (DE),A               // 28672
  INC HL
  INC D
  DJNZ DRAWSHEET_2
  INC IX                  // Move IX along to the next byte in the attribute buffer
  INC C                   // Have we drawn 256 tiles yet?
  JP NZ,DRAWSHEET_1       // If not, jump back to draw the next one
// The empty cavern has been drawn to the screen buffer at 28672. If we're in
// The Final Barrier, however, there is further work to do.
  LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
  CP 19                   // Is it The Final Barrier?
  RET NZ                  // Return if not
  LD HL,TITLESCR1         // Copy the graphic data from TITLESCR1 to the top
  LD DE,28672             // half of the screen buffer at 28672
  LD BC,2048
  LDIR
  RET

// Move Willy (1)
//
// Used by the routine at LOOP. This routine deals with Willy if he's jumping or
// falling.
MOVEWILLY:
  LD A,(AIRBORNE)         // Pick up the airborne status indicator from AIRBORNE
  CP 1                    // Is Willy jumping?
  JR NZ,MOVEWILLY_3       // Jump if not
// Willy is currently jumping.
  LD A,(JUMPING)          // Pick up the jumping animation counter (0-17) from JUMPING
  RES 0,A                 // Now -8<=A<=8 (and A is even)
  SUB 8
  LD HL,PIXEL_Y           // Adjust Willy's pixel y-coordinate at PIXEL_Y
  ADD A,(HL)              // depending on where Willy is in the jump
  LD (HL),A
  CALL MOVEWILLY_7        // Adjust Willy's attribute buffer location at LOCATION depending on his pixel y-coordinate
  LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  CP (HL)                 // Is the top-left cell of Willy's sprite overlapping a wall tile?
  JP Z,MOVEWILLY_10       // Jump if so
  INC HL                  // Point HL at the top-right cell occupied by Willy's sprite
  CP (HL)                 // Is the top-right cell of Willy's sprite overlapping a wall tile?
  JP Z,MOVEWILLY_10       // Jump if so
  LD A,(JUMPING)          // Increment the jumping animation counter at JUMPING
  INC A
  LD (JUMPING),A
  SUB 8                   // A=J-8, where J (1-18) is the new value of the jumping animation counter
  JP P,MOVEWILLY_0        // Jump if J>=8
  NEG                     // A=8-J (1<=J<=7, 1<=A<=7)
MOVEWILLY_0:
  INC A                   // A=1+ABS(J-8)
  RLCA                    // D=8*(1+ABS(J-8)); this value determines the pitch
  RLCA                    // of the jumping sound effect (rising as Willy rises,
  RLCA                    // falling as Willy falls)
  LD D,A
  LD C,32                 // C=32; this value determines the duration of the jumping sound effect
  LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
MOVEWILLY_1:
  OUT (254),A             // Make a jumping sound effect
  XOR 24
  LD B,D
MOVEWILLY_2:
  DJNZ MOVEWILLY_2
  DEC C
  JR NZ,MOVEWILLY_1
  LD A,(JUMPING)          // Pick up the jumping animation counter (1-18) from JUMPING
  CP 18                   // Has Willy reached the end of the jump?
  JP Z,MOVEWILLY_8        // Jump if so
  CP 16                   // Is the jumping animation counter now 16?
  JR Z,MOVEWILLY_3        // Jump if so
  CP 13                   // Is the jumping animation counter now 13?
  JP NZ,MOVEWILLY2_6      // Jump if not
// If we get here, then Willy is standing on the floor, or he's falling, or his
// jumping animation counter is 13 (at which point Willy is on his way down and
// is exactly two cell-heights above where he started the jump) or 16 (at which
// point Willy is on his way down and is exactly one cell-height above where he
// started the jump).
MOVEWILLY_3:
  LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  AND 15                  // Does Willy's sprite occupy six cells at the moment?
  JR NZ,MOVEWILLY_4       // Jump if so
  LD HL,(LOCATION)        // Pick up Willy's attribute buffer coordinates from LOCATION
  LD DE,64                // Point HL at the left-hand cell below Willy's sprite
  ADD HL,DE
  LD A,(CRUMBLING)        // Pick up the attribute byte of the crumbling floor tile for the current cavern from CRUMBLING
  CP (HL)                 // Does the left-hand cell below Willy's sprite contain a crumbling floor tile?
  CALL Z,CRUMBLE          // If so, make it crumble
  LD A,(NASTY1)           // Pick up the attribute byte of the first nasty tile for the current cavern from NASTY1
  CP (HL)                 // Does the left-hand cell below Willy's sprite contain a nasty tile?
  JR Z,MOVEWILLY_4        // Jump if so
  LD A,(NASTY2)           // Pick up the attribute byte of the second nasty tile for the current cavern from NASTY2
  CP (HL)                 // Does the left-hand cell below Willy's sprite contain a nasty tile?
  JR Z,MOVEWILLY_4        // Jump if so
  INC HL                  // Point HL at the right-hand cell below Willy's sprite
  LD A,(CRUMBLING)        // Pick up the attribute byte of the crumbling floor tile for the current cavern from CRUMBLING
  CP (HL)                 // Does the right-hand cell below Willy's sprite contain a crumbling floor tile?
  CALL Z,CRUMBLE          // If so, make it crumble
  LD A,(NASTY1)           // Pick up the attribute byte of the first nasty tile for the current cavern from NASTY1
  CP (HL)                 // Does the right-hand cell below Willy's sprite contain a nasty tile?
  JR Z,MOVEWILLY_4        // Jump if so
  LD A,(NASTY2)           // Pick up the attribute byte of the second nasty tile for the current cavern from NASTY2
  CP (HL)                 // Does the right-hand cell below Willy's sprite contain a nasty tile?
  JR Z,MOVEWILLY_4        // Jump if so
  LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  CP (HL)                 // Set the zero flag if the right-hand cell below Willy's sprite is empty
  DEC HL                  // Point HL at the left-hand cell below Willy's sprite
  JP NZ,MOVEWILLY2        // Jump if the right-hand cell below Willy's sprite is not empty
  CP (HL)                 // Is the left-hand cell below Willy's sprite empty?
  JP NZ,MOVEWILLY2        // Jump if not
MOVEWILLY_4:
  LD A,(AIRBORNE)         // Pick up the airborne status indicator from AIRBORNE
  CP 1                    // Is Willy jumping?
  JP Z,MOVEWILLY2_6       // Jump if so
// If we get here, then Willy is either in the process of falling or just about
// to start falling.
  LD HL,DMFLAGS           // Reset bit 1 at DMFLAGS: Willy is not moving left or
  RES 1,(HL)              // right
  OR A                    // Is Willy already falling?
  JP Z,MOVEWILLY_9        // Jump if not
  INC A                   // Increment the airborne status indicator at AIRBORNE
  LD (AIRBORNE),A
  RLCA                    // D=16*A; this value determines the pitch of the
  RLCA                    // falling sound effect
  RLCA
  RLCA
  LD D,A
  LD C,32                 // C=32; this value determines the duration of the falling sound effect
  LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
MOVEWILLY_5:
  OUT (254),A             // Make a falling sound effect
  XOR 24
  LD B,D
MOVEWILLY_6:
  DJNZ MOVEWILLY_6
  DEC C
  JR NZ,MOVEWILLY_5
  LD A,(PIXEL_Y)          // Add 8 to Willy's pixel y-coordinate at PIXEL_Y;
  ADD A,8                 // this moves Willy downwards by 4 pixels
  LD (PIXEL_Y),A
MOVEWILLY_7:
  AND 240                 // L=16*Y, where Y is Willy's screen y-coordinate
  LD L,A                  // (0-14)
  XOR A                   // Clear A and the carry flag
  RL L                    // Now L=32*(Y-8*INT(Y/8)), and the carry flag is set if Willy is in the lower half of the cavern (Y>=8)
  ADC A,92                // H=92 or 93 (MSB of the address of Willy's location
  LD H,A                  // in the attribute buffer)
  LD A,(LOCATION)         // Pick up Willy's screen x-coordinate (1-29) from
  AND 31                  // bits 0-4 at LOCATION
  OR L                    // Now L holds the LSB of Willy's attribute buffer
  LD L,A                  // address
  LD (LOCATION),HL        // Store Willy's updated attribute buffer location at LOCATION
  RET
// Willy has just finished a jump.
MOVEWILLY_8:
  LD A,6                  // Set the airborne status indicator at AIRBORNE to 6:
  LD (AIRBORNE),A         // Willy will continue to fall unless he's landed on a wall or floor block
  RET
// Willy has just started falling.
MOVEWILLY_9:
  LD A,2                  // Set the airborne status indicator at AIRBORNE to 2
  LD (AIRBORNE),A
  RET
// The top-left or top-right cell of Willy's sprite is overlapping a wall tile.
MOVEWILLY_10:
  LD A,(PIXEL_Y)          // Adjust Willy's pixel y-coordinate at PIXEL_Y so
  ADD A,16                // that the top row of cells of his sprite is just
  AND 240                 // below the wall tile
  LD (PIXEL_Y),A
  CALL MOVEWILLY_7        // Adjust Willy's attribute buffer location at LOCATION to account for this new pixel y-coordinate
  LD A,2                  // Set the airborne status indicator at AIRBORNE to 2:
  LD (AIRBORNE),A         // Willy has started falling
  LD HL,DMFLAGS           // Reset bit 1 at DMFLAGS: Willy is not moving left or
  RES 1,(HL)              // right
  RET

// Animate a crumbling floor tile in the current cavern
//
// Used by the routine at MOVEWILLY.
//
// HL Address of the crumbling floor tile's location in the attribute buffer at
//    23552
CRUMBLE:
  LD C,L                  // Point BC at the bottom row of pixels of the
  LD A,H                  // crumbling floor tile in the screen buffer at 28672
  ADD A,27
  OR 7
  LD B,A
CRUMBLE_0:
  DEC B                   // Collect the pixels from the row above in A
  LD A,(BC)
  INC B                   // Copy these pixels into the row below it
  LD (BC),A
  DEC B                   // Point BC at the next row of pixels up
  LD A,B                  // Have we dealt with the bottom seven pixel rows of
  AND 7                   // the crumbling floor tile yet?
  JR NZ,CRUMBLE_0         // If not, jump back to deal with the next one up
  XOR A                   // Clear the top row of pixels in the crumbling floor
  LD (BC),A               // tile
  LD A,B                  // Point BC at the bottom row of pixels in the
  ADD A,7                 // crumbling floor tile
  LD B,A
  LD A,(BC)               // Pick up the bottom row of pixels in A
  OR A                    // Is the bottom row clear?
  RET NZ                  // Return if not
// The bottom row of pixels in the crumbling floor tile is clear. Time to put a
// background tile in its place.
  LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  INC H                   // Set HL to the address of the crumbling floor tile's
  INC H                   // location in the attribute buffer at 24064
  LD (HL),A               // Set the attribute at this location to that of the background tile
  DEC H                   // Set HL back to the address of the crumbling floor
  DEC H                   // tile's location in the attribute buffer at 23552
  RET

// Move Willy (2)
//
// Used by the routine at MOVEWILLY. This routine checks the keyboard and
// joystick, and moves Willy left or right if necessary.
//
// HL Attribute buffer address of the left-hand cell below Willy's sprite
MOVEWILLY2:
  LD A,(AIRBORNE)         // Pick up the airborne status indicator from AIRBORNE
  CP 12                   // Has Willy just landed after falling from too great a height?
  JP NC,KILLWILLY_0       // If so, kill him
  LD E,255                // Initialise E to 255 (all bits set); it will be used to hold keyboard and joystick readings
  XOR A                   // Reset the airborne status indicator at AIRBORNE
  LD (AIRBORNE),A         // (Willy has landed safely)
  LD A,(CONVEYOR)         // Pick up the attribute byte of the conveyor tile for the current cavern from CONVEYOR
  CP (HL)                 // Does the attribute byte of the left-hand cell below Willy's sprite match that of the conveyor tile?
  JR Z,MOVEWILLY2_0       // Jump if so
  INC HL                  // Point HL at the right-hand cell below Willy's sprite
  CP (HL)                 // Does the attribute byte of the right-hand cell below Willy's sprite match that of the conveyor tile?
  JR NZ,MOVEWILLY2_1      // Jump if not
MOVEWILLY2_0:
  LD A,(CONVDIR)          // Pick up the direction byte of the conveyor definition from CONVDIR (0=left, 1=right)
  SUB 3                   // Now E=253 (bit 1 reset) if the conveyor is moving
  LD E,A                  // left, or 254 (bit 0 reset) if it's moving right
MOVEWILLY2_1:
  LD BC,57342             // Read keys P-O-I-U-Y (right, left, right, left,
  IN A,(C)                // right) into bits 0-4 of A
  AND 31                  // Set bit 5 and reset bits 6 and 7
  OR 32
  AND E                   // Reset bit 0 if the conveyor is moving right, or bit 1 if it's moving left
  LD E,A                  // Save the result in E
  LD BC,64510             // Read keys Q-W-E-R-T (left, right, left, right,
  IN A,(C)                // left) into bits 0-4 of A
  AND 31                  // Keep only bits 0-4, shift them into bits 1-5, and
  RLC A                   // set bit 0
  OR 1
  AND E                   // Merge this keyboard reading into bits 1-5 of E
  LD E,A
  LD B,247                // Read keys 1-2-3-4-5 ('5' is left) into bits 0-4 of
  IN A,(C)                // A
  RRCA                    // Rotate the result right and set bits 0-2 and 4-7;
  OR 247                  // this ignores every key except '5' (left)
  AND E                   // Merge this reading of the '5' key into bit 3 of E
  LD E,A
  LD B,239                // Read keys 0-9-8-7-6 ('8' is right) into bits 0-4 of
  IN A,(C)                // A
  OR 251                  // Set bits 0, 1 and 3-7; this ignores every key except '8' (right)
  AND E                   // Merge this reading of the '8' key into bit 2 of E
  LD E,A
  LD A,(KEMP)             // Collect the Kempston joystick indicator from KEMP
  OR A                    // Is the joystick connected?
  JR Z,MOVEWILLY2_2       // Jump if not
  LD BC,31                // Collect input from the joystick
  IN A,(C)
  AND 3                   // Keep only bits 0 (right) and 1 (left) and flip them
  CPL
  AND E                   // Merge this reading of the joystick right and left
  LD E,A                  // buttons into bits 0 and 1 of E
// At this point, bits 0-5 in E indicate the direction in which Willy is being
// moved or trying to move. If bit 0, 2 or 4 is reset, Willy is being moved or
// trying to move right; if bit 1, 3 or 5 is reset, Willy is being moved or
// trying to move left.
MOVEWILLY2_2:
  LD C,0                  // Initialise C to 0 (no movement)
  LD A,E                  // Copy the movement bits into A
  AND 42                  // Keep only bits 1, 3 and 5 (the 'left' bits)
  CP 42                   // Are any of these bits reset?
  JR Z,MOVEWILLY2_3       // Jump if not
  LD C,4                  // Set bit 2 of C: Willy is moving left
MOVEWILLY2_3:
  LD A,E                  // Copy the movement bits into A
  AND 21                  // Keep only bits 0, 2 and 4 (the 'right' bits)
  CP 21                   // Are any of these bits reset?
  JR Z,MOVEWILLY2_4       // Jump if not
  SET 3,C                 // Set bit 3 of C: Willy is moving right
MOVEWILLY2_4:
  LD A,(DMFLAGS)          // Pick up Willy's direction and movement flags from DMFLAGS
  ADD A,C                 // Point HL at the entry in the left-right movement
  LD C,A                  // table at LRMOVEMENT that corresponds to the
  LD B,0                  // direction Willy is facing, and the direction in
  LD HL,LRMOVEMENT        // which he is being moved or trying to move
  ADD HL,BC
  LD A,(HL)               // Update Willy's direction and movement flags at
  LD (DMFLAGS),A          // DMFLAGS with the entry from the left-right movement table
// That is left-right movement taken care of. Now check the jump keys.
  LD BC,32510             // Read keys SHIFT-Z-X-C-V and B-N-M-SS-SPACE
  IN A,(C)
  AND 31                  // Are any of these keys being pressed?
  CP 31
  JR NZ,MOVEWILLY2_5      // Jump if so
  LD B,239                // Read keys 0-9-8-7-6 into bits 0-4 of A
  IN A,(C)
  AND 9                   // Keep only bits 0 (the '0' key) and 3 (the '7' key)
  CP 9                    // Is '0' or '7' being pressed?
  JR NZ,MOVEWILLY2_5      // Jump if so
  LD A,(KEMP)             // Collect the Kempston joystick indicator from KEMP
  OR A                    // Is the joystick connected?
  JR Z,MOVEWILLY2_6       // Jump if not
  LD BC,31                // Collect input from the joystick
  IN A,(C)
  BIT 4,A                 // Is the fire button being pressed?
  JR Z,MOVEWILLY2_6       // Jump if not
// A jump key or the fire button is being pressed. Time to make Willy jump.
MOVEWILLY2_5:
  XOR A                   // Initialise the jumping animation counter at JUMPING
  LD (JUMPING),A
  INC A                   // Set the airborne status indicator at AIRBORNE to 1:
  LD (AIRBORNE),A         // Willy is jumping
// This entry point is used by the routine at MOVEWILLY.
MOVEWILLY2_6:
  LD A,(DMFLAGS)          // Pick up Willy's direction and movement flags from DMFLAGS
  AND 2                   // Is Willy moving?
  RET Z                   // Return if not
  LD A,(DMFLAGS)          // Pick up Willy's direction and movement flags from DMFLAGS
  AND 1                   // Is Willy facing right?
  JP Z,MOVEWILLY2_9       // Jump if so
// Willy is moving left.
  LD A,(FRAME)            // Pick up Willy's animation frame from FRAME
  OR A                    // Is it 0?
  JR Z,MOVEWILLY2_7       // If so, jump to move Willy's sprite left across a cell boundary
  DEC A                   // Decrement Willy's animation frame at FRAME
  LD (FRAME),A
  RET
// Willy's sprite is moving left across a cell boundary. In the comments that
// follow, (x,y) refers to the coordinates of the top-left cell currently
// occupied by Willy's sprite.
MOVEWILLY2_7:
  LD HL,(LOCATION)        // Collect Willy's attribute buffer coordinates from LOCATION
  DEC HL                  // Point HL at the cell at (x-1,y+1)
  LD DE,32
  ADD HL,DE
  LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  RET Z                   // Return if so without moving Willy (his path is blocked)
  LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  AND 15                  // Does Willy's sprite currently occupy only two rows of cells?
  JR Z,MOVEWILLY2_8       // Jump if so
  LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  ADD HL,DE               // Point HL at the cell at (x-1,y+2)
  CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  RET Z                   // Return if so without moving Willy (his path is blocked)
  OR A                    // Clear the carry flag for subtraction
  SBC HL,DE               // Point HL at the cell at (x-1,y+1)
MOVEWILLY2_8:
  LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  OR A                    // Clear the carry flag for subtraction
  SBC HL,DE               // Point HL at the cell at (x-1,y)
  CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  RET Z                   // Return if so without moving Willy (his path is blocked)
  LD (LOCATION),HL        // Save Willy's new attribute buffer coordinates (in HL) at LOCATION
  LD A,3                  // Change Willy's animation frame at FRAME from 0 to 3
  LD (FRAME),A
  RET
// Willy is moving right.
MOVEWILLY2_9:
  LD A,(FRAME)            // Pick up Willy's animation frame from FRAME
  CP 3                    // Is it 3?
  JR Z,MOVEWILLY2_10      // If so, jump to move Willy's sprite right across a cell boundary
  INC A                   // Increment Willy's animation frame at FRAME
  LD (FRAME),A
  RET
// Willy's sprite is moving right across a cell boundary. In the comments that
// follow, (x,y) refers to the coordinates of the top-left cell currently
// occupied by Willy's sprite.
MOVEWILLY2_10:
  LD HL,(LOCATION)        // Collect Willy's attribute buffer coordinates from LOCATION
  INC HL                  // Point HL at the cell at (x+2,y)
  INC HL
  LD DE,32                // Prepare DE for addition
  LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  ADD HL,DE               // Point HL at the cell at (x+2,y+1)
  CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  RET Z                   // Return if so without moving Willy (his path is blocked)
  LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  AND 15                  // Does Willy's sprite currently occupy only two rows of cells?
  JR Z,MOVEWILLY2_11      // Jump if so
  LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  ADD HL,DE               // Point HL at the cell at (x+2,y+2)
  CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  RET Z                   // Return if so without moving Willy (his path is blocked)
  OR A                    // Clear the carry flag for subtraction
  SBC HL,DE               // Point HL at the cell at (x+2,y+1)
MOVEWILLY2_11:
  LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  OR A                    // Clear the carry flag for subtraction
  SBC HL,DE               // Point HL at the cell at (x+2,y)
  CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  RET Z                   // Return if so without moving Willy (his path is blocked)
  DEC HL                  // Point HL at the cell at (x+1,y)
  LD (LOCATION),HL        // Save Willy's new attribute buffer coordinates (in HL) at LOCATION
  XOR A                   // Change Willy's animation frame at FRAME from 3 to 0
  LD (FRAME),A
  RET

// Kill Willy
//
// Used by the routine at WILLYATTR when Willy hits a nasty.
KILLWILLY:
  POP HL                  // Drop the return address from the stack
// This entry point is used by the routines at MOVEWILLY2 (when Willy lands
// after falling from too great a height), DRAWHG (when Willy collides with a
// horizontal guardian), EUGENE (when Willy collides with Eugene), VGUARDIANS
// (when Willy collides with a vertical guardian) and KONGBEAST (when Willy
// collides with the Kong Beast).
KILLWILLY_0:
  POP HL                  // Drop the return address from the stack
// This entry point is used by the routine at SKYLABS when a Skylab falls on
// Willy.
KILLWILLY_1:
  LD A,255                // Set the airborne status indicator at AIRBORNE to
  LD (AIRBORNE),A         // 255 (meaning Willy has had a fatal accident)
  JP LOOP_4               // Jump back into the main loop

// Move the horizontal guardians in the current cavern
//
// Used by the routine at LOOP.
MOVEHG:
  LD IY,HGUARDS           // Point IY at the first byte of the first horizontal guardian definition at HGUARDS
  LD DE,7                 // Prepare DE for addition (there are 7 bytes in a guardian definition)
// The guardian-moving loop begins here.
MOVEHG_0:
  LD A,(IY+0)             // Pick up the first byte of the guardian definition
  CP 255                  // Have we dealt with all the guardians yet?
  RET Z                   // Return if so
  OR A                    // Is this guardian definition blank?
  JR Z,MOVEHG_6           // If so, skip it and consider the next one
  LD A,(CLOCK)            // Pick up the value of the game clock at CLOCK
  AND 4                   // Move bit 2 (which is toggled on each pass through
  RRCA                    // the main loop) to bit 7 and clear all the other
  RRCA                    // bits
  RRCA
  AND (IY+0)              // Combine this bit with bit 7 of the first byte of the guardian definition, which specifies the guardian's animation speed: 0=normal, 1=slow
  JR NZ,MOVEHG_6          // Jump to consider the next guardian if this one is not due to be moved on this pass
// The guardian will be moved on this pass.
  LD A,(IY+4)             // Pick up the current animation frame (0-7)
  CP 3                    // Is it 3 (the terminal frame for a guardian moving right)?
  JR Z,MOVEHG_2           // Jump if so to move the guardian right across a cell boundary or turn it round
  CP 4                    // Is the current animation frame 4 (the terminal frame for a guardian moving left)?
  JR Z,MOVEHG_4           // Jump if so to move the guardian left across a cell boundary or turn it round
  JR NC,MOVEHG_1          // Jump if the animation frame is 5, 6 or 7
  INC (IY+4)              // Increment the animation frame (this guardian is moving right)
  JR MOVEHG_6             // Jump forward to consider the next guardian
MOVEHG_1:
  DEC (IY+4)              // Decrement the animation frame (this guardian is moving left)
  JR MOVEHG_6             // Jump forward to consider the next guardian
MOVEHG_2:
  LD A,(IY+1)             // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552
  CP (IY+6)               // Has the guardian reached the rightmost point in its path?
  JR NZ,MOVEHG_3          // Jump if not
  LD (IY+4),7             // Set the animation frame to 7 (turning the guardian round to face left)
  JR MOVEHG_6             // Jump forward to consider the next guardian
MOVEHG_3:
  LD (IY+4),0             // Set the animation frame to 0 (the initial frame for a guardian moving right)
  INC (IY+1)              // Increment the guardian's x-coordinate (moving it right across a cell boundary)
  JR MOVEHG_6             // Jump forward to consider the next guardian
MOVEHG_4:
  LD A,(IY+1)             // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552
  CP (IY+5)               // Has the guardian reached the leftmost point in its path?
  JR NZ,MOVEHG_5          // Jump if not
  LD (IY+4),0             // Set the animation frame to 0 (turning the guardian round to face right)
  JR MOVEHG_6             // Jump forward to consider the next guardian
MOVEHG_5:
  LD (IY+4),7             // Set the animation frame to 7 (the initial frame for a guardian moving left)
  DEC (IY+1)              // Decrement the guardian's x-coordinate (moving it left across a cell boundary)
// The current guardian definition has been dealt with. Time for the next one.
MOVEHG_6:
  ADD IY,DE               // Point IY at the first byte of the next horizontal guardian definition
  JR MOVEHG_0             // Jump back to deal with the next horizontal guardian

// Move and draw the light beam in Solar Power Generator
//
// Used by the routine at LOOP.
LIGHTBEAM:
  LD HL,23575             // Point HL at the cell at (0,23) in the attribute buffer at 23552 (the source of the light beam)
  LD DE,32                // Prepare DE for addition (the beam travels vertically downwards to start with)
// The beam-drawing loop begins here.
LIGHTBEAM_0:
  LD A,(FLOOR)            // Pick up the attribute byte of the floor tile for the cavern from FLOOR
  CP (HL)                 // Does HL point at a floor tile?
  RET Z                   // Return if so (the light beam stops here)
  LD A,(WALL)             // Pick up the attribute byte of the wall tile for the cavern from WALL
  CP (HL)                 // Does HL point at a wall tile?
  RET Z                   // Return if so (the light beam stops here)
  LD A,39                 // A=39 (INK 7: PAPER 4)
  CP (HL)                 // Does HL point at a tile with this attribute value?
  JR NZ,LIGHTBEAM_1       // Jump if not (the light beam is not touching Willy)
  EXX                     // Switch to the shadow registers briefly (to preserve DE and HL)
  CALL DECAIR             // Decrease the air supply by four units
  CALL DECAIR
  CALL DECAIR
  CALL DECAIR
  EXX                     // Switch back to the normal registers (restoring DE and HL)
  JR LIGHTBEAM_2          // Jump forward to draw the light beam over Willy
LIGHTBEAM_1:
  LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the cavern from BACKGROUND
  CP (HL)                 // Does HL point at a background tile?
  JR Z,LIGHTBEAM_2        // Jump if so (the light beam will not be reflected at this point)
  LD A,E                  // Toggle the value in DE between 32 and -1 (and
  XOR 223                 // therefore the direction of the light beam between
  LD E,A                  // vertically downwards and horizontally to the left):
  LD A,D                  // the light beam has hit a guardian
  CPL
  LD D,A
LIGHTBEAM_2:
  LD (HL),119             // Draw a portion of the light beam with attribute value 119 (INK 7: PAPER 6: BRIGHT 1)
  ADD HL,DE               // Point HL at the cell where the next portion of the light beam will be drawn
  JR LIGHTBEAM_0          // Jump back to draw the next portion of the light beam

// Draw the horizontal guardians in the current cavern
//
// Used by the routine at LOOP.
DRAWHG:
  LD IY,HGUARDS           // Point IY at the first byte of the first horizontal guardian definition at HGUARDS
// The guardian-drawing loop begins here.
DRAWHG_0:
  LD A,(IY+0)             // Pick up the first byte of the guardian definition
  CP 255                  // Have we dealt with all the guardians yet?
  RET Z                   // Return if so
  OR A                    // Is this guardian definition blank?
  JR Z,DRAWHG_2           // If so, skip it and consider the next one
  LD DE,31                // Prepare DE for addition
  LD L,(IY+1)             // Point HL at the address of the guardian's location
  LD H,(IY+2)             // in the attribute buffer at 23552
  AND 127                 // Reset bit 7 (which specifies the animation speed) of the attribute byte, ensuring no FLASH
  LD (HL),A               // Set the attribute bytes for the guardian in the
  INC HL                  // buffer at 23552
  LD (HL),A
  ADD HL,DE
  LD (HL),A
  INC HL
  LD (HL),A
  LD C,1                  // Prepare C for the call to the drawing routine at DRWFIX later on
  LD A,(IY+4)             // Pick up the animation frame (0-7)
  RRCA                    // Multiply it by 32
  RRCA
  RRCA
  LD E,A                  // Copy the result to E
  LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
  CP 7                    // Are we in one of the first seven caverns?
  JR C,DRAWHG_1           // Jump if so
  CP 9                    // Are we in The Endorian Forest?
  JR Z,DRAWHG_1           // Jump if so
  CP 15                   // Are we in The Sixteenth Cavern?
  JR Z,DRAWHG_1           // Jump if so
  SET 7,E                 // Add 128 to E (the horizontal guardians in this cavern use frames 4-7 only)
DRAWHG_1:
  LD D,129                // Point DE at the graphic data for the appropriate guardian sprite (at GGDATA+E)
  LD L,(IY+1)             // Point HL at the address of the guardian's location
  LD H,(IY+3)             // in the screen buffer at 24576
  CALL DRWFIX             // Draw the guardian to the screen buffer at 24576
  JP NZ,KILLWILLY_0       // Kill Willy if the guardian collided with him
// The current guardian definition has been dealt with. Time for the next one.
DRAWHG_2:
  LD DE,7                 // Point IY at the first byte of the next horizontal
  ADD IY,DE               // guardian definition
  JR DRAWHG_0             // Jump back to deal with the next horizontal guardian

// Move and draw Eugene in Eugene's Lair
//
// Used by the routine at LOOP. First we move Eugene up or down, or change his
// direction.
EUGENE:
  LD A,(ITEMATTR)         // Pick up the attribute of the last item drawn from ITEMATTR
  OR A                    // Have all the items been collected?
  JR Z,EUGENE_0           // Jump if so
  LD A,(EUGDIR)           // Pick up Eugene's direction from EUGDIR
  OR A                    // Is Eugene moving downwards?
  JR Z,EUGENE_0           // Jump if so
  LD A,(EUGHGT)           // Pick up Eugene's pixel y-coordinate from EUGHGT
  DEC A                   // Decrement it (moving Eugene up)
  JR Z,EUGENE_1           // Jump if Eugene has reached the top of the cavern
  LD (EUGHGT),A           // Update Eugene's pixel y-coordinate at EUGHGT
  JR EUGENE_2
EUGENE_0:
  LD A,(EUGHGT)           // Pick up Eugene's pixel y-coordinate from EUGHGT
  INC A                   // Increment it (moving Eugene down)
  CP 88                   // Has Eugene reached the portal yet?
  JR Z,EUGENE_1           // Jump if so
  LD (EUGHGT),A           // Update Eugene's pixel y-coordinate at EUGHGT
  JR EUGENE_2
EUGENE_1:
  LD A,(EUGDIR)           // Toggle Eugene's direction at EUGDIR
  XOR 1
  LD (EUGDIR),A
// Now that Eugene's movement has been dealt with, it's time to draw him.
EUGENE_2:
  LD A,(EUGHGT)           // Pick up Eugene's pixel y-coordinate from EUGHGT
  AND 127                 // Point DE at the entry in the screen buffer address
  RLCA                    // lookup table at SBUFADDRS that corresponds to
  LD E,A                  // Eugene's y-coordinate
  LD D,131
  LD A,(DE)               // Point HL at the address of Eugene's location in the
  OR 15                   // screen buffer at 24576
  LD L,A
  INC DE
  LD A,(DE)
  LD H,A
  LD DE,32992             // Draw Eugene to the screen buffer at 24576
  LD C,1
  CALL DRWFIX
  JP NZ,KILLWILLY_0       // Kill Willy if Eugene collided with him
  LD A,(EUGHGT)           // Pick up Eugene's pixel y-coordinate from EUGHGT
  AND 120                 // Point HL at the address of Eugene's location in the
  RLCA                    // attribute buffer at 23552
  OR 7
  SCF
  RL A
  LD L,A
  LD A,0
  ADC A,92
  LD H,A
  LD A,(ITEMATTR)         // Pick up the attribute of the last item drawn from ITEMATTR
  OR A                    // Set the zero flag if all the items have been collected
  LD A,7                  // Assume we will draw Eugene with white INK
  JR NZ,EUGENE_3          // Jump if there are items remaining to be collected
  LD A,(CLOCK)            // Pick up the value of the game clock at CLOCK
  RRCA                    // Move bits 2-4 into bits 0-2 and clear the other
  RRCA                    // bits; this value (which decreases by one on each
  AND 7                   // pass through the main loop) will be Eugene's INK colour
// This entry point is used by the routines at SKYLABS (to set the attributes
// for a Skylab), VGUARDIANS (to set the attributes for a vertical guardian) and
// KONGBEAST (to set the attributes for the Kong Beast).
EUGENE_3:
  LD (HL),A               // Save the INK colour in the attribute buffer temporarily
  LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  AND 248                 // Combine its PAPER colour with the chosen INK colour
  OR (HL)
  LD (HL),A               // Set the attribute byte for the top-left cell of the sprite in the attribute buffer at 23552
  LD DE,31                // Prepare DE for addition
  INC HL                  // Set the attribute byte for the top-right cell of
  LD (HL),A               // the sprite in the attribute buffer at 23552
  ADD HL,DE               // Set the attribute byte for the middle-left cell of
  LD (HL),A               // the sprite in the attribute buffer at 23552
  INC HL                  // Set the attribute byte for the middle-right cell of
  LD (HL),A               // the sprite in the attribute buffer at 23552
  ADD HL,DE               // Set the attribute byte for the bottom-left cell of
  LD (HL),A               // the sprite in the attribute buffer at 23552
  INC HL                  // Set the attribute byte for the bottom-right cell of
  LD (HL),A               // the sprite in the attribute buffer at 23552
  RET

// Move and draw the Skylabs in Skylab Landing Bay
//
// Used by the routine at LOOP.
SKYLABS:
  LD IY,VGUARDS           // Point IY at the first byte of the first vertical guardian definition at VGUARDS
// The Skylab-moving loop begins here.
SKYLABS_0:
  LD A,(IY+0)             // Pick up the first byte of the guardian definition
  CP 255                  // Have we dealt with all the Skylabs yet?
  JP Z,LOOP_3             // If so, re-enter the main loop
  LD A,(IY+2)             // Pick up the Skylab's pixel y-coordinate
  CP (IY+6)               // Has it reached its crash site yet?
  JR NC,SKYLABS_1         // Jump if so
  ADD A,(IY+4)            // Increment the Skylab's y-coordinate (moving it
  LD (IY+2),A             // downwards)
  JR SKYLABS_2
// The Skylab has reached its crash site. Start or continue its disintegration.
SKYLABS_1:
  INC (IY+1)              // Increment the animation frame
  LD A,(IY+1)             // Pick up the animation frame
  CP 8                    // Has the Skylab completely disintegrated yet?
  JR NZ,SKYLABS_2         // Jump if not
  LD A,(IY+5)             // Reset the Skylab's pixel y-coordinate
  LD (IY+2),A
  LD A,(IY+3)             // Add 8 to the Skylab's x-coordinate (wrapping around
  ADD A,8                 // at the right side of the screen)
  AND 31
  LD (IY+3),A
  LD (IY+1),0             // Reset the animation frame to 0
// Now that the Skylab's movement has been dealt with, time to draw it.
SKYLABS_2:
  LD E,(IY+2)             // Pick up the Skylab's pixel y-coordinate in E
  RLC E                   // Point DE at the entry in the screen buffer address
  LD D,131                // lookup table at SBUFADDRS that corresponds to the Skylab's pixel y-coordinate
  LD A,(DE)               // Point HL at the address of the Skylab's location in
  ADD A,(IY+3)            // the screen buffer at 24576
  LD L,A
  INC DE
  LD A,(DE)
  LD H,A
  LD A,(IY+1)             // Pick up the animation frame (0-7)
  RRCA                    // Multiply it by 32
  RRCA
  RRCA
  LD E,A                  // Point DE at the graphic data for the corresponding
  LD D,129                // Skylab sprite (at GGDATA+A)
  LD C,1                  // Draw the Skylab to the screen buffer at 24576
  CALL DRWFIX
  JP NZ,KILLWILLY_1       // Kill Willy if the Skylab collided with him
  LD A,(IY+2)             // Point HL at the address of the Skylab's location in
  AND 64                  // the attribute buffer at 23552
  RLCA
  RLCA
  ADD A,92
  LD H,A
  LD A,(IY+2)
  RLCA
  RLCA
  AND 224
  OR (IY+3)
  LD L,A
  LD A,(IY+0)             // Pick up the Skylab's attribute byte
  CALL EUGENE_3           // Set the attribute bytes for the Skylab
// The current guardian definition has been dealt with. Time for the next one.
  LD DE,7                 // Point IY at the first byte of the next vertical
  ADD IY,DE               // guardian definition
  JR SKYLABS_0            // Jump back to deal with the next Skylab

// Move and draw the vertical guardians in the current cavern
//
// Used by the routine at LOOP.
VGUARDIANS:
  LD IY,VGUARDS           // Point IY at the first byte of the first vertical guardian definition at VGUARDS
// The guardian-moving loop begins here.
VGUARDIANS_0:
  LD A,(IY+0)             // Pick up the first byte of the guardian definition
  CP 255                  // Have we dealt with all the guardians yet?
  RET Z                   // Return if so
  INC (IY+1)              // Increment the guardian's animation frame
  RES 2,(IY+1)            // Reset the animation frame to 0 if it overflowed to 4
  LD A,(IY+2)             // Pick up the guardian's pixel y-coordinate
  ADD A,(IY+4)            // Add the current y-coordinate increment
  CP (IY+5)               // Has the guardian reached the highest point of its path (minimum y-coordinate)?
  JR C,VGUARDIANS_1       // If so, jump to change its direction of movement
  CP (IY+6)               // Has the guardian reached the lowest point of its path (maximum y-coordinate)?
  JR NC,VGUARDIANS_1      // If so, jump to change its direction of movement
  LD (IY+2),A             // Update the guardian's pixel y-coordinate
  JR VGUARDIANS_2
VGUARDIANS_1:
  LD A,(IY+4)             // Negate the y-coordinate increment; this changes the
  NEG                     // guardian's direction of movement
  LD (IY+4),A
// Now that the guardian's movement has been dealt with, time to draw it.
VGUARDIANS_2:
  LD A,(IY+2)             // Pick up the guardian's pixel y-coordinate
  AND 127                 // Point DE at the entry in the screen buffer address
  RLCA                    // lookup table at SBUFADDRS that corresponds to the
  LD E,A                  // guardian's pixel y-coordinate
  LD D,131
  LD A,(DE)               // Point HL at the address of the guardian's location
  OR (IY+3)               // in the screen buffer at 24576
  LD L,A
  INC DE
  LD A,(DE)
  LD H,A
  LD A,(IY+1)             // Pick up the guardian's animation frame (0-3)
  RRCA                    // Multiply it by 32
  RRCA
  RRCA
  LD E,A                  // Point DE at the graphic data for the appropriate
  LD D,129                // guardian sprite (at GGDATA+A)
  LD C,1                  // Draw the guardian to the screen buffer at 24576
  CALL DRWFIX
  JP NZ,KILLWILLY_0       // Kill Willy if the guardian collided with him
  LD A,(IY+2)             // Pick up the guardian's pixel y-coordinate
  AND 64                  // Point HL at the address of the guardian's location
  RLCA                    // in the attribute buffer at 23552
  RLCA
  ADD A,92
  LD H,A
  LD A,(IY+2)
  RLCA
  RLCA
  AND 224
  OR (IY+3)
  LD L,A
  LD A,(IY+0)             // Pick up the guardian's attribute byte
  CALL EUGENE_3           // Set the attribute bytes for the guardian
// The current guardian definition has been dealt with. Time for the next one.
  LD DE,7                 // Point IY at the first byte of the next vertical
  ADD IY,DE               // guardian definition
  JR VGUARDIANS_0         // Jump back to deal with the next vertical guardian

// Draw the items in the current cavern and collect any that Willy is touching
//
// Used by the routine at LOOP.
DRAWITEMS:
  XOR A                   // Initialise the attribute of the last item drawn at
  LD (ITEMATTR),A         // ITEMATTR to 0 (in case there are no items left to draw)
  LD IY,ITEMS             // Point IY at the first byte of the first item definition at ITEMS
// The item-drawing loop begins here.
DRAWITEMS_0:
  LD A,(IY+0)             // Pick up the first byte of the item definition
  CP 255                  // Have we dealt with all the items yet?
  JR Z,DRAWITEMS_3        // Jump if so
  OR A                    // Has this item already been collected?
  JR Z,DRAWITEMS_2        // If so, skip it and consider the next one
  LD E,(IY+1)             // Point DE at the address of the item's location in
  LD D,(IY+2)             // the attribute buffer at 23552
  LD A,(DE)               // Pick up the current attribute byte at the item's location
  AND 7                   // Is the INK white (which happens if Willy is
  CP 7                    // touching the item)?
  JR NZ,DRAWITEMS_1       // Jump if not
// Willy is touching this item, so add it to his collection.
  LD HL,33836             // Add 100 to the score
  CALL INCSCORE_0
  LD (IY+0),0             // Set the item's attribute byte to 0 so that it will be skipped the next time
  JR DRAWITEMS_2          // Jump forward to consider the next item
// This item has not been collected yet.
DRAWITEMS_1:
  LD A,(IY+0)             // Pick up the item's current attribute byte
  AND 248                 // Keep the BRIGHT and PAPER bits, and set the INK to
  OR 3                    // 3 (magenta)
  LD B,A                  // Store this value in B
  LD A,(IY+0)             // Pick up the item's current attribute byte again
  AND 3                   // Keep only bits 0 and 1 and add the value in B; this
  ADD A,B                 // maintains the BRIGHT and PAPER bits, and cycles the INK colour through 3, 4, 5 and 6
  LD (IY+0),A             // Store the new attribute byte
  LD (DE),A               // Update the attribute byte at the item's location in the buffer at 23552
  LD (ITEMATTR),A         // Store the new attribute byte at ITEMATTR as well
  LD D,(IY+3)             // Point DE at the address of the item's location in the screen buffer at 24576
  LD HL,ITEM              // Point HL at the item graphic for the current cavern (at ITEM)
  LD B,8                  // There are eight pixel rows to copy
  CALL PRINTCHAR_0        // Draw the item to the screen buffer at 24576
// The current item definition has been dealt with. Time for the next one.
DRAWITEMS_2:
  INC IY                  // Point IY at the first byte of the next item
  INC IY                  // definition
  INC IY
  INC IY
  INC IY
  JR DRAWITEMS_0          // Jump back to deal with the next item
// All the items have been dealt with. Check whether there were any left.
DRAWITEMS_3:
  LD A,(ITEMATTR)         // Pick up the attribute of the last item drawn at ITEMATTR
  OR A                    // Were any items drawn?
  RET NZ                  // Return if so (some remain to be collected)
  LD HL,PORTAL            // Ensure that the portal is flashing by setting bit 7
  SET 7,(HL)              // of its attribute byte at PORTAL
  RET

// Draw the portal, or move to the next cavern if Willy has entered it
//
// Used by the routine at LOOP. First check whether Willy has entered the
// portal.
CHKPORTAL:
  LD HL,(PORTALLOC1)      // Pick up the address of the portal's location in the attribute buffer at 23552 from PORTALLOC1
  LD A,(LOCATION)         // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION
  CP L                    // Does it match that of the portal?
  JR NZ,CHKPORTAL_0       // Jump if not
  LD A,(32877)            // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877
  CP H                    // Does it match that of the portal?
  JR NZ,CHKPORTAL_0       // Jump if not
  LD A,(PORTAL)           // Pick up the portal's attribute byte from PORTAL
  BIT 7,A                 // Is the portal flashing?
  JR Z,CHKPORTAL_0        // Jump if not
  POP HL                  // Drop the return address from the stack
  JP NXSHEET              // Move Willy to the next cavern
// Willy has not entered the portal, or it's not flashing, so just draw it.
CHKPORTAL_0:
  LD A,(PORTAL)           // Pick up the portal's attribute byte from PORTAL
  LD (HL),A               // Set the attribute bytes for the portal in the
  INC HL                  // buffer at 23552
  LD (HL),A
  LD DE,31
  ADD HL,DE
  LD (HL),A
  INC HL
  LD (HL),A
  LD DE,PORTALG           // Point DE at the graphic data for the portal at PORTALG
  LD HL,(PORTALLOC2)      // Pick up the address of the portal's location in the screen buffer at 24576 from PORTALLOC2
  LD C,0                  // C=0: overwrite mode
// This routine continues into the one at DRWFIX.

// Draw a sprite
//
// Used by the routines at START (to draw Willy on the title screen), LOOP (to
// draw the remaining lives), ENDGAM (to draw Willy, the boot and the plinth
// during the game over sequence), DRAWHG (to draw horizontal guardians), EUGENE
// (to draw Eugene in Eugene's Lair), SKYLABS (to draw the Skylabs in Skylab
// Landing Bay), VGUARDIANS (to draw vertical guardians), CHKPORTAL (to draw the
// portal in the current cavern), NXSHEET (to draw Willy above ground and the
// swordfish graphic over the portal in The Final Barrier) and KONGBEAST (to
// draw the Kong Beast in Miner Willy meets the Kong Beast and Return of the
// Alien Kong Beast). If C=1 on entry, this routine returns with the zero flag
// reset if any of the set bits in the sprite being drawn collides with a set
// bit in the background.
//
// C Drawing mode: 0 (overwrite) or 1 (blend)
// DE Address of sprite graphic data
// HL Address to draw at
DRWFIX:
  LD B,16                 // There are 16 rows of pixels to draw
DRWFIX_0:
  BIT 0,C                 // Set the zero flag if we're in overwrite mode
  LD A,(DE)               // Pick up a sprite graphic byte
  JR Z,DRWFIX_1           // Jump if we're in overwrite mode
  AND (HL)                // Return with the zero flag reset if any of the set
  RET NZ                  // bits in the sprite graphic byte collide with a set bit in the background (e.g. in Willy's sprite)
  LD A,(DE)               // Pick up the sprite graphic byte again
  OR (HL)                 // Blend it with the background byte
DRWFIX_1:
  LD (HL),A               // Copy the graphic byte to its destination cell
  INC L                   // Move HL along to the next cell on the right
  INC DE                  // Point DE at the next sprite graphic byte
  BIT 0,C                 // Set the zero flag if we're in overwrite mode
  LD A,(DE)               // Pick up a sprite graphic byte
  JR Z,DRWFIX_2           // Jump if we're in overwrite mode
  AND (HL)                // Return with the zero flag reset if any of the set
  RET NZ                  // bits in the sprite graphic byte collide with a set bit in the background (e.g. in Willy's sprite)
  LD A,(DE)               // Pick up the sprite graphic byte again
  OR (HL)                 // Blend it with the background byte
DRWFIX_2:
  LD (HL),A               // Copy the graphic byte to its destination cell
  DEC L                   // Move HL to the next pixel row down in the cell on
  INC H                   // the left
  INC DE                  // Point DE at the next sprite graphic byte
  LD A,H                  // Have we drawn the bottom pixel row in this pair of
  AND 7                   // cells yet?
  JR NZ,DRWFIX_3          // Jump if not
  LD A,H                  // Otherwise move HL to the top pixel row in the cell
  SUB 8                   // below
  LD H,A
  LD A,L
  ADD A,32
  LD L,A
  AND 224                 // Was the last pair of cells at y-coordinate 7 or 15?
  JR NZ,DRWFIX_3          // Jump if not
  LD A,H                  // Otherwise adjust HL to account for the movement
  ADD A,8                 // from the top or middle third of the screen to the
  LD H,A                  // next one down
DRWFIX_3:
  DJNZ DRWFIX_0           // Jump back until all 16 rows of pixels have been drawn
  XOR A                   // Set the zero flag (to indicate no collision)
  RET

// Move to the next cavern
//
// Used by the routines at LOOP and CHKPORTAL.
NXSHEET:
  LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
  INC A                   // Increment the cavern number
  CP 20                   // Is the current cavern The Final Barrier?
  JR NZ,NXSHEET_3         // Jump if not
  LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  OR A                    // Are we in demo mode?
  JP NZ,NXSHEET_2         // Jump if so
  LD A,(CHEAT)            // Pick up the 6031769 key counter from CHEAT
  CP 7                    // Is cheat mode activated?
  JR Z,NXSHEET_2          // Jump if so
// Willy has made it through The Final Barrier without cheating.
  LD C,0                  // Draw Willy at (2,19) on the ground above the portal
  LD DE,WILLYR3
  LD HL,16467
  CALL DRWFIX
  LD DE,SWORDFISH         // Draw the swordfish graphic (see SWORDFISH) over the
  LD HL,16563             // portal
  CALL DRWFIX
  LD HL,22611             // Point HL at (2,19) in the attribute file
  LD DE,31                // Prepare DE for addition
  LD (HL),47              // Set the attributes for the upper half of Willy's
  INC HL                  // sprite at (2,19) and (2,20) to 47 (INK 7: PAPER 5)
  LD (HL),47
  ADD HL,DE               // Set the attributes for the lower half of Willy's
  LD (HL),39              // sprite at (3,19) and (3,20) to 39 (INK 7: PAPER 4)
  INC HL
  LD (HL),39
  ADD HL,DE               // Point HL at (5,19) in the attribute file
  INC HL
  ADD HL,DE
  LD (HL),69              // Set the attributes for the fish at (5,19) and
  INC HL                  // (5,20) to 69 (INK 5: PAPER 0: BRIGHT 1)
  LD (HL),69
  ADD HL,DE               // Set the attribute for the handle of the sword at
  LD (HL),70              // (6,19) to 70 (INK 6: PAPER 0: BRIGHT 1)
  INC HL                  // Set the attribute for the blade of the sword at
  LD (HL),71              // (6,20) to 71 (INK 7: PAPER 0: BRIGHT 1)
  ADD HL,DE               // Set the attributes at (7,19) and (7,20) to 0 (to
  LD (HL),0               // hide Willy's feet just below where the portal was)
  INC HL
  LD (HL),0
  LD BC,0                 // Prepare C and D for the celebratory sound effect
  LD D,50
  XOR A                   // A=0 (black border)
NXSHEET_0:
  OUT (254),A             // Produce the celebratory sound effect: Willy has
  XOR 24                  // escaped from the mine
  LD E,A
  LD A,C
  ADD A,D
  ADD A,D
  ADD A,D
  LD B,A
  LD A,E
NXSHEET_1:
  DJNZ NXSHEET_1
  DEC C
  JR NZ,NXSHEET_0
  DEC D
  JR NZ,NXSHEET_0
NXSHEET_2:
  XOR A                   // A=0 (the next cavern will be Central Cavern)
NXSHEET_3:
  LD (SHEET),A            // Update the cavern number at SHEET
// The next section of code cycles the INK and PAPER colours of the current
// cavern.
  LD A,63                 // Initialise A to 63 (INK 7: PAPER 7)
NXSHEET_4:
  LD HL,22528             // Set the attributes for the top two-thirds of the
  LD DE,22529             // screen to the value in A
  LD BC,511
  LD (HL),A
  LDIR
  LD BC,4                 // Pause for about 0.004s
NXSHEET_5:
  DJNZ NXSHEET_5
  DEC C
  JR NZ,NXSHEET_5
  DEC A                   // Decrement the attribute value in A
  JR NZ,NXSHEET_4         // Jump back until we've gone through all attribute values from 63 down to 1
  LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  OR A                    // Are we in demo mode?
  JP NZ,NEWSHT            // If so, demo the next cavern
// The following loop increases the score and decreases the air supply until it
// runs out.
NXSHEET_6:
  CALL DECAIR             // Decrease the air remaining in the current cavern
  JP Z,NEWSHT             // Move to the next cavern if the air supply is now gone
  LD HL,33838             // Add 1 to the score
  CALL INCSCORE_0
  LD IX,SCORBUF           // Print the new score at (19,26)
  LD C,6
  LD DE,20602
  CALL PMESS
  LD C,4                  // C=4; this value determines the duration of the sound effect
  LD A,(AIR)              // Pick up the remaining air supply (S) from AIR
  CPL                     // D=2*(63-S); this value determines the pitch of the
  AND 63                  // sound effect (which decreases with the amount of
  RLC A                   // air remaining)
  LD D,A
NXSHEET_7:
  LD A,0                  // Produce a short note
  OUT (254),A
  LD B,D
NXSHEET_8:
  DJNZ NXSHEET_8
  LD A,24
  OUT (254),A
  LD B,D
NXSHEET_9:
  DJNZ NXSHEET_9
  DEC C
  JR NZ,NXSHEET_7
  JR NXSHEET_6            // Jump back to decrease the air supply again

// Add to the score
//
// The entry point to this routine is at INCSCORE_0.
INCSCORE:
  LD (HL),48              // Roll the digit over from '9' to '0'
  DEC HL                  // Point HL at the next digit to the left
  LD A,L                  // Is this the 10000s digit?
  CP 42
  JR NZ,INCSCORE_0        // Jump if not
// Willy has scored another 10000 points. Give him an extra life.
  LD A,8                  // Set the screen flash counter at FLASH to 8
  LD (FLASH),A
  LD A,(NOMEN)            // Increment the number of lives remaining at NOMEN
  INC A
  LD (NOMEN),A
// The entry point to this routine is here and is used by the routines at
// DRAWITEMS, NXSHEET and KONGBEAST with HL pointing at the digit of the score
// (see SCORBUF) to be incremented.
INCSCORE_0:
  LD A,(HL)               // Pick up a digit of the score
  CP 57                   // Is it '9'?
  JR Z,INCSCORE           // Jump if so
  INC (HL)                // Increment the digit
  RET

// Move the conveyor in the current cavern
//
// Used by the routine at LOOP.
MVCONVEYOR:
  LD HL,(CONVLOC)         // Pick up the address of the conveyor's location in the screen buffer at 28672 from CONVLOC
  LD E,L                  // Copy this address to DE
  LD D,H
  LD A,(CONVLEN)          // Pick up the length of the conveyor from CONVLEN
  LD B,A                  // B will count the conveyor tiles
  LD A,(CONVDIR)          // Pick up the direction of the conveyor from CONVDIR
  OR A                    // Is the conveyor moving right?
  JR NZ,MVCONVEYOR_1      // Jump if so
// The conveyor is moving left.
  LD A,(HL)               // Copy the first pixel row of the conveyor tile to A
  RLC A                   // Rotate it left twice
  RLC A
  INC H                   // Point HL at the third pixel row of the conveyor
  INC H                   // tile
  LD C,(HL)               // Copy this pixel row to C
  RRC C                   // Rotate it right twice
  RRC C
MVCONVEYOR_0:
  LD (DE),A               // Update the first and third pixel rows of every
  LD (HL),C               // conveyor tile in the screen buffer at 28672
  INC L
  INC E
  DJNZ MVCONVEYOR_0
  RET
// The conveyor is moving right.
MVCONVEYOR_1:
  LD A,(HL)               // Copy the first pixel row of the conveyor tile to A
  RRC A                   // Rotate it right twice
  RRC A
  INC H                   // Point HL at the third pixel row of the conveyor
  INC H                   // tile
  LD C,(HL)               // Copy this pixel row to C
  RLC C                   // Rotate it left twice
  RLC C
  JR MVCONVEYOR_0         // Jump back to update the first and third pixel rows of every conveyor tile

// Move and draw the Kong Beast in the current cavern
//
// Used by the routine at LOOP.
KONGBEAST:
  LD HL,23558             // Flip the left-hand switch at (0,6) if Willy is
  CALL CHKSWITCH          // touching it
  LD A,(EUGDIR)           // Pick up the Kong Beast's status from EUGDIR
  CP 2                    // Is the Kong Beast already dead?
  RET Z                   // Return if so
  LD A,(29958)            // Pick up the sixth pixel row of the left-hand switch from the screen buffer at 28672
  CP 16                   // Has the switch been flipped?
  JP Z,KONGBEAST_8        // Jump if not
// The left-hand switch has been flipped. Deal with opening up the wall if that
// is still in progress.
  LD A,(24433)            // Pick up the attribute byte of the tile at (11,17) in the buffer at 24064
  OR A                    // Has the wall there been removed yet?
  JR Z,KONGBEAST_2        // Jump if so
  LD HL,32625             // Point HL at the bottom row of pixels of the wall tile at (11,17) in the screen buffer at 28672
KONGBEAST_0:
  LD A,(HL)               // Pick up a pixel row
  OR A                    // Is it blank yet?
  JR NZ,KONGBEAST_1       // Jump if not
  DEC H                   // Point HL at the next pixel row up
  LD A,H                  // Have we checked all 8 pixel rows yet?
  CP 119
  JR NZ,KONGBEAST_0       // If not, jump back to check the next one
  LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  LD (24433),A            // Change the attributes at (11,17) and (12,17) in the
  LD (24465),A            // buffer at 24064 to match the background tile (the wall there is now gone)
  LD A,114                // Update the seventh byte of the guardian definition
  LD (32971),A            // at HGUARD2 so that the guardian moves through the opening in the wall
  JR KONGBEAST_2
KONGBEAST_1:
  LD (HL),0               // Clear a pixel row of the wall tile at (11,17) in the screen buffer at 28672
  LD L,145                // Point HL at the opposite pixel row of the wall tile
  LD A,H                  // one cell down at (12,17)
  XOR 7
  LD H,A
  LD (HL),0               // Clear that pixel row as well
// Now check the right-hand switch.
KONGBEAST_2:
  LD HL,23570             // Flip the right-hand switch at (0,18) if Willy is
  CALL CHKSWITCH          // touching it (and it hasn't already been flipped)
  JR NZ,KONGBEAST_4       // Jump if the switch was not flipped
  XOR A                   // Initialise the Kong Beast's pixel y-coordinate at
  LD (EUGHGT),A           // EUGHGT to 0
  INC A                   // Update the Kong Beast's status at EUGDIR to 1: he
  LD (EUGDIR),A           // is falling
  LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  LD (24143),A            // Change the attributes of the floor beneath the Kong
  LD (24144),A            // Beast in the buffer at 24064 to match that of the background tile
  LD HL,28751             // Point HL at (2,15) in the screen buffer at 28672
  LD B,8                  // Clear the cells at (2,15) and (2,16), removing the
KONGBEAST_3:
  LD (HL),0               // floor beneath the Kong Beast
  INC L
  LD (HL),0
  DEC L
  INC H
  DJNZ KONGBEAST_3
KONGBEAST_4:
  LD A,(EUGDIR)           // Pick up the Kong Beast's status from EUGDIR
  OR A                    // Is the Kong Beast still on the ledge?
  JR Z,KONGBEAST_8        // Jump if so
// The Kong Beast is falling.
  LD A,(EUGHGT)           // Pick up the Kong Beast's pixel y-coordinate from EUGHGT
  CP 100                  // Has he fallen into the portal yet?
  JR Z,KONGBEAST_7        // Jump if so
  ADD A,4                 // Add 4 to the Kong Beast's pixel y-coordinate at
  LD (EUGHGT),A           // EUGHGT (moving him downwards)
  LD C,A                  // Copy the pixel y-coordinate to C; this value determines the pitch of the sound effect
  LD D,16                 // D=16; this value determines the duration of the sound effect
  LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
KONGBEAST_5:
  OUT (254),A             // Make a falling sound effect
  XOR 24
  LD B,C
KONGBEAST_6:
  DJNZ KONGBEAST_6
  DEC D
  JR NZ,KONGBEAST_5
  LD A,C                  // Copy the Kong Beast's pixel y-coordinate back into A
  RLCA                    // Point DE at the entry in the screen buffer address
  LD E,A                  // lookup table at SBUFADDRS that corresponds to the
  LD D,131                // Kong Beast's pixel y-coordinate
  LD A,(DE)               // Point HL at the address of the Kong Beast's
  OR 15                   // location in the screen buffer at 24576
  LD L,A
  INC DE
  LD A,(DE)
  LD H,A
  LD D,129                // Use bit 5 of the value of the game clock at CLOCK
  LD A,(CLOCK)            // (which is toggled once every eight passes through
  AND 32                  // the main loop) to point DE at the graphic data for
  OR 64                   // the appropriate Kong Beast sprite
  LD E,A
  LD C,0                  // Draw the Kong Beast to the screen buffer at 24576
  CALL DRWFIX
  LD HL,33836             // Add 100 to the score
  CALL INCSCORE_0
  LD A,(EUGHGT)           // Pick up the Kong Beast's pixel y-coordinate from EUGHGT
  AND 120                 // Point HL at the address of the Kong Beast's
  LD L,A                  // location in the attribute buffer at 23552
  LD H,23
  ADD HL,HL
  ADD HL,HL
  LD A,L
  OR 15
  LD L,A
  LD A,6                  // The Kong Beast is drawn with yellow INK
  JP EUGENE_3             // Set the attribute bytes for the Kong Beast
// The Kong Beast has fallen into the portal.
KONGBEAST_7:
  LD A,2                  // Set the Kong Beast's status at EUGDIR to 2: he is
  LD (EUGDIR),A           // dead
  RET
// The Kong Beast is still on the ledge.
KONGBEAST_8:
  LD A,(CLOCK)            // Pick up the value of the game clock at CLOCK
  AND 32                  // Use bit 5 of this value (which is toggled once
  LD E,A                  // every eight passes through the main loop) to point
  LD D,129                // DE at the graphic data for the appropriate Kong Beast sprite
  LD HL,24591             // Draw the Kong Beast at (0,15) in the screen buffer
  LD C,1                  // at 24576
  CALL DRWFIX
  JP NZ,KILLWILLY_0       // Kill Willy if he collided with the Kong Beast
  LD A,68                 // A=68 (INK 4: PAPER 0: BRIGHT 1)
  LD (23599),A            // Set the attribute bytes for the Kong Beast in the
  LD (23600),A            // buffer at 23552
  LD (23567),A
  LD (23568),A
  RET

// Flip a switch in a Kong Beast cavern if Willy is touching it
//
// Used by the routine at KONGBEAST. Returns with the zero flag set if Willy
// flips the switch.
//
// HL Address of the switch's location in the attribute buffer at 23552
CHKSWITCH:
  LD A,(LOCATION)         // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION
  INC A                   // Is it equal to or one less than the LSB of the
  AND 254                 // address of the switch's location?
  CP L
  RET NZ                  // Return (with the zero flag reset) if not
  LD A,(32877)            // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877
  CP H                    // Does it match the MSB of the address of the switch's location?
  RET NZ                  // Return (with the zero flag reset) if not
  LD A,(32869)            // Pick up the sixth byte of the graphic data for the switch tile from 32869
  LD H,117                // Point HL at the sixth row of pixels of the switch tile in the screen buffer at 28672
  CP (HL)                 // Has the switch already been flipped?
  RET NZ                  // Return (with the zero flag reset) if so
// Willy is flipping the switch.
  LD (HL),8               // Update the sixth, seventh and eighth rows of pixels
  INC H                   // of the switch tile in the screen buffer at 28672 to
  LD (HL),6               // make it appear flipped
  INC H
  LD (HL),6
  XOR A                   // Set the zero flag: Willy has flipped the switch
  OR A                    // This instruction is redundant
  RET

// Check and set the attribute bytes for Willy's sprite in the buffer at 23552
//
// Used by the routine at LOOP.
WILLYATTRS:
  LD HL,(LOCATION)        // Pick up the address of Willy's location in the attribute buffer at 23552 from LOCATION
  LD DE,31                // Prepare DE for addition
  LD C,15                 // Set C=15 for the top two rows of cells (to make the routine at WILLYATTR force white INK)
  CALL WILLYATTR          // Check and set the attribute byte for the top-left cell
  INC HL                  // Move HL to the next cell to the right
  CALL WILLYATTR          // Check and set the attribute byte for the top-right cell
  ADD HL,DE               // Move HL down a row and back one cell to the left
  CALL WILLYATTR          // Check and set the attribute byte for the mid-left cell
  INC HL                  // Move HL to the next cell to the right
  CALL WILLYATTR          // Check and set the attribute byte for the mid-right cell
  LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  LD C,A                  // Copy it to C
  ADD HL,DE               // Move HL down a row and back one cell to the left
  CALL WILLYATTR          // Check and set the attribute byte for the bottom-left cell
  INC HL                  // Move HL to the next cell to the right
  CALL WILLYATTR          // Check and set the attribute byte for the bottom-right cell
  JR DRAWWILLY            // Draw Willy to the screen buffer at 24576

// Check and set the attribute byte for a cell occupied by Willy's sprite
//
// Used by the routine at WILLYATTRS.
//
// C 15 or Willy's pixel y-coordinate
// HL Address of the cell in the attribute buffer at 23552
WILLYATTR:
  LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  CP (HL)                 // Does this cell contain a background tile?
  JR NZ,WILLYATTR_0       // Jump if not
  LD A,C                  // Set the zero flag if we are going to retain the INK
  AND 15                  // colour in this cell; this happens only if the cell is in the bottom row and Willy's sprite is confined to the top two rows
  JR Z,WILLYATTR_0        // Jump if we are going to retain the current INK colour in this cell
  LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  OR 7                    // Set bits 0-2, making the INK white
  LD (HL),A               // Set the attribute byte for this cell in the buffer at 23552
WILLYATTR_0:
  LD A,(NASTY1)           // Pick up the attribute byte of the first nasty tile for the current cavern from NASTY1
  CP (HL)                 // Has Willy hit a nasty of the first kind?
  JP Z,KILLWILLY          // Kill Willy if so
  LD A,(NASTY2)           // Pick up the attribute byte of the second nasty tile for the current cavern from NASTY2
  CP (HL)                 // Has Willy hit a nasty of the second kind?
  JP Z,KILLWILLY          // Kill Willy if so
  RET

} // NOTE: end of main() function!



// Draw Willy to the screen buffer at 24576
//
// Used by the routine at WILLYATTRS.
void DRAWWILLY() {
  LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  LD IXh,131              // Point IX at the entry in the screen buffer address
  LD IXl,A                // lookup table at SBUFADDRS that corresponds to Willy's y-coordinate
  LD A,(DMFLAGS)          // Pick up Willy's direction and movement flags from DMFLAGS
  AND 1                   // Now E=0 if Willy is facing right, or 128 if he's
  RRCA                    // facing left
  LD E,A
  LD A,(FRAME)            // Pick up Willy's animation frame (0-3) from FRAME
  AND 3                   // Point DE at the sprite graphic data for Willy's
  RRCA                    // current animation frame (see MANDAT)
  RRCA
  RRCA
  OR E
  LD E,A
  LD D,130
  LD B,16                 // There are 16 rows of pixels to copy
  LD A,(LOCATION)         // Pick up Willy's screen x-coordinate (0-31) from
  AND 31                  // LOCATION
  LD C,A                  // Copy it to C
DRAWWILLY_0:
  LD A,(IX+0)             // Set HL to the address in the screen buffer at 24576
  LD H,(IX+1)             // that corresponds to where we are going to draw the
  OR C                    // next pixel row of the sprite graphic
  LD L,A
  LD A,(DE)               // Pick up a sprite graphic byte
  OR (HL)                 // Merge it with the background
  LD (HL),A               // Save the resultant byte to the screen buffer
  INC HL                  // Move HL along to the next cell to the right
  INC DE                  // Point DE at the next sprite graphic byte
  LD A,(DE)               // Pick it up in A
  OR (HL)                 // Merge it with the background
  LD (HL),A               // Save the resultant byte to the screen buffer
  INC IX                  // Point IX at the next entry in the screen buffer
  INC IX                  // address lookup table at SBUFADDRS
  INC DE                  // Point DE at the next sprite graphic byte
  DJNZ DRAWWILLY_0        // Jump back until all 16 rows of pixels have been drawn
  RET
}



// Print a message
//
// Used by the routines at START, STARTGAME, LOOP, ENDGAM and NXSHEET.
//
// IX Address of the message
// C Length of the message
// DE Display file address
void PMESS() {
  LD A,(IX+0)             // Collect a character from the message
  CALL PRINTCHAR          // Print it
  INC IX                  // Point IX at the next character in the message
  INC E                   // Point DE at the next character cell (subtracting 8
  LD A,D                  // from D compensates for the operations performed by
  SUB 8                   // the routine at PRINTCHAR)
  LD D,A
  DEC C                   // Have we printed the entire message yet?
  JR NZ,PMESS             // If not, jump back to print the next character
  RET
}



// Print a single character
//
// Used by the routine at PMESS.
//
// A ASCII code of the character
// DE Display file address
void PRINTCHAR() {
  LD H,7                  // Point HL at the bitmap for the character (in the
  LD L,A                  // ROM)
  SET 7,L
  ADD HL,HL
  ADD HL,HL
  ADD HL,HL
  LD B,8                  // There are eight pixel rows in a character bitmap
// This entry point is used by the routine at DRAWITEMS to draw an item in the
// current cavern.
PRINTCHAR_0:
  LD A,(HL)               // Copy the character bitmap to the screen (or item
  LD (DE),A               // graphic to the screen buffer)
  INC HL
  INC D
  DJNZ PRINTCHAR_0
  RET
}



// Play the theme tune (The Blue Danube)
//
// Used by the routine at START. Returns with the zero flag reset if ENTER or
// the fire button is pressed while the tune is being played.
//
// IY THEMETUNE (tune data)
bool PLAYTUNE() {
  LD A,(IY+0)             // Pick up the next byte of tune data from the table at THEMETUNE
  CP 255                  // Has the tune finished?
  RET Z                   // Return (with the zero flag set) if so
  LD C,A                  // Copy the first byte of data for this note (which determines the duration) to C
  LD B,0                  // Initialise B, which will be used as a delay counter in the note-producing loop
  XOR A                   // Set A=0 (for no apparent reasaon)
  LD D,(IY+1)             // Pick up the second byte of data for this note
  LD A,D                  // Copy it to A
  CALL PIANOKEY           // Calculate the attribute file address for the corresponding piano key
  LD (HL),80              // Set the attribute byte for the piano key to 80 (INK 0: PAPER 2: BRIGHT 1)
  LD E,(IY+2)             // Pick up the third byte of data for this note
  LD A,E                  // Copy it to A
  CALL PIANOKEY           // Calculate the attribute file address for the corresponding piano key
  LD (HL),40              // Set the attribute byte for the piano key to 40 (INK 0: PAPER 5: BRIGHT 0)
PLAYTUNE_0:
  OUT (254),A             // Produce a sound based on the frequency parameters
  DEC D                   // in the second and third bytes of data for this note
  JR NZ,PLAYTUNE_1        // (copied into D and E)
  LD D,(IY+1)
  XOR 24
PLAYTUNE_1:
  DEC E
  JR NZ,PLAYTUNE_2
  LD E,(IY+2)
  XOR 24
PLAYTUNE_2:
  DJNZ PLAYTUNE_0
  DEC C
  JR NZ,PLAYTUNE_0
  CALL CHECKENTER         // Check whether ENTER or the fire button is being pressed
  RET NZ                  // Return (with the zero flag reset) if it is
  LD A,(IY+1)             // Pick up the second byte of data for this note
  CALL PIANOKEY           // Calculate the attribute file address for the corresponding piano key
  LD (HL),56              // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0)
  LD A,(IY+2)             // Pick up the third byte of data for this note
  CALL PIANOKEY           // Calculate the attribute file address for the corresponding piano key
  LD (HL),56              // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0)
  INC IY                  // Move IY along to the data for the next note in the
  INC IY                  // tune
  INC IY
  JR PLAYTUNE             // Jump back to play the next note
}


// Calculate the attribute file address for a piano key
//
// Used by the routine at PLAYTUNE. Returns with the attribute file address in
// HL.
//
// A Frequency parameter from the tune data table at THEMETUNE
void PIANOKEY() {
  SUB 8                   // Compute the piano key index (K) based on the
  RRCA                    // frequency parameter (F), and store it in bits 0-4
  RRCA                    // of A: K=31-INT((F-8)/8)
  RRCA
  CPL
  OR 224                  // A=224+K; this is the LSB
  LD L,A                  // Set HL to the attribute file address for the piano
  LD H,89                 // key
  RET
}


// Check whether ENTER or the fire button is being pressed
//
// Used by the routine at PLAYTUNE. Returns with the zero flag reset if ENTER or
// the fire button on the joystick is being pressed.
bool CHECKENTER() {
  LD A,(KEMP)             // Pick up the Kempston joystick indicator from KEMP
  OR A                    // Is the joystick connected?
  JR Z,CHECKENTER_0       // Jump if not
  IN A,(31)               // Collect input from the joystick
  BIT 4,A                 // Is the fire button being pressed?
  RET NZ                  // Return (with the zero flag reset) if so
CHECKENTER_0:
  LD BC,49150             // Read keys H-J-K-L-ENTER
  IN A,(C)
  AND 1                   // Keep only bit 0 of the result (ENTER)
  CP 1                    // Reset the zero flag if ENTER is being pressed
  RET
}


void print_score(int current_score) {
  sprintf(SCORE, "%6d", current_score);
  // MEM[33830]  // position for score to be printed.
}



// ----------------------------------------------
//   NOTE: source code remnants has been deleted
// ----------------------------------------------


// ----------------------------------------------------------
//   NOTE: data from here has been moved to the data.c file.
// ----------------------------------------------------------
