// Manic Miner C Port Copyright 2017 Michael R. Cook
// Manic Miner Copyright 1983 Bug-Byte Ltd.

#include "Globals.h"
#include "Helpers.h"
#include "Data.h"
#include "Music.h"

// Play the theme tune (The Blue Danube).
//
// Returns with the zero flag reset if ENTER or the fire button
// is pressed while the tune is being played.
//
// IY THEMETUNE (tune data)
bool PLAYTUNE() {
    uint8_t freq1, freq2;
    uint16_t addr;

    for (int i = 0; i < 95; i++) {
        uint8_t *note = THEMETUNE[i];

        // Copy the first byte of data for this note (which determines the duration) to C
        uint8_t duration = note[0];

        // Pick up the second byte of data for this note. Copy it to A.
        freq1 = note[1];
        // Calculate the attribute file address for the corresponding piano key.
        addr = PIANOKEY(freq1);
        // Set the attribute byte for the piano key to 80 (INK 0: PAPER 2: BRIGHT 1).
        speccy.writeMemory(addr, 80);

        // Pick up the third byte of data for this note.
        // Copy it to A.
        freq2 = note[2];
        // Calculate the attribute file address for the corresponding piano key.
        addr = PIANOKEY(freq2);
        // Set the attribute byte for the piano key to 40 (INK 0: PAPER 5: BRIGHT 0).
        speccy.writeMemory(addr, 40);

        speccy.redrawWindow();

        // Produce a sound based on the frequency parameters in the second
        // and third bytes of data for this note (copied into D and E).
        for (uint8_t d = duration; d > 0; d--) {
            // Speccy::OUT(pitch);

            if (freq1 == 0 || freq2 == 0) {
                millisleep(d / 3);
            } else if (d % 3 == 0) {
                speccy.beep(freq1, d, 5);
                speccy.beep(freq2, d, 5);
            }
        }

        // Check whether ENTER or the fire button is being pressed.
        if (processInput() == Keyboard::MM_KEY_ENTER) {
            return true;
        }

        // Pick up the second byte of data for this note.
        // Calculate the attribute file address for the corresponding piano key.
        addr = PIANOKEY(note[1]);
        // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0).
        speccy.writeMemory(addr, 56);

        // Pick up the third byte of data for this note.
        // Calculate the attribute file address for the corresponding piano key.
        addr = PIANOKEY(note[2]);
        // Set the attribute byte for the piano key back to 56 (INK 0: PAPER 7: BRIGHT 0).
        speccy.writeMemory(addr, 56);

        speccy.redrawWindow();
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
    frequency = Speccy::rotR(frequency, 3);
    frequency = (uint8_t) ~frequency;

    // A=224+K; this is the LSB.
    frequency |= 224;

    // Set HL to the attribute file address for the piano key.
    return Speccy::buildAddress(89, frequency);
}
