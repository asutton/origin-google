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

bool on(bool x) { return x; }

int main()
{
  using V = vector<int>;
  V v0 = {1, 1, 1, 0, 0, 0};

  assert(is_partitioned(v0, on) == is_partitioned(v0.begin(), v0.end(), on));


  V v1 = {1, 0, 1, 0, 1, 0};
  V v2 = v1;
  partition(v1, on);
  partition(v2.begin(), v2.end(), on);
  assert(v1 == v2);

  // Reset the input arguments.
  random_shuffle(v1);
  v2 = v1;
  stable_partition(v1, on);
  stable_partition(v2.begin(), v2.end(), on);
  assert(v1 == v2);

  // Reset again and create some output buffers.
  V vt1(v1.size());
  V vt2(v1.size());
  V vf1(v1.size());
  V vf2(v1.size());
  random_shuffle(v1);
  auto p1 = partition_copy(v1, vt1, vf1, on);
  auto p2 = partition_copy(v1.begin(), v1.end(), vt2.begin(), vf2.begin(), on);
  assert(equal(vt1.begin(), p1.first, vt2.begin()));
  assert(equal(vf1.begin(), p1.second, vf2.begin()));

  auto i1 = partition_point(v0, on);
  auto i2 = partition_point(v0.begin(), v0.end(), on);
  assert(i1 == i2);
}
