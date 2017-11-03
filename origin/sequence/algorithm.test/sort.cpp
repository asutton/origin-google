// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>

#include <origin/sequence/algorithm.hpp>

using namespace std;
using namespace origin;

// FIXME: Move these properties into the testing header so they can be exported
// as part of the testing library.


// Check that the range-based is_sorted algorithm is equivalent to the 
// iterator-based algorithm. The property can check the following properties:
//
//    is_sorted(range)       <=> is_sorted(begin(range), end(range))
//    is_sorted(range, comp) <=> is_sorted(begin(range), end(range), comp)
struct sorted_range
{
  template <typename R>
    bool operator()(const R& range) const
    {
      using std::begin;
      using std::end;
      auto first = begin(range);
      auto last = end(range);
      return is_sorted(range) == std::is_sorted(first, last);
    }

  template <typename R, typename C>
    bool operator()(const R& range, C comp) const
    {
      using std::begin;
      using std::end;
      auto first = begin(range);
      auto last = end(range);
      return is_sorted(range, comp) == std::is_sorted(first, last, comp);
    }
};

// Check that the range-based is_sorted_until algorithm is equivalent to the 
// iterator-based algorithm. The property can check the following properties:
//
//    is_sorted_until(range)       <=> is_sorted_until(begin(range), end(range))
//    is_sorted_utnil(range, comp) <=> is_sorted_until(begin(range), end(range), comp)
struct partially_sorted_range
{
  template <typename R>
    bool operator()(const R& range) const
    {
      using std::begin;
      using std::end;
      auto first = begin(range);
      auto last = end(range);
      return is_sorted_until(range) == std::is_sorted_until(first, last);
    }

  template <typename R, typename C>
    bool operator()(const R& range, C comp) const
    {
      using std::begin;
      using std::end;
      auto first = begin(range);
      auto last = end(range);
      return is_sorted_until(range, comp) == std::is_sorted_until(first, last, comp);
    }
};


int main()
{
  using V = vector<int>;
  V v1 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  V v2 = v1;
  std::less<int> lt;

  // It doesn't matter if vn is sorted or not. We only care that it compiles
  // and produces equivalent results.
  sort(v1);
  sort(v2.begin(), v2.end());
  assert(v1 == v2);

  sort(v1, lt);
  sort(v2.begin(), v2.end(), lt);
  assert(v1 == v2);


  stable_sort(v1);
  stable_sort(v2.begin(), v2.end());
  assert(v1 == v2);

  stable_sort(v1, lt);
  stable_sort(v2.begin(), v2.end(), lt);
  assert(v1 == v2);


  V v3(v1.size() / 2);
  V v4(v1.size() / 2);
  partial_sort_copy(v1, v3);
  partial_sort_copy(v1.begin(), v1.end(), v4.begin(), v4.end());
  assert(v3 == v4);

  partial_sort_copy(v1, v3, lt);
  partial_sort_copy(v1.begin(), v1.end(), v4.begin(), v4.end(), lt);
  assert(v3 == v4);

  assert(is_sorted(v1) == is_sorted(v1.begin(), v1.end()));
  assert(is_sorted(v1, lt) == is_sorted(v1.begin(), v1.end(), lt));

  assert(is_sorted(v1) == is_sorted(v1.begin(), v1.end()));
  assert(is_sorted(v1, lt) == is_sorted(v1.begin(), v1.end(), lt));

  assert(is_sorted_until(v1) == is_sorted_until(v1.begin(), v1.end()));
  assert(is_sorted_until(v1, lt) == is_sorted_until(v1.begin(), v1.end(), lt));
}
