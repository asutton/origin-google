# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# Turn on testing.
enable_testing()


# Turning this option off will cause any components named "testing" to be
# omitted from libraries. It is generally useful to export testing components
# in debug builds (but not release builds).
option(ORIGIN_EXPORT_TESTING "Include testing components in libraries" ON)


# Turning this option off will cause the unit tests not to be compiled
# with the main libraries.
option(ORIGIN_BUILD_TESTS "Build test suite" ON)


if(${ORIGIN_BUILD_TESTS})
  if(NOT ${ORIGIN_EXPORT_TESTING})
    message(FATAL_ERROR "Cannot build test suite without testing components.")
  endif()
endif()
