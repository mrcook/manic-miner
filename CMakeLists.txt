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
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)

set(source_dir ${PROJECT_SOURCE_DIR}/src)
set(SOURCE_FILES
        ${source_dir}/audio_system.h
        ${source_dir}/blip_buffer/blip_buffer.h
        ${source_dir}/cavern.h
        ${source_dir}/conveyor.h
        ${source_dir}/coord.h
        ${source_dir}/data.h
        ${source_dir}/game.h
        ${source_dir}/globals.h
        ${source_dir}/guardian_horizontal.h
        ${source_dir}/guardian_special.h
        ${source_dir}/guardian_vertical.h
        ${source_dir}/headers.h
        ${source_dir}/helpers.h
        ${source_dir}/item.h
        ${source_dir}/kong_beast.h
        ${source_dir}/music.h
        ${source_dir}/portal.h
        ${source_dir}/speccy.h
        ${source_dir}/speccy_colour.h
        ${source_dir}/speccy_display.h
        ${source_dir}/tile.h
        ${source_dir}/version.h
        ${source_dir}/willy.h
        ${source_dir}/window.h
        ${source_dir}/audio_system.cpp
        ${source_dir}/blip_buffer/blip_buffer.cpp
        ${source_dir}/cavern.cpp
        ${source_dir}/data/caverns.cpp
        ${source_dir}/data/guardian_sprites.cpp
        ${source_dir}/data/music.cpp
        ${source_dir}/data/tiles.cpp
        ${source_dir}/data/title_screen.cpp
        ${source_dir}/data/willy_sprites.cpp
        ${source_dir}/game.cpp
        ${source_dir}/globals.cpp
        ${source_dir}/guardian_horizontal.cpp
        ${source_dir}/guardian_special.cpp
        ${source_dir}/guardian_vertical.cpp
        ${source_dir}/helpers.cpp
        ${source_dir}/kong_beast.cpp
        ${source_dir}/main.cpp
        ${source_dir}/music.cpp
        ${source_dir}/speccy.cpp
        ${source_dir}/speccy_display.cpp
        ${source_dir}/speccy_font.cpp
        ${source_dir}/willy.cpp
        ${source_dir}/window.cpp
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
## Extract the Manic Miner version number from version.h
##
#file(STRINGS "${source_dir}/version.h" VERSION_HEADER)
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
