// Blip_Buffer 0.4.0. http://www.slack.net/~ant/

/* Copyright (C) 2005-2006 by Shay Green. Permission is hereby granted, free of
charge, to any person obtaining a copy of this software module and associated
documentation files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the
following conditions: The above copyright notice and this permission notice
shall be included in all copies or substantial portions of the Software. THE
SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

// Simple synchronous sound interface for SDL multimedia library

#pragma once

#include "SDL2/SDL.h"

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    // Initialize with specified sample rate, channel count, and latency.
    // Returns nullptr on success, otherwise error string.
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
    void fillBuffer(uint8_t &out, int byteCount);
    static void fillBuffer_(void *userData, uint8_t *out, int byteCount);
};
