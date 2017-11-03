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

// A type that is definitely not equality comparable.
struct fail { };

int main()
{
  // Basic checks for equality comparability.
  static_assert(Equality_comparable<int>(), "");
  static_assert(!Equality_comparable<fail>(), "");

  // Check cross-type equality comparable concepts.
  static_assert(Equality_comparable<char, int>(), "");
  static_assert(Equality_comparable<string, const char*>(), "");
  static_assert(!Equality_comparable<string, int>(), "");
}

