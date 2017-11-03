// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/sequence/algorithm.hpp>

using namespace std;
using namespace origin;


int main()
{
  using V = vector<int>;
  
  V v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto f = v.begin();
  auto l = v.end();

  const V& cv = v;
  auto cf = cv.begin();
  auto cl = cv.end();

  V sub = {7, 8, 9};

  std::equal_to<int> eq;

  static_assert(Same<decltype(search(v, sub)), V::iterator>(), "");
  static_assert(Same<decltype(search(cv, sub)), V::const_iterator>(), "");
  assert(search(v, sub) == search(f, l, sub.begin(), sub.end()));
  assert(search(cv, sub) == search(cf, cl, sub.begin(), sub.end()));
  
  static_assert(Same<decltype(search_n(v, 1, 5)), V::iterator>(), "");
  static_assert(Same<decltype(search_n(cv, 1, 5)), V::const_iterator>(), "");
  assert(search_n(v, 1, 5) == search_n(f, l, 1, 5));
  assert(search_n(cv, 1, 5) == search_n(cf, cl, 1, 5));
}
