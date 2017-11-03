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

  V v0 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  V v1 = v0;
  V v2 = v0;

  auto i1 = remove(v1, 0);
  auto i2 = remove(v2.begin(), v2.end(), 0);
  assert(equal(v1.begin(), i1, v2.begin()));;

  // Reset the inputs
  v1 = v0;
  v2 = v0;

  i1 = remove_if(v1, odd);
  i2 = remove_if(v2.begin(), v2.end(), odd);
  assert(equal(v1.begin(), i1, v2.begin()));;

  v1 = v0;
  v2 = v0;
  V v3(v1.size());
  V v4(v2.size());
  
  // FIXME: We should probably be asserting some property of the result
  // type here (i.e., as related to count?).
  remove_copy(v1, v3, 3);
  remove_copy(v1.begin(), v1.end(), v4.begin(), 3);
  assert(v3 == v4);

  remove_copy_if(v1, v3, odd);
  remove_copy_if(v1.begin(), v1.end(), v4.begin(), odd);
  assert(v3 == v4);
}
