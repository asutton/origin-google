// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/data/optional/optional.hpp>
#include <origin/data/optional/testing.hpp>

using namespace std;
using namespace origin;
using namespace origin::testing;

int main()
{
  context cxt;

  using T = optional<int>;


  // A default initialized optional is not initialized.
  T x;
  assert(!x);

  // An initialized optional is.
  T y = 1;
  assert(y);

  // Properties
  static_assert(Same<Common_type<T, int>, T>(), "");
  static_assert(Output_streamable<T>(), "");

  static_assert(Same<Value_type<T>, int>(), "");
  static_assert(Same<Reference_of<T>, int&>(), "");
  static_assert(Same<Reference_of<const T>, const int&>(), "");
  static_assert(Same<Pointer_of<T>, int*>(), "");
  static_assert(Same<Pointer_of<const T>, const int*>(), "");

  check_ordered<T>();
  check_equality_comparable<T, int>();
  check_equality_comparable<T, nullptr_t>();
  check_totally_ordered<T, int>();
  check_totally_ordered<T, nullptr_t>();

  return cxt.failures();
}
