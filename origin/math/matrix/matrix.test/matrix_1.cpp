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

// Tests for 1D use of matrices:

void test_init()
{
  // Value initialization
  matrix<int, 1> m1 { 1, 2, 3, 4 };
  assert(m1.extent(0) == 4);
  assert(m1.size() == 4);
  cout << m1 << '\n';

  // Check initialization patterns for 1D matrices (vectors).
  // Using parens calls the size initializer.
  matrix<int, 1> m2(5);
  assert(m2.extent(0) == 5);
  cout << m2 << '\n';

  // Make sure that we can be completely ambiguous and still get the right
  // thing. That is, 3 is interpreted as a dimension and not a value.
  matrix<std::size_t, 1> m3(3);
  assert(m3.extent(0) == 3);
  cout << m3 << '\n';

  // Using parens *always* is always a bounds initializer.
  matrix<std::size_t, 1> m4(2ul);
  cout << m4 << '\n';

  // Using braces is always a value initializer.
  // This should emit a deleted function error since it would require 
  // narrowing (the deleted constructor is selected).
  // matrix<std::size_t, 1> m5{1, 2, 3};

  // But this needs to be ok since we have an exact match for value
  // initialization.
  matrix<std::size_t, 1> m6{1ul, 2ul};
  cout << m6 << '\n';
}

void test_access()
{
  // Test element access.
  matrix<int, 1> m {0, 1, 2, 3};
  assert(m(0) == 0);

  // Test slicing. Note that slices in 1D are proxies.
  assert(m[0] == 0);
  assert(m.row(0) == 0);

  // Iterators increase monotonically.
  auto i = m.begin();
  int n = 0;
  for ( ; i != m.end(); ++i, ++n)
    assert(*i == n);
}

void test_slice()
{
  cout << "--- slice ---\n";
  matrix<int, 1> m {0, 1, 2, 3, 4, 5};

  auto s1 = m(2);
  assert(s1 == 2);

  // FIXME: Actually implement these checks.

  auto s2 = m(slice(1, 3));
  cout << s2 << '\n'; // 1 2 3
  
  auto s3 = m(slice(1, 5)); // Check at the boundary.
  cout << s3 << '\n'; // 1 2 3 4 5

  // Some identities
  assert(m(slice(0)) == m);
  assert(m(slice::all) == m);

  cout << "----------\n";
}

void test_ops()
{
  // Test operations
  matrix<int, 1> m {0, 1, 2, 3};
  m[0] = 1;
  assert(m(0) == 1);

  m[0] += 1;
  assert(m(0) == 2);

  m[0] -= 1;
  assert(m(0) == 1);

  m[0] *= 10;
  assert(m(0) == 10);

  m[0] /= 5;
  assert(m(0) == 2);

  m[0] %= 2;
  assert(m(0) == 0);

  matrix<int, 1> m2 {4, 3, 2, 1};
  m += m2; // 4, 4, 4, 4
  cout << m << '\n';
  m -= m2; // 0, 1, 2, 3
  cout << m << '\n';
}


int main()
{
  test_init();
  test_access();
  test_slice();
  test_ops();


  matrix<int, 1> m {0, 1, 2, 3, 4, 5, 6 };

  for (int i = 0; i != 7; ++i) {
    // assert(&m.s(i) == m.data() + i);
  }

  m(0);
  m(slice(1, 2));

}
