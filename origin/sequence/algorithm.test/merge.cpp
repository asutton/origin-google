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
  V v1 {0, 1, 2};
  V v2 {3, 4, 5};
  std::less<int> lt;

  V v3(v1.size() + v2.size());
  V v4(v1.size() + v2.size());

  merge(v1, v2, v3);
  merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin());
  assert(v3 == v4);

  merge(v1, v2, v3, lt);
  merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v4.begin(), lt);
  assert(v3 == v4);
}
