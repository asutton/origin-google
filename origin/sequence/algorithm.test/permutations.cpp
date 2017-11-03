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
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      cout << x << ' ';
    cout << '\n';
  }

int main()
{
  using V = vector<int>;
  
  V v0 {0, 1, 2};
  V v1 {0, 1, 2};
  auto f1 = v1.begin();
  auto l1 = v1.end();

  V v2 {0, 1, 2};
  auto f2 = v2.begin();
  // auto l2 = v2.end();

  std::equal_to<int> eq;
  std::less<int> lt;

  assert(range_is_permutation(v1, v2) == is_permutation(f1, l1, f2));
  assert(range_is_permutation(v1, v2, eq) == is_permutation(f1, l1, f2, eq));


  // Next permutation
  while (1) {
    auto x = next_permutation(v1);
    auto y = next_permutation(v2.begin(), v2.end());
    assert(x == y && v1 == v2);
    if (!x)
      break;
  }


  // Prev permtuation
  reverse(v1);
  v2 = v1;
  while (1) {
    auto x = prev_permutation(v1);
    auto y = prev_permutation(v2.begin(), v2.end());
    assert(x == y && v1 == v2);
    if (!x)
      break;
  }


  // Next permutation (lt)
  reverse(v1);
  v2 = v1;
  while (1) {
    auto x = next_permutation(v1, lt);
    auto y = next_permutation(v2.begin(), v2.end(), lt);
    assert(x == y && v1 == v2);
    if (!x)
      break;
  }


  // Prev permtuation
  reverse(v1);
  v2 = v1;
  while (1) {
    auto x = prev_permutation(v1, lt);
    auto y = prev_permutation(v2.begin(), v2.end(), lt);
    assert(x == y && v1 == v2);
    if (!x)
      break;
  }
}
