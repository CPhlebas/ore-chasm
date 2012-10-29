# - Try to find Chipmunk
# Once done this will define
#  CHIPMUNK_FOUND - System has Chipmunk
#  CHIPMUNK_INCLUDE_DIRS - The Chipmunk include directories
#  CHIPMUNK_LIBRARIES - The libraries needed to use Chipmunk
#  CHIPMUNK_DEFINITIONS - Compiler switches required for using Chipmunk

find_path(CHIPMUNK_INCLUDE_DIR chipmunk/chipmunk.h
    HINTS ${CHIPMUNK_INCLUDE_HINTS})

find_library(CHIPMUNK_LIBRARY NAMES chipmunk
    HINTS ${CHIPMUNK_LIBRARY_HINTS})

set(CHIPMUNK_LIBRARIES ${CHIPMUNK_LIBRARY})
set(CHIPMUNK_INCLUDE_DIRS ${CHIPMUNK_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set CHIPMUNK_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Chipmunk DEFAULT_MSG
    CHIPMUNK_LIBRARY
    CHIPMUNK_INCLUDE_DIR
)

mark_as_advanced(CHIPMUNK_INCLUDE_DIR CHIPMUNK_LIBRARY)

