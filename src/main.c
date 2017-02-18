// Manic Miner C Port Copyright 2017 Michael R. Cook
// Copyright 1983 Bug-Byte Ltd (Manic Miner)

// Manic Miner disassembly
// Copyright 2010, 2012-2016 Richard Dymond
// http://skoolkit.ca/

#include "headers.h"
#include "externs.h"

#include "terminal.h"

const int FRAMES_PER_SECOND = 25;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

// returns the current number of clock ticks
// that have elapsed since the system was started
clock_t current_time;

// The game has just loaded
int main(void) {
    Game_initialize();

    // Set initial clock tick value - for calculating FSP.
    current_time = clock();

    while (true) {
        // Initialise the game mode indicator at DEMO.
        game.DEMO = 64;

        // Display the title screen and play the theme tune
        Game_play_intro();

        // Initialise a new game (reset score, etc.)
        Game_new();

        if (play_new_cavern()) {
            break; // aka QUIT!
        }
    }

    Terminal_exit();
    return 0;
}

// Returning true quits the game!
// FIXME: uses `goto` statements!
bool play_new_cavern() {
  Cavern_initialize();

  // FIXME: disable DEMO mode
  game.DEMO = 0;

  // This entry point is used by the routines at LOOP (when teleporting into a cavern
  // or reinitialising the current cavern after Willy has lost a life) and NXSHEET.
NEWSHT:

  // FIXME: only using CAVERN0, while porting -MRC-

  // Copy the cavern's attribute bytes into the buffer at 24064
  // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
  // SLA A                   // Point HL at the first byte of the cavern definition
  // SLA A
  // ADD A,176
  // LD H,A
  // LD L,0
  // LD DE,24064
  // LD BC,512
  // LDIR
  // FIXME: uses Attr Buff (Blank): Attr File = 22528
  for (int i = 0; i < 512; i++) {
      Speccy_writeAttribute(24064 + i, cavern.layout[i]);
  }

  // Copy the rest of the cavern definition into the game status buffer at 32768
  // LD DE,CAVERNNAME
  // LD BC,512
  // LDIR
  initialize_cavern0();
  tick(); // FIXME: required here otherwise screen is messed up!

  // Draw the current cavern to the screen buffer at 28672
  // CALL DRAWSHEET
  DRAWSHEET();

  // Clear the bottom third of the display file
  // LD HL,20480
  // LD DE,20481
  // LD BC,2047
  // LD (HL),0
  // LDIR
  for (int i = 0; i < 2048; i++) {
      Speccy_writeScreen(20480 + i, 0);
  }

  // Print the cavern name (see CAVERNNAME) at 20480 (16,0)
  // LD IX,CAVERNNAME
  // LD C,32
  // LD DE,20480
  // CALL PMESS
  PMESS(cavern.CAVERNNAME, 20480, 32);

  // Print 'AIR' (see MESSAIR) at 20512 (17,0)
  // LD IX,MESSAIR
  // LD C,3
  // LD DE,20512
  // CALL PMESS
  PMESS(&MESSAIR, 20512, 3);

  // Initialise A to 82; this is the MSB of the display file address at which to start drawing the bar that represents the air supply
  // LD A,82
  // STARTGAME_0:
  for (uint8_t a = 82; a < 86; a++) {
    // LD H,A                  // Prepare HL and DE for drawing a row of pixels in the air bar
    // LD D,A
    // LD L,36
    // LD E,37
    uint16_t addr = build_address(a, 36);

    // LD B,A                  // Save the display file address MSB in B briefly
    // LD A,(AIR)              // Pick up the value of the initial air supply from AIR
    // SUB 36                  // Now C determines the length of the air bar (in cell widths)
    // LD C,A
    // LD A,B                  // Restore the display file address MSB to A
    // LD B,0                  // Now BC determines the length of the air bar (in cell widths)
    // LD (HL),255
    // LDIR

    // Draw a single row of pixels across C cells
    for (uint16_t i = 0; i < cavern.AIR - 36; i++) {
      // speccy.memory[addr + i] = 255;
      Speccy_writeScreen(addr + i, 255);
    }

    // INC A                   // Increment the display file address MSB in A (moving down to the next row of pixels)
    // CP 86                   // Have we drawn all four rows of pixels in the air bar yet?
    // JR NZ,STARTGAME_0       // If not, jump back to draw the next one
  }

  // Print 'High Score 000000   Score 000000' (see MESSHSSC) at 20576 (19,0)
  // LD IX,MESSHSSC
  // LD DE,20576
  // LD C,32
  // CALL PMESS
  PMESS(&MESSHSSC, 20576, 32);

    Terminal_redraw();

  // LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
  // LD C,254                // Set the border colour
  // OUT (C),A
  OUT(cavern.BORDER);

  // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  // OR A                    // Are we in demo mode?
  // JR Z,LOOP               // If not, enter the main loop now
  if (game.DEMO) {
    // Reset the game mode indicator at DEMO to 64 (we're in demo mode)
    // LD A,64
    // LD (DEMO),A
    game.DEMO = 64;
  }
  // This routine continues into the main loop at LOOP.

  // Main loop
  //
  // The routine at STARTGAME continues here.
  //
  // The first thing to do is check whether there are any remaining lives to draw
  // at the bottom of the screen.
  //LOOP:
  while (true) {
    tick();

    // IMPORTANT: moved code for drawing the remaining lives to its own function.
    draw_remaining_lives();

    // Next, prepare the screen and attribute buffers for drawing to the screen.
    // LOOP_2:
    // LD HL,24064             // Copy the contents of the attribute buffer at 24064
    // LD DE,23552             // (the attributes for the empty cavern) into the
    // LD BC,512               // attribute buffer at 23552
    // LDIR
    // FIXME: all good, uses the Display File
    for (int i = 0; i < 512; i++) {
      speccy.memory[23552 + i] = speccy.memory[24064 + i];
    }

    // LD HL,28672             // Copy the contents of the screen buffer at 28672
    // LD DE,24576             // (the tiles for the empty cavern) into the screen
    // LD BC,4096              // buffer at 24576
    // LDIR
    // FIXME: all good, uses the Display File
    for (int i = 0; i < 4096; i++) {
      speccy.memory[24576 + i] = speccy.memory[28672 + i];
    }

    // CALL MOVEHG             // Move the horizontal guardians in the current cavern
    MOVEHG();

    // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
    // OR A                    // Are we in demo mode?
    // CALL Z,MOVEWILLY        // If not, move Willy
    // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
    // OR A                    // Are we in demo mode?
    // CALL Z,WILLYATTRS       // If not, check and set the attribute bytes for Willy's sprite in the buffer at 23552, and draw Willy to the screen buffer at 24576
    if (game.DEMO == 0) {
       if (MOVEWILLY()) {
         goto LOOP_4; // Willy has died!
       }
      if (WILLYATTRS()) {
        goto LOOP_4; // Willy has died!
      }
    }

    // CALL DRAWHG             // Draw the horizontal guardians in the current cavern
    DRAWHG();
    // CALL MVCONVEYOR         // Move the conveyor in the current cavern
    MVCONVEYOR();
    // CALL DRAWITEMS          // Draw the items in the current cavern and collect any that Willy is touching
    DRAWITEMS();

    switch (cavern.SHEET) {
      case 4:
        // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
        // CP 4                    // Are we in Eugene's Lair?
        // CALL Z,EUGENE           // If so, move and draw Eugene
        if (EUGENE()) {
          goto LOOP_4; // Willy has died!
        }
        break;
      case 13:
        // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
        // CP 13                   // Are we in Skylab Landing Bay?
        // JP Z,SKYLABS            // If so, move and draw the Skylabs
        if (SKYLABS()) {
          goto LOOP_4; // Willy has died!
        }
        // NOTE: else { goto LOOP_3 }
        break;
      case 8:
        // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
        // CP 8                    // Are we in Wacky Amoebatrons or beyond?
        // CALL NC,VGUARDIANS      // If so, move and draw the vertical guardians
        if (VGUARDIANS()) {
          goto LOOP_4; // Willy has died!
        }
        break;
      case 7:
        // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
        // CP 7                    // Are we in Miner Willy meets the Kong Beast?
        // CALL Z,KONGBEAST        // If so, move and draw the Kong Beast
        if (KONGBEAST()) {
          goto LOOP_4; // Willy has died!
        }
        break;
      case 11:
        // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
        // CP 11                   // Are we in Return of the Alien Kong Beast?
        // CALL Z,KONGBEAST        // If so, move and draw the Kong Beast
        if (KONGBEAST()) {
          goto LOOP_4; // Willy has died!
        }
        break;
      case 18:
        // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
        // CP 18                   // Are we in Solar Power Generator?
        // CALL Z,LIGHTBEAM        // If so, move and draw the light beam
        LIGHTBEAM();
        break;
      default:
        ; // NOOP
    }

    // This entry point is used by the routine at SKYLABS.
    // LOOP_3:
    // CALL CHKPORTAL          // Draw the portal, or move to the next cavern if Willy has entered it
    if (CHKPORTAL()) {
      goto NEWSHT;
    }

    // This entry point is used by the routine at KILLWILLY.
LOOP_4:
    // LD HL,24576             // Copy the contents of the screen buffer at 24576 to the display file
    // LD DE,16384
    // LD BC,4096
    // LDIR
    // FIXME: all good, uses the Display File
    for (int i = 0; i < 4096; i++) {
        Speccy_writeScreen(16384 + i, speccy.memory[24576 + i]);
    }
    Terminal_redraw();

    // LD A,(FLASH)            // Pick up the screen flash counter from FLASH
    // OR A                    // Is it zero?
    // JR Z,LOOP_5             // Jump if so
    if (game.FLASH > 0) {
      // Decrement the screen flash counter at FLASH
      // DEC A
      // LD (FLASH),A
      game.FLASH--;

      // RLCA                    // Move bits 0-2 into bits 3-5 and clear all the other
      // RLCA                    // bits
      // RLCA
      // AND 56
      uint8_t new_flash_value = (uint8_t) (rotl(game.FLASH, 3) & 56);
      // LD HL,23552             // Set every attribute byte in the buffer at 23552 to
      // LD DE,23553             // this value
      // LD BC,511
      // LD (HL),A
      // LDIR
    // FIXME: Uses Attr Buffer: Attr File= 22528
      for (int i = 0; i < 512; i++) {
        speccy.memory[23552 + i] = new_flash_value;
      }
        Terminal_redraw();
    }

    // LOOP_5:
    // LD HL,23552             // Copy the contents of the attribute buffer at 23552
    // LD DE,22528             // to the attribute file
    // LD BC,512
    // LDIR
    // FIXME: all good, uses the Display File
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(22528 + i, speccy.memory[23552 + i]);
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

      Terminal_redraw();

    // Decrease the air remaining in the current cavern
    // Jump if there's no air left
    // CALL DECAIR
    // JP Z,MANDEAD
    if (DECAIR()) {
      if (MANDEAD()) {
        return false; // goto START, and don't quit!
      } else {
        goto NEWSHT;
      }
    }

    // Now check whether SHIFT and SPACE are being pressed.
    // LD BC,65278             // Read keys SHIFT-Z-X-C-V
    // IN A,(C)
    // LD E,A                  // Save the result in E
    // LD B,127                // Read keys B-N-M-SS-SPACE
    // IN A,(C)
    // OR E                    // Combine the results
    // AND 1                   // Are SHIFT and SPACE being pressed?
    // JP Z,START              // If so, quit the game
//    if (check_quit_keypress()) {
//      return true; // goto START, and make sure to quit the game!
//    }

    // Now read the keys A, S, D, F and G (which pause the game).
    // LD B,253                // Read keys A-S-D-F-G
    // IN A,(C)
    // AND 31                  // Are any of these keys being pressed?
    // CP 31
    // JR Z,LOOP_7             // Jump if not
//    if (check_pause_keypress()) {
//      // LOOP_6:
//      do {
//        millisleep(50); // keep the FPS under control
//        // LD B,2                  // Read every half-row of keys except A-S-D-F-G
//        // IN A,(C)
//        // AND 31                  // Are any of these keys being pressed?
//        // CP 31
//        // JR Z,LOOP_6             // Jump back if not (the game is still paused)
//      } while (!check_pause_keypress());
//    }

    // Here we check whether Willy has had a fatal accident.
    // LOOP_7:
    // LD A,(AIRBORNE)         // Pick up the airborne status indicator from AIRBORNE
    // CP 255                  // Has Willy landed after falling from too great a height, or collided with a nasty or a guardian?
    // JP Z,MANDEAD            // Jump if so
    if (willy.AIRBORNE == 255) {
      if (MANDEAD()) {
        return false; // goto START, and don't quit!
      } else {
        goto NEWSHT;
      }
    }

    // Check key press to toggle the in-game music.
    if (Terminal_keyMuteMusic()) {
      game.playMusic = ~game.playMusic;
    }

    // LOOP_9:
    // BIT 1,(HL)              // Has the in-game music been switched off?
    // JR NZ,NONOTE4           // Jump if so
    if (game.playMusic) {
      // The next section of code plays a note of the in-game music.

      // Increment the in-game music note index at NOTEINDEX
      // LD A,(NOTEINDEX)
      // INC A
      // LD (NOTEINDEX),A
      game.NOTEINDEX++;

      // AND 126                 // Point HL at the appropriate entry in the tune data
      // RRCA                    // table at GAMETUNE
      uint8_t index = rotr((uint8_t)(game.NOTEINDEX & 126), 1);
      // LD E,A
      // LD D,0
      // LD HL,GAMETUNE
      //  ADD HL,DE
      //  LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
      uint8_t note = cavern.BORDER;
      //  LD E,(HL)               // Initialise the pitch delay counter in E
      uint8_t pitch_delay_counter = GAMETUNE[index];
      //  LD BC,3                 // Initialise the duration delay counters in B (0) and C (3)

      // TM51:
      for (int i = 0; i < 3; i++) {
        // OUT (254),A             // Produce a note of the in-game music
        OUT(note);

        // SEE37708:
        // DEC E
        pitch_delay_counter--;
        // JR NZ,NOFLP6
        if (pitch_delay_counter > 0) {
          // LD E,(HL)
          pitch_delay_counter = GAMETUNE[index];
          // XOR 24
          note ^= 24;
        }

        // NOFLP6:
        // DJNZ TM51
        // DEC C
        // JR NZ,TM51
        millisleep(1);
      }
    }

    // If we're in demo mode, check the keyboard and joystick and return to the
    // title screen if there's any input.
    // NONOTE4:
    // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
    // OR A                    // Are we in demo mode?
    // JR Z,NODEM1             // Jump if not
    if (game.DEMO > 0) {
      // We're in demo mode; is it time to show the next cavern?

      // DEC A
      // JP Z,MANDEAD            // Jump if so
      if (game.DEMO - 1 == 0) {
        if (MANDEAD()) {
          return false; // goto START, and don't quit!
        } else {
          goto NEWSHT;
        }
      }
      // LD (DEMO),A             // Update the game mode indicator at DEMO
      game.DEMO--;

      // LD BC,254               // Read every row of keys on the keyboard
      // IN A,(C)
      // AND 31                  // Are any keys being pressed?
      // CP 31
      // JP NZ,START             // If so, return to the title screen
      if (Terminal_keyAny()) {
        return false; // goto START, and don't quit!
      }

      // LD A,(KEMP)             // Pick up the Kempston joystick indicator from KEMP
      // OR A                    // Is there a joystick connected?
      // JR Z,NODEM1             // Jump if not
      if (game.KEMP) {
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
  */

    // JP LOOP                 // Jump back to the start of the main loop
    tick();
  }
}


// The air in the cavern has run out, or Willy has had a fatal accident, or it's
// demo mode and it's time to show the next cavern.
// IMPORTANT: return TRUE is goto START, FALSE is goto NEWSHT -MRC-
bool MANDEAD() {
  // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  // OR A                    // Is it demo mode?
  // JP NZ,NXSHEET           // If so, move to the next cavern
  if (game.DEMO) {
    // IMPORTANT: no need to check NXSHEET, we know we should `goto NEWSHT` after it -MRC-
    NXSHEET();
    return false; // goto NEWSHT;
  }

//  uint8_t pitch, duration;
//  // LD A,71                 // A=71 (INK 7: PAPER 0: BRIGHT 1)
//
//  // The following loop fills the top two thirds of the attribute file with a
//  // single value (71 TO 64 STEP -1) and makes a sound effect.
//  // LPDEAD1:
//  for (uint8_t attr = 71; attr > 64; attr--) {
//    // LD HL,22528             // Fill the top two thirds of the attribute file with
//    // LD DE,22529             // the value in A
//    // LD BC,511
//    // LD (HL),A
//    // LDIR
//    for (int i = 0; i < 512; i++) {
//      speccy.memory[22528 + i] = attr;
//    }
//
//    // LD E,A                  // Save the attribute byte (64-71) in E for later retrieval
//
//    // CPL                     // D=63-8*(E AND 7); this value determines the pitch
//    pitch = ~attr;
//    // AND 7                   // of the short note that will be played
//    pitch = (uint8_t)(pitch & 7);
//    // RLCA
//    // RLCA
//    // RLCA
//    pitch = rotl(pitch, 3);
//    // OR 7
//    pitch |= 7;
//    // LD D,A
//
//    // LD C,E                  // C=8+32*(E AND 7); this value determines the
//    duration = attr;
//    // RRC C                   // duration of the short note that will be played
//    // RRC C
//    // RRC C
//    duration = rotr(duration, 3);
//    // OR 16                   // Set bit 4 of A (for no apparent reason)
//    duration |= 16;
//
//    // XOR A                   // Set A=0 (this will make the border black)
//    uint8_t border = 0;
//    // TM21:
//    for (int d = duration; d > 0; d--) {
//      // OUT(254), A             // Produce a short note whose pitch is determined by D
//      OUT(border);
//      // XOR 24                  // and whose duration is determined by C
//      border ^= 24;
//
//      // LD B, D
//      // TM22:
//      // DJNZ TM22
//      millisleep(pitch); // hmm, are we sleeping for 1ms
//
//      // DEC C
//      // JR NZ, TM21
//    }
//
//    // LD A,E                  // Restore the attribute byte (originally 71) to A
//    // DEC A                   // Decrement it (effectively decrementing the INK colour)
//    // CP 63                   // Have we used attribute value 64 (INK 0) yet?
//    // JR NZ,LPDEAD1           // If not, jump back to update the INK colour in the top two thirds of the screen and make another sound effect
//  }

  // Finally, check whether any lives remain.
  // LD HL,NOMEN             // Pick up the number of lives remaining from NOMEN
  // LD A,(HL)
  // OR A
  // JP Z,ENDGAM
  // Are there any lives remaining?
  if (willy.NOMEN < 1) {
    // If not, display the game over sequence
    ENDGAM();
    return true; // goto START;
  } else {
    // Decrease the number of lives remaining by one
    // DEC (HL)
    willy.NOMEN--;
  }

  // Jump back to reinitialise the current cavern
  // JP NEWSHT
  return false; // goto NEWSHT;
}

// Display the game over sequence
//
// Used by the routine at LOOP. First check whether we have a new high score.
void ENDGAM() {
  //   LD HL,HGHSCOR           // Point HL at the high score at HGHSCOR
  //   LD DE,SCORBUF           // Point DE at the current score at SCORBUF
  //   LD B,6                  // There are 6 digits to compare
  // LPHGH:
  //   LD A,(DE)               // Pick up a digit of the current score
  //   CP (HL)                 // Compare it with the corresponding digit of the high score
  //   JP C,FEET               // Jump if it's less than the corresponding digit of the high score
  //   JP NZ,NEWHGH            // Jump if it's greater than the corresponding digit of the high score
  //   INC HL                  // Point HL at the next digit of the high score
  //   INC DE                  // Point DE at the next digit of the current score
  //   DJNZ LPHGH              // Jump back to compare the next pair of digits
  // NEWHGH:
  //   LD HL,SCORBUF           // Replace the high score with the current score
  //   LD DE,HGHSCOR
  //   LD BC,6
  //   LDIR
  if (game.current_score > game.highscore) {
    game.highscore = game.current_score;
    memcpy(HGHSCOR, SCORBUF, sizeof(&SCORBUF));
  }

  // Now prepare the screen for the game over sequence.
  // FEET:
  // Clear the top two-thirds of the display file
  // LD HL,16384
  // LD DE,16385
  // LD BC,4095
  // LD (HL),0
  // LDIR
  for (int i = 0; i < 4096; i++) {
      Speccy_writeScreen(16384 + i, 0);
  }
    Terminal_redraw();

  // XOR A                   // Initialise the game status buffer variable at
  // LD (EUGHGT),A           // EUGHGT; this variable will determine the distance of the boot from the top of the screen
  EUGHGT = 0;

  // LD DE,WILLYR2           // Draw Willy at (12,15)
  // LD HL,18575
  // LD C,0
  // CALL DRWFIX
  DRWFIX(&willy.sprites[64], 18575, 0);

  // LD DE,PLINTH            // Draw the plinth (see PLINTH) underneath Willy at
  // LD HL,18639             // (14,15)
  // LD C,0
  // CALL DRWFIX
  DRWFIX(&PLINTH, 18639, 0);

    Terminal_redraw();

  uint8_t msb, lsb;
  uint16_t addr;

  // The following loop draws the boot's descent onto the plinth that supports Willy.
  // LOOPFT:
  for (EUGHGT = 0; EUGHGT < 98; EUGHGT += 4) {
    // LD A,(EUGHGT)           // Pick up the distance variable from EUGHGT
    // LD C,A                  // Point BC at the corresponding entry in the screen
    // LD B,131                // buffer address lookup table at SBUFADDRS
    // LD A,(BC)               // Point HL at the corresponding location in the
    // OR 15                   // display file
    // LD L,A
    // INC BC
    // LD A,(BC)
    // SUB 32
    // LD H,A
    split_address(SBUFADDRS[EUGHGT], &msb, &lsb);
    addr = build_address(msb-32, lsb|15); // center of screen

    // LD DE,BOOT              // Draw the boot (see BOOT) at this location, without
    // LD C,0                  // erasing the boot at the previous location; this
    // CALL DRWFIX             // leaves the portion of the boot sprite that's above the ankle in place, and makes the boot appear as if it's at the end of a long, extending trouser leg
    DRWFIX(&BOOT, addr, 0);

    // LD A,(EUGHGT)           // Pick up the distance variable from EUGHGT
    // CPL                     // A=255-A
    uint8_t distance = (uint8_t)(255 - EUGHGT);

    // LD E,A                  // Store this value (63-255) in E; it determines the (rising) pitch of the sound effect that will be made
    // XOR A                   // A=0 (black border)
    uint8_t border = 0;
    // LD BC,64                // C=64; this value determines the duration of the sound effect

    // TM111:
    for (int i = 0; i < 64; i++) {
      // OUT(254), A             // Produce a short note whose pitch is determined by E
      OUT(border);
      // XOR 24
      border ^= 24;

      // LD B, E
      // TM112:
      // DJNZ TM112
      millisleep(distance/216);

      // DEC C
      // JR NZ, TM111
    }

    // LD HL,22528             // Prepare BC, DE and HL for setting the attribute
    // LD DE,22529             // bytes in the top two-thirds of the screen
    // LD BC,511
    // LD A,(EUGHGT)           // Pick up the distance variable from EUGHGT
    // AND 12                  // Keep only bits 2 and 3
    distance = (uint8_t)(EUGHGT & 12);
    // RLCA                    // Shift bits 2 and 3 into bits 3 and 4; these bits determine the PAPER colour: 0, 1, 2 or 3
    distance = rotl(distance, 1);
    // OR 71                   // Set bits 0-2 (INK 7) and 6 (BRIGHT 1)
    distance |= 71;
    // LD (HL),A               // Copy this attribute value into the top two-thirds
    // LDIR                    // of the screen
    for (int i = 0; i < 512; i++) {
        Speccy_writeAttribute(22528 + i, distance);
    }

    // LD A,(EUGHGT)           // Add 4 to the distance variable at EUGHGT; this will
    // ADD A,4                 // move the boot sprite down two pixel rows
    // LD (EUGHGT),A
    // CP 196                  // Has the boot met the plinth yet?
    // JR NZ,LOOPFT            // Jump back if not
    tick();
  }

  // Now print the "Game Over" message, just to drive the point home.

  // Print "Game" (see MESSG) at (6,10)
  // LD IX,MESSG
  // LD C,4
  // LD DE,16586
  // CALL PMESS
  PMESS(&MESSG, 16586, 4);

  // Print "Over" (see MESSO) at (6,18)
  // LD IX,MESSO
  // LD C,4
  // LD DE,16594
  // CALL PMESS
  PMESS(&MESSO, 16594, 4);

  // LD BC,0                 // Prepare the delay counters for the following loop;
  // LD D,6                  // the counter in C will also determine the INK colours to use for the "Game Over" message

  // The following loop makes the "Game Over" message glisten for about 1.57s.
  // TM91:
  for (int d = 6; d > 0; d--) {
    // IMPORTANT: sleep handled at end of loop
    // Delay for about a millisecond
    // DJNZ TM91
    tick();

    for (int a = 0; a < 8; a++) {
      // LD A,C                  // Change the INK colour of the "G" in "Game" at
      // AND 7                   // (6,10)
      // OR 64
      // LD (22730),A
      // INC A                   // Change the INK colour of the "a" in "Game" at
      // AND 7                   // (6,11)
      // OR 64
      // LD (22731),A
      // INC A                   // Change the INK colour of the "m" in "Game" at
      // AND 7                   // (6,12)
      // OR 64
      // LD (22732),A
      // INC A                   // Change the INK colour of the "e" in "Game" at
      // AND 7                   // (6,13)
      // OR 64
      // LD (22733),A
      // INC A                   // Change the INK colour of the "O" in "Over" at
      // AND 7                   // (6,18)
      // OR 64
      // LD (22738),A
      // INC A                   // Change the INK colour of the "v" in "Over" at
      // AND 7                   // (6,19)
      // OR 64
      // LD (22739),A
      // INC A                   // Change the INK colour of the "e" in "Over" at
      // AND 7                   // (6,20)
      // OR 64
      // LD (22740),A
      // INC A                   // Change the INK colour of the "r" in "Over" at
      // AND 7                   // (6,21)
      // OR 64
      // LD (22741),A

      // IMPORTANT: haha, and you think this is correct? -MRC-
      Speccy_writeAttribute(22730 + a, (uint8_t) (((d + a) & 7) | 64));
      Terminal_redraw();
    }

    // DEC C                   // Decrement the counter in C
    // JR NZ,TM91              // Jump back unless it's zero
    // DEC D                   // Decrement the counter in D (initially 6)
    // JR NZ,TM91              // Jump back unless it's zero
  }

  // Display the title screen and play the theme tune
  // JP START
  // IMPORTANT: caller must return to START
}



// Decrease the air remaining in the current cavern
//
// Used by the routines at LOOP, LIGHTBEAM and NXSHEET. Returns with the zero
// flag set if there is no air remaining.
bool DECAIR() {
  // LD A,(CLOCK)            // Update the game clock at CLOCK
  // SUB 4
  // LD (CLOCK),A
  cavern.CLOCK -= 4;
  // CP 252                  // Was it just decreased from zero?
  // JR NZ,DECAIR_0          // Jump if not
  if (cavern.CLOCK == 252) {
    // LD A,(AIR)              // Pick up the value of the remaining air supply from AIR
    // CP 36                   // Has the air supply run out?
    // RET Z                   // Return (with the zero flag set) if so
    if (cavern.AIR == 36) {
      return true;
    }

    // DEC A                   // Decrement the air supply at AIR
    // LD (AIR),A
    cavern.AIR--;

    // LD A,(CLOCK)            // Pick up the value of the game clock at CLOCK
  }

  // DECAIR_0:
  // AND 224                 // A=INT(A/32); this value specifies how many pixels
  uint8_t count = (uint8_t)(cavern.CLOCK & 224);
  // RLCA                    // to draw from left to right in the cell at the right
  // RLCA                    // end of the air bar
  // RLCA
  count = rotl(count, 3);
  // LD E,0                  // Initialise E to 0 (all bits reset)
  uint8_t pixels = 0;
  // OR A                    // Do we need to draw any pixels in the cell at the right end of the air bar?
  // JR Z,DECAIR_2           // Jump if not
  if (count != 0) {
    // LD B,A                  // Copy the number of pixels to draw (1-7) to B
    // DECAIR_1:
    for (int i = count; i > 0; i--) {
      // RRC E                   // Set this many bits in E (from bit 7 towards bit 0)
      pixels = rotr(pixels, 1);
      // SET 7,E
      pixels |= 1 << 7;
      // DJNZ DECAIR_1
    }
  }

  // DECAIR_2:
  // LD A,(AIR)              // Pick up the value of the remaining air supply from AIR
  // LD L,A                  // Set HL to the display file address at which to draw
  // LD H,82                 // the top row of pixels in the cell at the right end of the air bar
  // LD B,4                  // There are four rows of pixels to draw
  // DECAIR_3:
  for (uint8_t msb = 82; msb < 86; msb++) {
    // Draw the four rows of pixels at the right end of the air bar
    // LD (HL),E
    Speccy_writeScreen(build_address(msb, cavern.AIR), pixels);
    // INC H
    // DJNZ DECAIR_3
  }
  Terminal_redraw();

  // XOR A                   // Reset the zero flag to indicate that there is still
  // INC A                   // some air remaining; these instructions are redundant, since the zero flag is already reset at this point
  // RET
  return false;
}


// Draw the current cavern to the screen buffer at 28672
//
// Used by the routine at STARTGAME.
void DRAWSHEET() {
  // FIXME: Screen Buffer: Screen File = 16384
  uint16_t addr = 28672;
  uint8_t msb, lsb;

  uint8_t *sprite;

  int offset = 0;
  int col;

  // The following loop draws the 512 tiles for the cavern to the screen buffer at 28672.
  for (int i = 0; i < 0 + 512; i++) {
    if (i > 255) {
      offset = 2048;
    }

    uint8_t tile_id = cavern.layout[i];

    if (cavern.BACKGROUND.id == tile_id) {
      sprite = &cavern.BACKGROUND.sprite[0];
    } else if (cavern.FLOOR.id == tile_id) {
      sprite = &cavern.FLOOR.sprite[0];
    } else if (cavern.CRUMBLING.id == tile_id) {
      sprite = &cavern.CRUMBLING.sprite[0];
    } else if (cavern.WALL.id == tile_id) {
      sprite = &cavern.WALL.sprite[0];
    } else if (cavern.CONVEYOR.id == tile_id) {
      sprite = &cavern.CONVEYOR.sprite[0];
    } else if (cavern.NASTY1.id == tile_id) {
      sprite = &cavern.NASTY1.sprite[0];
    } else if (cavern.NASTY2.id == tile_id) {
      sprite = &cavern.NASTY2.sprite[0];
    } else if (cavern.EXTRA.id == tile_id) {
      sprite = &cavern.EXTRA.sprite[0];
    }

    // Copy the graphic bytes to their destination cells
    uint16_t row_addr = addr;
    for (int b = 0; b < 8; b++) {
      speccy.memory[row_addr + offset] = sprite[b];
      split_address(row_addr, &msb, &lsb);
      msb++;
      row_addr = build_address(msb, lsb);
    }
    col++;

    split_address(addr, &msb, &lsb);
    if (col == 31) {
      col = 0;
      msb++;
    }
    lsb++;
    addr = build_address(msb, lsb);
  }

  // The empty cavern has been drawn to the screen buffer at 28672.
  // If we're in The Final Barrier, however, there is further work to do.
  if (cavern.SHEET == 19) {
    // Copy the graphic data from TITLESCR1 to the top half of the screen buffer at 28672
    // FIXME: Blank Screen Buffer: Screen File = 16384
    for (int i = 0; i <= 2048; i++) {
      speccy.memory[28672 + i] = TITLESCR1[i];
    }
  }
}
// IMPORTANT: this function has been rewritten above.
//
// DRAWSHEET:
// LD IX,24064             // Point IX at the first byte of the attribute buffer at 24064
// LD A,112                // Set the operand of the 'LD D,n' instruction at
// LD (35484),A            // SBMSB (below) to 112
// CALL DRAWSHEET_0        // Draw the tiles for the top half of the cavern to the screen buffer at 28672
// LD IX,24320             // Point IX at the 256th byte of the attribute buffer at 24064 in preparation for
//                         // drawing the bottom half of the cavern; this instruction is redundant, since
//                         // IX already holds 24320
// LD A,120                // Set the operand of the 'LD D,n' instruction at
// LD (35484),A            // SBMSB (below) to 120

// DRAWSHEET_0:
// LD C,0                  // C will count 256 tiles
// // The following loop draws 256 tiles (for either the top half or the bottom
// // half of the cavern) to the screen buffer at 28672.
// DRAWSHEET_1:
//   LD E,C                  // E holds the LSB of the screen buffer address
//   LD A,(IX+0)             // Pick up an attribute byte from the buffer at 24064; this identifies the type of tile to draw
//   LD HL,BACKGROUND        // Move HL through the attribute bytes and graphic
//   LD BC,72                // data of the background, floor, crumbling floor,
//   CPIR                    // wall, conveyor and nasty tiles starting at BACKGROUND until we find a byte that matches the attribute byte of the tile to be drawn
//   LD C,E                  // Restore the value of the tile counter in C
//   LD B,8                  // There are eight bytes in the tile
// SBMSB:
//   LD D,0                  // This instruction is set to either 'LD D,112' or 'LD D,120' above; now DE holds the appropriate address in the screen buffer at 28672
// DRAWSHEET_2:
//   LD A,(HL)               // Copy the tile graphic data to the screen buffer at
//   LD (DE),A               // 28672
//   INC HL
//   INC D
//   DJNZ DRAWSHEET_2
// INC IX                  // Move IX along to the next byte in the attribute buffer
// INC C                   // Have we drawn 256 tiles yet?
// JP NZ,DRAWSHEET_1       // If not, jump back to draw the next one
// // The empty cavern has been drawn to the screen buffer at 28672. If we're in
// // The Final Barrier, however, there is further work to do.
// LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
// CP 19                   // Is it The Final Barrier?
// RET NZ                  // Return if not
// LD HL,TITLESCR1         // Copy the graphic data from TITLESCR1 to the top
// LD DE,28672             // half of the screen buffer at 28672
// LD BC,2048
// LDIR
// RET



// Move Willy (1)
//
// Used by the routine at LOOP. This routine deals with Willy if he's jumping or falling.
//
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool MOVEWILLY() {
  // Is Willy jumping?
  // LD A,(AIRBORNE)         // Pick up the airborne status indicator from AIRBORNE
  // CP 1
  // JR NZ,MOVEWILLY_3       // Jump if not
  if (willy.AIRBORNE == 1) {
    // Willy is currently jumping.
    // LD A,(JUMPING)          // Pick up the jumping animation counter (0-17) from JUMPING
    uint8_t jump = willy.JUMPING;
    // RES 0,A                 // Now -8<=A<=8 (and A is even)
    jump &= ~(1 << 0);
    // SUB 8
    jump -= 8;
    // LD HL,PIXEL_Y           // Adjust Willy's pixel y-coordinate at PIXEL_Y
    // ADD A,(HL)              // depending on where Willy is in the jump
    // LD (HL),A
    willy.PIXEL_Y += jump;

    // CALL MOVEWILLY_7        // Adjust Willy's attribute buffer location at LOCATION depending on his pixel y-coordinate
    MOVEWILLY_7(willy.PIXEL_Y);

    // LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
    // CP (HL)                 // Is the top-left cell of Willy's sprite overlapping a wall tile?
    // JP Z,MOVEWILLY_10       // Jump if so
    // INC HL                  // Point HL at the top-right cell occupied by Willy's sprite
    // CP (HL)                 // Is the top-right cell of Willy's sprite overlapping a wall tile?
    // JP Z,MOVEWILLY_10       // Jump if so
    if ( memcmp(&speccy.memory[willy.PIXEL_Y/2], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 || memcmp(&speccy.memory[(willy.PIXEL_Y/2) + 1], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 ) {
      MOVEWILLY_10();
      return false;
    }

    // Increment the jumping animation counter at JUMPING
    // LD A,(JUMPING)
    // INC A
    // LD (JUMPING),A
    willy.JUMPING++;

    // A=J-8, where J (1-18) is the new value of the jumping animation counter
    // SUB 8
    // JP P,MOVEWILLY_0        // Jump if J>=8
    // NEG                     // A=8-J (1<=J<=7, 1<=A<=7)
    uint8_t anim_counter;
    if (willy.JUMPING < 8) {
      anim_counter = (uint8_t)(8 - willy.JUMPING);
    } else {
      anim_counter = (uint8_t)(willy.JUMPING - 8);
    }

    // MOVEWILLY_0:
    // INC A                   // A=1+ABS(J-8)
    anim_counter++;
    // RLCA                    // D=8*(1+ABS(J-8)); this value determines the pitch
    // RLCA                    // of the jumping sound effect (rising as Willy rises,
    // RLCA                    // falling as Willy falls)
    anim_counter = rotl(anim_counter, 3);

    // LD D,A
    uint8_t delay = anim_counter;
    // LD C,32                 // C=32; this value determines the duration of the jumping sound effect

    // Pick up the border colour for the current cavern from BORDER
    // LD A,(BORDER)
    uint8_t border = cavern.BORDER;

    // MOVEWILLY_1:
    for (int i = 32; i > 0; i--) {
      // OUT (254),A             // Make a jumping sound effect
      OUT(border);
      // XOR 24
      border ^= 24;

      // LD B,D
      // MOVEWILLY_2:
      // DJNZ MOVEWILLY_2
      millisleep(1); // FIXME: probably not the correct delay length.

      // DEC C
      // JR NZ,MOVEWILLY_1
    }

    // LD A,(JUMPING)          // Pick up the jumping animation counter (1-18) from JUMPING

    // CP 18                   // Has Willy reached the end of the jump?
    // JP Z,MOVEWILLY_8        // Jump if so
    // CP 16                   // Is the jumping animation counter now 16?
    // JR Z,MOVEWILLY_3        // Jump if so
    // CP 13                   // Is the jumping animation counter now 13?
    // JP NZ,MOVEWILLY2_6      // Jump if not
    if (willy.JUMPING == 18) {
      MOVEWILLY_8();
      return false;
    } else if (willy.JUMPING == 16) {
      // jump to MOVEWILLY_3
    } else if (willy.JUMPING != 13) {
      MOVEWILLY2_6();
      return false;
    }
  }

  // If we get here, then Willy is standing on the floor, or he's falling, or his
  // jumping animation counter is 13 (at which point Willy is on his way down and
  // is exactly two cell-heights above where he started the jump) or 16 (at which
  // point Willy is on his way down and is exactly one cell-height above where he
  // started the jump).
  // MOVEWILLY_3:

  uint16_t addr;

  // LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  // AND 15                  // Does Willy's sprite occupy six cells at the moment?
  // JR NZ,MOVEWILLY_4       // Jump if so
  if (willy.PIXEL_Y & 15) {
    // LD HL,(LOCATION)        // Pick up Willy's attribute buffer coordinates from LOCATION
    // LD DE,64                // Point HL at the left-hand cell below Willy's sprite
    // ADD HL,DE
    addr = (uint16_t)(willy.LOCATION + 64);
    // LD A,(CRUMBLING)        // Pick up the attribute byte of the crumbling floor tile for the current cavern from CRUMBLING
    // CP (HL)                 // Does the left-hand cell below Willy's sprite contain a crumbling floor tile?
    // CALL Z,CRUMBLE          // If so, make it crumble
    if ( memcmp(&speccy.memory[addr], cavern.CRUMBLING.sprite, sizeof(cavern.CRUMBLING.sprite)) == 0 ) {
      CRUMBLE(addr);
    }

    if ( memcmp(&speccy.memory[addr], cavern.NASTY1.sprite, sizeof(cavern.NASTY1.sprite)) != 0 ) {
      // LD A,(NASTY1)           // Pick up the attribute byte of the first nasty tile for the current cavern from NASTY1
      // CP (HL)                 // Does the left-hand cell below Willy's sprite contain a nasty tile?
      // JR Z,MOVEWILLY_4        // Jump if so
    } else if ( memcmp(&speccy.memory[addr], cavern.NASTY2.sprite, sizeof(cavern.NASTY2.sprite)) == 0 ) {
      // LD A,(NASTY2)           // Pick up the attribute byte of the second nasty tile for the current cavern from NASTY2
      // CP (HL)                 // Does the left-hand cell below Willy's sprite contain a nasty tile?
      // JR Z,MOVEWILLY_4        // Jump if so
    } else {
      // INC HL                  // Point HL at the right-hand cell below Willy's sprite
      addr++;

      // LD A,(CRUMBLING)        // Pick up the attribute byte of the crumbling floor tile for the current cavern from CRUMBLING
      // CP (HL)                 // Does the right-hand cell below Willy's sprite contain a crumbling floor tile?
      // CALL Z,CRUMBLE          // If so, make it crumble
      if ( memcmp(&speccy.memory[addr], cavern.CRUMBLING.sprite, sizeof(cavern.CRUMBLING.sprite)) == 0 ) {
        CRUMBLE(addr);
      }

      if ( memcmp(&speccy.memory[addr], cavern.NASTY1.sprite, sizeof(cavern.NASTY1.sprite)) == 0 ) {
        // LD A,(NASTY1)           // Pick up the attribute byte of the first nasty tile for the current cavern from NASTY1
        // CP (HL)                 // Does the right-hand cell below Willy's sprite contain a nasty tile?
        // JR Z,MOVEWILLY_4        // Jump if so
      } else if ( memcmp(&speccy.memory[addr], cavern.NASTY2.sprite, sizeof(cavern.NASTY2.sprite)) == 0 ) {
        // LD A,(NASTY2)           // Pick up the attribute byte of the second nasty tile for the current cavern from NASTY2
        // CP (HL)                 // Does the right-hand cell below Willy's sprite contain a nasty tile?
        // JR Z,MOVEWILLY_4        // Jump if so
      } else {
        // LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
        // CP (HL)                 // Set the zero flag if the right-hand cell below Willy's sprite is empty
        // DEC HL                  // Point HL at the left-hand cell below Willy's sprite
        // JP NZ,MOVEWILLY2        // Jump if the right-hand cell below Willy's sprite is not empty
        if ( memcmp(&speccy.memory[addr], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite)) != 0 ) {
          return MOVEWILLY2((uint8_t)(addr - 1));
        }
        addr--;
        // CP (HL)                 // Is the left-hand cell below Willy's sprite empty?
        // JP NZ,MOVEWILLY2        // Jump if not
        if ( memcmp(&speccy.memory[addr], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite)) != 0 ) {
          return MOVEWILLY2(addr);
        }
      }
    }
  }

  // MOVEWILLY_4:
  // LD A,(AIRBORNE)         // Pick up the airborne status indicator from AIRBORNE
  // CP 1                    // Is Willy jumping?
  // JP Z,MOVEWILLY2_6       // Jump if so
  if (willy.AIRBORNE == 1) {
    MOVEWILLY2_6();
    return false;
  }

  // If we get here, then Willy is either in the process of falling or just about to start falling.
  // LD HL,DMFLAGS           // Reset bit 1 at DMFLAGS: Willy is not moving left or
  // RES 1,(HL)              // right
  willy.DMFLAGS &= ~(1 << 1);

  // OR A                    // Is Willy already falling?
  // JP Z,MOVEWILLY_9        // Jump if not
  if (willy.AIRBORNE != 1) {
    MOVEWILLY_9();
    return false;
  }

  // INC A                   // Increment the airborne status indicator at AIRBORNE
  // LD (AIRBORNE),A
  willy.AIRBORNE++;

  // RLCA                    // D=16*A; this value determines the pitch of the
  // RLCA                    // falling sound effect
  // RLCA
  // RLCA
  // LD D,A
  uint8_t pitch = rotl(willy.AIRBORNE, 4);

  // LD C,32                 // C=32; this value determines the duration of the falling sound effect

  // LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
  uint8_t border = cavern.BORDER;

  // MOVEWILLY_5:
  for (int i = 32; i > 0; i--) {
    // OUT(254), A             // Make a falling sound effect
    OUT(border);
    // XOR 24
    border ^= 24;

    // LD B, D
    // MOVEWILLY_6:
    // DJNZ MOVEWILLY_6
    millisleep(1);

    // DEC C
    // JR NZ, MOVEWILLY_5
  }

  // LD A,(PIXEL_Y)          // Add 8 to Willy's pixel y-coordinate at PIXEL_Y;
  // ADD A,8                 // this moves Willy downwards by 4 pixels
  // LD (PIXEL_Y),A
  willy.PIXEL_Y += 8;

  MOVEWILLY_7(willy.PIXEL_Y); // IMPORTANT: implicit call to this subroutine.

  return false; // FIXME: is this the correct return value?
}

void MOVEWILLY_7(uint8_t y_coord) {
  uint8_t msb, lsb;
  uint8_t x_msb, x_lsb;

  // AND 240                 // L=16*Y, where Y is Willy's screen y-coordinate
  // LD L,A                  // (0-14)
  lsb = (uint8_t)(y_coord & 240);

  // XOR A                   // Clear A and the carry flag
  // RL L                    // Now L=32*(Y-8*INT(Y/8)), and the carry flag is set if Willy is in the lower half of the cavern (Y>=8)
  // if LSB bit 7 is set the `RL L` will set the Carry, which means we need to add one to the MSB
  // ADC A,92                // H=92 or 93 (MSB of the address of Willy's location
  // LD H,A                  // in the attribute buffer)
  msb = 0;
  if ((lsb >> 7) & 1) {
    msb = 1;
  }
  lsb = lsb << 1;
  msb += 92;

  // LD A,(LOCATION)         // Pick up Willy's screen x-coordinate (1-29) from
  // AND 31                  // bits 0-4 at LOCATION
  split_address(willy.LOCATION, &x_msb, &x_lsb);
  // OR L                    // Now L holds the LSB of Willy's attribute buffer
  // LD L,A                  // address
  x_lsb |= lsb;

  // LD (LOCATION),HL        // Store Willy's updated attribute buffer location at LOCATION
  willy.LOCATION = build_address(msb, x_lsb);

  // RET
}

// Willy has just finished a jump.
void MOVEWILLY_8() {
  // LD A,6                  // Set the airborne status indicator at AIRBORNE to 6:
  // LD (AIRBORNE),A         // Willy will continue to fall unless he's landed on a wall or floor block
  willy.AIRBORNE = 6;
  // RET
}

// Willy has just started falling.
void MOVEWILLY_9() {
  // LD A,2                  // Set the airborne status indicator at AIRBORNE to 2
  // LD (AIRBORNE),A
  willy.AIRBORNE = 2;
  // RET
}

// The top-left or top-right cell of Willy's sprite is overlapping a wall tile.
void MOVEWILLY_10() {
  // LD A,(PIXEL_Y)          // Adjust Willy's pixel y-coordinate at PIXEL_Y so
  // ADD A,16                // that the top row of cells of his sprite is just
  // AND 240                 // below the wall tile
  // LD (PIXEL_Y),A
  willy.PIXEL_Y = (uint8_t)((willy.PIXEL_Y + 16) & 240);

  // CALL MOVEWILLY_7        // Adjust Willy's attribute buffer location at LOCATION to account for this new pixel y-coordinate
  MOVEWILLY_7(willy.PIXEL_Y);

  // LD A,2                  // Set the airborne status indicator at AIRBORNE to 2:
  // LD (AIRBORNE),A         // Willy has started falling
  willy.AIRBORNE = 2;

  // LD HL,DMFLAGS           // Reset bit 1 at DMFLAGS: Willy is not moving left or
  // RES 1,(HL)              // right
  willy.DMFLAGS &= ~(1 << 1);

  // RET
}



// Animate a crumbling floor tile in the current cavern
//
// Used by the routine at MOVEWILLY.
//
// HL Address of the crumbling floor tile's location in the attribute buffer at 23552
void CRUMBLE(uint16_t addr) {
  uint8_t msb, lsb;

  // LD C,L                  // Point BC at the bottom row of pixels of the
  // LD A,H                  // crumbling floor tile in the screen buffer at 28672
  split_address(addr, &msb, &lsb);

  // ADD A,27
  msb += 27;
  // OR 7
  // LD B,A
  msb = (uint8_t)(msb | 7);

  // CRUMBLE_0:
  while (true) {
    // DEC B                   // Collect the pixels from the row above in A
    msb--;

    // LD A,(BC)
    // INC B                   // Copy these pixels into the row below it
    // LD (BC),A
    // DEC B                   // Point BC at the next row of pixels up
    speccy.memory[build_address(msb, lsb)] = speccy.memory[build_address((uint8_t)(msb + 1), lsb)];

    // LD A,B                  // Have we dealt with the bottom seven pixel rows of
    // AND 7                   // the crumbling floor tile yet?
    // JR NZ,CRUMBLE_0         // If not, jump back to deal with the next one up
    if (msb & 7) {
      break;
    }
  }

  // XOR A                   // Clear the top row of pixels in the crumbling floor
  // LD (BC),A               // tile
  speccy.memory[build_address(msb, lsb)] = 0;

  // LD A,B                  // Point BC at the bottom row of pixels in the
  // ADD A,7                 // crumbling floor tile
  // LD B,A
  msb += 7;

  // LD A,(BC)               // Pick up the bottom row of pixels in A
  // OR A                    // Is the bottom row clear?
  // RET NZ                  // Return if not
  if (speccy.memory[build_address(msb, lsb)] == 0) {
    return;
  }

  // The bottom row of pixels in the crumbling floor tile is clear. Time to put a background tile in its place.

  // LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  split_address(addr, &msb, &lsb);
  // INC H                   // Set HL to the address of the crumbling floor tile's
  // INC H                   // location in the attribute buffer at 24064
  // LD (HL),A               // Set the attribute at this location to that of the background tile
  // DEC H                   // Set HL back to the address of the crumbling floor
  // DEC H                   // tile's location in the attribute buffer at 23552
  memcpy(&speccy.memory[build_address((uint8_t)(msb + 2), lsb)], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite));

  // RET
}



// Move Willy (2)
//
// Used by the routine at MOVEWILLY. This routine checks the keyboard and
// joystick, and moves Willy left or right if necessary.
//
// HL Attribute buffer address of the left-hand cell below Willy's sprite
bool MOVEWILLY2(uint16_t addr) {
  // LD A,(AIRBORNE)         // Pick up the airborne status indicator from AIRBORNE
  // CP 12                   // Has Willy just landed after falling from too great a height?
  // JP NC,KILLWILLY_0       // If so, kill him
  if (willy.AIRBORNE == 12) {
    KILLWILLY_0();
    return true;
  }

  // LD E,255                // Initialise E to 255 (all bits set); it will be used to hold keyboard and joystick readings
  uint8_t input = 255;

  // XOR A                   // Reset the airborne status indicator at AIRBORNE
  // LD (AIRBORNE),A         // (Willy has landed safely)
  willy.AIRBORNE = 0;

  // LD A,(CONVEYOR)         // Pick up the attribute byte of the conveyor tile for the current cavern from CONVEYOR
  // CP (HL)                 // Does the attribute byte of the left-hand cell below Willy's sprite match that of the conveyor tile?
  // JR Z,MOVEWILLY2_0       // Jump if so
  // INC HL                  // Point HL at the right-hand cell below Willy's sprite
  // CP (HL)                 // Does the attribute byte of the right-hand cell below Willy's sprite match that of the conveyor tile?
  // JR NZ,MOVEWILLY2_1      // Jump if not
  if ( memcmp(&speccy.memory[addr], cavern.CONVEYOR.sprite, sizeof(cavern.CONVEYOR.sprite)) == 0 || memcmp(&speccy.memory[addr + 1], cavern.CONVEYOR.sprite, sizeof(cavern.CONVEYOR.sprite)) == 0 ) {
    // MOVEWILLY2_0:
    // LD A,(CONVDIR)          // Pick up the direction byte of the conveyor definition from CONVDIR (0=left, 1=right)
    // SUB 3                   // Now E=253 (bit 1 reset) if the conveyor is moving
    // LD E,A                  // left, or 254 (bit 0 reset) if it's moving right
    input = (uint8_t)(cavern.CONVEYOR.CONVDIR - 3);
  }
  // MOVEWILLY2_1:
  // LD BC,57342             // Read keys P-O-I-U-Y (right, left, right, left,
  // IN A,(C)                // right) into bits 0-4 of A
  // AND 31                  // Set bit 5 and reset bits 6 and 7
  // OR 32
  // AND E                   // Reset bit 0 if the conveyor is moving right, or bit 1 if it's moving left
  // LD E,A                  // Save the result in E
  input = (uint8_t)(((IN(57342) & 31) | 32) & input);
  // LD BC,64510             // Read keys Q-W-E-R-T (left, right, left, right,
  // IN A,(C)                // left) into bits 0-4 of A
  // AND 31                  // Keep only bits 0-4, shift them into bits 1-5, and
  // RLC A                   // set bit 0
  // OR 1
  // AND E                   // Merge this keyboard reading into bits 1-5 of E
  // LD E,A
  input = (uint8_t)((rotl(IN(64510), 1) | 1) & input);
  // LD B,247                // Read keys 1-2-3-4-5 ('5' is left) into bits 0-4 of
  // IN A,(C)                // A
  // RRCA                    // Rotate the result right and set bits 0-2 and 4-7;
  // OR 247                  // this ignores every key except '5' (left)
  // AND E                   // Merge this reading of the '5' key into bit 3 of E
  // LD E,A
  input = (uint8_t)((rotr(IN(63486), 1) | 247) & input);
  // LD B,239                // Read keys 0-9-8-7-6 ('8' is right) into bits 0-4 of
  // IN A,(C)                // A
  // OR 251                  // Set bits 0, 1 and 3-7; this ignores every key except '8' (right)
  // AND E                   // Merge this reading of the '8' key into bit 2 of E
  // LD E,A
  input = (uint8_t)((IN(61438) | 251) & input);
  // LD A,(KEMP)             // Collect the Kempston joystick indicator from KEMP
  // OR A                    // Is the joystick connected?
  // JR Z,MOVEWILLY2_2       // Jump if not
  if (game.KEMP) {
    /* TODO
        LD BC,31                // Collect input from the joystick
        IN A,(C)
        AND 3                   // Keep only bits 0 (right) and 1 (left) and flip them
        CPL
        AND E                   // Merge this reading of the joystick right and left
        LD E,A                  // buttons into bits 0 and 1 of E
    */
  }
  // At this point, bits 0-5 in E indicate the direction in which Willy is being
  // moved or trying to move. If bit 0, 2 or 4 is reset, Willy is being moved or
  // trying to move right; if bit 1, 3 or 5 is reset, Willy is being moved or
  // trying to move left.
  // MOVEWILLY2_2:
  // LD C,0                  // Initialise C to 0 (no movement)
  // LD A,E                  // Copy the movement bits into A
  // AND 42                  // Keep only bits 1, 3 and 5 (the 'left' bits)
  // CP 42                   // Are any of these bits reset?
  // JR Z,MOVEWILLY2_3       // Jump if not
  // LD C,4                  // Set bit 2 of C: Willy is moving left
  // MOVEWILLY2_3:
  // LD A,E                  // Copy the movement bits into A
  // AND 21                  // Keep only bits 0, 2 and 4 (the 'right' bits)
  // CP 21                   // Are any of these bits reset?
  // JR Z,MOVEWILLY2_4       // Jump if not
  // SET 3,C                 // Set bit 3 of C: Willy is moving right
  // FIXME: this is not yet handling the CONVDIR direction movement from above.
  uint8_t movement = 0;
  if (Terminal_keyLeft()) {
    movement = 4;
  } else if (Terminal_keyRight()) {
    movement = 8;
  }

  // MOVEWILLY2_4:
  // LD A,(DMFLAGS)          // Pick up Willy's direction and movement flags from DMFLAGS
  // ADD A,C                 // Point HL at the entry in the left-right movement
  // LD C,A                  // table at LRMOVEMENT that corresponds to the
  movement = willy.DMFLAGS + movement;
  // LD B,0                  // direction Willy is facing, and the direction in
  // LD HL,LRMOVEMENT        // which he is being moved or trying to move
  // ADD HL,BC
  // LD A,(HL)               // Update Willy's direction and movement flags at
  // LD (DMFLAGS),A          // DMFLAGS with the entry from the left-right movement table
  willy.DMFLAGS = LRMOVEMENT[movement];

  // That is left-right movement taken care of. Now check the jump keys.
  // LD BC,32510             // Read keys SHIFT-Z-X-C-V and B-N-M-SS-SPACE
  // IN A,(C)
  // AND 31                  // Are any of these keys being pressed?
  // CP 31
  // JR NZ,MOVEWILLY2_5      // Jump if so
  // LD B,239                // Read keys 0-9-8-7-6 into bits 0-4 of A
  // IN A,(C)
  // AND 9                   // Keep only bits 0 (the '0' key) and 3 (the '7' key)
  // CP 9                    // Is '0' or '7' being pressed?
  // JR NZ,MOVEWILLY2_5      // Jump if so
  // LD A,(KEMP)             // Collect the Kempston joystick indicator from KEMP
  // OR A                    // Is the joystick connected?
  // JR Z,MOVEWILLY2_6       // Jump if not
  // LD BC,31                // Collect input from the joystick
  // IN A,(C)
  // BIT 4,A                 // Is the fire button being pressed?
  // JR Z,MOVEWILLY2_6       // Jump if not
  if (Terminal_keyJump()) {
    // A jump key or the fire button is being pressed. Time to make Willy jump.
    // MOVEWILLY2_5:
    // XOR A                   // Initialise the jumping animation counter at JUMPING
    // LD (JUMPING),A
    willy.JUMPING = 0;

    // INC A                   // Set the airborne status indicator at AIRBORNE to 1:
    // LD (AIRBORNE),A         // Willy is jumping
    willy.AIRBORNE = 1;
  }

  MOVEWILLY2_6(); // IMPORTANT: implicit function call -MRC-
  return false; // willy is not dead. -MRC-
}

// This entry point is used by the routine at MOVEWILLY.
void MOVEWILLY2_6() {
  // LD A,(DMFLAGS)          // Pick up Willy's direction and movement flags from DMFLAGS
  // AND 2                   // Is Willy moving?
  // RET Z                   // Return if not
  if ((willy.DMFLAGS & 2) == 0) {
    return;
  }

  // LD A,(DMFLAGS)          // Pick up Willy's direction and movement flags from DMFLAGS
  // AND 1                   // Is Willy facing right? (0 = right, 1 = left)
  // JP Z,MOVEWILLY2_9       // Jump if so
  if (((willy.DMFLAGS >> 0) & 1) == 0) {
    MOVEWILLY2_9();
    return;
  }

  // Willy is moving left.
  // LD A,(FRAME)            // Pick up Willy's animation frame from FRAME
  // OR A                    // Is it 0?
  // JR Z,MOVEWILLY2_7       // If so, jump to move Willy's sprite left across a cell boundary
  if (willy.FRAME == 0) {
    MOVEWILLY2_7();
    return;
  }

  // DEC A                   // Decrement Willy's animation frame at FRAME
  // LD (FRAME),A
  willy.FRAME--;

  // RET
}

// Willy's sprite is moving left across a cell boundary. In the comments that
// follow, (x,y) refers to the coordinates of the top-left cell currently
// occupied by Willy's sprite.
void MOVEWILLY2_7() {
  // LD HL,(LOCATION)        // Collect Willy's attribute buffer coordinates from LOCATION
  // DEC HL                  // Point HL at the cell at (x-1,y+1)
  uint16_t addr = (uint16_t)(willy.LOCATION - 1);
  // LD DE,32
  // ADD HL,DE
  addr += 32;
  // LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  // CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  // RET Z                   // Return if so without moving Willy (his path is blocked)
  if ( memcmp(&speccy.memory[addr], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 ) {
    return;
  }

  // LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  // AND 15                  // Does Willy's sprite currently occupy only two rows of cells?
  // JR Z,MOVEWILLY2_8       // Jump if so
  if ((willy.PIXEL_Y & 15) != 0) {
    // LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
    // ADD HL,DE               // Point HL at the cell at (x-1,y+2)
    // CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
    // RET Z                   // Return if so without moving Willy (his path is blocked)
    if ( memcmp(&speccy.memory[addr + 32], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 ) {
      return;
    }

    // FIXME: does this Carry flag get used anywhere?
    // OR A                    // Clear the carry flag for subtraction
    // SBC HL,DE               // Point HL at the cell at (x-1,y+1)
  }

  // MOVEWILLY2_8:
  // LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  // FIXME: does this Carry flag get used anywhere?
  // OR A                    // Clear the carry flag for subtraction
  // SBC HL,DE               // Point HL at the cell at (x-1,y)
  addr -= 32;
  // CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  // RET Z                   // Return if so without moving Willy (his path is blocked)
  if ( memcmp(&speccy.memory[addr], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 ) {
    return;
  }

  // LD (LOCATION),HL        // Save Willy's new attribute buffer coordinates (in HL) at LOCATION
  willy.LOCATION = addr;

  // LD A,3                  // Change Willy's animation frame at FRAME from 0 to 3
  // LD (FRAME),A
  willy.FRAME = 3;

  // RET
}

// Willy is moving right.
void MOVEWILLY2_9() {
  // LD A,(FRAME)            // Pick up Willy's animation frame from FRAME
  // CP 3                    // Is it 3?
  // JR Z,MOVEWILLY2_10      // If so, jump to move Willy's sprite right across a cell boundary
  if (willy.FRAME == 3) {
    MOVEWILLY2_10();
  } else {
    // INC A                   // Increment Willy's animation frame at FRAME
    // LD (FRAME),A
    willy.FRAME++;
  }
  // RET
}

// Willy's sprite is moving right across a cell boundary. In the comments that
// follow, (x,y) refers to the coordinates of the top-left cell currently
// occupied by Willy's sprite.
void MOVEWILLY2_10() {
  // LD HL,(LOCATION)        // Collect Willy's attribute buffer coordinates from LOCATION
  // INC HL                  // Point HL at the cell at (x+2,y)
  // INC HL
  uint16_t addr = willy.LOCATION += 2;

  // LD DE,32                // Prepare DE for addition
  // LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  // ADD HL,DE               // Point HL at the cell at (x+2,y+1)
  addr += 32;
  // CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  // RET Z                   // Return if so without moving Willy (his path is blocked)
  if ( memcmp(&speccy.memory[addr], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 ) {
    return;
  }

  // LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  // AND 15                  // Does Willy's sprite currently occupy only two rows of cells?
  // JR Z,MOVEWILLY2_11      // Jump if so
  if ((willy.PIXEL_Y & 15) != 0) {
    // LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
    // ADD HL,DE               // Point HL at the cell at (x+2,y+2)
    // CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
    // RET Z                   // Return if so without moving Willy (his path is blocked)
    if ( memcmp(&speccy.memory[addr + 32], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 ) {
      return;
    }
    // OR A                    // Clear the carry flag for subtraction
    // SBC HL,DE               // Point HL at the cell at (x+2,y+1)
  }

  // MOVEWILLY2_11:
  // LD A,(WALL)             // Pick up the attribute byte of the wall tile for the current cavern from WALL
  // OR A                    // Clear the carry flag for subtraction
  // SBC HL,DE               // Point HL at the cell at (x+2,y)
  addr -= 32;
  // CP (HL)                 // Is there a wall tile in the cell pointed to by HL?
  // RET Z                   // Return if so without moving Willy (his path is blocked)
  if ( memcmp(&speccy.memory[addr], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 ) {
    return;
  }

  // DEC HL                  // Point HL at the cell at (x+1,y)
  addr--;
  // LD (LOCATION),HL        // Save Willy's new attribute buffer coordinates (in HL) at LOCATION
  willy.LOCATION = addr;

  // XOR A                   // Change Willy's animation frame at FRAME from 3 to 0
  // LD (FRAME),A
  willy.FRAME = 0;

  // RET
}


// Kill Willy
//
// Used by the routine at WILLYATTR when Willy hits a nasty.
bool KILLWILLY() {
  // POP HL                  // Drop the return address from the stack
  return KILLWILLY_0();
}

// This entry point is used by the routines at MOVEWILLY2 (when Willy lands
// after falling from too great a height), DRAWHG (when Willy collides with a
// horizontal guardian), EUGENE (when Willy collides with Eugene), VGUARDIANS
// (when Willy collides with a vertical guardian) and KONGBEAST (when Willy
// collides with the Kong Beast).
bool KILLWILLY_0() {
  // POP HL                  // Drop the return address from the stack
  return KILLWILLY_1();
}

// This entry point is used by the routine at SKYLABS when a Skylab falls on Willy.
bool KILLWILLY_1() {
  // LD A,255                // Set the airborne status indicator at AIRBORNE to
  // LD (AIRBORNE),A         // 255 (meaning Willy has had a fatal accident)
  willy.AIRBORNE = 255;

  // JP LOOP_4               // Jump back into the main loop
  return true;
}


// Move the horizontal guardians in the current cavern
//
// Used by the routine at LOOP.
void MOVEHG() {
  uint8_t msb, lsb;

  // The guardian-moving loop begins here.
  for (int i = 0; i < 4; i++) {
    GuardianHorizontal *guardian = &HGUARDS[i];

    if (GuardianH_is_blank(guardian)) {
        continue;
    }

    uint8_t current_clock = cavern.CLOCK;
    current_clock &= 4;
    current_clock = rotr(current_clock, 3);

    // Jump to consider the next guardian if this one is not due to be moved on this pass
    if ((current_clock & guardian->speedColour)) {
      continue;
    }

    // The guardian will be moved on this pass.
    // Pick up the current animation frame (0-7)

    // Is it 3 (the terminal frame for a guardian moving right)?
    // If so move the guardian right across a cell boundary or turn it round
    if (guardian->frame == 3) {
      // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552
      split_address(guardian->attributeAddress, &msb, &lsb);

      // Has the guardian reached the rightmost point in its path?
      if (lsb == guardian->addressRightLSB) {
        // Set the animation frame to 7 (turning the guardian round to face left)
        guardian->frame = 7;
        // Jump forward to consider the next guardian
      } else {
        // Set the animation frame to 0 (the initial frame for a guardian moving right)
        guardian->frame = 0;
        // Increment the guardian's x-coordinate (moving it right across a cell boundary)
        lsb++;
        guardian->attributeAddress = build_address(msb, lsb);
        // Jump forward to consider the next guardian
      }

    // Is the current animation frame 4 (the terminal frame for a guardian moving left)?
    // If so move the guardian left across a cell boundary or turn it round
    } else if (guardian->frame == 4) {
      // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552
      split_address(guardian->attributeAddress, &msb, &lsb);

      // Has the guardian reached the leftmost point in its path?
      if (lsb == guardian->addressLeftLSB) {
        // Set the animation frame to 0 (turning the guardian round to face right)
        guardian->frame = 0;
      } else {
        // Set the animation frame to 7 (the initial frame for a guardian moving left)
        guardian->frame = 7;
        // Decrement the guardian's x-coordinate (moving it left across a cell boundary)
        lsb--;
        guardian->attributeAddress = build_address(msb, lsb);
      }

    // Jump if the animation frame is 5, 6 or 7
    } else if (guardian->frame == 5 || guardian->frame == 6 || guardian->frame == 7) {
      // Decrement the animation frame (this guardian is moving left)
      guardian->frame--;
    } else {
      // Increment the animation frame (this guardian is moving right)
      guardian->frame++;
    }

    // The current guardian definition has been dealt with. Time for the next one.
  }
}
// FIXME: rewrote this function above
void MOVEHG_XX() {
  // LD IY,HGUARDS           // Point IY at the first byte of the first horizontal guardian definition at HGUARDS
  // LD DE,7                 // Prepare DE for addition (there are 7 bytes in a guardian definition)

  // IMPORTANT: each guardian is an array, containing 6 uint16_t entries.

  uint8_t msb;
  uint8_t lsb;

  // The guardian-moving loop begins here.
  // MOVEHG_0:
  for (int i = 0; i < 4; i++) {
    // LD A,(IY+0)             // Pick up the first byte of the guardian definition
    // CP 255                  // Have we dealt with all the guardians yet?
    // RET Z                   // Return if so

    GuardianHorizontal *guardian = &HGUARDS[i];

    // OR A                    // Is this guardian definition blank?
    // JR Z,MOVEHG_6           // If so, skip it and consider the next one
    if (GuardianH_is_blank(guardian)) {
        continue;
    }

      // LD A,(CLOCK)            // Pick up the value of the game clock at CLOCK
    uint8_t current_clock = cavern.CLOCK;
    // AND 4                   // Move bit 2 (which is toggled on each pass through
    current_clock &= 4;
    // RRCA                    // the main loop) to bit 7 and clear all the other
    // RRCA                    // bits
    // RRCA
    current_clock = rotr(current_clock, 3);

    // AND (IY+0)              // Combine this bit with bit 7 of the first byte of the guardian definition,]
                               // which specifies the guardian's animation speed: 0=normal, 1=slow
    // JR NZ,MOVEHG_6          // Jump to consider the next guardian if this one is not due to be moved on this pass
    if ((current_clock & guardian->speedColour)) {
        continue;
    }

    // The guardian will be moved on this pass.
    // LD A,(IY+4)             // Pick up the current animation frame (0-7)
    // CP 3                    // Is it 3 (the terminal frame for a guardian moving right)?
    // JR Z,MOVEHG_2           // Jump if so to move the guardian right across a cell boundary or turn it round
    if (guardian->frame != 3) {
      // CP 4                    // Is the current animation frame 4 (the terminal frame for a guardian moving left)?
      // JR Z,MOVEHG_4           // Jump if so to move the guardian left across a cell boundary or turn it round
      if (guardian->frame != 4) {
        // JR NC,MOVEHG_1          // Jump if the animation frame is 5, 6 or 7
        if (guardian->frame == 5 || guardian->frame == 6 || guardian->frame == 7) {
          // MOVEHG_1:
          // DEC (IY+4)              // Decrement the animation frame (this guardian is moving left)
          guardian->frame--;
          // JR MOVEHG_6             // Jump forward to consider the next guardian
          continue;
        } else {
          // INC (IY+4)              // Increment the animation frame (this guardian is moving right)
          guardian->frame++;
          // JR MOVEHG_6             // Jump forward to consider the next guardian
          continue;
        }
      }
    } else {
      // MOVEHG_2:
      // LD A,(IY+1)             // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552
      split_address(guardian->attributeAddress, &msb, &lsb);

      // CP (IY+6)               // Has the guardian reached the rightmost point in its path?
      // JR NZ,MOVEHG_3          // Jump if not
      if (lsb == guardian->addressRightLSB) {
        // LD (IY+4),7             // Set the animation frame to 7 (turning the guardian round to face left)
        guardian->frame = 7;
        // JR MOVEHG_6             // Jump forward to consider the next guardian
        continue;
      } else {
        // MOVEHG_3:
        // LD (IY+4),0             // Set the animation frame to 0 (the initial frame for a guardian moving right)
        guardian->frame = 0;
        // INC (IY+1)              // Increment the guardian's x-coordinate (moving it right across a cell boundary)
        lsb++;
        guardian->attributeAddress = build_address(msb, lsb);
        // JR MOVEHG_6             // Jump forward to consider the next guardian
        continue;
      }
    }

    // MOVEHG_4:
    // LD A,(IY+1)             // Pick up the LSB of the address of the guardian's location in the attribute buffer at 23552
    split_address(guardian->attributeAddress, &msb, &lsb);

    // CP (IY+5)               // Has the guardian reached the leftmost point in its path?
    // JR NZ,MOVEHG_5          // Jump if not
    if (lsb == guardian->addressLeftLSB) {
      // LD (IY+4),0             // Set the animation frame to 0 (turning the guardian round to face right)
      guardian->frame = 0;
      // JR MOVEHG_6             // Jump forward to consider the next guardian
      continue;
    } else {
      // MOVEHG_5:
      // LD (IY+4),7             // Set the animation frame to 7 (the initial frame for a guardian moving left)
      guardian->frame = 7;
      // DEC (IY+1)              // Decrement the guardian's x-coordinate (moving it left across a cell boundary)
      lsb--;
      guardian->attributeAddress = build_address(msb, lsb);
    }

    // The current guardian definition has been dealt with. Time for the next one.
    // MOVEHG_6:
    // ADD IY,DE               // Point IY at the first byte of the next horizontal guardian definition
    // JR MOVEHG_0             // Jump back to deal with the next horizontal guardian
  }
}


// Move and draw the light beam in Solar Power Generator
//
// Used by the routine at LOOP.
void LIGHTBEAM() {
  // LD HL,23575             // Point HL at the cell at (0,23) in the attribute buffer at 23552 (the source of the light beam)
  // FIXME: Screen Buffer: Screen File = 22551
  uint16_t addr = 23575;

  // LD DE,32                // Prepare DE for addition (the beam travels vertically downwards to start with)
  int8_t dir = 32; // gets toggled between 32 and -1.

  // The beam-drawing loop begins here.
  // LIGHTBEAM_0:
  while (true) {
    // LD A,(FLOOR)            // Pick up the attribute byte of the floor tile for the cavern from FLOOR
    // CP (HL)                 // Does HL point at a floor tile?
    // RET Z                   // Return if so (the light beam stops here)
    // LD A,(WALL)             // Pick up the attribute byte of the wall tile for the cavern from WALL
    // CP (HL)                 // Does HL point at a wall tile?
    // RET Z                   // Return if so (the light beam stops here)
    if ( memcmp(&speccy.memory[addr], cavern.FLOOR.sprite, sizeof(cavern.FLOOR.sprite)) == 0 || memcmp(&speccy.memory[addr], cavern.WALL.sprite, sizeof(cavern.WALL.sprite)) == 0 ) {
      return;
    }

    // LD A,39                 // A=39 (INK 7: PAPER 4)
    // CP (HL)                 // Does HL point at a tile with this attribute value?
    // JR NZ,LIGHTBEAM_1       // Jump if not (the light beam is not touching Willy)
    if (speccy.memory[addr] == 39) {
      // EXX                     // Switch to the shadow registers briefly (to preserve DE and HL)
      // CALL DECAIR             // Decrease the air supply by four units
      DECAIR();
      // CALL DECAIR
      DECAIR();
      // CALL DECAIR
      DECAIR();
      // CALL DECAIR
      DECAIR();
      // EXX                     // Switch back to the normal registers (restoring DE and HL)
      // JR LIGHTBEAM_2          // Jump forward to draw the light beam over Willy
    } else {
      // LIGHTBEAM_1:
      // LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the cavern from BACKGROUND
      // CP (HL)                 // Does HL point at a background tile?
      // JR Z,LIGHTBEAM_2        // Jump if so (the light beam will not be reflected at this point)
      if ( memcmp(&speccy.memory[addr], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite)) == 0 ) {
        // LD A,E                  // Toggle the value in DE between 32 and -1 (and
        // XOR 223                 // therefore the direction of the light beam between
        dir ^= 223;
        // LD E,A                  // vertically downwards and horizontally to the left):
        // LD A,D                  // the light beam has hit a guardian
        // CPL
        // LD D,A
        dir = ~dir;
      }
    }

    // LIGHTBEAM_2:
    // LD (HL),119             // Draw a portion of the light beam with attribute value 119 (INK 7: PAPER 6: BRIGHT 1)
    speccy.memory[addr] = 119;
    // ADD HL,DE               // Point HL at the cell where the next portion of the light beam will be drawn
    addr += dir;
    // JR LIGHTBEAM_0          // Jump back to draw the next portion of the light beam
  }
}


// Draw the horizontal guardians in the current cavern
//
// Used by the routine at LOOP.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool DRAWHG() {
  uint8_t msb, lsb;
  uint16_t addr;

  // LD IY,HGUARDS           // Point IY at the first byte of the first horizontal guardian definition at HGUARDS

  // The guardian-drawing loop begins here.
  // DRAWHG_0:
  for (int i = 0; i < 4; i++) {
    // LD A,(IY+0)             // Pick up the first byte of the guardian definition
    // CP 255                  // Have we dealt with all the guardians yet?
    // RET Z                   // Return if so

    GuardianHorizontal *guardian = &HGUARDS[i];

    // OR A                    // Is this guardian definition blank?
    // JR Z,DRAWHG_2           // If so, skip it and consider the next one
    if (guardian->attributeAddress == 0) {
      continue;
    }

    // LD DE,31                // Prepare DE for addition

    // LD L,(IY+1)             // Point HL at the address of the guardian's location
    // LD H,(IY+2)             // in the attribute buffer at 23552
    addr = guardian->attributeAddress;

    uint8_t attr = (uint8_t)guardian->speedColour;
    // AND 127                 // Reset bit 7 (which specifies the animation speed) of the attribute byte, ensuring no FLASH
    attr &= 127;
    // LD (HL),A               // Set the attribute bytes for the guardian in the
    speccy.memory[addr] = attr;
    // INC HL                  // buffer at 23552
    addr++;
    // LD (HL),A
    speccy.memory[addr] = attr;

    // ADD HL,DE
    addr += 31;
    // LD (HL),A
    speccy.memory[addr] = attr;

    // INC HL
    addr++;
    // LD (HL),A
    speccy.memory[addr] = attr;

    // LD C,1                  // Prepare C for the call to the drawing routine at DRWFIX later on

    // LD A,(IY+4)             // Pick up the animation frame (0-7)
    uint8_t anim_frame = (uint8_t)guardian->frame;
    // RRCA                    // Multiply it by 32
    // RRCA
    // RRCA
    // LD E,A                  // Copy the result to E
    anim_frame = rotr(anim_frame, 3);

    // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
    // CP 7                    // Are we in one of the first seven caverns?
    // JR C,DRAWHG_1           // Jump if so
    // CP 9                    // Are we in The Endorian Forest?
    // JR Z,DRAWHG_1           // Jump if so
    // CP 15                   // Are we in The Sixteenth Cavern?
    // JR Z,DRAWHG_1           // Jump if so
    if (cavern.SHEET == 7 || cavern.SHEET == 9 || cavern.SHEET == 15) {
      // SET 7,E                 // Add 128 to E (the horizontal guardians in this cavern use frames 4-7 only)
      anim_frame |= (1 << 7);
    }

    // DRAWHG_1:
    // LD D,129                // Point DE at the graphic data for the appropriate guardian sprite (at GGDATA+E)
    // LD L,(IY+1)             // Point HL at the address of the guardian's location
    // LD H,(IY+3)             // in the screen buffer at 24576
    split_address(guardian->attributeAddress, &msb, &lsb);
    addr = build_address((uint8_t)guardian->addressMSB, lsb);

    // CALL DRWFIX             // Draw the guardian to the screen buffer at 24576
    bool kill_willy = DRWFIX(&guardian->GGDATA[anim_frame], addr, 1);

    // JP NZ,KILLWILLY_0       // Kill Willy if the guardian collided with him
    if (kill_willy) {
      KILLWILLY_0();
      return true;
    }

    // // The current guardian definition has been dealt with. Time for the next one.
    // DRAWHG_2:
    // LD DE,7                 // Point IY at the first byte of the next horizontal
    // ADD IY,DE               // guardian definition
    // JR DRAWHG_0             // Jump back to deal with the next horizontal guardian
  }

  return false; // IMPORTANT Willy has not died! -MRC-
}


// Move and draw Eugene in Eugene's Lair
//
// Used by the routine at LOOP. First we move Eugene up or down, or change his direction.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool EUGENE() {
  // LD A,(ITEMATTR)         // Pick up the attribute of the last item drawn from ITEMATTR
  // OR A                    // Have all the items been collected?
  // JR Z,EUGENE_0           // Jump if so
  // LD A,(EUGDIR)           // Pick up Eugene's direction from EUGDIR
  // OR A                    // Is Eugene moving downwards?
  // JR Z,EUGENE_0           // Jump if so
  if (game.ITEMATTR != 0 && EUGDIR != 0) {
    // LD A,(EUGHGT)           // Pick up Eugene's pixel y-coordinate from EUGHGT
    // DEC A                   // Decrement it (moving Eugene up)
    // JR Z,EUGENE_1           // Jump if Eugene has reached the top of the cavern
    if (EUGHGT - 1 == 0) {
      EUGDIR = (uint8_t)!EUGDIR;
    } else {
      // LD (EUGHGT),A           // Update Eugene's pixel y-coordinate at EUGHGT
      EUGHGT--;
      // JR EUGENE_2
    }
  } else {
    // EUGENE_0:
    // LD A,(EUGHGT)           // Pick up Eugene's pixel y-coordinate from EUGHGT
    // INC A                   // Increment it (moving Eugene down)
    // CP 88                   // Has Eugene reached the portal yet?
    // JR Z,EUGENE_1           // Jump if so
    if (EUGHGT + 1 == 88) {
      EUGDIR = (uint8_t)!EUGDIR;
    } else {
      // LD (EUGHGT),A           // Update Eugene's pixel y-coordinate at EUGHGT
      EUGHGT++;
      // JR EUGENE_2
    }
  }

  // IMPORTANT: this toggle has been inlined in the if statement above.
  // EUGENE_1:
  // LD A,(EUGDIR)           // Toggle Eugene's direction at EUGDIR
  // XOR 1
  // LD (EUGDIR),A

  uint8_t msb, lsb;
  uint8_t x_msb, x_lsb;
  uint16_t addr;

  // Now that Eugene's movement has been dealt with, it's time to draw him.
  // EUGENE_2:
  // LD A,(EUGHGT)           // Pick up Eugene's pixel y-coordinate from EUGHGT
  // AND 127                 // Point DE at the entry in the screen buffer address
  uint8_t y_coord = (uint8_t)(EUGHGT & 127);
  // RLCA                    // lookup table at SBUFADDRS that corresponds to
  y_coord = rotl(y_coord, 1);
  // LD E,A                  // Eugene's y-coordinate
  // LD D,131

  // LD A,(DE)               // Point HL at the address of Eugene's location in the
  addr = SBUFADDRS[y_coord];
  // OR 15                   // screen buffer at 24576
  addr |= 15;

  split_address(addr, &msb, &lsb);
  // LD L,A
  // INC DE
  y_coord++;
  // LD A,(DE)
  addr = SBUFADDRS[y_coord];
  split_address(addr, &x_msb, &x_lsb);
  // LD H,A
  addr = build_address(x_msb, lsb);

  // LD DE,32992             // Draw Eugene to the screen buffer at 24576
  // LD C,1
  // CALL DRWFIX
  bool kill_willy = DRWFIX(&EUGENEG, addr, 1);

  // JP NZ,KILLWILLY_0       // Kill Willy if Eugene collided with him
  if (kill_willy) {
    KILLWILLY_0();
    return true;
  }


  // LD A,(EUGHGT)           // Pick up Eugene's pixel y-coordinate from EUGHGT
  // AND 120                 // Point HL at the address of Eugene's location in the
  y_coord = (uint8_t)(EUGHGT & 120);
  // RLCA                    // attribute buffer at 23552
  y_coord = rotl(y_coord, 1);
  // OR 7
  y_coord |= 7;

  // IMPORTANT: SCF should set the carry flag, and the following RL loads that into bit 0 -MRC-
  // SCF
  // RL A
  msb = 0;
  if ((y_coord >> 7) & 1) {
    msb = 1;
  }
  y_coord = rotl(y_coord, 1);
  y_coord |= 1 << 0;
  // LD L,A

  // LD A,0
  // ADC A,92
  msb += 92;
  // LD H,A
  addr = build_address(msb, y_coord);

  // LD A,(ITEMATTR)         // Pick up the attribute of the last item drawn from ITEMATTR
  // OR A                    // Set the zero flag if all the items have been collected
  // LD A,7                  // Assume we will draw Eugene with white INK
  uint8_t ink_color = 7;
  // JR NZ,EUGENE_3          // Jump if there are items remaining to be collected
  if (game.ITEMATTR == 0) {
    // LD A,(CLOCK)            // Pick up the value of the game clock at CLOCK
    // RRCA                    // Move bits 2-4 into bits 0-2 and clear the other
    // RRCA                    // bits; this value (which decreases by one on each
    // AND 7                   // pass through the main loop) will be Eugene's INK colour
    ink_color = (uint8_t)(rotr(cavern.CLOCK, 2) & 7);
  }
  EUGENE_3(addr, ink_color); // IMPORTANT: implicit jump -MRC-

  return false;
}


// This entry point is used by the routines at SKYLABS (to set the attributes
// for a Skylab), VGUARDIANS (to set the attributes for a vertical guardian) and
// KONGBEAST (to set the attributes for the Kong Beast).
void EUGENE_3(uint16_t addr, uint8_t ink_color) {
  // LD (HL),A               // Save the INK colour in the attribute buffer temporarily
  speccy.memory[addr] = ink_color;

  // LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  // AND 248                 // Combine its PAPER colour with the chosen INK colour
  // OR (HL)
  // LD (HL),A               // Set the attribute byte for the top-left cell of the sprite in the attribute buffer at 23552
  speccy.memory[addr] = (uint8_t)((cavern.BACKGROUND.sprite[0] & 248) | speccy.memory[addr]);

  // LD DE,31                // Prepare DE for addition

  // INC HL                  // Set the attribute byte for the top-right cell of
  addr++;
  // LD (HL),A               // the sprite in the attribute buffer at 23552
  speccy.memory[addr] = ink_color;

  // ADD HL,DE               // Set the attribute byte for the middle-left cell of
  addr += 31;
  // LD (HL),A               // the sprite in the attribute buffer at 23552
  speccy.memory[addr] = ink_color;

  // INC HL                  // Set the attribute byte for the middle-right cell of
  addr++;
  // LD (HL),A               // the sprite in the attribute buffer at 23552
  speccy.memory[addr] = ink_color;

  // ADD HL,DE               // Set the attribute byte for the bottom-left cell of
  addr += 31;
  // LD (HL),A               // the sprite in the attribute buffer at 23552
  speccy.memory[addr] = ink_color;

  // INC HL                  // Set the attribute byte for the bottom-right cell of
  addr++;
  // LD (HL),A               // the sprite in the attribute buffer at 23552
  speccy.memory[addr] = ink_color;

  // RET
}


// Move and draw the Skylabs in Skylab Landing Bay
//
// Used by the routine at LOOP.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool SKYLABS() {
  // LD IY,VGUARDS           // Point IY at the first byte of the first vertical guardian definition at VGUARDS

  uint8_t msb;
  uint8_t lsb;
  uint16_t addr;

  // The Skylab-moving loop begins here.
  // SKYLABS_0:
  for (int i = 0; i < 4; i++) {
    GuardianVertical *guardian = &VGUARDS[i];

    // LD A,(IY+0)             // Pick up the first byte of the guardian definition
    // CP 255                  // Have we dealt with all the Skylabs yet?
    // JP Z,LOOP_3             // If so, re-enter the main loop
    if (guardian->attribute == 255) {
        break;
    }

    // LD A,(IY+2)             // Pick up the Skylab's pixel y-coordinate
    // CP (IY+6)               // Has it reached its crash site yet?
    // JR NC,SKYLABS_1         // Jump if so
    if (guardian->yCoord != guardian->yCoordMaximum) {
      // ADD A,(IY+4)            // Increment the Skylab's y-coordinate (moving it
      // LD (IY+2),A             // downwards)
      guardian->yCoord += guardian->yPixelIncrement;
      // JR SKYLABS_2
    } else {
      // The Skylab has reached its crash site. Start or continue its disintegration.
      // SKYLABS_1:
      // INC (IY+1)              // Increment the animation frame
      guardian->frame++;

      // LD A,(IY+1)             // Pick up the animation frame
      // CP 8                    // Has the Skylab completely disintegrated yet?
      // JR NZ,SKYLABS_2         // Jump if not
      if (guardian->frame == 8) {
        // LD A,(IY+5)             // Reset the Skylab's pixel y-coordinate
        // LD (IY+2),A
        guardian->yCoord = guardian->yCoordMinimum;

        // LD A,(IY+3)             // Add 8 to the Skylab's x-coordinate (wrapping around
        // ADD A,8                 // at the right side of the screen)
        guardian->xCoord += 8;
        // AND 31
        // LD (IY+3),A
        guardian->xCoord &= 31;

        // LD (IY+1),0             // Reset the animation frame to 0
        guardian->frame = 0;
      }
    }

    // Now that the Skylab's movement has been dealt with, time to draw it.

    // FIXME: are these address calculations correct? -MRC-
    // Are x/y coords uint16_t or uint8_t values?

    // SKYLABS_2:
    // LD E,(IY+2)             // Pick up the Skylab's pixel y-coordinate in E
    // RLC E                   // Point DE at the entry in the screen buffer address
    // LD D,131                // lookup table at SBUFADDRS that corresponds to the Skylab's pixel y-coordinate
    uint8_t y_coord = rotl(guardian->yCoord, 1);
    // LD A,(DE)               // Point HL at the address of the Skylab's location in
    addr = SBUFADDRS[y_coord];
    // ADD A,(IY+3)            // the screen buffer at 24576
    addr += guardian->xCoord;
    split_address(addr, &msb, &lsb);
    // LD L,A
    // INC DE
    y_coord++;
    // LD A,(DE)
    addr = SBUFADDRS[y_coord];

    uint8_t y_msb, y_lsb;
    split_address(addr, &y_msb, &y_lsb);
    // LD H,A
    addr = build_address(y_msb, lsb);

    // LD A,(IY+1)             // Pick up the animation frame (0-7)
    // RRCA                    // Multiply it by 32
    // RRCA
    // RRCA
    // LD E,A                  // Point DE at the graphic data for the corresponding
    // LD D,129                // Skylab sprite (at GGDATA+A)
    uint8_t sprite_offset = rotr(guardian->frame, 3);
    // LD C,1                  // Draw the Skylab to the screen buffer at 24576
    // CALL DRWFIX
    bool kill_willy = DRWFIX(&guardian->GGDATA[sprite_offset], addr, 1);
    if (kill_willy) {
      // JP NZ,KILLWILLY_1       // Kill Willy if the Skylab collided with him
      KILLWILLY_0();
      return true;
    }

    // LD A,(IY+2)             // Point HL at the address of the Skylab's location in
    // AND 64                  // the attribute buffer at 23552
    // RLCA
    // RLCA
    // ADD A,92
    // LD H,A
    addr = (uint16_t)(rotl((uint8_t)(guardian->yCoord & 64), 2) + 92);
    split_address(addr, &msb, &lsb);
    uint8_t msb_bak = msb;

    // LD A,(IY+2)
    // RLCA
    // RLCA
    // AND 224
    addr = (uint8_t)(rotl(guardian->yCoord, 2) & 224);
    // OR (IY+3)
    addr |= guardian->xCoord;
    split_address(addr, &msb, &lsb);
    // LD L,A
    addr = build_address(msb_bak, lsb);

    // LD A,(IY+0)             // Pick up the Skylab's attribute byte
    // CALL EUGENE_3           // Set the attribute bytes for the Skylab
    EUGENE_3(addr, guardian->attribute);

    // The current guardian definition has been dealt with. Time for the next one.
    // LD DE,7                 // Point IY at the first byte of the next vertical
    // ADD IY,DE               // guardian definition
    // JR SKYLABS_0            // Jump back to deal with the next Skylab
  }

  return false; // Willy is not dead!
}

// Move and draw the vertical guardians in the current cavern
//
// Used by the routine at LOOP.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool VGUARDIANS() {
  uint8_t msb, lsb;
  uint8_t msb_bak, lsb_bak;

  // LD IY,VGUARDS           // Point IY at the first byte of the first vertical guardian definition at VGUARDS

  // The guardian-moving loop begins here.
  // VGUARDIANS_0:
  for (int i = 0; i < 4; i++) {
    GuardianVertical *guardian = &VGUARDS[i];

    // LD A,(IY+0)             // Pick up the first byte of the guardian definition
    // CP 255                  // Have we dealt with all the guardians yet?
    // RET Z                   // Return if so
    if (guardian->attribute == 255) {
      return false;
    }

    // INC (IY+1)              // Increment the guardian's animation frame
    // RES 2,(IY+1)            // Reset the animation frame to 0 if it overflowed to 4
    if (guardian->frame < 3) {
      guardian->frame++;
    } else {
      guardian->frame = 0;
    }

    // LD A,(IY+2)             // Pick up the guardian's pixel y-coordinate
    // ADD A,(IY+4)            // Add the current y-coordinate increment
    uint16_t y_coord = guardian->yCoord + guardian->yPixelIncrement;

    // FIXME: JR based on C(arry) flag
    // CP (IY+5)               // Has the guardian reached the highest point of its path (minimum y-coordinate)?
    // JR C,VGUARDIANS_1       // If so, jump to change its direction of movement
    // CP (IY+6)               // Has the guardian reached the lowest point of its path (maximum y-coordinate)?
    // JR NC,VGUARDIANS_1      // If so, jump to change its direction of movement
    if (y_coord != guardian->yCoordMinimum && y_coord != guardian->yCoordMaximum) {
      // LD (IY+2),A             // Update the guardian's pixel y-coordinate
      guardian->yCoord += y_coord;
      // JR VGUARDIANS_2
    } else {
      // VGUARDIANS_1:
      // LD A,(IY+4)             // Negate the y-coordinate increment; this changes the
      // NEG                     // guardian's direction of movement
      // LD (IY+4),A

      // IMPORTANT: this of course should still be a positive number! -MRC-
      guardian->yPixelIncrement = (uint8_t)(-guardian->yPixelIncrement);
    }

    // Now that the guardian's movement has been dealt with, time to draw it.
    // VGUARDIANS_2:
    // LD A,(IY+2)             // Pick up the guardian's pixel y-coordinate
    // AND 127                 // Point DE at the entry in the screen buffer address
    // RLCA                    // lookup table at SBUFADDRS that corresponds to the
    y_coord = rotl((uint8_t)(guardian->yCoord & 127), 1);
    // LD E,A                  // guardian's pixel y-coordinate
    // LD D,131
    // LD A,(DE)               // Point HL at the address of the guardian's location
    uint16_t addr = SBUFADDRS[y_coord];
    split_address(addr, &msb, &lsb);
    // OR (IY+3)               // in the screen buffer at 24576
    // LD L,A
    lsb_bak = lsb | guardian->xCoord;
    // INC DE
    y_coord++;
    // LD A,(DE)
    addr = SBUFADDRS[y_coord];
    // LD H,A
    split_address(addr, &msb, &lsb);
    addr = build_address(msb, lsb_bak);

    // LD A,(IY+1)             // Pick up the guardian's animation frame (0-3)
    // RRCA                    // Multiply it by 32
    // RRCA
    // RRCA
    uint8_t anim_frame = rotr(guardian->frame, 3);
    // LD E,A                  // Point DE at the graphic data for the appropriate
    // LD D,129                // guardian sprite (at GGDATA+A)
    // LD C,1                  // Draw the guardian to the screen buffer at 24576
    // CALL DRWFIX
    bool kill_willy = DRWFIX(&guardian->GGDATA[anim_frame], addr, 1);

    // JP NZ,KILLWILLY_0       // Kill Willy if the guardian collided with him
    if (kill_willy) {
      KILLWILLY_0();
      return true;
    }

    // LD A,(IY+2)             // Pick up the guardian's pixel y-coordinate
    // AND 64                  // Point HL at the address of the guardian's location
    // RLCA                    // in the attribute buffer at 23552
    // RLCA
    // ADD A,92
    // LD H,A
    addr = (uint16_t)(rotl((uint8_t)(guardian->yCoord & 64), 2) + 92);
    split_address(addr, &msb, &lsb);
    msb_bak = msb;

    // LD A,(IY+2)
    // RLCA
    // RLCA
    // AND 224
    addr = (uint16_t)(rotl(guardian->yCoord, 2) & 224);
    split_address(addr, &msb, &lsb);
    // OR (IY+3)
    lsb |= guardian->xCoord;
    // LD L,A
    addr = build_address(msb_bak, lsb);

    // LD A,(IY+0)             // Pick up the guardian's attribute byte
    // CALL EUGENE_3           // Set the attribute bytes for the guardian
    EUGENE_3(addr, guardian->attribute);

    // The current guardian definition has been dealt with. Time for the next one.
    //   LD DE,7                 // Point IY at the first byte of the next vertical
    //   ADD IY,DE               // guardian definition
    //   JR VGUARDIANS_0         // Jump back to deal with the next vertical guardian
  }

  return false; // Willy is not dead.
}


// Draw the items in the current cavern and collect any that Willy is touching
//
// Used by the routine at LOOP.
void DRAWITEMS() {
  uint8_t msb;
  uint8_t lsb;
  uint16_t addr;

  // XOR A                   // Initialise the attribute of the last item drawn at
  // LD (ITEMATTR),A         // ITEMATTR to 0 (in case there are no items left to draw)
  game.ITEMATTR = 0;

  // LD IY,ITEMS             // Point IY at the first byte of the first item definition at ITEMS

  // The item-drawing loop begins here.
  // DRAWITEMS_0:
  for (int i = 0; i < 5; i++) {
    // LD A,(IY+0)             // Pick up the first byte of the item definition
    // CP 255                  // Have we dealt with all the items yet?
    // JR Z,DRAWITEMS_3        // Jump if so

    // OR A                    // Has this item already been collected?
    // JR Z,DRAWITEMS_2        // If so, skip it and consider the next one
    if (cavern.ITEMS[i].attribute == 0) {
      continue;
    }

    // LD E,(IY+1)             // Point DE at the address of the item's location in
    // LD D,(IY+2)             // the attribute buffer at 23552
    addr = cavern.ITEMS[i].address;

    // LD A,(DE)               // Pick up the current attribute byte at the item's location
    // AND 7                   // Is the INK white (which happens if Willy is
    // CP 7                    // touching the item)?
    // JR NZ,DRAWITEMS_1       // Jump if not
    if ((speccy.memory[addr] & 7) == 7) {
      // Willy is touching this item, so add it to his collection.
      // LD HL,33836             // Add 100 to the score
      // CALL INCSCORE_0
      game.current_score += 100;
      INCSCORE_0(33836);

      // LD (IY+0),0             // Set the item's attribute byte to 0 so that it will be skipped the next time
      cavern.ITEMS[i].attribute = 0;
      // JR DRAWITEMS_2          // Jump forward to consider the next item
    } else {
      // This item has not been collected yet.
      // DRAWITEMS_1:
      // LD A,(IY+0)             // Pick up the item's current attribute byte
      // AND 248                 // Keep the BRIGHT and PAPER bits, and set the INK to
      // OR 3                    // 3 (magenta)
      // LD B,A                  // Store this value in B
      uint8_t attribute = (uint8_t)((cavern.ITEMS[i].attribute & 248) | 3);
      // LD A,(IY+0)             // Pick up the item's current attribute byte again
      // AND 3                   // Keep only bits 0 and 1 and add the value in B; this
      // ADD A,B                 // maintains the BRIGHT and PAPER bits, and cycles the INK colour through 3, 4, 5 and 6
      attribute += (cavern.ITEMS[i].attribute & 3);
      // LD (IY+0),A             // Store the new attribute byte
      cavern.ITEMS[i].attribute = attribute;

      // LD (DE),A               // Update the attribute byte at the item's location in the buffer at 23552
      speccy.memory[addr] = attribute;

      // LD (ITEMATTR),A         // Store the new attribute byte at ITEMATTR as well
      game.ITEMATTR = attribute;

      // LD D,(IY+3)             // Point DE at the address of the item's location in the screen buffer at 24576
      split_address(addr, &msb, &lsb);
      msb = (uint8_t)cavern.ITEMS[i].addressMSB;
      addr = build_address(msb, lsb);
      // LD HL,ITEM              // Point HL at the item graphic for the current cavern (at ITEM)
      // LD B,8                  // There are eight pixel rows to copy
      // CALL PRINTCHAR_0        // Draw the item to the screen buffer at 24576
      PRINTCHAR_0(cavern.ITEMS[i].tile, addr, 8);
    }

    // The current item definition has been dealt with. Time for the next one.
    // DRAWITEMS_2:
    // INC IY                  // Point IY at the first byte of the next item
    // INC IY                  // definition
    // INC IY
    // INC IY
    // INC IY
    // JR DRAWITEMS_0          // Jump back to deal with the next item
  }

  // All the items have been dealt with. Check whether there were any left.
  // DRAWITEMS_3:
  // LD A,(ITEMATTR)         // Pick up the attribute of the last item drawn at ITEMATTR
  // OR A                    // Were any items drawn?
  // RET NZ                  // Return if so (some remain to be collected)
  if (game.ITEMATTR != 0) {
    return;
  }

  // LD HL,PORTAL            // Ensure that the portal is flashing by setting bit 7
  // SET 7,(HL)              // of its attribute byte at PORTAL
  cavern.portal.PORTAL |= (1 << 7);

  // RET
}


// Draw the portal, or move to the next cavern if Willy has entered it
//
// Used by the routine at LOOP. First check whether Willy has entered the portal.
// IMPORTANT: the CALLers should be able to handle the `goto NEWSHT` themselves, on the return. -MRC-
bool CHKPORTAL() {
  uint8_t msb, lsb;
  uint8_t w_msb, w_lsb;

  // LD HL,(PORTALLOC1)      // Pick up the address of the portal's location in the attribute buffer at 23552 from PORTALLOC1
  uint16_t addr = cavern.portal.PORTALLOC1;
  split_address(addr, &msb, &lsb);

  // LD A,(LOCATION)         // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION
  uint16_t w_addr = willy.LOCATION;
  split_address(w_addr, &w_msb, &w_lsb);

  // CP L                    // Does it match that of the portal?
  // JR NZ,CHKPORTAL_0       // Jump if not
  if (lsb == w_lsb) {
    // LD A,(32877)            // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877
    // CP H                    // Does it match that of the portal?
    // JR NZ,CHKPORTAL_0       // Jump if not
    if (w_msb == msb) {
      // LD A,(PORTAL)           // Pick up the portal's attribute byte from PORTAL
      // BIT 7,A                 // Is the portal flashing?
      // JR Z,CHKPORTAL_0        // Jump if not
      if (((cavern.portal.PORTAL >> 7) & 1) == 1) {
        // POP HL                  // Drop the return address from the stack
        // JP NXSHEET              // Move Willy to the next cavern

        // IMPORTANT: no need to check NXSHEET, we know we should `goto NEWSHT` -MRC-
        NXSHEET();
        return true;
      }
    }
  }

  // Willy has not entered the portal, or it's not flashing, so just draw it.
  // CHKPORTAL_0:
  // LD A,(PORTAL)           // Pick up the portal's attribute byte from PORTAL
  // LD (HL),A               // Set the attribute bytes for the portal in the
  speccy.memory[addr] = cavern.portal.PORTAL;
  // INC HL                  // buffer at 23552
  addr++;
  // LD (HL),A
  speccy.memory[addr] = cavern.portal.PORTAL;
  // LD DE,31
  // ADD HL,DE
  addr += 31;
  // LD (HL),A
  speccy.memory[addr] = cavern.portal.PORTAL;
  // INC HL
  addr++;
  // LD (HL),A
  speccy.memory[addr] = cavern.portal.PORTAL;

  // LD DE,PORTALG           // Point DE at the graphic data for the portal at PORTALG
  // LD HL,(PORTALLOC2)      // Pick up the address of the portal's location in the screen buffer at 24576 from PORTALLOC2
  // LD C,0                  // C=0: overwrite mode
  DRWFIX(&cavern.portal.PORTALG, cavern.portal.PORTALLOC2, 0);
  // This routine continues into the one at DRWFIX.

  return false; // NOTE: callers should not `goto NEWSHT`.
}


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
bool DRWFIX(void *gfx_sprite, uint16_t addr, uint8_t mode) {
  uint8_t *sprite = gfx_sprite;

  uint8_t msb;
  uint8_t lsb;

  // Note: each sprite is 16x16 pixels, so we need to work in pairs of bytes
  for (int i = 0; i < 32; i += 2) {
    // Are we in blend mode?
    if (mode == 1) {
      if (sprite[i] & Speccy_readScreen(addr) || sprite[i + 1] & Speccy_readScreen(addr + 1)) {
        return true; // collision detected
      }
      sprite[i] |= Speccy_readScreen(addr);
      sprite[i + 1] |= Speccy_readScreen(addr + 1);
    }

    // Copy the graphic bytes to their destination cells
    Speccy_writeScreen(addr, sprite[i]);
    Speccy_writeScreen(addr + 1, sprite[i + 1]);

    // Move down to the next pixel row
    split_address(addr, &msb, &lsb);
    msb++;

    // Have we drawn the bottom pixel row in this pair of cells yet?
    if (msb & 7) {
      addr = build_address(msb, lsb);
      continue;
    }

    // Otherwise move to the top pixel row in the cell below
    msb -= 8;
    lsb += 32;

    // Was the last pair of cells at y-coordinate 7 or 15?
    // Otherwise adjust to account for the movement from the top
    // or middle third of the screen to the next one down
    if ((lsb & 224) == 0) {
      msb += 8;
    }

    addr = build_address(msb, lsb);
  }

  return false; // no collision detected
}
// IMPORTANT: replaced this with the function above
//bool DRWFIX2(void *gfx_sprite, uint16_t addr, uint8_t mode) {
//  uint8_t *sprite = gfx_sprite;
//
//  uint8_t msb;
//  uint8_t lsb;
//
//  // LD B,16                 // There are 16 rows of pixels to draw
//
//  // DRWFIX_0:
//  for (int i = 0; i < 32; ) {
//    split_address(addr, &msb, &lsb);  // IMPORTANT: just to make sure msb/lsb are up-to-date -MRC-
//
//    // BIT 0,C                 // Set the zero flag if we're in overwrite mode
//    // LD A,(DE)               // Pick up a sprite graphic byte
//    // JR Z,DRWFIX_1           // Jump if we're in overwrite mode
//    if (mode == 1) {
//      // AND (HL)                // Return with the zero flag reset if any of the set
//      // RET NZ                  // bits in the sprite graphic byte collide with a set bit in the background (e.g. in Willy's sprite)
//      if (sprite[i] & speccy.memory[addr]) {
//        return true;
//      }
//      // LD A,(DE)               // Pick up the sprite graphic byte again
//      // OR (HL)                 // Blend it with the background byte
//      sprite[i] |= speccy.memory[addr];
//    }
//
//    // DRWFIX_1:
//    // LD (HL),A               // Copy the graphic byte to its destination cell
//    speccy.memory[addr] = sprite[i];
//
//    // INC L                   // Move HL along to the next cell on the right
//    lsb++;
//    addr = build_address(msb, lsb);
//
//    // INC DE                  // Point DE at the next sprite graphic byte
//    i++;
//
//    // BIT 0,C                 // Set the zero flag if we're in overwrite mode
//    // LD A,(DE)               // Pick up a sprite graphic byte
//    // JR Z,DRWFIX_2           // Jump if we're in overwrite mode
//    if (mode == 1) {
//      // AND (HL)                // Return with the zero flag reset if any of the set
//      // RET NZ                  // bits in the sprite graphic byte collide with a set bit in the background (e.g. in Willy's sprite)
//      if (sprite[i] & speccy.memory[addr]) {
//        return true;
//      }
//      // LD A,(DE)               // Pick up the sprite graphic byte again
//      // OR (HL)                 // Blend it with the background byte
//      sprite[i] |= speccy.memory[addr];
//    }
//
//    // DRWFIX_2:
//    // LD (HL),A               // Copy the graphic byte to its destination cell
//    speccy.memory[addr] = sprite[i];
//
//    // DEC L                   // Move HL to the next pixel row down in the cell on
//    lsb--;
//    // INC H                   // the left
//    msb++;
//    addr = build_address(msb, lsb);
//
//    // INC DE                  // Point DE at the next sprite graphic byte
//    i++;
//
//    // LD A,H                  // Have we drawn the bottom pixel row in this pair of
//    // AND 7                   // cells yet?
//    // JR NZ,DRWFIX_3          // Jump if not
//    if ((msb & 7) != 0) {
//      continue;
//    }
//
//    // LD A,H                  // Otherwise move HL to the top pixel row in the cell
//    // SUB 8                   // below
//    // LD H,A
//    msb -= 8;
//    // LD A,L
//    // ADD A,32
//    // LD L,A
//    lsb += 32;
//
//    // AND 224                 // Was the last pair of cells at y-coordinate 7 or 15?
//    // JR NZ,DRWFIX_3          // Jump if not
//    if ((lsb & 224) == 0) {
//      // LD A,H                  // Otherwise adjust HL to account for the movement
//      // ADD A,8                 // from the top or middle third of the screen to the
//      // LD H,A                  // next one down
//      msb += 8;
//    }
//
//    // DRWFIX_3:
//    // DJNZ DRWFIX_0           // Jump back until all 16 rows of pixels have been drawn
//    addr = build_address(msb, lsb); // IMPORTANT: just to make sure addr is up-to-date -MRC-
//  }
//
//  // XOR A                   // Set the zero flag (to indicate no collision)
//  // RET
//  return false; // NOTE: no collision -MRC-
//}


// Move to the next cavern
//
// Used by the routines at LOOP and CHKPORTAL.
// IMPORTANT: the CALLers should be able to handle the `goto NEWSHT` themselves, on the return. -MRC-
bool NXSHEET() {
  uint16_t addr;

  // LD A,(SHEET)            // Pick up the number of the current cavern from SHEET
  // INC A                   // Increment the cavern number
//  uint8_t next_sheet = (uint8_t)(SHEET + 1);
  uint8_t next_sheet = 0; // FIXME: always cervern 0 while testing!

  // CP 20                   // Is the current cavern The Final Barrier?
  // JR NZ,NXSHEET_3         // Jump if not
  if (next_sheet == 20) {
    // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
    // OR A                    // Are we in demo mode?
    // JP NZ,NXSHEET_2         // Jump if so
    // LD A,(CHEAT)            // Pick up the 6031769 key counter from CHEAT
    // CP 7                    // Is cheat mode activated?
    // JR Z,NXSHEET_2          // Jump if so
    if (game.DEMO == 0 && game.CHEAT != 7) {
      // Willy has made it through The Final Barrier without cheating.
      // LD C,0                  // Draw Willy at (2,19) on the ground above the portal
      // LD DE,WILLYR3
      // LD HL,16467
      // CALL DRWFIX
      DRWFIX(&willy.sprites[96], 16467, 0);

      // LD DE,SWORDFISH         // Draw the swordfish graphic (see SWORDFISH) over the
      // LD HL,16563             // portal
      // CALL DRWFIX
      DRWFIX(&SWORDFISH, 16563, 0);

      // LD DE,31                // Prepare DE for addition

      // LD HL,22611             // Point HL at (2,19) in the attribute file
      addr = 22611;

      // LD (HL),47              // Set the attributes for the upper half of Willy's
      Speccy_writeAttribute(addr, 47);
      // INC HL                  // sprite at (2,19) and (2,20) to 47 (INK 7: PAPER 5)
      addr++;
      // LD (HL),47
      Speccy_writeAttribute(addr, 47);
      // ADD HL,DE               // Set the attributes for the lower half of Willy's
      addr += 31;
      // LD (HL),39              // sprite at (3,19) and (3,20) to 39 (INK 7: PAPER 4)
      Speccy_writeAttribute(addr, 39);
      // INC HL
      addr++;
      // LD (HL),39
      Speccy_writeAttribute(addr, 39);
      // ADD HL,DE               // Point HL at (5,19) in the attribute file
      addr += 31;
      // INC HL
      addr++;
      // ADD HL,DE
      addr += 31;
      // LD (HL),69              // Set the attributes for the fish at (5,19) and
      Speccy_writeAttribute(addr, 69);
      // INC HL                  // (5,20) to 69 (INK 5: PAPER 0: BRIGHT 1)
      addr++;
      // LD (HL),69
      Speccy_writeAttribute(addr, 69);
      // ADD HL,DE               // Set the attribute for the handle of the sword at
      addr += 31;
      // LD (HL),70              // (6,19) to 70 (INK 6: PAPER 0: BRIGHT 1)
      Speccy_writeAttribute(addr, 70);
      // INC HL                  // Set the attribute for the blade of the sword at
      addr++;
      // LD (HL),71              // (6,20) to 71 (INK 7: PAPER 0: BRIGHT 1)
      Speccy_writeAttribute(addr, 71);
      // ADD HL,DE               // Set the attributes at (7,19) and (7,20) to 0 (to
      addr += 31;
      // LD (HL),0               // hide Willy's feet just below where the portal was)
      Speccy_writeAttribute(addr, 0);
      // INC HL
      addr++;
      // LD (HL),0
      Speccy_writeAttribute(addr, 0);

      // LD BC,0                 // Prepare C and D for the celebratory sound effect
      // LD D,50
      // XOR A                   // A=0 (black border)
      uint8_t border = 0;
      // NXSHEET_0:
      for (int i = 0; i < 50; i++) {
        // OUT (254),A             // Produce the celebratory sound effect: Willy has
        OUT(border);
        // XOR 24                  // escaped from the mine
        border ^= 24;

        // LD E,A // backup
/* TODO
        LD A,C
        ADD A,D
        ADD A,D
        ADD A,D
        LD B,A
*/
        // LD A,E // restore

        // NXSHEET_1:
        // DJNZ NXSHEET_1
        millisleep(1);
/* TODO
        DEC C
        JR NZ,NXSHEET_0
*/
        // DEC D
        // JR NZ,NXSHEET_0
      }
    }

    // NXSHEET_2:
    // XOR A                   // A=0 (the next cavern will be Central Cavern)
    next_sheet = 0;
  }

  // NXSHEET_3:
  // LD (SHEET),A            // Update the cavern number at SHEET
  cavern.SHEET = next_sheet;

  // The next section of code cycles the INK and PAPER colours of the current cavern.

  // LD A,63                 // Initialise A to 63 (INK 7: PAPER 7)
  uint8_t colors = 63;

  // NXSHEET_4:
  for (uint8_t ink = colors; ink > 0; ink--) {
    // LD HL,22528             // Set the attributes for the top two-thirds of the
    // LD DE,22529             // screen to the value in A
    // LD BC,511
    // LD (HL),A
    // LDIR
    for (int i = 0; i < 512; i++) {
      Speccy_writeAttribute(22528 + i, ink);
    }

    //   LD BC,4                 // Pause for about 0.004s
    // NXSHEET_5:
    //   DJNZ NXSHEET_5
    //   DEC C
    //   JR NZ,NXSHEET_5
    millisleep(4);

    // DEC A                   // Decrement the attribute value in A
    // JR NZ,NXSHEET_4         // Jump back until we've gone through all attribute values from 63 down to 1
      Terminal_redraw();
  }

  // LD A,(DEMO)             // Pick up the game mode indicator from DEMO
  // OR A                    // Are we in demo mode?
  if (game.DEMO) {
    // JP NZ,NEWSHT            // If so, demo the next cavern
    return true;
  }

  // The following loop increases the score and decreases the air supply until it runs out.
  // NXSHEET_6:
  while (true) {
    // CALL DECAIR             // Decrease the air remaining in the current cavern
    if (DECAIR()) {
      // JP Z,NEWSHT             // Move to the next cavern if the air supply is now gone
      return true;
    }

    // LD HL,33838             // Add 1 to the score
    // CALL INCSCORE_0
    game.current_score++;
    INCSCORE_0(33838);

    // LD IX,SCORBUF           // Print the new score at (19,26)
    // LD C,6
    // LD DE,20602
    // CALL PMESS
    PMESS(&SCORBUF, 20602, 6);

    // LD C,4                  // C=4; this value determines the duration of the sound effect
    uint8_t duration = 4;

    // LD A,(AIR)              // Pick up the remaining air supply (S) from AIR
    // CPL                     // D=2*(63-S); this value determines the pitch of the
    // AND 63                  // sound effect (which decreases with the amount of
    // RLC A                   // air remaining)
    // LD D,A
    uint8_t pitch = rotl((uint8_t)(~cavern.AIR & 63), 1);

    // NXSHEET_7:
    for (int i = duration; i > 0; i--) {
      // LD A, 0                  // Produce a short note
      // OUT(254), A
      OUT(0);

      // LD B, D
      // NXSHEET_8:
      // DJNZ NXSHEET_8
      millisleep(pitch);

      // LD A, 24
      // OUT(254), A
      OUT(24);

      // LD B, D
      // NXSHEET_9:
      // DJNZ NXSHEET_9
      millisleep(1);

      // DEC C
      // JR NZ, NXSHEET_7
    }

    // JR NXSHEET_6            // Jump back to decrease the air supply again
  }

  return false; // IMPORTANT: no `goto NEWSHT` required. -MRC-
}


// IMPORTANT: rewrite of the old functions to remove the jumps.
//
// The entry point to this routine is here and is used by the routines at
// DRAWITEMS, NXSHEET and KONGBEAST with HL pointing at the digit of the score
// (see SCORBUF) to be incremented.
void INCSCORE_0(uint16_t addr) {
  uint8_t msb;
  uint8_t lsb;

  for (;;) {
    // Pick up a digit of the score, is it '9'?
    if (speccy.memory[addr] < 57) {
      speccy.memory[addr]++;
      return;
    }

    // Roll the digit over from '9' to '0'
    speccy.memory[addr] = 48;

    // Point HL at the next digit to the left
    addr--;

    // Is this the 10000s digit?
    split_address(addr, &msb, &lsb);
    if (lsb == 42) {
      game.FLASH = 8;
      willy.NOMEN++;
    }
  }
}
// // Add to the score
// //
// // The entry point to this routine is at INCSCORE_0.
// void INCSCORE(uint16_t addr) {
//   LD (HL),48              // Roll the digit over from '9' to '0'
//   DEC HL                  // Point HL at the next digit to the left
//   LD A,L                  // Is this the 10000s digit?
//   CP 42
//   JR NZ,INCSCORE_0        // Jump if not

//   // Willy has scored another 10000 points. Give him an extra life.
//   LD A,8                  // Set the screen flash counter at FLASH to 8
//   LD (FLASH),A

//   LD A,(NOMEN)            // Increment the number of lives remaining at NOMEN
//   INC A
//   LD (NOMEN),A
// }
// // The entry point to this routine is here and is used by the routines at
// // DRAWITEMS, NXSHEET and KONGBEAST with HL pointing at the digit of the score
// // (see SCORBUF) to be incremented.
// void INCSCORE_0() {
//   LD A,(HL)               // Pick up a digit of the score
//   CP 57                   // Is it '9'?
//   JR Z,INCSCORE           // Jump if so
//   INC (HL)                // Increment the digit
//   RET
// }


// Move the conveyor in the current cavern
//
// Used by the routine at LOOP.
void MVCONVEYOR() {
  uint8_t h, l, d, e; // MSB/LSB for the address.
  uint8_t pixels_a, pixels_c;

  // LD HL,(CONVLOC)         // Pick up the address of the conveyor's location in the screen buffer at 28672 from CONVLOC
  uint16_t row_hl = cavern.CONVEYOR.CONVLOC;
  // LD E,L                  // Copy this address to DE
  // LD D,H
  uint16_t row_de = cavern.CONVEYOR.CONVLOC;

  // NOTE: using CONVLEN directly in loop below. -MRC-
  // LD A,(CONVLEN)          // Pick up the length of the conveyor from CONVLEN
  // LD B,A                  // B will count the conveyor tiles

  // LD A,(CONVDIR)          // Pick up the direction of the conveyor from CONVDIR
  // OR A                    // Is the conveyor moving right?
  // JR NZ,MVCONVEYOR_1      // Jump if so
  if (cavern.CONVEYOR.CONVDIR == 0) {
    // The conveyor is moving left.
    // LD A,(HL)               // Copy the first pixel row of the conveyor tile to A
    // RLC A                   // Rotate it left twice
    // RLC A
    pixels_a = rotl(speccy.memory[row_hl], 2);

    split_address(row_hl, &h, &l);
    // INC H                   // Point HL at the third pixel row of the conveyor
    // INC H                   // tile
    h += 2;
    row_hl = build_address(h, l);

    // LD C,(HL)               // Copy this pixel row to C
    // RRC C                   // Rotate it right twice
    // RRC C
    pixels_c = rotr(speccy.memory[row_hl], 2);

    // IMPORTANT: moved MVCONVEYOR_0 below, as the `else` takes card of the needed jumps and RET.

    // RET
  } else {
    // The conveyor is moving right.
    // MVCONVEYOR_1:
    // LD A,(HL)               // Copy the first pixel row of the conveyor tile to A
    // RRC A                   // Rotate it right twice
    // RRC A
    pixels_a = rotr(speccy.memory[row_hl], 2);

    split_address(row_hl, &h, &l);
    // INC H                   // Point HL at the third pixel row of the conveyor
    // INC H                   // tile
    h += 2;
    row_hl = build_address(h, l);

    // LD C,(HL)               // Copy this pixel row to C
    // RLC C                   // Rotate it left twice
    // RLC C
    pixels_c = rotl(speccy.memory[row_hl], 2);

    // JR MVCONVEYOR_0         // Jump back to update the first and third pixel rows of every conveyor tile
  }

  // IMPORTANT: moved here so both if/else blocks can use it, without the need of goto's.

  // MVCONVEYOR_0:
  for (int b = cavern.CONVEYOR.CONVLEN; b > 0; b--) {
    // LD (DE),A               // Update the first and third pixel rows of every
    speccy.memory[row_de] = pixels_a;

    // LD (HL),C               // conveyor tile in the screen buffer at 28672
    speccy.memory[row_hl] = pixels_c;

    split_address(row_hl, &h, &l);
    // INC L
    l++;
    row_hl = build_address(h, l);

    split_address(row_de, &d, &e);
    // INC E
    e++;
    row_de = build_address(d, e);

    // DJNZ MVCONVEYOR_0
  }
}


// Move and draw the Kong Beast in the current cavern
//
// Used by the routine at LOOP.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool KONGBEAST() {
  uint16_t addr;
  uint8_t msb, lsb;

  // LD HL,23558             // Flip the left-hand switch at (0,6) if Willy is
  // CALL CHKSWITCH          // touching it
  // FIXME: Screen Buffer: Screen File = 22534
  CHKSWITCH(23558);

  // LD A,(EUGDIR)           // Pick up the Kong Beast's status from EUGDIR
  // CP 2                    // Is the Kong Beast already dead?
  // RET Z                   // Return if so
  if (EUGDIR == 2) {
    return false; // NOTE: willy is not dead!
  }

  // LD A,(29958)            // Pick up the sixth pixel row of the left-hand switch from the screen buffer at 28672
  // CP 16                   // Has the switch been flipped?
  // JP Z,KONGBEAST_8        // Jump if not
  // FIXME: Blank Screen Buffer: Screen File = 17670
  if (speccy.memory[29958] != 16) {
    return KONGBEAST_8();  // return dead-ness state of Willy! -MRC-
  }

  // The left-hand switch has been flipped. Deal with opening up the wall if that is still in progress.

  // LD A,(24433)            // Pick up the attribute byte of the tile at (11,17) in the buffer at 24064
  // OR A                    // Has the wall there been removed yet?
  // JR Z,KONGBEAST_2        // Jump if so
  // FIXME: Blank Screen Buffer: Screen File = 22897
  if (speccy.memory[24433] != 0) {
    // LD HL,32625             // Point HL at the bottom row of pixels of the wall tile at (11,17) in the screen buffer at 28672
  // FIXME: Blank Screen Buffer: Screen File = 20337
    addr = 32625;

    // KONGBEAST_0:
    while (true) {
      split_address(addr, &msb, &lsb);

      // LD A,(HL)               // Pick up a pixel row
      // OR A                    // Is it blank yet?
      // JR NZ,KONGBEAST_1       // Jump if not
      if (speccy.memory[addr] != 0) {
        // IMPORTANT: moving KONGBEAST_1 as it's the only place that calls it, and it saves some goto's -MRC-
        // KONGBEAST_1:
        // LD (HL),0               // Clear a pixel row of the wall tile at (11,17) in the screen buffer at 28672
        speccy.memory[addr] = 0;

        // LD L,145                // Point HL at the opposite pixel row of the wall tile
        lsb = 145;
        // LD A,H                  // one cell down at (12,17)
        // XOR 7
        // LD H,A
        msb ^= 7;
        addr = build_address(msb, lsb);

        // LD (HL),0               // Clear that pixel row as well
        speccy.memory[addr] = 0;

        break;
      }

      // DEC H                   // Point HL at the next pixel row up
      msb--;
      // LD A,H                  // Have we checked all 8 pixel rows yet?
      // CP 119
      // JR NZ,KONGBEAST_0       // If not, jump back to check the next one
      if (msb != 119) {
        break;
      }

      addr = build_address(msb, lsb);
    }

    // LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
    // LD (24433),A            // Change the attributes at (11,17) and (12,17) in the
    // FIXME: Blank Screen Buffer: Screen File = 22897
    memcpy(&speccy.memory[24433], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite));
    // LD (24465),A            // buffer at 24064 to match the background tile (the wall there is now gone)
    // FIXME: Blank Screen Buffer: Screen File = 22929
    memcpy(&speccy.memory[24465], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite));

    // FIXME: I guess we need to update HGUARD2 directly rather than this memeory address.
    // LD A,114                // Update the seventh byte of the guardian definition
    // LD (32971),A            // at HGUARD2 so that the guardian moves through the opening in the wall
    speccy.memory[32971] = 114;

    // JR KONGBEAST_2

    // IMPORTANT: KONGBEAST_1 is never reached without the jump above, so moving this block up there -MRC-
  }

  // Now check the right-hand switch.
  // KONGBEAST_2:
  // LD HL,23570             // Flip the right-hand switch at (0,18) if Willy is
  // CALL CHKSWITCH          // touching it (and it hasn't already been flipped)
  // JR NZ,KONGBEAST_4       // Jump if the switch was not flipped
  // FIXME: Screen Buffer: Screen File = 22546
  if ( !CHKSWITCH(23570) ) {
    // Initialise the Kong Beast's pixel y-coordinate at EUGHGT to 0
    // XOR A
    // LD (EUGHGT),A
    EUGHGT = 0;

    // Update the Kong Beast's status at EUGDIR to 1: he is falling
    // INC A
    // LD (EUGDIR),A
    EUGDIR = 1;

    // LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
    // LD (24143),A            // Change the attributes of the floor beneath the Kong
    // FIXME: Blank Screen Buffer: Screen File = 22607
    memcpy(&speccy.memory[24143], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite));
    // LD (24144),A            // Beast in the buffer at 24064 to match that of the background tile
    // FIXME: Blank Screen Buffer: Screen File = 16463
    memcpy(&speccy.memory[24144], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite));

    // LD HL,28751             // Point HL at (2,15) in the screen buffer at 28672
    // FIXME: Blank Screen Buffer: Screen File = 22608
    addr = 28751;
    // LD B,8                  // Clear the cells at (2,15) and (2,16), removing the
    // KONGBEAST_3:
    for (int i = 8; i > 0; i--) {
      split_address(addr, &msb, &lsb);

      // LD (HL),0               // floor beneath the Kong Beast
      speccy.memory[addr] = 0;

      // INC L
      lsb++;
      addr = build_address(msb, lsb);

      // LD (HL),0
      speccy.memory[addr] = 0;

      // DEC L
      // INC H
      lsb--;
      msb++;

      // DJNZ KONGBEAST_3
      addr = build_address(msb, lsb);
    }
  }

  // KONGBEAST_4:
  // LD A,(EUGDIR)           // Pick up the Kong Beast's status from EUGDIR
  // OR A                    // Is the Kong Beast still on the ledge?
  // JR Z,KONGBEAST_8        // Jump if so
  if (EUGDIR == 0) {
    return KONGBEAST_8();  // return dead-ness state of Willy! -MRC-
  }

  // The Kong Beast is falling.
  // LD A,(EUGHGT)           // Pick up the Kong Beast's pixel y-coordinate from EUGHGT
  // CP 100                  // Has he fallen into the portal yet?
  // JR Z,KONGBEAST_7        // Jump if so
  if (EUGHGT != 100) {
    // ADD A,4                 // Add 4 to the Kong Beast's pixel y-coordinate at
    // LD (EUGHGT),A           // EUGHGT (moving him downwards)
    EUGHGT += 4;

    // LD C,A                  // Copy the pixel y-coordinate to C; this value determines the pitch of the sound effect

    // LD D,16                 // D=16; this value determines the duration of the sound effect
    // LD A,(BORDER)           // Pick up the border colour for the current cavern from BORDER
    uint8_t border = cavern.BORDER;

    // KONGBEAST_5:
    for (int i = 0; i < 16; i++) {
      // OUT (254),A             // Make a falling sound effect
      OUT(border);
      // XOR 24
      border ^= 24;

      // LD B,C
      // KONGBEAST_6:
      // DJNZ KONGBEAST_6
      millisleep(1);

      // DEC D
      // JR NZ,KONGBEAST_5
    }

    // LD A,C                  // Copy the Kong Beast's pixel y-coordinate back into A
    // RLCA                    // Point DE at the entry in the screen buffer address
    // LD E,A                  // lookup table at SBUFADDRS that corresponds to the
    // LD D,131                // Kong Beast's pixel y-coordinate
    // LD A,(DE)               // Point HL at the address of the Kong Beast's
    // OR 15                   // location in the screen buffer at 24576
    // LD L,A
    // INC DE
    // LD A,(DE)
    // LD H,A
    addr = (uint16_t)(SBUFADDRS[rotl(EUGHGT, 1)] | 15);
    // LD D,129                // Use bit 5 of the value of the game clock at CLOCK
    // LD A,(CLOCK)            // (which is toggled once every eight passes through
    // AND 32                  // the main loop) to point DE at the graphic data for
    // OR 64                   // the appropriate Kong Beast sprite
    // LD E,A
    uint16_t sprite_id = (uint16_t)((cavern.CLOCK & 32) | 64);
    // LD C,0                  // Draw the Kong Beast to the screen buffer at 24576
    // CALL DRWFIX
    DRWFIX(&GGDATA[sprite_id], addr, 0);

    // LD HL,33836             // Add 100 to the score
    // CALL INCSCORE_0
    game.current_score += 100;
    INCSCORE_0(33836);

    // LD A,(EUGHGT)           // Pick up the Kong Beast's pixel y-coordinate from EUGHGT
    // AND 120                 // Point HL at the address of the Kong Beast's
    // LD L,A                  // location in the attribute buffer at 23552
    // LD H,23
    lsb = (uint8_t)(EUGHGT & 120);
    msb = 23;
    addr = build_address(msb, lsb);

    // FIXME: HL + HL ? what is this actually doing?
    // ADD HL,HL
    addr += addr;
    // ADD HL,HL
    addr += addr;

    split_address(addr, &msb, &lsb);
    // LD A,L
    // OR 15
    // LD L,A
    lsb |= 15;
    addr = build_address(msb, lsb);

    // LD A,6                  // The Kong Beast is drawn with yellow INK
    // JP EUGENE_3             // Set the attribute bytes for the Kong Beast
    EUGENE_3(addr, 6);
    // return false; // FIXME: do we need to return here?
  }

  // The Kong Beast has fallen into the portal.
  // KONGBEAST_7:
  // LD A,2                  // Set the Kong Beast's status at EUGDIR to 2: he is
  // LD (EUGDIR),A           // dead
  EUGDIR = 2;

  // RET
  return false; // return dead-ness state of Willy! -MRC-
}

