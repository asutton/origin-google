# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# Set up the build to work with CPack to create binary distributions. Basically,
# we have to define a bunch of variables prior to including the CPack module.

# There are actually TWO packaging commands: one that makes a binary package and
# one that makes a source The source package is basically a rendering of the
# project directory, whereas the binary package is constructed off install
# targets.

# Basic CPack configuration stuff.
set(CPACK_PACKAGE_NAME "origin")
set(CPACK_PACKAGE_VERSION_MAJOR ${ORIGIN_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${ORIGIN_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${ORIGIN_VERSION_PATCH})
set(CPACK_PACKAGE_VENDOR "Software Development Laboratory <SDML>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Origin C++0x Libraries")

# Configure the source package
# TODO: This is kind of broken. We should /not/ be including the included HTML
# and XML with
set(CPACK_SOURCE_PACKAGE_FILE_NAME "origin-${ORIGIN_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "build/" ".svn/" "/html/$")
set(CPACK_SOURCE_GENERATOR TBZ2)

# TODO Configure the binary package. The binary package is kind of a misnomer
# since we really don't have any binary components (yet), so there's really
# nothing to install. However, I would still like to be able to generate a
# installation of just the header files... which seems to be somewhat difficult.
# set(CPACK_GENERATOR TBZ2)

# This macro just includes the CPack module, which actually does all of the
# hard work in terms of defining the installation. This must be invoked LAST.
macro(origin_package)
    include(CPack)
endmacro(origin_package)