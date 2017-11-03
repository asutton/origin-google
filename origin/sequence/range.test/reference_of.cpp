// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <iterator>
#include <vector>

#include <origin/sequence/range.hpp>
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

// Check that the Reference_of can be applied to all kinds of ranges.

int main()
{
  using V = vector<int>;

  static_assert(Same<Reference_of<V>, V::reference>(), "");
  static_assert(Same<Reference_of<const V>, V::const_reference>(), "");

  using I = istream_iterator<int>;
  using B = bounded_range<I>;
  static_assert(Same<Reference_of<B>, const int&>(), "");
}
