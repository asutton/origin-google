// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/type/typestr.hpp>
#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

int main()
{
  // TODO: Check common type traits and concepts.
  //
  // Note that most common type traits (e.g,. Size_type, Difference_type)
  // don't actually matter unless you're doing weird stuff with an Allocator.
  // But nobody (rightfully) does weird stuff with allocators.
  using M = matrix<int, 1>;
  static_assert(Same<Size_type<M>, std::size_t>(), "");
}
