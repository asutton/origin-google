# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.



# This module contains macros used to build performance testing targets.
# Each performance test is hosted in a different directory (usually as a
# subdirectory of tests/perf), and contains one or more programs that are
# compiled and run (using 'make performance') to produce and compile results.
#
# Note that a full system for compiling performance test results is still
# being worked out.


# Tools used to analyze and graph results

set(ORIGIN_PLOT_COMP_RATIO ${ORIGIN_ROOT}/tools/)


# Build a head-to-head performance comparison for two different source code
# files implementing similar tests.
#
#   origin_perf_comparison(target COMPARE file1 file2 [REPEAT n])
#
# If REPEAT is not given, the default value is 1. For head-to-head performance
# tests, that isn't generally a good idea.
#
# TODO: Support arguments for annotating the resulting documents or selecting
# the output format. Currently, we generate pdf, but generating png might be
# useful for web reporting. Or build commands that generate generate all such
# outputs and build a single target that intgrates the performance reporting.
#
# FIXME: I will need to change the call mechanism when I get the performance
# tester's argument parsing in better shape. Repetitions will probably be
# passed as an option (-r or --repeat).
macro(origin_perf_comparison id)
  # Parse the command line arguments.
  parse_arguments(parsed "COMPARE;REPEAT" "" ${ARGN})
  set(files ${parsed_COMPARE})
  if(parsed_REPEAT)
    set(repeat ${parsed_REPEAT})
  else()
    set(repeat 1)
  endif()

  set(bin ${CMAKE_CURRENT_BINARY_DIR})
  set(main "perf_${id}")

  set(results)
  foreach(i ${files})
    get_filename_component(name ${i} NAME_WE)
    set(tgt ${main}_${name})

    # Create a target for the test program.
    origin_executable(${tgt} ${i})
    target_link_libraries(${tgt} origin)

    # And create a command that will generate its output.
    # FIXME: This probably won't work in a non-unix environment or
    # anywhere where executable targets have a non-empty suffix.
    set(exe ${bin}/${tgt})
    set(txt ${exe}.txt)
    add_custom_command(
      OUTPUT ${txt}
      COMMAND ${exe} ${repeat} > ${txt}
      DEPENDS ${tgt})

    # Add the output text to the list of dependencies that this
    # performance comparison will depend upon.
    list(APPEND results ${txt})
  endforeach()

  # Generate a command to synthesize the output into a graph of some kind.
  set(pdf ${main}.pdf)
  add_custom_command(
    OUTPUT ${pdf}
    COMMAND ${ORIGIN_PLOT_COMP_RATIO}.py -o ${pdf} ${results}
    DEPENDS ${results})

  # Build a target for this performance comparison
  add_custom_target(${main} DEPENDS ${pdf})

  # Register this performance test as a dependency of the perf target.
  add_dependencies(perform ${main})

endmacro()
