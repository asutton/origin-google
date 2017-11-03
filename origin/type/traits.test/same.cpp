// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;


int main()
{
  static_assert(Same<int, int>(), "");
  static_assert(Same<const int, const int>(), "");
  static_assert(Same<volatile int, volatile int>(), "");
  static_assert(Same<const volatile int, const volatile int>(), "");
  
  static_assert(Same<int&, int&>(), "");
  static_assert(Same<const int&, const int&>(), "");
  static_assert(Same<volatile int&, volatile int&>(), "");
  static_assert(Same<const volatile int&, const volatile int&>(), "");

  static_assert(Same<int&&, int&&>(), "");
  static_assert(Same<const int&&, const int&&>(), "");
  static_assert(Same<volatile int&&, volatile int&&>(), "");
  static_assert(Same<const volatile int&&, const volatile int&&>(), "");

  static_assert(Same<int*, int*>(), "");
  static_assert(Same<const int*, const int*>(), "");
  static_assert(Same<volatile int*, volatile int*>(), "");
  static_assert(Same<const volatile int*, const volatile int*>(), "");

  // Same with aliases.
  using X = int;
  static_assert(Same<int, X>(), "");
  static_assert(Same<const int, const X>(), "");
  static_assert(Same<int&, X&>(), "");
  static_assert(Same<int&&, X&&>(), "");
  static_assert(Same<int*, X*>(), "");

  // Same with lots of arguments
  static_assert(Same<int, int, int, int, int>(), "");
  static_assert(!Same<int, int, int&, int>(), "");

  // Different
  static_assert(Different<int, const int>(), "");
  static_assert(Different<const int, int>(), "");

  static_assert(Different<int, int&>(), "");
  static_assert(Different<int&, int>(), "");

  static_assert(Different<int, int&&>(), "");
  static_assert(Different<int&&, int>(), "");

  static_assert(Different<int, int*>(), "");
  static_assert(Different<int*, int>(), "");

  // Test the expander.
  using List = type_list<int, int, int>;
  static_assert(Same<Expand<List>>(), "");
  static_assert(!Different<Expand<List>>(), "");

  using Tuple = std::tuple<int, int, int>;
  static_assert(Same<Expand<Tuple>>(), "");
  static_assert(!Different<Expand<Tuple>>(), "");
}
