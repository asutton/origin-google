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

enum {
  a = 10, b, c, d, e, f, g, h, i, j ,k, l
};

int main()
{
  using M = matrix<int, 2>;
  matrix<M, 2> m { // 3x2
    { // row 0
      {{1, 2}, {3, 4}}, // col 0
      {{4, 5}, {6, 7}}, // col 1
    },
    { // row 1
      {{8, 9}, {a, b}}, // col 0
      {{c, d}, {e, f}}, // col 1
    },
    { // row 2
      {{1, 2}, {3, 4}}, // col 0
      {{4, 5}, {6, 7}}, // col 1
    },
  };
  assert(m.extent(0) == 3);
  assert(m.extent(1) == 2);
  assert(m.size() == 6);

  cout << m(0, 0) << '\n';
  cout << m(0, 1) << '\n';
  cout << m(1, 0) << '\n';
  cout << m(1, 1) << '\n';
  cout << m(2, 0) << '\n';
  cout << m(2, 1) << '\n';
  cout << '\n';
  // cout << m << '\n';
}
