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
  V v1 {0, 1, 2};
  V v2 {3, 4, 5};

  auto f1 = v1.begin(), l1 = v1.end();
  auto f2 = v2.begin(), l2 = v2.end();

  std::less<int> lt;

  assert(includes(v1, v2) == includes(f1, l1, f2, l2));

  V v3(v1.size() + v2.size());
  V v4(v1.size() + v2.size());

  set_union(v1, v2, v3);
  set_union(f1, l1, f2, l2, v4.begin());
  assert(v3 == v4);

  auto i1 = set_union(v1, v2, v3, lt);
  auto i2 = set_union(f1, l1, f2, l2, v4.begin(), lt);
  assert(equal(v3.begin(), i1, v4.begin()));


  i1 = set_intersection(v1, v2, v3);
  i2 = set_intersection(f1, l1, f2, l2, v4.begin());
  assert(equal(v3.begin(), i1, v4.begin()));

  i1 = set_intersection(v1, v2, v3, lt);
  i2 = set_intersection(f1, l1, f2, l2, v4.begin(), lt);
  assert(equal(v3.begin(), i1, v4.begin()));


  i1 = set_difference(v1, v2, v3);
  i2 = set_difference(f1, l1, f2, l2, v4.begin());
  assert(equal(v3.begin(), i1, v4.begin()));

  i1 = set_difference(v1, v2, v3, lt);
  i2 = set_difference(f1, l1, f2, l2, v4.begin(), lt);
  assert(equal(v3.begin(), i1, v4.begin()));


  i1 = set_symmetric_difference(v1, v2, v3);
  i2 = set_symmetric_difference(f1, l1, f2, l2, v4.begin());
  assert(equal(v3.begin(), i1, v4.begin()));

  i1 = set_symmetric_difference(v1, v2, v3, lt);
  i2 = set_symmetric_difference(f1, l1, f2, l2, v4.begin(), lt);
  assert(equal(v3.begin(), i1, v4.begin()));
}
