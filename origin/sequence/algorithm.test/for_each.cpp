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

struct counter
{
  counter(int& n) : num(n) { }
  
  void operator()(int n)
  {
    ++num;
  }

  bool operator==(const counter& x) const
  {
    return num == x.num;
  }

  int& num;
};

struct doubler
{
  void operator()(int& n) const
  {
    n *= 2;
  }
};


int main()
{
  using V = vector<int>;
  V v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const V& cv = v;

  // Test for_each with non-modifying functions
  int n1 = 0, n2 = 0;
  counter c1(n1), c2(n2);
  assert(for_each(cv, c1) == for_each(cv.begin(), cv.end(), c2));
  assert(for_each(v, c1) == for_each(v.begin(), v.end(), c2));

  // Test with modifications
  V v2 = v;
  for_each(v, doubler {});
  for_each(v2, doubler {});
  assert(v == v2);
}
