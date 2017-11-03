// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <string>

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;

struct base { };
struct derived : base { };

int main()
{
  // Non-class types cannot be derived.
  static_assert(!Derived<int, int>(), "");

  // Derived is reflexive.
  static_assert(Derived<base, base>(), "");
  
  // Derivation defines a subtype relation.
  static_assert(Derived<derived, base>(), "");
  static_assert(!Derived<base, derived>(), "");

  // TODO: Write tests for multiple and virtual inheritance.
}
