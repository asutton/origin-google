# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

## Declare the Origin distribution. This must only be used in the top-level
## CMake file. Use origin_library for everything else.
##
##   origin_distribution(VERSION x.y.z)
macro(origin_distribution)
  parse_arguments(parsed "VERSION" "" ${ARGV})
  parse_version(ORIGIN ${parsed_VERSION})
  set(ORIGIN_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
endmacro()


## Declare a new Origin component.
##
## origin_component(name
##                  [DEPENDS dep1 dep2 ...]
##                  [AUTHORS author1 author2 ...]
##                  [MAINTAINERS maintainer1 maintainer2 ...]
##                  [DESCRIPTION description])
##
## This macro sets the following variables that describe the library.
##
## The ORIGIN_${name}_ROOT variable gives the path to the root directory
## of the described project. This macro currently assumes a fixed, modular
## layout.
##
## The ORIGIN_${name}_INCLUDE_DIR variable gives the path to the primary
## include directory for the libaray.
##
## The ORIGIN_${name}_INCLUDE_DIRS variable is a list of all the include
## directories computed from the dependencies given in the macro. If empty,
## this will only include the include directory (above).
##
##
## TODO: There's probably a lot more work that could be done describing library
## projects. For example, we should be able list several source, test, or include
## directories.
macro(origin_component name)
    parse_arguments(
      parsed
      "VERSION;DEPENDS;AUTHORS;MAINTAINERS;DESCRIPTION"
      ""
      ${ARGN})

    # Generate upper/lowercase versions of the library name.
    string(TOUPPER ${name} upper_name)
    string(TOLOWER ${name} lower_name)

    # Define some variables that are useful locally...
    set(ORIGIN_CURRENT_PROJECT ${lower_name})
    string(TOUPPER "ORIGIN_${name}" prefix)

    # Parse out the version string from the component.
    parse_version(${prefix} ${parsed_VERSION})

    # The _ROOT points to the root directory of this project (which happens to
    # be the current source directory).
    set(${prefix}_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

    # The _INCLUDE_DIR points to the comnponents include path.
    set(${prefix}_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

    # Enumerate dependencies on other libraries. For now, this will translate
    # the dependent libraries into path names that are required in order for
    # the build to succeed. For now, we're just assuming that we can find them
    # in a trunk relative to the source (i.e. root/dep). In other words, we're
    # relying on a fixed directory structure rather than trying to be
    # flexible.
    # FIXME: This doesn't really work as well as I'd like it to. What if you
    # want to re-target the dependency (i.e,. tags/core-1.2) or something.
    # set("ORIGIN_${prefix}_INCLUDE_DIRS")
    set(include_dirs "${prefix}_INCLUDE_DIRS")
    if(parsed_DEPENDS)
        # Define a globally available variable that stores the dependencies
        # for the library.
        set(deps "${prefix}_DEPENDS")
        set(${deps} ${parsed_DEPENDS})
        foreach(dep ${${deps}})
            set(dep_root ${ORIGIN_ROOT}/${dep})
            set(dep_inc ${dep_root}/include)
            list(APPEND incs ${dep_inc})
        endforeach()
        set(${include_dirs} ${incs})
    endif()

    # Set include_dirs to the this libraries include directory plus those of
    # the configured dependencies. Also, automatically register these include
    # directories for the project.
    include_directories(${${include_dirs}} ${${prefix}_INCLUDE_DIR})
endmacro()
