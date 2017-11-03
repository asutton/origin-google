# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# This module is responsible for the configuration of the build system with
# respect to the local environment.

# Is GMP installed?
find_package(GMP)
if (GMP_FOUND)
	option(ORIGIN_USE_GMP "Enable the use of GMP" ON)
endif()

# Is MPFR installed?
find_package(MPFR)
if (MPFR_FOUND)
	option(ORIGIN_USE_MPFR "Enable the use of MPFR" ON)
endif()


# Define a Profile build mode.
# TODO: Support for profiling is not very good right now. This should be
# improved substantially.
# set(CMAKE_C_FLAGS_PROFILE "-pg" CACHE STRING "Profile")
# set(CMAKE_CXX_FLAGS_PROFILE "-pg" CACHE STRING "Profile")


# FIXME: I'm going to have to add -I's, -L's and -l's to some of the build
# configurations. We can't use GMP and MPFR without linking against them.

# Build the set of CPP definitions.
set(ORIGIN_INCLUDE_DIRS "")
set(ORIGIN_LIBRARY_DIRS "")
set(ORIGIN_LIBRARIES)
set(ORIGIN_DEFINES "")
if (ORIGIN_USE_GMP)
	set(ORIGIN_INCLUDE_DIRS "${ORIGIN_INCLUDE_DIRS} -I${GMP_INCLUDE_DIR}")
        set(ORIGIN_LIBRARY_DIRS "${ORIGIN_LIBRARY_DIRS} -I${GMP_LIBRARY_DIR}")
	list(APPEND ORIGIN_LIBRARIES gmp)
	set(ORIGIN_DEFINES "${ORIGIN_DEFINES} -DORIGIN_USE_GMP")

        include_directories(${GMP_INCLUDE_DIR})
        link_directories(${GMP_LIBRARY_DIR})
endif()
if (ORIGIN_USE_MPFR)
	set(ORIGIN_INCLUDE_DIRS "${ORIGIN_INCLUDE_DIRS} -I${MPFR_INCLUDE_DIR}")
        set(ORIGIN_LIBRARY_DIRS "${ORIGIN_LIBRARY_DIRS} -I${MPFR_LIBRARY_DIR}")
	list(APPEND ORIGIN_LIBRARIES mpfr)
	set(ORIGIN_DEFINES "${ORIGIN_DEFINES} -DORIGIN_USE_MPFR")

        include_directories(${MPFR_INCLUDE_DIR})
        link_directories(${MPFR_LIBRARY_DIR})
endif()


# Every origin executable must be linked against the Origin binary.
# FIXME: This probably needs to be tied into the dependency stuff. For example,
# if Graph depends on Foo (and Foo has a binary part), then all Graph 
# executables should link against origin-foo.a (or.so or .dll).
list(APPEND ORIGIN_LIBRARIES origin)


# Build a set of warning flags.
set(ORIGIN_WARNINGS "-Wall -Wno-unused-variable -Wno-unused-value")

# Build the entire set of flags
set(ORIGIN_CXX_FLAGS "${ORIGIN_DEFINES} ${ORIGIN_WARNINGS} -std=c++11")

# Compile using these settings
set(CMAKE_CXX_FLAGS ${ORIGIN_CXX_FLAGS})
