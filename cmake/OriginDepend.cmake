# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# Explicitly declare a dependence on an Origin library. This differs from using
# the DEPENDS in the origin_project macro since it does not establish a
# top-level dependency. This can be used to establish weak dependencies, e.g.,
# in test directories.
#
# This simply adds the include directories for each given module to the search
# path for the current project.
#
# origin_depend(Args...)
macro(origin_depend)
    foreach(arg ${ARGN})
        set(inc ${ORIGIN_ROOT}/${arg}/include)
        list(APPEND incs ${inc})
    endforeach()
    include_directories(${incs})
endmacro()

