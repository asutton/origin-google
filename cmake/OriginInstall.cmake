# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# This module is responsible for various installation information. Generally,
# we just bootstrap the install mechanism off of the CMake install stuff.

# Set up some installation prefix variables..
set(ORIGIN_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(ORIGIN_INCLUDE_PREFIX ${ORIGIN_INSTALL_PREFIX}/include)
set(ORIGIN_LIBRARY_PREFIX ${ORIGIN_INSTALL_PREFIX}/lib)
set(ORIGIN_BINARY_PREFIX ${ORIGIN_INSTALL_PREFIX}/bin)

# The package version denotes the primary "evolution" of the Origin libraries
# and only includes the major and minor version. This is used to build the
# header installation and shared library versioning sysetms and allows us to
# distribute patch versions without upsetting the overall hierarchy scheme.
set(ORIGIN_INSTALL_VERSION
    "${ORIGIN_VERSION_MAJOR}.${ORIGIN_VERSION_MINOR}")

# The origin directories are generally installed in ${prefix}/include/origin.
# Note that the origin part is automatically taken from install path.
set(ORIGIN_INSTALL_HEADER_DIR ${ORIGIN_INCLUDE_PREFIX})

# Install the headers of a library poject. This will essentially performs a
# recursive copy on the include/origin module, placing the headers into the
# correct path.
# TODO: Allow the use of compoents... We can actually add a COMPONENT argument
# to install() that "tags" the module. This would let us select the origin
# components that the user wanted to install. Documentation here.
# http://www.itk.org/Wiki/CMake:Component_Install_With_CPack
macro(origin_install_headers)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/origin
    DESTINATION ${ORIGIN_INSTALL_HEADER_DIR}
    PATTERN ".svn" EXCLUDE)
endmacro(origin_install_headers)
