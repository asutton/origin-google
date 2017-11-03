// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <memory>
#include <string>
#include <vector>

#include <origin/type/concepts.hpp>

using namespace std;
using namespace origin;

int main()
{
  // Regular types are copyable.
  static_assert(Copyable<int>(), "");
  static_assert(Copyable<string>(), "");
  static_assert(Copyable<vector<int>>(), "");
  
  
  // Resources are not copyable
  static_assert(!Copyable<unique_ptr<int>>(), "");

  // Function objects are generally copyable.
  static_assert(Copyable<std::less<int>>(), "");
  
  // Lambda expressions are copy constructible, but not assignable.
  auto f = [](){ return; };
  using F = decltype(f);
  static_assert(Copy_constructible<F>(), "");
  static_assert(!Copy_assignable<F>(), "");
  static_assert(!Copyable<F>(), "");
}

