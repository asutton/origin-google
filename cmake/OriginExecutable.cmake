# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# Declare a new executable program. This is currently a simple wrapper around
# the add_executable macro.
#
# origin_executable(progname [Args...])
#
# See the CMake documentation for add_executable() arguments.
macro(origin_executable progname)
    # Register the executable as before.
    add_executable(${progname} ${ARGN})
    target_link_libraries(${progname} ${ORIGIN_LIBRARIES})
endmacro()

