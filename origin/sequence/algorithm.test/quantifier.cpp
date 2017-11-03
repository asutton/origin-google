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

bool odd(int n) { return n & 1; }

void
check_all_match()
{
  vector<int> v {0, 0, 0};
  assert(all_match(v, 0));
  assert(all_match(v, 0, equal_to<int>{}));
}

void
check_any_match()
{
  vector<int> v {0, 1, 0};
  assert(any_match(v, 1));
  assert(any_match(v, 1, equal_to<int>{}));
}

void
check_none_match()
{
  vector<int> v {0, 0, 0};
  assert(none_match(v, 1));
  assert(none_match(v, 1, equal_to<int>{}));
}

int 
main()
{
  using V = vector<int>;
  V v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  assert(all_of(v, odd) == all_of(v.begin(), v.end(), odd));
  assert(any_of(v, odd) == any_of(v.begin(), v.end(), odd));
  assert(none_of(v, odd) == none_of(v.begin(), v.end(), odd));

  check_all_match();
  check_any_match();
  check_none_match();
}
