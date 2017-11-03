// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

// Tests for 2D use of matrices:

int main()
{
  // Purely for the purpose of initializing matrices with 24 elements.
  enum Extended_numbers {
    a = 10,
    b = 11, 
    c = 12, 
    d = 13, 
    e = 14, 
    f = 15, 
    g = 16, 
    h = 17, 
    i = 18, 
    j = 19, 
    k = 20, 
    l = 21, 
    m = 22, 
    n = 23, 
    o = 24
  };

  // This is a 3x4x2 matrix (3 rows, 4 columns, and 2 elements in each "z"
  // cell). There are 24 elements in this table.
  matrix<int, 3> m1 {
    {{0, 1}, {2, 3}, {4, 5}, {6, 7}},
    {{8, 9}, {a, b}, {c, d}, {e, f}},
    {{h, i}, {j, k}, {l, m}, {n, o}}
  };
  assert(m1.extent(0) == 3);
  assert(m1.extent(1) == 4);
  assert(m1.extent(2) == 2);
  assert(m1.size() == 24);
  cout << m1 << '\n';

  // Using parens calls the size initializer
  matrix<int, 3> m2(5, 4, 3);
  assert(m2.extent(0) == 5);
  assert(m2.extent(1) == 4);
  assert(m2.extent(2) == 3);
  assert(m2.size() == 5 * 4 * 3);
  cout << m2 << '\n';

  // Cannot value-initialize a matrix from a flat list. We can't deduce the
  // extents of the matrix.
  // matrix<int, 3> m3{1, 2, 3};

  // Must be non-jagged.
  // matrix<int, 3> m3 { // This must fail.
  //   {
  //     {{1, 2}, {1, 2}},
  //     {{1},    {1}}
  //   }
  // };
}
