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

  random_shuffle(v1);
  assert(range_is_permutation(v1, v0));

  random_shuffle(v1, [](size_t n) { return rand() % n; });
  assert(range_is_permutation(v1, v0));

  std::minstd_rand prng;
  shuffle(v1, prng);
  assert(range_is_permutation(v1, v0));
}
