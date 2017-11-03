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

  V v0 {1, 2, 3, 4, 5};
  V v1 = v0;
  V v2 = v0;

  reverse(v1);
  reverse(v2.begin(), v2.end());
  assert(v1 == v2);

  v1 = v0;
  V v3(v1.size());
  V v4(v2.size());
  reverse_copy(v1, v3);
  reverse_copy(v1.begin(), v1.end(), v4.begin());
  assert(v3 == v4);
}
