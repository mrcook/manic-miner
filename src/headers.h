// Manic Miner C/C++ port Copyright (c) 2016-2019 Michael R. Cook

// Header includes for standard libraries

#pragma once

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
#include <cstring>
#include <string>
#include <ctime>

#include <cassert>