// The Kong Beast is still on the ledge.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool KONGBEAST_8() {
  // LD A,(CLOCK)            // Pick up the value of the game clock at CLOCK
  // AND 32                  // Use bit 5 of this value (which is toggled once
  // LD E,A                  // every eight passes through the main loop) to point
  uint8_t sprite_id = (uint8_t)(cavern.CLOCK & 32);
  // LD D,129                // DE at the graphic data for the appropriate Kong Beast sprite
  // LD HL,24591             // Draw the Kong Beast at (0,15) in the screen buffer
  // LD C,1                  // at 24576
  // CALL DRWFIX
  // FIXME: Screen Buffer: Screen File = 16399
  bool kill_willy = DRWFIX(&GGDATA[sprite_id], 24591, 1);

  // JP NZ,KILLWILLY_0       // Kill Willy if he collided with the Kong Beast
  if (kill_willy) {
    return true;
  }

  // LD A,68                 // A=68 (INK 4: PAPER 0: BRIGHT 1)
  // LD (23599),A            // Set the attribute bytes for the Kong Beast in the
  // LD (23600),A            // buffer at 23552
  // LD (23567),A
  // LD (23568),A
  // FIXME: Screen Buffer: Screen File = 22575
  speccy.memory[23599] = 68;
  speccy.memory[23600] = 68;
  // FIXME: Screen Buffer: Screen File = 22543
  speccy.memory[23567] = 68;
  speccy.memory[23568] = 68;

  // RET
  return false; // NOTE: willy is not dead.
}


