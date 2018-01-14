#include "beeper.h"

Beeper::Beeper() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        return;
    }

    SDL_AudioSpec desiredSpec;

    desiredSpec.freq = FREQUENCY;
    desiredSpec.format = AUDIO_S16SYS;
    desiredSpec.channels = 1;
    desiredSpec.samples = 1024;
    desiredSpec.callback = audio_callback;
    desiredSpec.userdata = this;

    SDL_AudioSpec obtainedSpec;

    if (SDL_OpenAudio(&desiredSpec, &obtainedSpec) < 0) {
        return;
    }

    // start play audio
    SDL_PauseAudio(0);
}

Beeper::~Beeper() {
    SDL_PauseAudio(1);
    SDL_CloseAudio();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Beeper::generateSamples(int16_t *stream, int length) {
    int i = 0;
    while (i < length) {

        if (beeps.empty()) {
            while (i < length) {
                stream[i] = 0;
                i++;
            }
            return;
        }
        BeepObject &bo = beeps.front();

        int samplesToDo = std::min(i + bo.samplesLeft, length);
        bo.samplesLeft -= samplesToDo - i;

        while (i < samplesToDo) {
            stream[i] = AMPLITUDE * std::sin(v * 2 * M_PI / FREQUENCY);
            i++;
            v += bo.freq;
        }

        if (bo.samplesLeft == 0) {
            beeps.pop();
        }
    }
}

void Beeper::playNote(double hz, int duration) {
    // hz = 261.63; // middle C
    // duration = 5 // a nice short blip!
    duration = 10;
    beep(hz, duration);
    wait();
}

void Beeper::beep(double freq, int duration) {
    BeepObject bo;
    bo.freq = freq;
    bo.samplesLeft = duration * FREQUENCY / 1000;
    SDL_LockAudio();
    beeps.push(bo);
    SDL_UnlockAudio();
}

void Beeper::wait() {
    int size;
    do {
        SDL_Delay(10);
        SDL_LockAudio();
        size = beeps.size();
        SDL_UnlockAudio();
    } while (size > 0);
}

void Beeper::audio_callback(void *_beeper, uint8_t *_stream, int _length) {
    int16_t *stream = (int16_t *) _stream;
    int length = _length / 2;
    Beeper *beeper = (Beeper *) _beeper;

    beeper->generateSamples(stream, length);
}
