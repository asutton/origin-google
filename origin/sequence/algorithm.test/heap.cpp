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

template <typename V>
  void check_push(V& v1, V& v2, Value_type<V> n)
  {
    v1.push_back(n);
    push_heap(v1);

    v2.push_back(n);
    push_heap(v2.begin(), v2.end());

    assert(v1 == v2);
  }

template <typename V, typename C>
  void check_push(V& v1, V& v2, Value_type<V> n, C comp)
  {
    v1.push_back(n);
    push_heap(v1, comp);

    v2.push_back(n);
    push_heap(v2.begin(), v2.end(), comp);

    assert(v1 == v2);
  }


template <typename V>
  void check_pop(V& v1, V& v2)
  {
    pop_heap(v1);
    v1.pop_back();

    pop_heap(v2);
    v2.pop_back();

    assert(v1 == v2);
  }

template <typename V, typename C>
  void check_pop(V& v1, V& v2, C comp)
  {
    pop_heap(v1, comp);
    v1.pop_back();

    pop_heap(v2.begin(), v2.end(), comp);
    v2.pop_back();

    assert(v1 == v2);
  }

int main()
{
  using V = vector<int>;
  V v1;
  V v2;
  std::less<int> lt;

  // Check push and pop
  check_push(v1, v2, 0);
  check_pop(v1, v2);

  // Check push and pop with comp
  check_push(v1, v2, 1, lt);
  check_pop(v1, v2, lt);


  v1 = {0, 1, 2, 3, 4};
  v2 = v1;

  // Check make, is, and sort
  make_heap(v1);
  make_heap(v2.begin(), v2.end());
  assert(v1 == v2);

  assert(is_heap(v1) == is_heap(v1.begin(), v1.end()));
  assert(is_heap_until(v1) == is_heap_until(v1.begin(), v1.end()));

  sort_heap(v1);
  sort_heap(v2.begin(), v2.end());
  assert(v1 == v2);

  
  // Check make, is, and sort with comp
  make_heap(v1, lt);
  make_heap(v2.begin(), v2.end(), lt);
  assert(v1 == v2);

  // assert(is_heap(v1, lt) == is_heap(v1.begin(), v1.end(), lt));
  // assert(is_heap_until(v1, lt) == is_heap_until(v1.begin(), v1.end(), lt));

  sort_heap(v1, lt);
  sort_heap(v2.begin(), v2.end(), lt);
  assert(v1 == v2);
}
