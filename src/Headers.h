// Manic Miner C Port Copyright 2017 Michael R. Cook

#ifndef MANIC_MINER_HEADERS_H
#define MANIC_MINER_HEADERS_H


// Header includes for standard libraries

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <io.h>

#elif __APPLE__

#include <unistd.h>

#elif __linux__

#include <unistd.h>

#else
error "Unknown compiler"
#endif


// Headers we can use on all supported systems!

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>

#include <cassert>


#endif //MANIC_MINER_HEADERS_H
