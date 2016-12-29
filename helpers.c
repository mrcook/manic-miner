// Manic Miner C Port Copyright 2016 Michael R. Cook

// Cross Platform Helper Functions for Manic Miner

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
        usleep(milliseconds * 1000);
    #endif
}
