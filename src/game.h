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
