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
    uint8_t CHEAT;
} Game;

// Initialize game "boot up" variables
void Game_initialize(void);

// Called before each new game is started to reset vars: current_score, etc.
void Game_new(void);

// Plays the intro music and text message
void Game_play_intro(void);