// Flip a switch in a Kong Beast cavern if Willy is touching it
//
// Used by the routine at KONGBEAST. Returns with the zero flag set if Willy
// flips the switch.
//
// HL Address of the switch's location in the attribute buffer at 23552
bool CHKSWITCH(uint16_t addr) {
  uint8_t msb, lsb;
  uint16_t w_addr;

  uint8_t sw_msb, sw_lsb;
  split_address(addr, &sw_msb, &sw_lsb);

  // LD A,(LOCATION)         // Pick up the LSB of the address of Willy's location in the attribute buffer at 23552 from LOCATION
  w_addr = willy.LOCATION;
  split_address(w_addr, &msb, &lsb);
  // INC A                   // Is it equal to or one less than the LSB of the
  lsb++;
  // AND 254                 // address of the switch's location?
  lsb &= 254;
  // CP L
  // RET NZ                  // Return (with the zero flag reset) if not
  // LD A,(32877)            // Pick up the MSB of the address of Willy's location in the attribute buffer at 23552 from 32877
  // CP H                    // Does it match the MSB of the address of the switch's location?
  // RET NZ                  // Return (with the zero flag reset) if not
  if (msb == sw_msb || lsb == sw_lsb) {
    return false;
  }

  // IMPORTANT: I believe 32869 should be the EXTRA[9] array. -MRC-
  // LD A,(32869)            // Pick up the sixth byte of the graphic data for the switch tile from 32869
  // LD H,117                // Point HL at the sixth row of pixels of the switch tile in the screen buffer at 28672
  sw_msb = 117;
  addr = build_address(sw_msb, sw_lsb);
  // CP (HL)                 // Has the switch already been flipped?
  // RET NZ                  // Return (with the zero flag reset) if so
  if (memcpy(&speccy.memory[addr], cavern.EXTRA.sprite, sizeof(cavern.EXTRA.sprite)) == 0) {
    return true;
  }

  // Willy is flipping the switch.
  // LD (HL),8               // Update the sixth, seventh and eighth rows of pixels
  speccy.memory[addr] = 8;

  // INC H                   // of the switch tile in the screen buffer at 28672 to
  sw_msb++;

  addr = build_address(sw_msb, sw_lsb);
  // LD (HL),6               // make it appear flipped
  speccy.memory[addr] = 6;

  // INC H
  sw_msb++;

  addr = build_address(sw_msb, sw_lsb);
  // LD (HL),6
  speccy.memory[addr] = 6;

  // XOR A                   // Set the zero flag: Willy has flipped the switch
  // OR A                    // This instruction is redundant
  // RET
  return true;
}

