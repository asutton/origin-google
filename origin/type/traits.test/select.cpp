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
  static_assert(Same<Select<0, char, short, int, long>, char>(), "");
  static_assert(Same<Select<1, char, short, int, long>, short>(), "");
  static_assert(Same<Select<2, char, short, int, long>, int>(), "");
  static_assert(Same<Select<3, char, short, int, long>, long>(), "");


  // Ill-formed program!
  // using X = Select<2, char, short>;
}
