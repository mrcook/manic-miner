cmake_minimum_required(VERSION 3.6)

project(manic_miner)

# Current game version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/Headers.h
        src/Globals.h
        src/Helpers.h
        src/Helpers.cpp
        src/Globals.cpp
        src/Speccy.h
        src/SpeccyFont.cpp
        src/Speccy.cpp
        src/Display.h
        src/Display.cpp
        src/Window.h
        src/Window.cpp
        src/Sound.h
        src/Sound.cpp
        src/Colour.h
        src/Coord.h
        src/Data.h
        src/data/Caverns.cpp
        src/data/GuardianSprites.cpp
        src/data/Music.cpp
        src/data/Tiles.cpp
        src/data/TitleScreen.cpp
        src/data/WillySprites.cpp
        src/Conveyor.h
        src/Cavern.h
        src/Cavern.cpp
        src/GuardianHorizontal.h
        src/GuardianHorizontal.cpp
        src/GuardianVertical.h
        src/GuardianVertical.cpp
        src/KongBeast.h
        src/KongBeast.cpp
        src/Item.h
        src/Portal.h
        src/Tile.h
        src/Willy.h
        src/Willy.cpp
        src/Game.h
        src/Game.cpp
        src/Main.cpp)

add_executable(manic_miner ${SOURCE_FILES})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Werror -Wshadow -std=c++14")

# Link SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2})
target_link_libraries(manic_miner ${SDL2_LIBRARIES})

# add the package install targets
install(TARGETS manic_miner
        RUNTIME
        DESTINATION "./"
        COMPONENT applications)