// Check and set the attribute bytes for Willy's sprite in the buffer at 23552
//
// Used by the routine at LOOP.
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool WILLYATTRS() {
  bool kill_willy = false;

  // LD HL,(LOCATION)        // Pick up the address of Willy's location in the attribute buffer at 23552 from LOCATION
  uint16_t addr = willy.LOCATION;

  // LD DE,31                // Prepare DE for addition

  // LD C,15                 // Set C=15 for the top two rows of cells (to make the routine at WILLYATTR force white INK)
  uint8_t ink = 15;

  // CALL WILLYATTR          // Check and set the attribute byte for the top-left cell
  if (WILLYATTR(addr, ink)) {
    kill_willy = true;
  }

  // INC HL                  // Move HL to the next cell to the right
  addr++;
  // CALL WILLYATTR          // Check and set the attribute byte for the top-right cell
  if (WILLYATTR(addr, ink)) {
    kill_willy = true;
  }

  // ADD HL,DE               // Move HL down a row and back one cell to the left
  addr += 31;
  // CALL WILLYATTR          // Check and set the attribute byte for the mid-left cell
  if (WILLYATTR(addr, ink)) {
    kill_willy = true;
  }

  // INC HL                  // Move HL to the next cell to the right
  addr++;
  // CALL WILLYATTR          // Check and set the attribute byte for the mid-right cell
  if (WILLYATTR(addr, ink)) {
    kill_willy = true;
  }

  // LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  // LD C,A                  // Copy it to C
  ink = willy.PIXEL_Y;

  // ADD HL,DE               // Move HL down a row and back one cell to the left
  addr += 31;
  // CALL WILLYATTR          // Check and set the attribute byte for the bottom-left cell
  if (WILLYATTR(addr, ink)) {
    kill_willy = true;
  }

  // INC HL                  // Move HL to the next cell to the right
  addr++;
  // CALL WILLYATTR          // Check and set the attribute byte for the bottom-right cell
  if (WILLYATTR(addr, ink)) {
    kill_willy = true;
  }

  // JR DRAWWILLY            // Draw Willy to the screen buffer at 24576
  DRAWWILLY();

  // Is Willy dead?
  if (kill_willy) {
    return true;
  } else {
    return false;
  }
}

