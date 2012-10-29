cmake_minimum_required(VERSION 2.6)
project (buildarrhea)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -Woverloaded-virtual -g3 -ggdb")
    set(CMAKE_C_FLAGS "-Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -g3 -ggdb")
else(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -Woverloaded-virtual")
    set(CMAKE_C_FLAGS "-O2 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# Set version information in a config.h file
set(myproject_VERSION_MAJOR 1)
set(myproject_VERSION_MINOR 0)

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

# Define sources and executable
set(EXECUTABLE_NAME "buildarrhea")

set(SRC
src/main.cpp
src/game.cpp
src/renderer.cpp
)

add_executable(${EXECUTABLE_NAME} ${SRC})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 1.6 REQUIRED system window graphics network audio)

if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${buildarrhea_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${buildarrhea_VERSION_MINOR}")
include(CPack)


add_subdirectory(${CMAKE_SOURCE_DIR}/src/)

# Install target
install(DIRECTORY textures/ DESTINATION "textures") #FILES_MATCHING PATTERN "*")

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
