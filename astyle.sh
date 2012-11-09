#!/bin/bash

# apply kdelibs coding style to all c, cpp and header files in and below the current directory
#
# the coding style is defined in http://techbase.kde.org/Policies/Kdelibs_Coding_Style
#
# requirements: installed astyle

astyle --indent=spaces=4 --brackets=linux \
      --indent-labels --pad-oper --unpad-paren --pad-header \
      --keep-one-line-statements --convert-tabs \
      --indent-preprocessor --suffix=none \
      `find $1 -type f -name '*.c' -or -name '*.cpp' -or -name '*.cc' -or -name '*.h'`
