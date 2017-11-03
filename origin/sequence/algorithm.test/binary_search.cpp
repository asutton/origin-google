// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
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

  std::less<int> lt;

  assert(lower_bound(v, 5) == lower_bound(f, l, 5));
  assert(lower_bound(v, 5, lt) == lower_bound(f, l, 5, lt));

  assert(upper_bound(v, 5) == upper_bound(f, l, 5));
  assert(upper_bound(v, 5, lt) == upper_bound(f, l, 5, lt));

  assert(equal_range(v, 5) == equal_range(f, l, 5));
  assert(equal_range(v, 5, lt) == equal_range(f, l, 5, lt));

  assert(binary_search(v, 5) == binary_search(f, l, 5));
  assert(binary_search(v, 5, lt) == binary_search(f, l, 5, lt));

}
