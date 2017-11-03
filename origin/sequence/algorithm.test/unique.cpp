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

  V v0 {0, 0, 1, 1, 2, 2};
  V v1 = v0;
  V v2 = v0;
  std::equal_to<int> eq;

  // FIXME: We should be asserting some property ralated to the resulting
  // iterator.
  auto i1 = unique(v1);
  auto i2 = unique(v2.begin(), v2.end());
  assert(equal(v1.begin(), i1, v2.begin()));;

  // Reset the inputs
  v1 = v0;
  v2 = v0;

  i1 = unique(v1, eq);
  i2 = unique(v2.begin(), v2.end(), eq);
  assert(equal(v1.begin(), i1, v2.begin()));;

  // Reset again, and create some output buffers.
  v1 = v0;
  V v3(v1.size());
  V v4(v2.size());
  
  // FIXME: We should probably be asserting some property of the result value.
  range_unique_copy(v1, v3);
  unique_copy(v1.begin(), v1.end(), v4.begin());
  assert(v3 == v4);

  range_unique_copy(v1, v3, eq);
  unique_copy(v1.begin(), v1.end(), v4.begin(), eq);
  assert(v3 == v4);
}
