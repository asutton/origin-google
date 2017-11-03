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


int main()
{
  context cxt;

  // Equality comparable
  check_equality_comparable<short>();
  check_equality_comparable<int, long>();
  // check_equality_comparable<string, const char*>(cxt);  

  // Weakly ordered 
  check_weakly_ordered<int>();
  check_weakly_ordered<int, long>();
  // check_weakly_ordered<string, const char*>(cxt);

  // Totally ordered
  check_totally_ordered<int>();
  check_totally_ordered<int, long>();
  // check_totally_ordered<string, const char*>(cxt);


  // Move
  check_move_constructible<int>();
  check_movable<int>();

  // Copy
  check_copy_constructible<int>();
  check_copyable<int>();

  // Default constructible.
  check_default_constructible<int>();


  // Semiregular and regular
  check_semiregular<int>();
  check_regular<int>();

  return cxt.failures();
}
