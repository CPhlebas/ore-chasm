cmake_minimum_required(VERSION 2.6)
project (ore_chasm)

# Enable debug symbols by default
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)

    #  cmake has this annoying fucking bug that took me a while to figure out how i can use clang: http://www.cmake.org/pipermail/cmake/2011-February/042629.html

    set(CMAKE_CXX_FLAGS "-std=c++11 -O0 -g3 -fpermissive")
# -Wall -Wno-unused-variable -Wno-unused-function -funroll-loops -Woverloaded-virtual -g3 -ggdb -ftrapv -fverbose-asm -Wextra -Wfloat-equal -Winit-self -Wuninitialized -Werror-implicit-function-declaration -Wundef -Wshadow  -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wswitch-default -Wconversion -Wno-switch -Wunreachable-code -Wformat=2 -fstack-check -Wdouble-promotion
    set(CMAKE_C_FLAGS "-Wall -Wno-unused-variable -Wno-unused-function -fpermissive -funroll-loops -fexpensive-optimizations -g3 -ggdb")
else(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_CXX_FLAGS "-std=c++11 -O2 -Wall -Wno-unused-variable -Wno-unused-function -fexpensive-optimizations -Woverloaded-virtual")
    set(CMAKE_C_FLAGS "-O2 -Wall -Wno-unused-variable -Wno-unused-function -fexpensive-optimizations")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
message("PATH: " + ${CMAKE_MODULE_PATH})
# Set version information in a config.h file
set(ore_chasm_VERSION_MAJOR 0)
set(ore_chasm_VERSION_MINOR 1)

find_package( PugiXML REQUIRED)
find_package( OpenGL REQUIRED )

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

if (NOT PugiXML_FOUND)
    message(FATAL_ERROR "Could not find pugixml library.")
endif()

include_directories( ${CMAKE_SOURCE_DIR} ${PugiXML_INCLUDE_DIR})

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

# Define sources and executable
set(EXECUTABLE_NAME "ore-chasm")

set(SRC
src/main.cpp
src/game.cpp
src/imagemanager.cpp
src/player.cpp
src/debug.cpp
src/world.cpp
src/renderable.cpp
src/entity.cpp
src/block.cpp
src/cloudsystem.cpp
src/sky.cpp
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

add_executable(${EXECUTABLE_NAME} ${SRC})

find_package(SFML 2.0 REQUIRED system window graphics audio) #network

if (NOT SFML_FOUND)
    message(FATAL_ERROR "Could not find SFML.")
endif()

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR})
include_directories(${SFML_INCLUDE_DIR} )
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${OPENGL_LIBRARIES} ${PugiXML_LIBRARY})


# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR "${ore_chasm_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ore_chasm_VERSION_MINOR}")
include(CPack)


add_subdirectory(${CMAKE_SOURCE_DIR}/src/)

# Install target
install(DIRECTORY textures/ DESTINATION "textures") #FILES_MATCHING PATTERN "*")
install(DIRECTORY font/ DESTINATION "font") #FILES_MATCHING PATTERN "*")

install(DIRECTORY src/ DESTINATION bin FILES_MATCHING PATTERN "*.frag" )
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
