// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "globals.h"
#include "helpers.h"
#include "data.h"
#include "terminal.h"
#include "sound.h"

// Play the theme tune (The Blue Danube).
//
// Returns with the zero flag reset if ENTER or the fire button
// is pressed while the tune is being played.
//
// IY THEMETUNE (tune data)
bool PLAYTUNE() {
    // FIXME: no need to play the tune just yet, so return early :)
    if (true) {
        return false;
    }

    uint8_t freq1, freq2;
    uint16_t addr;

    for (int i = 0; i < 95; i++) {
        uint8_t *note = THEMETUNE[i];

        // Pick up the second byte of data for this note. Copy it to A.
        freq1 = note[1];

        // Calculate the attribute file address for the corresponding piano key.
        addr = PIANOKEY(freq1);

        // Set the attribute byte for the piano key to 80 (INK 0: PAPER 2: BRIGHT 1).
        Speccy_write(addr, 80);

        // Pick up the third byte of data for this note.
        freq2 = note[2];

        // Copy it to A.
        uint8_t pitch = freq2;

        // Calculate the attribute file address for the corresponding piano key.
        addr = PIANOKEY(freq2);

        // Set the attribute byte for the piano key to 40 (INK 0: PAPER 5: BRIGHT 0).
        Speccy_write(addr, 40);

        for (uint8_t d = note[0]; d > 0; d--) {
            // Produce a sound based on the frequency parameters in the second
            // and third bytes of data for this note (copied into D and E).
            OUT(pitch);

            freq1--;
            if (freq1 == 0) {
                freq1 = note[1];
                pitch ^= 24;
            }
            freq2--;
            if (freq2 == 0) {
                freq2 = note[2];
                pitch ^= 24;
            }

            // FIXME: reg B (delay) is initialized to 0, but never set anywhere else, so this code is obsolete...?
            millisleep(1);
        }

        // Check whether ENTER or the fire button is being pressed.
        if (Terminal_getKey() == MM_KEY_ENTER) {
            return true;
        }

        // Pick up the second byte of data for this note.
        // Calculate the attribute file address for the corresponding piano key.
        addr = PIANOKEY(note[1]);

        // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0).
        Speccy_write(addr, 56);

        // Pick up the third byte of data for this note.
        // Calculate the attribute file address for the corresponding piano key.
        addr = PIANOKEY(note[2]);

        // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0).
        Speccy_write(addr, 56);
    }

    return false;
}

// Calculate the attribute file address for a piano key.
// Returns with the attribute file address in HL.
// A Frequency parameter from the tune data table at THEMETUNE.
uint16_t PIANOKEY(uint8_t frequency) {
    // Compute the piano key index (K) based on the frequency parameter (F),
    // and store it in bits 0-4 of A: K=31-INT((F-8)/8).
    frequency -= 8;
    frequency = rotr(frequency, 3);
    frequency = (uint8_t) ~frequency;

    // A=224+K; this is the LSB.
    frequency |= 224;

    // Set HL to the attribute file address for the piano key.
    return build_address(89, frequency);
}
