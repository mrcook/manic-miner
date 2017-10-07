cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

# Do not allow in-source builds
if (${CMAKE_SOURCE_DIR} STREQUAL "${PROJECT_SOURCE_DIR}/src")
    message(FATAL_ERROR "CMake generation is not allowed within the source directory!")
endif ()


#
# Set a default build type
#
set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
else ()
    message(STATUS "Build type set to '${CMAKE_BUILD_TYPE}'")
endif ()

# Compiler settings (this must come before calling project)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

project(manic_miner)


#
# Core set of warnings
#
set(cxx_warnings "-Wall")
set(cxx_warnings "${cxx_warnings} -Wextra")
set(cxx_warnings "${cxx_warnings} -Wpedantic")
set(cxx_warnings "${cxx_warnings} -Wshadow")
set(cxx_warnings "${cxx_warnings} -Werror")
set(cxx_warnings "${cxx_warnings} -pedantic-errors")
#set(cxx_warnings "${cxx_warnings} -Weffc++ ")

# Additional warnings
set(cxx_warnings "${cxx_warnings} -Wcast-align")
set(cxx_warnings "${cxx_warnings} -Wdisabled-optimization")
set(cxx_warnings "${cxx_warnings} -Wfloat-equal")
set(cxx_warnings "${cxx_warnings} -Winline")
set(cxx_warnings "${cxx_warnings} -Winvalid-pch")
set(cxx_warnings "${cxx_warnings} -Wmissing-format-attribute")
set(cxx_warnings "${cxx_warnings} -Wmissing-include-dirs")
set(cxx_warnings "${cxx_warnings} -Wpacked")
set(cxx_warnings "${cxx_warnings} -Wredundant-decls")
set(cxx_warnings "${cxx_warnings} -Wswitch-default")
set(cxx_warnings "${cxx_warnings} -Wswitch-enum")
set(cxx_warnings "${cxx_warnings} -Wunreachable-code")
set(cxx_warnings "${cxx_warnings} -Wwrite-strings")

# Some very strict warnings, that will be nice to use, but require some hefty refactoring
# set(cxx_warnings "${cxx_warnings} -Wcast-qual")
# set(cxx_warnings "${cxx_warnings} -Wconversion")
# set(cxx_warnings "${cxx_warnings} -Wformat=2")
# set(cxx_warnings "${cxx_warnings} -Wpadded")
# set(cxx_warnings "${cxx_warnings} -Wstrict-overflow")
# set(cxx_warnings "${cxx_warnings} -fno-strict-aliasing")

#
# Set the flags and warnings for the debug/release builds
#
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0 ${cxx_warnings}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 ${cxx_warnings}")

# Current game version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(source_dir ${PROJECT_SOURCE_DIR}/src)
set(SOURCE_FILES
        ${source_dir}/Version.h
        ${source_dir}/Headers.h
        ${source_dir}/Globals.h
        ${source_dir}/Helpers.h
        ${source_dir}/Helpers.cpp
        ${source_dir}/Globals.cpp
        ${source_dir}//BlipBuffer/Blip_Buffer.h
        ${source_dir}//BlipBuffer/Blip_Buffer.cpp
        ${source_dir}/Speccy.h
        ${source_dir}/SpeccyFont.cpp
        ${source_dir}/Speccy.cpp
        ${source_dir}/SpeccyDisplay.h
        ${source_dir}/SpeccyDisplay.cpp
        ${source_dir}/Window.h
        ${source_dir}/Window.cpp
        ${source_dir}/AudioSystem.h
        ${source_dir}/AudioSystem.cpp
        ${source_dir}/Music.h
        ${source_dir}/Music.cpp
        ${source_dir}/SpeccyColour.h
        ${source_dir}/Coord.h
        ${source_dir}/Data.h
        ${source_dir}/data/Caverns.cpp
        ${source_dir}/data/GuardianSprites.cpp
        ${source_dir}/data/Music.cpp
        ${source_dir}/data/Tiles.cpp
        ${source_dir}/data/TitleScreen.cpp
        ${source_dir}/data/WillySprites.cpp
        ${source_dir}/Conveyor.h
        ${source_dir}/Cavern.h
        ${source_dir}/Cavern.cpp
        ${source_dir}/GuardianHorizontal.h
        ${source_dir}/GuardianHorizontal.cpp
        ${source_dir}/GuardianSpecial.h
        ${source_dir}/GuardianSpecial.cpp
        ${source_dir}/GuardianVertical.h
        ${source_dir}/GuardianVertical.cpp
        ${source_dir}/KongBeast.h
        ${source_dir}/KongBeast.cpp
        ${source_dir}/Item.h
        ${source_dir}/Portal.h
        ${source_dir}/Tile.h
        ${source_dir}/Willy.h
        ${source_dir}/Willy.cpp
        ${source_dir}/Game.h
        ${source_dir}/Game.cpp
        ${source_dir}/Main.cpp
)

#
# Set up the install paths and files
#
set(build_dir "manic-miner")
file(MAKE_DIRECTORY ${build_dir})
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${build_dir})
set(RUNTIME_OUTPUT_DIRECTORY ${build_dir})

##
## Extract the Manic Miner version number from Version.h
##
#file(STRINGS "${source_dir}/Version.h" VERSION_HEADER)
#
#string(REGEX MATCH "VERSION_MAJOR = ([0-9]+);" ${VERSION_HEADER})
#set(mm_version_major ${CMAKE_MATCH_1})
#
#string(REGEX MATCH "VERSION_MINOR = ([0-9]+);" ${VERSION_HEADER})
#set(mm_version_minor ${CMAKE_MATCH_1})
#
#string(REGEX MATCH "VERSION_PATCH = ([0-9]+);" ${VERSION_HEADER})
#set(umm_version_patch ${CMAKE_MATCH_1})
#
#set(mm_version "${mm_version_major}.${mm_version_minor}.${mm_version_patch}")

# Also add resources to the target so they are visible in the IDE
add_executable(manic_miner ${SOURCE_FILES})

# Link SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2})
target_link_libraries(manic_miner ${SDL2_LIBRARIES})

# add the package install targets
# Build and install the manic miner binary
install(TARGETS manic_miner RUNTIME DESTINATION ${build_dir})
