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

bool odd(int n) { return n & 1; }

int main()
{
  using V = vector<int>;
  
  V v1 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  V v2(v1.size());
  V v3(v1.size());

  copy(v1, v2);
  copy(v1.begin(), v1.end(), v3.begin());
  assert(v2 == v3);

  V v4(v1.size() / 2);
  V v5(v1.size() / 2);
  copy_if(v1, v4, odd);
  copy_if(v1.begin(), v1.end(), v5.begin(), odd);
  assert(v4 == v5);
}
