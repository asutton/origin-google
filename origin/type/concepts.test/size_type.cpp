// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <vector>

#include <origin/type/concepts.hpp>
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

struct fail { };

int main()
{
  // Check the value type for a container
  using V = vector<int>;
  static_assert(Has_size_type<V>(), "");
  static_assert(Same<Size_type<V>, std::size_t>(), "");

  // Value type must see through references.
  static_assert(Same<Size_type<V&>, std::size_t>(), "");
  static_assert(Same<Size_type<V&&>, std::size_t>(), "");
  static_assert(Same<Size_type<const V&>, std::size_t>(), "");

  // Some types don't have value types.
  static_assert(!Has_size_type<fail>(), "");
  static_assert(!Has_size_type<int*>(), "");
}

