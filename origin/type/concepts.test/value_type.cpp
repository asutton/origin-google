// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>

#include <origin/type/concepts.hpp>

using namespace std;
using namespace origin;

struct fail { };

int main()
{
  // Check the value type for a container
  using V = vector<int>;
  static_assert(Has_value_type<V>(), "");
  static_assert(Same<Value_type<V>, int>(), "");

  // Value type must see through references.
  static_assert(Same<Value_type<V&>, int>(), "");
  static_assert(Same<Value_type<V&&>, int>(), "");
  static_assert(Same<Value_type<const V&>, int>(), "");


  // Some types don't have value types.
  static_assert(!Has_value_type<fail>(), "");

  
  // Check the value type for pointers.
  static_assert(Has_value_type<int*>(), "");
  static_assert(Same<Value_type<int*>, int>(), "");

  // The value types are the same modulo cv-qualifiers.
  static_assert(Has_value_type<int const*>(), "");
  static_assert(Same<Value_type<int const*>, int>(), "");
}

