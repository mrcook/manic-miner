#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <queue>
#include <cmath>
#include <iostream>

// Constants used for standard audio playback
const int AMPLITUDE = 28000 * 2;
const int FREQUENCY = 44100;

// Helper struct used to generate samples
struct BeepObject {
    double freq;
    int samplesLeft;
};


class Beeper {
private:
    double v;
    std::queue<BeepObject> beeps;
public:
    Beeper();
    ~Beeper();
    void beep(double, int);
    void generateSamples(int16_t *stream, int length);
    void wait();
    static void audio_callback(void *_beeper, uint8_t *_stream, int _length);
    void playNote(double hz, int duration);
};
