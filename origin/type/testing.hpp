// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_TESTING_HPP
#define ORIGIN_TYPE_TESTING_HPP

#include <functional>
#include <limits>
#include <random>

#include "type.hpp"

namespace origin
{
  namespace testing
  {
    // A type used to force ADL lookups in this namespace. 
    struct adl_t { };
  
  } // namespace testing

  //////////////////////////////////////////////////////////////////////////////
  // Testing
  //
  // This library contains features supporting the writing of unit tests
  // against components in this library. In particular, this library includes
  // archetypes for standard concepts (including the core framework), and
  // testable properties for those concepts' axioms.
  //////////////////////////////////////////////////////////////////////////////


// Support for randomized tessting.
#include "testing.impl/random.hpp"

// The basic testing context and support functions.
#include "testing.impl/context.hpp"

// Include test support for the type library.
#include "testing.impl/properties.hpp"
#include "testing.impl/concepts.hpp"

} // namespace origin


#endif
