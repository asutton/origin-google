// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;

constexpr bool True_func() { return true; }
constexpr bool False_func() { return false; }

int main()
{
  static_assert(All(true, true, true), "");
  static_assert(!All(true, true, false), "");

  // Check with type traits
  static_assert(All(True_type{}), "");
  static_assert(All(True_func()), "");
  static_assert(!All(False_func()), "");
  static_assert(!All(False_type{}), "");

  // All is true when there are no arguments.
  static_assert(All(), "");
}
