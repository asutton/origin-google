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
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

bool odd(int n) { return n & 1; }


void test_find_next()
{
  // find_next if is defined in terms of find.
  // f != l ? find_next(f, l, x) <=> find(next(f), l, x) : l;
  int a[] {0, 1, 0};
  assert(find_next(a, a, 0) == a);
  assert(find_next(a, a + 3, 0) == a + 2);
}

void test_find_next_if()
{
  // find_next_if is defined in terms of find.
  // f != l ? find_next_if(f, l, p) <=> find(next(f), l, p) : l;
  int a[] {1, 0, 1};
  assert(find_next_if(a, a, odd) == a);
  assert(find_next_if(a, a + 3, odd) == a + 2);
}

int main()
{
  using V = vector<int>;

  V v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto f = v.begin();
  auto l = v.end();

  const V& cv = v;
  auto cf = cv.begin();
  auto cl = cv.end();

  V sub {7, 8, 9};

  std::equal_to<int> eq;


  // Check find
  static_assert(Same<decltype(find(v, 0)), V::iterator>(), "");
  static_assert(Same<decltype(find(cv, 0)), V::const_iterator>(), "");
  assert(find(v, 5) == find(f, l, 5));

  // Check find_if
  static_assert(Same<decltype(find_if(v, 0)), V::iterator>(), "");
  static_assert(Same<decltype(find_if(cv, 0)), V::const_iterator>(), "");
  assert(find_if(v, odd) == find_if(f, l, odd));


  // Check find_if_not
  static_assert(Same<decltype(find_if_not(v, odd)), V::iterator>(), "");
  static_assert(Same<decltype(find_if_not(cv, odd)), V::const_iterator>(), "");
  assert(find_if_not(v, odd) == find_if_not(f, l, odd));

  // Check find_end
  static_assert(Same<decltype(find_end(v, sub)), V::iterator>(), "");
  static_assert(Same<decltype(find_end(cv, sub)), V::const_iterator>(), "");
  static_assert(Same<decltype(find_end(v, sub, eq)), V::iterator>(), "");
  static_assert(Same<decltype(find_end(cv, sub, eq)), V::const_iterator>(), "");
  assert(find_end(v, sub) == find_end(f, l, sub.begin(), sub.end()));
  assert(find_end(v, sub) == find_end(f, l, sub.begin(), sub.end(), eq));

  // Check adjacent find.
  static_assert(Same<decltype(adjacent_find(v)), V::iterator>(), "");
  static_assert(Same<decltype(adjacent_find(cv)), V::const_iterator>(), "");
  static_assert(Same<decltype(adjacent_find(v, eq)), V::iterator>(), "");
  static_assert(Same<decltype(adjacent_find(cv, eq)), V::const_iterator>(), "");
  assert(adjacent_find(v) == adjacent_find(f, l));
  assert(adjacent_find(v, eq) == adjacent_find(f, l, eq));
}
