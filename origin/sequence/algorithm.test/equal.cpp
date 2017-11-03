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
  
  V v1 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto f1 = v1.begin();
  auto l1 = v1.end();

  V v2 {0, 1, 2, 3, 5, 6, 7, 8, 9};
  auto f2 = v2.begin();
  // auto l2 = v2.end();

  std::equal_to<int> eq;

  assert(range_equal(v1, v2) == equal(f1, l1, f2));
  assert(range_equal(v1, v2, eq) == equal(f1, l1, f2, eq));
}
