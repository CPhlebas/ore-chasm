cmake_minimum_required(VERSION 2.4 FATAL_ERROR)

# Set build type. Do this *before* we set the project name
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo Profile."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" CACHE INTERNAL "internal")

if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(Jazz C)

# Search in the `cmake' directory for additional CMake modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(CMakeDependentOption)

# Declare program options
option(WANT_WARN   "Enable compiler warnings" on)
option(WANT_NATIVE "Optimise for the current machine" off)
option(WANT_ASSERT "Enable assertions in the code (for debugging)" off)
option(WANT_OPT    "Standard optimisations on/off" on)
option(WANT_FAST   "Agressive optimisations on/off" on)
option(WANT_SSE42  "Switch on SSE 4.2" on)
option(WANT_GUI    "Wether you want to build the GUI or not (requires Allegro)" off)
option(WANT_XBOARD "Wether you want to build the XBoard interface" on)
option(WANT_UCI    "Wether you want to build the UCI interface" on)
option(WANT_TEST   "Wether you want to build the test utilities" on)
option(WANT_DYNAMIC "Wether you want to build a dynamic version of the library" off)
cmake_dependent_option(WANT_WERROR "Treat warnings as errors" on "WANT_WARN" on)

# Parse user options
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --param large-function-growth=180 --param max-inline-insns-single=800")
if(WANT_WARN)
   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Winline -Wno-deprecated-declarations -Wuninitialized")
endif(WANT_WARN)
if(WANT_OPT)
   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -finline")
endif(WANT_OPT)
if(WANT_WERROR)
   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif(WANT_WERROR)
if(WANT_NATIVE)
   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -mtune=native")
endif(WANT_NATIVE)
if(WANT_FAST)
   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fast")
endif(WANT_FAST)
if(WANT_SSE42)
   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.2")
endif(WANT_SSE42)
if(WANT_ASSERT)
   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DDEBUGMODE")
endif(WANT_ASSERT)

# Search for C header files in these directories.
include_directories(
   ${CMAKE_SOURCE_DIR}/include
   ${CMAKE_BINARY_DIR}/include
)

# Subversion revision number
find_package(Subversion)
if(Subversion_FOUND)
   find_program(Subversion_SVNVERSION_EXECUTABLE svnversion)
   if(Subversion_SVNVERSION_EXECUTABLE)
      EXECUTE_PROCESS(COMMAND ${Subversion_SVNVERSION_EXECUTABLE}  ${CMAKE_SOURCE_DIR}
         OUTPUT_VARIABLE Project_WC_REVISION
         OUTPUT_STRIP_TRAILING_WHITESPACE)
   else(Subversion_SVNVERSION_EXECUTABLE)
      Subversion_WC_INFO(${CMAKE_HOME_DIRECTORY} Project)
   endif(Subversion_SVNVERSION_EXECUTABLE)

   set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DSVNVERSION=\\\"${Project_WC_REVISION}\\\"")
   message("Current revision is ${Project_WC_REVISION}")
endif(Subversion_FOUND)


