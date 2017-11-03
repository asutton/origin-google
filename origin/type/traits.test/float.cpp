// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;

int main()
{
  static_assert(Floating_point<float>(), "");
  static_assert(Floating_point<double>(), "");
  static_assert(Floating_point<long double>(), "");

  // Some things are not floating point types.
  static_assert(!Floating_point<int>(), "");
}