// Check and set the attribute byte for a cell occupied by Willy's sprite
//
// Used by the routine at WILLYATTRS.
//
// C 15 or Willy's pixel y-coordinate
// HL Address of the cell in the attribute buffer at 23552
// IMPORTANT: return value is Willy's "death" state: true/false -MRC-
bool WILLYATTR(uint16_t addr, uint8_t ink) {
  // LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
  // CP (HL)                 // Does this cell contain a background tile?
  // JR NZ,WILLYATTR_0       // Jump if not
  if ( memcmp(&speccy.memory[addr], cavern.BACKGROUND.sprite, sizeof(cavern.BACKGROUND.sprite)) == 0 ) {
    // LD A,C                  // Set the zero flag if we are going to retain the INK
    // AND 15                  // colour in this cell; this happens only if the cell is in the bottom row and Willy's sprite is confined to the top two rows
    // JR Z,WILLYATTR_0        // Jump if we are going to retain the current INK colour in this cell
    if (ink & 15) {
      // LD A,(BACKGROUND)       // Pick up the attribute byte of the background tile for the current cavern from BACKGROUND
      // OR 7                    // Set bits 0-2, making the INK white
      // LD (HL),A               // Set the attribute byte for this cell in the buffer at 23552
      speccy.memory[addr] = (uint8_t)(cavern.BACKGROUND.id | 7);
    }
  }

  // WILLYATTR_0:
  // LD A,(NASTY1)           // Pick up the attribute byte of the first nasty tile for the current cavern from NASTY1
  // CP (HL)                 // Has Willy hit a nasty of the first kind?
  // JP Z,KILLWILLY          // Kill Willy if so
  // LD A,(NASTY2)           // Pick up the attribute byte of the second nasty tile for the current cavern from NASTY2
  // CP (HL)                 // Has Willy hit a nasty of the second kind?
  // JP Z,KILLWILLY          // Kill Willy if so
  if ( memcmp(&speccy.memory[addr], cavern.NASTY1.sprite, sizeof(cavern.NASTY1.sprite)) == 0 || memcmp(&speccy.memory[addr], cavern.NASTY2.sprite, sizeof(cavern.NASTY2.sprite)) == 0 ) {
    KILLWILLY();
    return true;
  }

  // RET
  return false;
}



