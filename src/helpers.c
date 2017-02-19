// Manic Miner C Port Copyright 2017 Michael R. Cook

// Cross Platform Helper Functions for Manic Miner

#include "headers.h"

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
    usleep((useconds_t)milliseconds * 1000);
#endif
}

// Difference between two times in milliseconds
int timediff(clock_t t1, clock_t t2) {
    return (int)(((double)t2 - t1) / CLOCKS_PER_SEC * 1000);
}

// Returns the current clock() tick in milliseconds
int getTickCount() {
#ifdef _WIN32
    return GetTickCount();
#elif _POSIX_C_SOURCE >= 199309L
    return (int)(clock() / (CLOCKS_PER_SEC * 1000));
#else
    return (int)(clock() / (CLOCKS_PER_SEC * 1000));
#endif
}
