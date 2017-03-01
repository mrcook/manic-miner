cmake_minimum_required(VERSION 3.6)

project(manic_miner)

# Current game version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES
        src/headers.h
        src/globals.h
        src/helpers.h
        src/helpers.c
        src/globals.c
        src/speccy.h
        src/speccy.c
        src/terminal.h
        src/terminal.c
        src/colour.h
        src/coord.h
        src/data.h
        src/data/data_caverns.c
        src/data/data_guardian_sprites.c
        src/data/data_music.c
        src/data/data_tiles.c
        src/data/data_title_screen.c
        src/data/data_willy_sprites.c
        src/conveyor.h
        src/cavern.h
        src/cavern.c
        src/guardian_horizontal.h
        src/guardian_horizontal.c
        src/guardian_vertical.h
        src/guardian_vertical.c
        src/kong_beast.h
        src/kong_beast.c
        src/item.h
        src/portal.h
        src/tile.h
        src/willy.h
        src/willy.c
        src/game.h
        src/game.c
        src/main.c)

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