// Draw Willy to the screen buffer at 24576
//
// Used by the routine at WILLYATTRS.
void DRAWWILLY() {
  uint8_t h, l;
  uint8_t msb, lsb;
  uint16_t addr;

  // LD A,(PIXEL_Y)          // Pick up Willy's pixel y-coordinate from PIXEL_Y
  // LD IXh,131              // Point IX at the entry in the screen buffer address
  // LD IXl,A                // lookup table at SBUFADDRS that corresponds to Willy's y-coordinate
  uint8_t pixy = (uint8_t)(willy.PIXEL_Y/2);

  // LD A,(DMFLAGS)          // Pick up Willy's direction and movement flags from DMFLAGS
  // AND 1                   // Now E=0 if Willy is facing right, or 128 if he's
  // RRCA                    // facing left
  // LD E,A
  uint8_t frame = rotr((uint8_t)(willy.DMFLAGS & 1), 1);
  // LD A,(FRAME)            // Pick up Willy's animation frame (0-3) from FRAME
  // AND 3                   // Point DE at the sprite graphic data for Willy's
  // RRCA                    // current animation frame (see MANDAT)
  // RRCA
  // RRCA
  // OR E
  frame = rotr((uint8_t)(willy.FRAME & 3), 3) | frame;
  // LD E,A
  // LD D,130

  // LD B,16                 // There are 16 rows of pixels to copy

  // LD A,(LOCATION)         // Pick up Willy's screen x-coordinate (0-31) from
  // AND 31                  // LOCATION
  // LD C,A                  // Copy it to C
  split_address(willy.LOCATION, &msb, &lsb);

  // DRAWWILLY_0:
  for (int i = 16; i > 0; i--) {
    // LD A,(IX+0)             // Set HL to the address in the screen buffer at 24576
    // LD H,(IX+1)             // that corresponds to where we are going to draw the
    split_address(SBUFADDRS[pixy], &h, &l);
    // OR C                    // next pixel row of the sprite graphic
    // LD L,A
    addr = build_address(h, l|lsb);

    // LD A,(DE)               // Pick up a sprite graphic byte
    // OR (HL)                 // Merge it with the background
    // LD (HL),A               // Save the resultant byte to the screen buffer
    speccy.memory[addr] = willy.sprites[frame] | speccy.memory[addr];

    // INC HL                  // Move HL along to the next cell to the right
    addr++;
    // INC DE                  // Point DE at the next sprite graphic byte
    frame++;

    // LD A,(DE)               // Pick it up in A
    // OR (HL)                 // Merge it with the background
    // LD (HL),A               // Save the resultant byte to the screen buffer
    speccy.memory[addr] = willy.sprites[frame] | speccy.memory[addr];

    // INC IX                  // Point IX at the next entry in the screen buffer
    // INC IX                  // address lookup table at SBUFADDRS
    pixy += 1;

    // INC DE                  // Point DE at the next sprite graphic byte
    frame++;

    // DJNZ DRAWWILLY_0        // Jump back until all 16 rows of pixels have been drawn
  }

  // RET
}



