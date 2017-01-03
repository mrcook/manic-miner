cmake_minimum_required(VERSION 3.6)
project(manic_miner_assembly_port)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/headers.h
        src/externs.h
        src/helpers.c
        src/globals.c
        src/character_set.c
        src/data.c
        src/main.c)

add_executable(manic_miner_assembly_port ${SOURCE_FILES})