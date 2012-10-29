cmake_minimum_required(VERSION 2.6)

if(EXISTS ${CMAKE_SOURCE_DIR}/CMakeCache.txt)
        file(REMOVE ${CMAKE_SOURCE_DIR}/CMakeCache.txt)
        file(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/CMakeFiles)
endif(EXISTS ${CMAKE_SOURCE_DIR}/CMakeCache.txt)

if(EXISTS ${CMAKE_BINARY_DIR}/Makefile)
        file(REMOVE ${CMAKE_BINARY_DIR}/Makefile)
endif(EXISTS ${CMAKE_BINARY_DIR}/Makefile)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
                message(FATAL_ERROR "Oops. Don't do an in-source build. Create an extra directory change into it and run cmake pointing to the base directory. IE: \nmkdir mybuild && cd mybuild && cmake ../ && make\nYou may need to remove CMakeCache.txt and the CMakeFiles directory in ${CMAKE_SOURCE_DIR} if you can't get rid of this error.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})


set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

project (buildarrhea)

include(CheckCSourceCompiles)
include(CheckCXXSourceCompiles)
include(${ALLEGRO_INCLUDE_DIR})

option(DEBUG "Compile buildarrhea with debug information?" OFF)

if($ENV{DEBUG})
    set(DEBUG ON)
elseif(NOT $ENV{DEBUG})
    set(DEBUG OFF)
endif($ENV{DEBUG})

if(DEBUG)
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -Woverloaded-virtual -g3 -ggdb")
    set(CMAKE_C_FLAGS "-Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -g3 -ggdb")
else(DEBUG)
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -Woverloaded-virtual")
    set(CMAKE_C_FLAGS "-O2 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations")
endif(DEBUG)

find_package(ALLEGRO5)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/)
install(DIRECTORY textures/ DESTINATION "textures") #FILES_MATCHING PATTERN "*")

set(SOURCES
src/main.cpp
src/game.cpp
src/renderer.cpp
)
add_executable (buildarrhea ${SOURCES})

target_link_libraries(buildarrhea ${ALLEGRO_LIBRARY})

install(TARGETS buildarrhea DESTINATION bin)