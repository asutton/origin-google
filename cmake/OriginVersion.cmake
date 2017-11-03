# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

## Parse a version string, creating a number of variables describing its
## contents.
##
##   parse_vsersion(version)
##
## where prefix is the prefix of the output variables and version is the version
## string (which need not be quoted). The version string must be of the form
## a.b.c where a, b, and c are the major version, minor version, and patch
## level, respectively.
##
## This sets the following macros:
##
##    ORIGIN_CURRENT_VERSION       -- The version string
##    ORIGIN_CURRENT_VERSION_MAJOR -- The major version number
##    ORIGIN_CURRENT_VERSION_MINOR -- The minor version number
##    ORIGIN_CURRENT_VERSION_PATCH -- The patch number
##
macro(parse_version version)
  string(REPLACE "." " " space ${version})
  separate_arguments(nums UNIX_COMMAND ${space})
  list(GET nums 0 ORIGIN_CURRENT_VERSION_MAJOR)
  list(GET nums 1 ORIGIN_CURRENT_VERSION_MINOR)
  list(GET nums 2 ORIGIN_CURRENT_VERSION_PATCH)
  set(ORIGIN_CURRENT_VERSION ${version})
endmacro()
