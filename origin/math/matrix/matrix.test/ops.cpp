// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

template <typename M1, typename M2>
Common_type<Value_type<M1>, Value_type<M2>>
dot_product(const M1& a, const M2& b)
{
  static_assert(a.order == 1, "");
  static_assert(b.order == 1, "");
  assert(a.size() == b.size());
  return inner_product(a.begin(), a.end(), b.begin(), size_t(0));
}

// Implement multiplication using slices.
template <typename T, typename U>
matrix<Common_type<T, U>, 2>
multiply(const matrix<T, 2>& a, const matrix<U, 2>& b)
{
  assert(a.cols() == b.rows());
  matrix<Common_type<T, U>, 2> r(a.rows(), b.cols());
  for (size_t i = 0; i < a.rows(); ++i)
    for (size_t j = 0; j < b.cols(); ++j)
      r(i, j) = dot_product(a.row(i), b.col(j));
  return r;
}


int main()
{
  matrix<int, 2> a {
    {1, 0, -2},
    {0, 3, -1}
  };
  matrix<int, 2> b {
    {0, 3},
    {-2, -1},
    {0, 4}
  };
  cout << multiply(a, b) << '\n';

  {
    matrix<int,2> m1 = {{1,2,3}, {4,5,6}};
    matrix<int,2> m2 = {{1,2}, {3,4}, {5,6}};
    cout << m1 * m2 << '\n';
  }
}
