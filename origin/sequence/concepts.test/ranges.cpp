// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <forward_list>
#include <list>
#include <string>
#include <vector>

#include <origin/sequence/concepts.hpp>
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

// Test the Iterator_of trait. It needs to give the correct iterator type
// based on the const-ness of the its argument.

void check_iterator()
{
  using V = vector<int>;
  static_assert(Same<Iterator_of<V>, V::iterator>(), "");
  static_assert(Same<Iterator_of<const V>, V::const_iterator>(), "");
}

// Test that Range<R> is valid even when we're forwarding reference types.
template <typename R>
  void check_range(R&& range)
  {
    static_assert(Range<R>(), "");
  }


int main()
{
  check_iterator();

  // static_assert(Range<int[3]>(), "");
  static_assert(Range<vector<int>>(), "");
  static_assert(Range<list<int>>(), "");
  static_assert(Range<forward_list<int>>(), "");
  static_assert(Range<string>(), "");


  // Check that the concept works when forwarded.
  int a[3];
  check_range(a);

  vector<int> v;
  const vector<int>& cv = v;
  check_range(v);
  check_range(cv);

  list<int> l;
  const list<int>& cl = l;
  check_range(l);
  check_range(cl);

  forward_list<int> f;
  const forward_list<int> cf = f;
  check_range(f);
  check_range(cf);
}
