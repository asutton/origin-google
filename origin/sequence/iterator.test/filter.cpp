// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/sequence/iterator.hpp>

using namespace std;
using namespace origin;

bool odd(int n) { return n & 1; }

int main()
{
  using V = vector<int>;
  V v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  auto first = make_filter(v.begin(), v.end(), odd);
  auto last = make_filter(v.end(), odd);
  for ( ; first != last; ++first)
    cout << *first << '\n';

}
