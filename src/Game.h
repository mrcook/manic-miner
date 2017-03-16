// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#ifndef MANIC_MINER_GAME_H
#define MANIC_MINER_GAME_H


#include "Headers.h"

enum GameInput {
    MM_KEY_NONE,
    MM_KEY_LEFT,
    MM_KEY_RIGHT,
    MM_KEY_JUMP,
    MM_KEY_LEFT_JUMP,
    MM_KEY_RIGHT_JUMP,

    MM_KEY_ENTER,
    MM_KEY_PAUSE,
    MM_KEY_QUIT,
    MM_KEY_MUTE,
    MM_KEY_COLOUR_MODE,
};

typedef struct Game_ {
    int playerScore;
    int highScore;

    // Attribute (ID?) of the last item drawn
    // Holds the attribute byte of the last item drawn,
    // or 0 if all the items have been collected.
    uint8_t ITEMATTR;

    // Screen flash counter
    uint8_t FLASH;

    // Game mode indicator
    // Holds 0 when a game is in progress, or a value from 1 to 64 when in demo mode.
    uint8_t DEMO;

    // In-game music note index
    uint8_t NOTEINDEX;

    // Kempston joystick indicator
    // Holds 1 if a joystick is present, 0 otherwise.
    bool KEMP;

    // Play or mute in-game music
    bool playMusic;

    // Cheat code: 6031769 key counter
    bool CHEAT;

    // Play only the given cavern number if `teleportMode == true`
    bool teleportMode;
    int teleportCavernNumber;

    // Game UI texts
    char airLabel[3]; // was MESSAIR
    char MESSHSSC[32];
    char MESSG[4];
    char MESSO[4];
} Game;

// Initialize game "boot up" variables
void Game_initialize(bool cheat, int teleport);

// Clears all four game buffers: Display/Attributes and empty ones
void Game_clearBuffers(void);

// Plays the intro music and text message
void Game_play_intro(void);

// Play the game, or DEMO.
bool Game_play(void);

// Increment the play er score by the given amount
void Game_scoreAdd(int amount);

//
// We don't really want these public, store here temporarily.
//

// private functions

// Read keyboard and return key enums
int processInput(void);

// Check to see if player is pressing movement + jump keys
int processMoveJumpInput(int firstInput);

// Draws the remaining lives at the bottom of the screen
void drawRemainingLives(void);

// Loads the data for the next cavern (was the NEWSHT goto)
void loadCurrentCavern(void);

// Flashes the screen colours
void flashScreen(void);

// Copies the screen buffer data to the display file
void copyScrBufToDisplayFile(void);

// Copies the attributes buffer data to the attributes file
void copyAttrBufToAttrFile(void);

// Copies empty cavern data to the Screen/Attributes buffers
void resetScreenAttrBuffers(void);

void resetCurrentScore();

// INCSCORE_0 - Award extra life if player reaches milestone
void awardExtraLife();

void printCurrentScore(void);

void printHighScore(void);

void printScores(void);

void playGameMusic(void);

bool MANDEAD(void);

void ENDGAM(void);

void DRAWSHEET(void);

bool MOVEWILLY(int keyIntput);

void CRUMBLE(uint16_t addr);

void LIGHTBEAM(void);

bool EUGENE(void);

void EUGENE_3(uint16_t addr, uint8_t ink_colour);

bool SKYLABS(void);

void DRAWITEMS(void);

bool CHKPORTAL(void);

bool DRWFIX(void *sprite, uint16_t addr, uint8_t mode);

bool NXSHEET(void);


#endif //MANIC_MINER_GAME_H
