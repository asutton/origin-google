# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.


## Define an Origin module in the current directory.
##
##    origin_module(
##      VERSION version 
##      AUTHORS authors
##      IMPORT libs
##      EXPORT comps
##
##      TEST comp)
##
## A module is...
##
## The list of IMPORTS gives the set of Origin modules which this module 
## depends. Each imported module results in a physical dependency for the 
## constructed binary.
##
## The list of EXPORTS is a list of files in the module directory without
## their corresponding extensions (e.g. foo bar baz). Each export defines
## three associated components: foo.hpp (the interface), foo.cpp (the
## implementation), and foo.test.cpp (a unit test). These files are required
## for every export.
##
## The TEST command builds a target specifically for testing. An associated
## testing target is produced separately from the main library. The component
## name following the TEST label is the name of the component that implements
## the testing library. Note that not all libraries need to define a TEST
## library. These are typically associated with libraries that define concepts.
##
## This defines a number of macros that are used to configure the build of
## the module, its test suite, and its documentation.
##
##    ORIGIN_CURRENT_MODULE         -- The current module name (all lower case)
##    ORIGIN_CURRENT_LIBRARY_TARGET -- The binary built by the current mdoule
## 
macro(origin_module)

  parse_arguments(
    parsed
    "VERSION;AUTHORS;IMPORT;EXPORT"
    ""
    ${ARGN})

  # Compute the module name by finding the relative path to the current
  # module directory and pre-pending "origin". 
  if(ORIGIN_OUT_OF_TRUNK)
    # If we're out of the trunk, then pretend that the current source dir
    # would be a module hanging off of the module root (i.e., origin.fake).
    file(RELATIVE_PATH path ${ORIGIN_FAKE_ROOT} ${CMAKE_CURRENT_SOURCE_DIR})
    if(NOT path)
      get_filename_component(path ${ORIGIN_FAKE_ROOT} NAME)
    endif()
    message(${path})
  else()
    file(RELATIVE_PATH path ${ORIGIN_MODULE_ROOT} ${CMAKE_CURRENT_SOURCE_DIR})
  endif()
  string(REPLACE "/" "." path ${path})
  set(module origin.${path})

  # Define some variables that are useful locally...
  set(ORIGIN_CURRENT_MODULE ${module})

  # Parse out the version string from the component.
  #
  # FIXME: There's probably some other stuff I could do here. For example,
  # I might generate version.hpp/version.cpp for the component that includes
  # the version numbers and the author/maintainer data.
  parse_version(${parsed_VERSION})


  # Set the current library target. This is the binary component that
  # corresponds to this module.
  set(ORIGIN_CURRENT_LIBRARY_TARGET ${module})


  # Check the arguments and do some other processing on these lists. Namely,
  # create the variables ORIGIN_CURRENT_IMPORTS and ORIGIN_CURRENT_EXPORTS
  # for future processong.
  check_imports(${parsed_IMPORT})
  check_exports(${parsed_EXPORT})


  # Build the main library
  build_component()

  # Build the test suite targets
  if(${ORIGIN_BUILD_TESTS})
    build_test_suite(${parsed_EXPORT})
  endif()
endmacro()



# Verify that the imports are valid and create a variable that stores
# the list.
#
# FIXME: Can we accept imports like gmp, mpfr, 
macro(check_imports)
  # FIXME: Is there nothing we can do here?
  set(ORIGIN_CURRENT_IMPORTS ${ARGV})
endmacro()



# Verify that each export has the required components!
macro(check_exports)
  # Compute the list of exported properties. We may filter some components
  # based on the configuration (e.g., testing).
  set(ORIGIN_CURRENT_EXPORTS)
  foreach(i ${ARGV})
    if(${i} STREQUAL "testing")
      if(${ORIGIN_EXPORT_TESTING})
        list(APPEND ORIGIN_CURRENT_EXPORTS ${i})
      endif()
    else()
      list(APPEND ORIGIN_CURRENT_EXPORTS ${i})
    endif()
  endforeach()

  # Do a number of things with the list of exports. For example, generate the
  # list of source files.
  foreach(i ${ARGV})
    # FIXME: Actually verify these things. Issue build warnings if the required
    # components do not exist. We could even refuse to build if we're creating
    # a distro!
  endforeach()
endmacro()




# Save the list of imports and create a dependency of the current library
# to the dependent target.
macro(build_component)
  # Generate a target for the for the module.
  #
  # FIXME: For nested modules, the name can't simply be origin_xxx it has
  # to be fully scoped to the origin root (e.g., origin_math_matrix).
  #

  # Translate the exports into the source files that will be incorporated
  # into the build.
  set(src)
  foreach(i ${ORIGIN_CURRENT_EXPORTS})
    list(APPEND src ${i}.cpp)
  endforeach()

  # Build the library...
  add_library(${ORIGIN_CURRENT_LIBRARY_TARGET} STATIC ${src})
  
  # And link our dependencies.
  foreach(i ${ORIGIN_CURRENT_IMPORTS})
    target_link_libraries(${target} ${i})
  endforeach()
endmacro()




# Generate a unit test for each file in ${export}.test
macro(build_test_suite)
  foreach(i ${ARGV})
    build_component_test(${i})
  endforeach()
endmacro()

# For each exported test, generate a unit test for each file in the
# corresponding test directory. Note that the corresponding test directory
# must exist!
macro(build_component_test exp)
  # Find the set of test files
  file(GLOB files ${exp}.test/*.cpp)
  foreach(i ${files})
    build_unit_test(${exp} ${i})
  endforeach()
endmacro()


# Build a test for the file
macro(build_unit_test exp file)
  # Get the name of the test and generate target names for the exe and te
  # test
  get_filename_component(name ${file} NAME_WE)
  set(exe ${ORIGIN_CURRENT_MODULE}.${exp}.${name})
  set(test ${exe}.test)

  # Generate the executable in a export-specific build directory.
  # Note that we don't have to do this, but it keeps the binary dirs from
  # becoming polluted with lots of test programs.
  set(out ${CMAKE_CURRENT_BINARY_DIR}/${exp})

  # Generate the executable target.
  add_executable(${exe} ${file})
  set_target_properties(${exe} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${out})
  link_imports(${exe} ${ORIGIN_CURRENT_MODULE})
  
  # Generate the test. Be sure to point to the correct executable!
  add_test(${test} ${out}/${exe})
endmacro()



# Link the imported modules to the specied target. Additional dependencies
# may be specified after the target.
macro(link_imports target)
  # Link the current library against each imported requirement.
  foreach (i ${ORIGIN_CURRENT_IMPORTS} ${ARGN})
    target_link_libraries(${target} ${i})
  endforeach()
endmacro()

