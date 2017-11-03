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

template <typename T>
  using Pair_of = pair<T, T>;

int main()
{
  using V = vector<int>;
  V v {0, 1, 2, 3, 4};
  const V& cv = v;

  std::less<int> lt;

  // NOTE: Checking addresses is the identity test. That's only possible when
  // v's reference is actually a reference.

  // Min
  static_assert(Same<decltype(min(v)), V::reference>(), "");
  static_assert(Same<decltype(min(cv)), V::const_reference>(), "");
  assert(&min(v) == &*min_element(v.begin(), v.end()));

  static_assert(Same<decltype(min(v, lt)), V::reference>(), "");
  static_assert(Same<decltype(min(cv, lt)), V::const_reference>(), "");
  assert(&min(v, lt) == &*min_element(v.begin(), v.end(), lt));


  // Max
  static_assert(Same<decltype(max(v)), V::reference>(), "");
  static_assert(Same<decltype(max(cv)), V::const_reference>(), "");
  assert(&max(v) == &*max_element(v.begin(), v.end()));

  static_assert(Same<decltype(max(v, lt)), V::reference>(), "");
  static_assert(Same<decltype(max(cv, lt)), V::const_reference>(), "");
  assert(&max(v, lt) == &*max_element(v.begin(), v.end(), lt));


  // Minmax
  static_assert(Same<decltype(minmax(v)), Pair_of<V::reference>>(), "");
  static_assert(Same<decltype(minmax(cv)), Pair_of<V::const_reference>>(), "");
  auto p1 = minmax(v);
  auto p2 = minmax_element(v.begin(), v.end());
  assert(&p1.first == &*p2.first);
  assert(&p1.second == &*p2.second);

  static_assert(Same<decltype(minmax(v, lt)), Pair_of<V::reference>>(), "");
  static_assert(Same<decltype(minmax(cv, lt)), Pair_of<V::const_reference>>(), "");
  auto p3 = minmax(v, lt);
  auto p4 = minmax_element(v.begin(), v.end(), lt);
  assert(&p3.first == &*p4.first);
  assert(&p3.second == &*p4.second);


  // Min element
  static_assert(Same<decltype(min_element(v)), V::iterator>(), "");
  static_assert(Same<decltype(min_element(cv)), V::const_iterator>(), "");
  assert(min_element(v) == min_element(v.begin(), v.end()));

  static_assert(Same<decltype(min_element(v, lt)), V::iterator>(), "");
  static_assert(Same<decltype(min_element(cv, lt)), V::const_iterator>(), "");
  assert(min_element(v, lt) == min_element(v.begin(), v.end(), lt));


  // Max element
  static_assert(Same<decltype(max_element(v)), V::iterator>(), "");
  static_assert(Same<decltype(max_element(cv)), V::const_iterator>(), "");
  assert(max_element(v) == max_element(v.begin(), v.end()));

  static_assert(Same<decltype(max_element(v, lt)), V::iterator>(), "");
  static_assert(Same<decltype(max_element(cv, lt)), V::const_iterator>(), "");
  assert(max_element(v, lt) == max_element(v.begin(), v.end(), lt));


  // Minmax element
  static_assert(Same<decltype(minmax_element(v)), Pair_of<V::iterator>>(), "");
  static_assert(Same<decltype(minmax_element(cv)), Pair_of<V::const_iterator>>(), "");
  assert(minmax_element(v) == minmax_element(v.begin(), v.end()));
  
  static_assert(Same<decltype(minmax_element(v, lt)), Pair_of<V::iterator>>(), "");
  static_assert(Same<decltype(minmax_element(cv, lt)), Pair_of<V::const_iterator>>(), "");
  assert(minmax_element(v, lt) == minmax_element(v.begin(), v.end(), lt));
}
