// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <string>

#include <origin/type/concepts.hpp>

using namespace std;
using namespace origin;

// A type that is not totally ordered.
struct fail { };

int main()
{
  // Basic checks for totoal ordering
  static_assert(Totally_ordered<int>(), "");
  static_assert(!Totally_ordered<fail>(), "");

  // Cross-type checks for total ordering.
  static_assert(Totally_ordered<char, int>(), "");
  static_assert(Totally_ordered<string, const char*>(), "");
  static_assert(!Totally_ordered<string, int>(), "");
}

