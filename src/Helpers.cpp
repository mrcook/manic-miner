// Manic Miner C Port Copyright 2017 Michael R. Cook

// Cross Platform Helper Functions for Manic Miner

#include "Headers.h"

// Sleep for the given number of milliseconds
void millisleep(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#elif _POSIX_C_SOURCE >= 199309L
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
#else
    usleep((useconds_t) milliseconds * 1000);
#endif
}

// Returns the current clock() tick in milliseconds
int getTickCount() {
#ifdef _WIN32
    return GetTickCount();
#elif _POSIX_C_SOURCE >= 199309L
    return (int) (clock() * 1000.0 / CLOCKS_PER_SEC);
#else
    return (int) (clock() * 1000.0 / CLOCKS_PER_SEC);
#endif
}
