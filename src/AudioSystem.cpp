// Blip_Buffer 0.4.0. http://www.slack.net/~ant/

#include "BlipBuffer/Blip_Buffer.h"
#include "AudioSystem.h"

#include <assert.h>

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

const long sampleRate = 44100;  // 44.1 kHz sample rate
const long clockRate = 1000000; // 1 MHz clock rate
//const int frame_rate = 16;      // 16 frames of sound per second

// temporary buffer to read samples into
const int bufferSize = 10000;
static blip_sample_t samples[bufferSize];

static Blip_Buffer blipBuffer;
static Blip_Synth<blip_med_quality, 20> blipSynth;

// Return current SDL_GetError() string, or str if SDL didn't have a string
static const char *sdl_error(const char *str) {
    const char *sdlString = SDL_GetError();

    if (sdlString && *sdlString) {
        str = sdlString;
    }

    return str;
}

AudioSystem::AudioSystem() {
    // must initialize to a default value
    frame_rate = 16;

    bufferCount = 2;
    buffers = 0;
    freeSemaphore = 0;
    soundOpen = 0;

    setGain(1.0);

    initAudio();
}

AudioSystem::~AudioSystem() {
    stop();
}

const char *AudioSystem::start(int rate, int channelCount, int latencyMilliseconds) {
    stop();

    readBuffer = 0;
    writeBuffer = 0;
    writePosition = 0;

    int sampleLatency = latencyMilliseconds * rate * channelCount / 1000;
    bufferCount = sampleLatency / bufferSize;
    if (bufferCount < 2) {
        bufferCount = 2;
    }

    buffers = (Sample *) malloc((long) bufferSize * bufferCount * sizeof *buffers);
    if (!buffers) {
        return "Out of memory";
    }

    freeSemaphore = SDL_CreateSemaphore((uint32_t) bufferCount - 1);
    if (!freeSemaphore) {
        return sdl_error("Couldn't create semaphore");
    }

    SDL_AudioSpec as = {
            .freq = rate,
            .format = AUDIO_S16SYS,
            .channels = (uint8_t) channelCount,
            .silence = 0,
            .samples = (uint16_t) (bufferSize / channelCount),
            .size = 0,
            .callback = fillBuffer_,
            .userdata = this,
    };

    if (SDL_OpenAudio(&as, 0) < 0) {
        return sdl_error("Couldn't open SDL audio");
    }

    SDL_PauseAudio(0);
    soundOpen = 1;

    return 0; // success
}

void AudioSystem::stop() {
    if (soundOpen) {
        soundOpen = 0;
        SDL_PauseAudio(1);
        SDL_CloseAudio();
    }

    if (freeSemaphore) {
        SDL_DestroySemaphore(freeSemaphore);
        freeSemaphore = 0;
    }

    free(buffers);
    buffers = 0;

    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

int AudioSystem::sampleCount() const {
    if (!freeSemaphore) {
        return 0;
    }

    int bufferFree = SDL_SemValue(freeSemaphore) * bufferSize + (bufferSize - writePosition);

    return bufferSize * bufferCount - bufferFree;
}

inline AudioSystem::Sample *AudioSystem::buffer(int index) {
    assert(index < bufferCount);

    return buffers + (long) index * bufferSize;
}

void AudioSystem::write(const Sample *in, int remaining) {
    while (remaining) {
        int count = bufferSize - writePosition;

        if (count > remaining) {
            count = remaining;
        }

        Sample *out = buffer(writeBuffer) + writePosition;

        if (gain != (1L << gainBits)) {
            int audioGain = (int) this->gain;
            for (int n = count; n--;) {
                *out++ = (Sample) ((*in++ * audioGain) >> gainBits);
            }
        } else {
            memcpy(out, in, count * sizeof(Sample));
            in += count;
        }

        writePosition += count;
        remaining -= count;

        if (writePosition >= bufferSize) {
            writePosition = 0;
            writeBuffer = (writeBuffer + 1) % bufferCount;
            SDL_SemWait(freeSemaphore);
        }
    }
}

void AudioSystem::fillBuffer(uint8_t &out, int byteCount) {
    if (SDL_SemValue(freeSemaphore) < (uint32_t) bufferCount - 1) {
        memcpy(&out, buffer(readBuffer), byteCount);
        readBuffer = (readBuffer + 1) % bufferCount;
        SDL_SemPost(freeSemaphore);
    } else {
        memset(&out, 0, byteCount);
    }
}

void AudioSystem::fillBuffer_(void *userData, uint8_t *out, int byteCount) {
    ((AudioSystem *) userData)->fillBuffer(*out, byteCount);
}

int AudioSystem::initAudio() {
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        return 1;
    }

    // atexit(SDL_Quit);

    // Setup blipBuffer
    blipBuffer.clock_rate(clockRate);
    if (blipBuffer.set_sample_rate(sampleRate, 1000 / frame_rate * 100)) {
        return 1; // out of memory
    }

    // Setup synth
    blipSynth.volume(0.5);
    blipSynth.treble_eq(-37.0);
    blipSynth.output(&blipBuffer);

    // Start audio
    if (start(sampleRate)) {
        return 1;
    }

    return 0;
}

int AudioSystem::playNote(int note, int duration, int volume) {
    int time = 0;

    int amplitude = 5;
    if (volume > 0) {
        amplitude = volume;
    }

    // IMPORTANT: the duration calculations currently make the delay
    // twice as long as it should be, so we should not multiply by 2.
    // Of course, this may change in the future.
    long durationInMilliseconds = duration / frame_rate / 2;
    // long durationInMilliseconds = duration / frame_rate * 2;

    for (long n = durationInMilliseconds; n > 0; n--) {
        // Fill buffer with frame of sound
        long length = (int) clockRate / frame_rate / 3;

        while (time < length) {
            amplitude = -amplitude;
            blipSynth.update(time, amplitude);

            // IMPORTANT: at the moment we need to multiply the note to get the correct pitch!
            time += note * 24;
        }

        blipBuffer.end_frame(length);
        time -= length;

        // Read and play samples
        int count = (int) blipBuffer.read_samples(samples, bufferSize);
        write(samples, count);
    }

    return time;
}
