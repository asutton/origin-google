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


// Variadic transformed

int main()
{
  using V = vector<int>;
  
  V v1 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  V v2 = v1;
  V v3(v1.size());
  V v4 = v3;

  std::negate<int> neg;
  std::plus<int> add;

  // Unary transform
  range_transform(v1, v3, neg);
  transform(v1.begin(), v1.end(), v4.begin(), neg);
  assert(v3 == v4);

  // Binary transform
  range_transform(v1, v2, v3, add);
  transform(v1.begin(), v1.end(), v2.begin(), v4.begin(), add);
  assert(v3 == v4);
}
