cmake_minimum_required(VERSION 3.6)
project(manic_miner_assembly_port)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        character_set.c
        data.c
        externs.h
        globals.c
        headers.h
        helpers.c
        mm.c)

add_executable(manic_miner_assembly_port ${SOURCE_FILES})