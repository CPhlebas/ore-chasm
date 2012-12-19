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

##### Allegro 5 #######
find_package(Allegro5 REQUIRED)
find_package(Allegro5Main REQUIRED)
find_package(Allegro5Primitives REQUIRED)
find_package(Allegro5Font REQUIRED)
find_package(Allegro5TTF REQUIRED)
find_package(Allegro5Image REQUIRED)
find_package(Allegro5Audio REQUIRED)
find_package(Allegro5ACodec REQUIRED)
find_package(Allegro5Dialog REQUIRED)

if (NOT ALLEGRO5_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 library")
endif(NOT ALLEGRO5_FOUND)

if (NOT ALLEGRO5_MAIN_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 Main addon")
endif(NOT ALLEGRO5_MAIN_FOUND)

if (NOT ALLEGRO5_PRIMITIVES_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 Primitives addon")
endif(NOT ALLEGRO5_PRIMITIVES_FOUND)

if (NOT ALLEGRO5_FONT_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 Font addon")
endif(NOT ALLEGRO5_FONT_FOUND)

if (NOT ALLEGRO5_TTF_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 TTF addon")
endif(NOT ALLEGRO5_TTF_FOUND)

if (NOT ALLEGRO5_IMAGE_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 Image addon")
endif(NOT ALLEGRO5_IMAGE_FOUND)

if (NOT ALLEGRO5_AUDIO_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 Audio addon")
endif(NOT ALLEGRO5_AUDIO_FOUND)

if (NOT ALLEGRO5_ACODEC_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 ACodec addon")
endif(NOT ALLEGRO5_ACODEC_FOUND)

if (NOT ALLEGRO5_DIALOG_FOUND)
    message(FATAL_ERROR "Can't find Allegro 5 Native Dialog addon")
endif(NOT ALLEGRO5_DIALOG_FOUND)



list(APPEND ALLEGRO_LIBRARIES
    ${ALLEGRO5_LIBRARIES}
    ${ALLEGRO5_MAIN_LIBRARIES}
    ${ALLEGRO5_PRIMITIVES_LIBRARIES}
    ${ALLEGRO5_FONT_LIBRARIES}
    ${ALLEGRO5_TTF_LIBRARIES}
    ${ALLEGRO5_IMAGE_LIBRARIES}
    ${ALLEGRO5_DIALOG_LIBRARIES}
    ${ALLEGRO5_AUDIO_LIBRARIES}
    ${ALLEGRO5_ACODEC_LIBRARIES}
)

list(APPEND ALLEGRO_INCLUDE_DIRS
    ${ALLEGRO5_INCLUDE_DIRS}
    ${ALLEGRO5_MAIN_INCLUDE_DIRS}
    ${ALLEGRO5_PRIMITIVES_INCLUDE_DIRS}
    ${ALLEGRO5_FONT_INCLUDE_DIRS}
    ${ALLEGRO5_TTF_INCLUDE_DIRS}
    ${ALLEGRO5_AUDIO_INCLUDE_DIRS}
    ${ALLEGRO5_ACODEC_INCLUDE_DIRS}
)

##################

find_package( PugiXML REQUIRED)
find_package( OpenGL REQUIRED )

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

include_directories(${CMAKE_SOURCE_DIR} ${ALLEGRO_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${ALLEGRO_LIBRARIES} ${OPENGL_LIBRARIES} ${PugiXML_LIBRARY})

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
