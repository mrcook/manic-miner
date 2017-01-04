cmake_minimum_required(VERSION 3.6)

project(manic_miner)

# Current game version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        src/headers.h
        src/externs.h
        src/main.c
        src/emulator.c
        src/helpers.c
        src/globals.c
        src/character_set.c
        src/data.c src/io.c)

add_executable(manic_miner ${SOURCE_FILES})

#set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -g -Wall -Werror -Wshadow -std=gnu11")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -g -Wall -std=gnu11")

# Link Curses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(manic_miner ${CURSES_LIBRARIES})

# add the package install targets
install(TARGETS manic_miner
        RUNTIME
        DESTINATION "./"
        COMPONENT applications)
