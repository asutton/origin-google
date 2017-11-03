
// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include "testing.hpp"

namespace origin
{
  namespace testing
  {
    ////////////////////////////////////////////////////////////////////////////
    // Context Implementation

    // The context instance.
    context* context::inst = nullptr;

    context::context()
      : prng(), os(&std::cerr), repeat(100), fail(0)
    {
      assert(!inst);
      inst = this;
    }

    context::~context()
    {
      inst = nullptr;
    }

    context&
    context::instance()
    {
      assert(inst);
      return *inst;
    }

  } // namespace tessting
} // namespace origin


