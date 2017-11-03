// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>

#include <origin/type/testing.hpp>

using namespace std;
using namespace origin;
using namespace origin::testing;

template <typename T>
  struct foo 
  {
    static_assert(Integer<T>(), "");
  };

int main()
{
  foo<float>* p = nullptr;
}