// Print a message
//
// Used by the routines at START, STARTGAME, LOOP, ENDGAM and NXSHEET.
//
// IX Address of the message
// C Length of the message
// DE Display file address
void PMESS(void *msg, uint16_t addr, uint8_t len) {
  uint8_t *ch = msg;

  for (int i = 0; i < len; i++) {
    // LD A,(IX+0)             // Collect a character from the message
    // CALL PRINTCHAR          // Print it
    PRINTCHAR(ch[i], addr);
    // INC IX                  // Point IX at the next character in the message
    // INC E                   // Point DE at the next character cell (subtracting 8
    // LD A,D                  // from D compensates for the operations performed by
    // SUB 8                   // the routine at PRINTCHAR)
    // LD D,A
    addr++;

    // DEC C                   // Have we printed the entire message yet?
    // JR NZ,PMESS             // If not, jump back to print the next character
  }

  // RET
}



// Print a single character
//
// Used by the routine at PMESS.
//
// A ASCII code of the character
// DE Display file address
void PRINTCHAR(char ch, uint16_t addr) {
  // LD H,7                  // Point HL at the bitmap for the character (in the
  // LD L,A                  // ROM)
  // SET 7,L
  // ADD HL,HL
  // ADD HL,HL
  // ADD HL,HL
  uint8_t ch_index_id = (uint8_t)(ch - 32);

  // LD B,8                  // There are eight pixel rows in a character bitmap

  PRINTCHAR_0(&character_set[ch_index_id], addr, 8); // IMPORTANT: implicit call to function.
}

