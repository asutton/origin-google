// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;

// FIXME: Why doesn't this work? For some reason GCC is not deducing the 
// template argument T.

template <typename T>
  void f(Identity<T>&& x)
  { }

struct s { };

int main()
{
  // s x;
  // f(x);

  assert(false);
}
