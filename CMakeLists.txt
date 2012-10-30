cmake_minimum_required(VERSION 2.6)
project (buildarrhea)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)


    # because cmake has this annoying fucking bug that took me a while to figure out how i can use clang: http://www.cmake.org/pipermail/cmake/2011-February/042629.html
    # HELP ME! CVARS DOESN"T BUILD WITH CLANG..SAD FACE, so for now, use..gcrapcrap :(
    set(CMAKE_CXX_COMPILER "g++")

    set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -Woverloaded-virtual -g3 -ggdb")
    set(CMAKE_C_FLAGS "-Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -g3 -ggdb")
else(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations -Woverloaded-virtual")
    set(CMAKE_C_FLAGS "-O2 -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -fexpensive-optimizations")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

# Set version information in a config.h file
set(buildarrhea_VERSION_MAJOR 0)
set(buildarrhea_VERSION_MINOR 1)

find_package( GLUT REQUIRED )
find_package( GLU REQUIRED )
find_package( OpenGL REQUIRED )
find_package(CVARS REQUIRED)

if (NOT CVARS_FOUND)
    message(FATAL_ERROR "Could not find CVARS (for quake console).")
endif()

include_directories( ${CMAKE_SOURCE_DIR} ${GLUT_INCLUDE_DIR} ${CVARS_INCLUDE_DIR})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

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
src/imagemanager.cpp
src/player.cpp
src/world.cpp
src/renderer.cpp
src/renderable.cpp
)

#YAML, needed later
## pkg-config-based library
include(FindPkgConfig)
#pkg_check_modules(yaml-cpp REQUIRED yaml-cpp>=0.2.5)
#if(yaml-cpp_FOUND)
#  include_directories(${yaml-cpp_INCLUDE_DIRS})
#  link_directories(${yaml-cpp_LIBRARY_DIRS})
#endif()
#add_executable(${EXECUTABLE_NAME} ${SOURCES})
#target_link_libraries(${EXECUTABLE_NAME} ${yaml-cpp_LIBRARIES})
#

find_package(Chipmunk 5.3.4 REQUIRED chipmunk)
if (CHIPMUNK_FOUND)
    include_directories(${CHIPMUNK_INCLUDE_DIRS})
    link_directories(${CHIPMUNK_LIBRARIES})
endif()

if (NOT CHIPMUNK_FOUND)
    message(FATAL_ERROR "Could not find chipmunk 2D physics library.")
endif()

add_executable(${EXECUTABLE_NAME} ${SRC})

find_package(SFML 2.0 REQUIRED system window graphics audio) #network

if (NOT SFML_FOUND)
    message(FATAL_ERROR "Could not find SFML.")
endif()
                                                                    INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} ${GLUT_INCLUDE_DIR} )
include_directories(${SFML_INCLUDE_DIR} ${CVARS_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${CHIPMUNK_LIBRARIES} ${CVARS_LIBRARIES} ${GLUT_LIBRARIES} ${GLU_LIBRARIES} ${OPENGL_LIBRARIES})


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${buildarrhea_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${buildarrhea_VERSION_MINOR}")
include(CPack)


add_subdirectory(${CMAKE_SOURCE_DIR}/src/)

# Install target
install(DIRECTORY textures/ DESTINATION "textures") #FILES_MATCHING PATTERN "*")
install(DIRECTORY font/ DESTINATION "font") #FILES_MATCHING PATTERN "*")

install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
