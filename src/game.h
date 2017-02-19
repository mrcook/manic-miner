// Manic Miner C Port Copyright 2017 Michael R. Cook

#pragma once

#include "headers.h"

typedef struct Game_ {
    // Attribute (ID?) of the last item drawn
    // Holds the attribute byte of the last item drawn,
    // or 0 if all the items have been collected.
    uint8_t ITEMATTR;

    int current_score;
    int highscore;

    // Screen flash counter
    uint8_t FLASH;

    // Kempston joystick indicator
    // Holds 1 if a joystick is present, 0 otherwise.
    bool KEMP;

    // Game mode indicator
    // Holds 0 when a game is in progress, or a value from 1 to 64 when in demo mode.
    uint8_t DEMO;

    // In-game music note index
    uint8_t NOTEINDEX;

    // Play or mute in-game music
    bool playMusic;

    // Cheat code: 6031769 key counter
    bool CHEAT;

    // Game UI texts
    char airLabel[3]; // was MESSAIR
    char HGHSCOR[6];
    char SCORE[4];
    char SCORBUF[6];
    char MESSHSSC[32];
    char MESSG[4];
    char MESSO[4];
} Game;

// Initialize game "boot up" variables
void Game_initialize(void);

// Plays the intro music and text message
void Game_play_intro(void);

// Play the game, or DEMO.
bool Game_play(void);


//
// We don't really want these public, store here temporarily.
//

// private functions

// check if the user has entered the cheat code
bool checkCheatCode(void);

// Draws the AIR bar graphics to the screen
void drawAirBar(void);

// Draws the remaining lives at the bottom of the screen
void drawRemainingLives(void);

// Loads the data for the next cavern (was the NEWSHT goto)
void loadCavern(void);

// Flashes the screen colours
void flashScreen(void);

// Copies the screen buffer data to the display file
void copyScrBufToDisplayFile(void);

// Copies the attributes buffer data to the attributes file
void copyAttrBufToAttrFile(void);

// Copies empty cavern data to the Screen/Attributes buffers
void resetScreenAttrBuffers(void);

// Prints the Scores and Highscores to the screen
void printScores(void);

// Play the in-game music
void playGameMusic(void);

bool MANDEAD(void);
void ENDGAM(void);
void DECAIR(void);
void DRAWSHEET(void);
bool MOVEWILLY(void);
void MOVEWILLY_7(uint8_t y_coord);
void MOVEWILLY_10(void);
void CRUMBLE(uint16_t addr);
bool MOVEWILLY2(uint16_t addr);
void MOVEWILLY2_6(void);
void MOVEWILLY2_7(void);
void MOVEWILLY2_9(void);
void MOVEWILLY2_10(void);
bool KILLWILLY(void);
bool KILLWILLY_0(void);
bool KILLWILLY_1(void);
void MOVEHG(void);
void LIGHTBEAM(void);
bool DRAWHG(void);
bool EUGENE(void);
void EUGENE_3(uint16_t addr, uint8_t ink_color);
bool SKYLABS(void);
bool VGUARDIANS(void);
void DRAWITEMS(void);
bool CHKPORTAL(void);
bool DRWFIX(void *sprite, uint16_t addr, uint8_t mode);
bool NXSHEET(void);
void INCSCORE_0(uint16_t addr);
void MVCONVEYOR(void);
bool KONGBEAST(void);
bool KONGBEAST_8(void);
bool CHKSWITCH(uint16_t addr);
bool WILLYATTRS(void);
bool WILLYATTR(uint16_t addr, uint8_t ink);
void DRAWWILLY(void);
void PMESS(void *msg, uint16_t addr, uint8_t len);
void PRINTCHAR(char ch, uint16_t addr);
void PRINTCHAR_0(void *chr, uint16_t addr, uint8_t len);
bool PLAYTUNE(void);
uint16_t PIANOKEY(uint8_t frequency);
bool CHECKENTER(void);
