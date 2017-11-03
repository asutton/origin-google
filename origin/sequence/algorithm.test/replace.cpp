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
bool neg(int n) { return n < 0; }

int main()
{
  using V = vector<int>;
  
  V v1 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  V v2 = v1;

  replace(v1, 0, 1);
  replace(v2.begin(), v2.end(), 0, 1);
  assert(v1 == v2);

  replace_if(v1, odd, 0);
  replace_if(v2.begin(), v2.end(), odd, 0);
  assert(v1 == v2);

  V v3(v1.size());
  V v4(v1.size());
  
  replace_copy(v1, v3, 0, -1);
  replace_copy(v1.begin(), v1.end(), v4.begin(), 0, -1);
  assert(v3 == v4);

  replace_copy_if(v1, v3, neg, 0);
  replace_copy_if(v1.begin(), v1.end(), v4.begin(), neg, 0);
  assert(v3 == v4);
}
