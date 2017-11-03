// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <vector>

#include <origin/memory/concepts.hpp>
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

int main()
{
  using A = std::allocator<int>;

  static_assert(Has_value_type<A>(), "");
  static_assert(Same<Value_type<A>, int>(), "");

  static_assert(Same<Pointer_of<A>, int*>(), "");
  static_assert(Same<Pointer_of<const A>, const int*>(), "");

  static_assert(Same<Reference_of<A>, int&>(), "");
  static_assert(Same<Reference_of<const A>, const int&>(), "");

  // I'm not sure if these are required to be std::size_t and std::ptrdiff_t
  // or not. Probably they are for std::allocator.
  static_assert(Has_size_type<A>(), "");
  static_assert(Has_difference_type<A>(), "");

  static_assert(Allocator<A>(), "");

  // Check the allocator constructor.
  using V = std::vector<int>;
  static_assert(Allocator_constructible<V>(), "");           // default
  static_assert(Allocator_constructible<V, V&&>(), "");      // move
  static_assert(Allocator_constructible<V, const V&>(), ""); // copy
}
