// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/type/type.hpp>
#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

int main()
{
  // Create some test matrices.
  matrix<int, 2> m0 = {
    {0, 1, 2},
    {3, 4, 5}
  };
  matrix<int, 2> m1 = m0;
  matrix<int, 2> m2 = m1;
  matrix<int, 2> m3 = m2;

  // Make m2 and m3 different from m1.
  next_permutation(m2.begin(), m2.end());
  next_permutation(m3.begin(), m3.end());

  cout << m1 << '\n';
  cout << m2 << '\n';
  cout << m3 << '\n';

  // Create a sub-matrix bound to m1.
  matrix_ref<int, 2> sm1 = m1;
  matrix_ref<int, 2> sm2 = m3;
  assert(sm1 == m1);


  // Assignment to a matrix reads from the original, overwriting the original.
  matrix<int, 2> m4;
  m4 = m3;
  assert(m4 == m3);

  // The behavior is the same for sub-matrices.
  matrix<int, 2> m5;
  m5 = sm1;
  assert(m5 == sm1);
  assert(m5 == m1);


  // Assignment through a sub-matrix writes through the sub-matrix.
  sm1 = m2;
  assert(m1 == m2);

  sm1 = m3;
  assert(m1 == m3);

  sm1 = m0;
  assert(m1 == m0);
}