if(WANT_GUI)
   # Find zlib
   find_package(ZLIB)
   if (ZLIB_FOUND)
      list(APPEND PLATFORM_LIBS ${ZLIB_LIBRARIES})
      include_directories(SYSTEM ${ZLIB_INCLUDE_DIRS})
   else (ZLIB_FOUND)
      message(FATAL_ERROR "Can't find zlib library")
   endif(ZLIB_FOUND)

   # Find Allegro
   find_package(Allegro5)

   if (ALLEGRO5_FOUND)
      list(APPEND GUI_LIBS ${ALLEGRO5_LIBRARIES})
      include_directories(SYSTEM ${ALLEGRO5_INCLUDE_DIRS})
   else (ALLEGRO5_FOUND)
      message(FATAL_ERROR "Can't find Allegro library")
   endif(ALLEGRO5_FOUND)

   # Find Allegro addons
   find_package(Allegro5Main)
   if (ALLEGRO5MAIN_FOUND)
      list(APPEND GUI_LIBS ${ALLEGRO5MAIN_LIBRARIES})
      include_directories(SYSTEM ${ALLEGRO5MAIN_INCLUDE_DIRS})
   else (ALLEGRO5MAIN_FOUND)
      message(FATAL_ERROR "Can't find Allegro main addon")
   endif(ALLEGRO5MAIN_FOUND)

   find_package(Allegro5Primitives)
   if (ALLEGRO5PRIMITIVES_FOUND)
      list(APPEND GUI_LIBS ${ALLEGRO5PRIMITIVES_LIBRARIES})
      include_directories(SYSTEM ${ALLEGRO5PRIMITIVES_INCLUDE_DIRS})
   else (ALLEGRO5PRIMITIVES_FOUND)
      message(FATAL_ERROR "Can't find Allegro Primitives addon")
   endif(ALLEGRO5PRIMITIVES_FOUND)

   find_package(Allegro5Font)
   if (ALLEGRO5FONT_FOUND)
      list(APPEND GUI_LIBS ${ALLEGRO5FONT_LIBRARIES})
      include_directories(SYSTEM ${ALLEGRO5FONT_INCLUDE_DIRS})
   else (ALLEGRO5FONT_FOUND)
      message(FATAL_ERROR "Can't find Allegro font addon")
   endif(ALLEGRO5FONT_FOUND)

   find_package(Allegro5TTF)
   if (ALLEGRO5TTF_FOUND)
      list(APPEND GUI_LIBS ${ALLEGRO5TTF_LIBRARIES})
      include_directories(SYSTEM ${ALLEGRO5TTF_INCLUDE_DIRS})
   else (ALLEGRO5TTF_FOUND)
      message(FATAL_ERROR "Can't find Allegro TTF addon")
   endif(ALLEGRO5TTF_FOUND)

   find_package(Allegro5Image)
   if (ALLEGRO5IMAGE_FOUND)
      list(APPEND GUI_LIBS ${ALLEGRO5IMAGE_LIBRARIES})
      include_directories(SYSTEM ${ALLEGRO5IMAGE_INCLUDE_DIRS})
   else (ALLEGRO5IMAGE_FOUND)
      message(FATAL_ERROR "Can't find Allegro image addon")
   endif(ALLEGRO5IMAGE_FOUND)

   find_package(Allegro5Dialog)
   if (ALLEGRO5DIALOG_FOUND)
      list(APPEND GUI_LIBS ${ALLEGRO5DIALOG_LIBRARIES})
      include_directories(SYSTEM ${ALLEGRO5DIALOG_INCLUDE_DIRS})
   else (ALLEGRO5DIALOG_FOUND)
      message(FATAL_ERROR "Can't find Allegro DIALOG addon")
   endif(ALLEGRO5DIALOG_FOUND)
endif(WANT_GUI)

# code source files
include(FileList)

# Make a link library out of the source files
if(WANT_DYNAMIC)
   add_library ("jazz" SHARED ${JAZZ_SRC_FILES})
else(WANT_DYNAMIC)
   add_library ("jazz" STATIC ${JAZZ_SRC_FILES})
endif(WANT_DYNAMIC)

# Main executable
if(WANT_XBOARD)
   add_executable ("xbjazz" src/interface/xboard.c)
   target_link_libraries("xbjazz" jazz)
endif(WANT_XBOARD)

if(WANT_UCI)
   add_executable ("ucijazz" src/interface/uci.c)
   target_link_libraries("ucijazz" jazz)
endif(WANT_UCI)

if(WANT_TEST)
   add_executable ("test_jazz" src/interface/test_jazz.c)
   target_link_libraries("test_jazz" jazz)
   add_executable ("cjazz" src/interface/cjazz.c)
   target_link_libraries("cjazz" jazz)
   add_executable ("perftest" src/interface/perftest.c)
   target_link_libraries("perftest" jazz)
endif(WANT_TEST)

if(WANT_GUI)
   add_executable ("a5jazz" src/interface/a5jazz.c)
   target_link_libraries("a5jazz" jazz)
   target_link_libraries("a5jazz" ${GUI_LIBS})
endif(WANT_GUI)


