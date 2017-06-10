// Simple synchronous sound interface for SDL multimedia library

#ifndef SYNC_AUDIO_H
#define SYNC_AUDIO_H

#include "SDL2/SDL.h"

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    // Initialize with specified sample rate, channel count, and latency.
    // Returns NULL on success, otherwise error string.
    const char *start(int rate, int channelCount = 1, int latencyMilliseconds = 200);

    // Set gain, where 1.0 leaves sound unaltered
    void setGain(double g) { gain = (long) (g * (1L << gainBits)); }

    // Number of samples in blipBuffer waiting to be played
    int sampleCount() const;

    // Write samples to blipBuffer, first waiting until enough space is available
    typedef short Sample;
    void write(const Sample *in, int remaining);

    // Stop audio output
    void stop();

    int initAudio();
    int playNote(int note, int duration, int volume);

    int frame_rate;

private:
    enum {
        gainBits = 16
    };
    enum {
        bufferSize = 1024
    };

    Sample *volatile buffers;
    SDL_sem *volatile freeSemaphore;
    int volatile bufferCount;
    int volatile readBuffer;
    int writeBuffer;
    int writePosition;
    long gain;
    int soundOpen;

    Sample *buffer(int index);
    void fillBuffer(uint8_t *out, int byteCount);
    static void fillBuffer_(void *userData, uint8_t *out, int byteCount);
};

#endif