// This entry point is used by the routine at DRAWITEMS to draw an item in the
// current cavern.
void PRINTCHAR_0(void *character, uint16_t addr, uint8_t len) {
  uint8_t *chr = character;
  uint8_t msb, lsb;

  for (int i = 0; i < len; i++) {

    // LD A,(HL)               // Copy the character bitmap to the screen (or item
    // LD (DE),A               // graphic to the screen buffer)
    // INC HL
    speccy.memory[addr] = chr[i];

    // INC D
    split_address(addr, &msb, &lsb);
    msb++;
    addr = build_address(msb, lsb);

    // DJNZ PRINTCHAR_0
  }
  // RET
}



// Play the theme tune (The Blue Danube)
//
// Used by the routine at START. Returns with the zero flag reset if ENTER or
// the fire button is pressed while the tune is being played.
//
// IY THEMETUNE (tune data)
bool PLAYTUNE() {
  uint8_t freq1, freq2;
  uint16_t addr;

  for (int i = 0; i < 95; i++) {
    // LD A,(IY+0)             // Pick up the next byte of tune data from the table at THEMETUNE
    // CP 255                  // Has the tune finished?
    // RET Z                   // Return (with the zero flag set) if so
    uint8_t *note = THEMETUNE[i];

    // NOTE: `duration` used directly in the loop below -MRC-
    // LD C,A                  // Copy the first byte of data for this note (which determines the duration) to C

    // LD B,0                  // Initialise B, which will be used as a delay counter in the note-producing loop
    // uint8_t delay = 0; // FIXME: seems to never get set anywhere else?

    // XOR A                   // Set A=0 (for no apparent reasaon)

    // LD D,(IY+1)             // Pick up the second byte of data for this note
    // LD A,D                  // Copy it to A
    freq1 = note[1];
    // CALL PIANOKEY           // Calculate the attribute file address for the corresponding piano key
    addr = PIANOKEY(freq1);
    // LD (HL),80              // Set the attribute byte for the piano key to 80 (INK 0: PAPER 2: BRIGHT 1)
    speccy.memory[addr] = 80;

    // LD E,(IY+2)             // Pick up the third byte of data for this note
    freq2 = note[2];
    // LD A,E                  // Copy it to A
    uint8_t pitch = freq2;
    // CALL PIANOKEY           // Calculate the attribute file address for the corresponding piano key
    addr = PIANOKEY(freq2);
    // LD (HL),40              // Set the attribute byte for the piano key to 40 (INK 0: PAPER 5: BRIGHT 0)
    speccy.memory[addr] = 40;

    // PLAYTUNE_0:
    for (uint8_t d = note[0]; d > 0; d--) {
      // OUT (254),A             // Produce a sound based on the frequency parameters
      OUT(pitch);

      // DEC D                   // in the second and third bytes of data for this note
      freq1--;
      // JR NZ,PLAYTUNE_1        // (copied into D and E)
      if (freq1 == 0) {
        // LD D,(IY+1)
        freq1 = note[1];

        // XOR 24
        pitch ^= 24;
      }

      // PLAYTUNE_1:
      // DEC E
      freq2--;
      // JR NZ,PLAYTUNE_2
      if (freq2 == 0) {
        // LD E,(IY+2)
        freq2 = note[2];

        // XOR 24
        pitch ^= 24;
      }

      // FIXME: reg B (delay) is initialized to 0, but never set anywhere else, so this code is obsolete...?
      // PLAYTUNE_2:
      // DJNZ PLAYTUNE_0
      millisleep(1);

      // DEC C
      // JR NZ,PLAYTUNE_0
    }

    // CALL CHECKENTER         // Check whether ENTER or the fire button is being pressed
    // RET NZ                  // Return (with the zero flag reset) if it is
    if ( CHECKENTER() ) {
      return true;
    }

    // LD A,(IY+1)             // Pick up the second byte of data for this note
    // CALL PIANOKEY           // Calculate the attribute file address for the corresponding piano key
    addr = PIANOKEY(note[1]);
    // LD (HL),56              // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0)
    speccy.memory[addr] = 56;

    // LD A,(IY+2)             // Pick up the third byte of data for this note
    // CALL PIANOKEY           // Calculate the attribute file address for the corresponding piano key
    addr = PIANOKEY(note[2]);
    // LD (HL),56              // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0)
    speccy.memory[addr] = 56;

    // INC IY                  // Move IY along to the data for the next note in the
    // INC IY                  // tune
    // INC IY
    // JR PLAYTUNE             // Jump back to play the next note
  }

  return false;
}


// Calculate the attribute file address for a piano key
//
// Used by the routine at PLAYTUNE. Returns with the attribute file address in HL.
//
// A Frequency parameter from the tune data table at THEMETUNE
uint16_t PIANOKEY(uint8_t frequency) {
  // SUB 8                   // Compute the piano key index (K) based on the
  frequency -= 8;
  // RRCA                    // frequency parameter (F), and store it in bits 0-4
  // RRCA                    // of A: K=31-INT((F-8)/8)
  // RRCA
  frequency = rotr(frequency, 3);
  // CPL
  frequency = (uint8_t)~frequency;
  // OR 224                  // A=224+K; this is the LSB
  frequency |= 224;
  // LD L,A                  // Set HL to the attribute file address for the piano
  // LD H,89                 // key
  // RET
  return build_address(89, frequency);
}


// Check whether ENTER or the fire button is being pressed
//
// Used by the routine at PLAYTUNE. Returns with the zero flag reset if ENTER or
// the fire button on the joystick is being pressed.
bool CHECKENTER() {
  // LD A,(KEMP)             // Pick up the Kempston joystick indicator from KEMP
  // OR A                    // Is the joystick connected?
  // JR Z,CHECKENTER_0       // Jump if not
  if (game.KEMP) {
    /* TODO
      IN A,(31)               // Collect input from the joystick
      BIT 4,A                 // Is the fire button being pressed?
      RET NZ                  // Return (with the zero flag reset) if so
    */
  }

  // CHECKENTER_0:
  // LD BC,49150             // Read keys H-J-K-L-ENTER
  // IN A,(C)
  // AND 1                   // Keep only bit 0 of the result (ENTER)
  // CP 1                    // Reset the zero flag if ENTER is being pressed
  // RET
  if (Terminal_keyEnter()) {
    return true;
  } else {
    return false;
  }
}


// ----------------------------------------------
//   NOTE: source code remnants has been deleted
// ----------------------------------------------


// ----------------------------------------------------------
//   NOTE: data from here has been moved to the data.c file.
// ----------------------------------------------------------


// The following functions have been created by extracting code from other places, in particular main()


// Tick the world over.
// Call this whenever the display needs updating or FPS syncing.
void tick() {
  if (Terminal_keyExit()) {
      Terminal_exit();
  }
    Terminal_redraw();

  millisleep(29);
  return;

  clock_t last_tick = clock();
  int sleep_time = SKIP_TICKS - timediff(current_time, last_tick);

  if (sleep_time > 0) {
    usleep((useconds_t) sleep_time * 1000);
    current_time = clock();
  } else {
    // Shit, we are running behind!
  }
}

void draw_remaining_lives() {
  // LD HL,20640             // Set HL to the display file address at which to draw the first Willy sprite
  uint16_t addr = 20640;

  // LD A,(NOMEN)            // Pick up the number of lives remaining from NOMEN
  // OR A                    // Are there any lives remaining?
  // JR Z,LOOP_1             // Jump if not
  // LD B,A                  // Initialise B to the number of lives remaining

  // The following loop draws the remaining lives at the bottom of the screen.
  // LOOP_0:
  for (int i = 0; i < willy.NOMEN; i++) {
    // LD C,0                  // C=0; this tells the sprite-drawing routine at DRWFIX to overwrite any existing graphics

    // PUSH HL                 // Save HL and BC briefly
    // PUSH BC
    // LD A,(NOTEINDEX)        // Pick up the in-game music note index from NOTEINDEX; this will determine the animation frame for the Willy sprites
    // RLCA                    // Now A=0 (frame 0), 32 (frame 1), 64 (frame 2) or 96 (frame 3)
    // RLCA
    // RLCA
    // AND 96
    uint8_t anim_frame = (uint8_t)(rotl(game.NOTEINDEX, 3) & 96);

    // LD E,A                  // Point DE at the corresponding Willy sprite (at MANDAT+A)
    // LD D,130
    // IMPORTANT: swap MANDAT with new WILLYDATA, to access all anim frames -MRC-
    uint8_t *sprite = &willy.sprites[anim_frame];

    // CALL DRWFIX             // Draw the Willy sprite on the screen
    DRWFIX(sprite, addr, 0);

    // POP BC                  // Restore HL and BC
    // POP HL
    // INC HL                  // Move HL along to the location at which to draw the
    // INC HL                  // next Willy sprite
    addr += 2;
    // DJNZ LOOP_0             // Jump back to draw any remaining sprites
  }

  // Now draw a boot if cheat mode has been activated.
  // LOOP_1:
  // LD A,(CHEAT)            // Pick up the 6031769 key counter from CHEAT
  // CP 7                    // Has 6031769 been keyed in yet?
  // JR NZ,LOOP_2            // Jump if not
  if (game.CHEAT == 7) {
    // LD DE,BOOT              // Point DE at the graphic data for the boot (at BOOT)
    // LD C,0                  // C=0 (overwrite mode)
    // CALL DRWFIX             // Draw the boot at the bottom of the screen next to the remaining lives
    DRWFIX(&BOOT, addr, 0);
  }
}
