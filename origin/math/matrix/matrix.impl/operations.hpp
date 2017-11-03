// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif

// ------------------------------------------------------------------------ //
//                          Matrix Operations
//
// The following operations are defined for all Matrix types.
//
// TODO: Some of these algorithms should be moved into a more general
// purpose linear algebra library.



// Returns the number of rows in a matrix with rank > 1. The number of rows
// is the same as the extent in the 1st dimension.
template <typename M>
  inline auto rows(const M& m) -> decltype(m.extent(0))
  {
    static_assert(0 < M::order, "");
    return m.extent(0);
  }


// Returns the number of columns in a matrix with rank > 1. This number of
// columns is the same as the extent in the 2nd dimension.
template <typename M>
  inline auto cols(const M& m) -> decltype(m.extent(1))
  {
    static_assert(1 < M::order, "");
    return m.extent(1);
  }



// ------------------------------------------------------------------------ //
//                          Equality Comparable
//
// Two matrices compare equal when they have the same elements. Comparing
// matrices described by different slices is undefined behavior.

template <typename M1, typename M2>
  inline Requires<Matrix<M1>() && Matrix<M2>(), bool> 
  operator==(const M1& a, const M2& b)
  { 
    assert(same_extents(a, b));
    return std::equal(a.begin(), a.end(), b.begin());
  }

template <typename M1, typename M2>
  inline Requires<Matrix<M1>() && Matrix<M2>(), bool> 
  operator!=(const M1& a, const M2& b)
  { 
    return !(a == b);
  }


// NOTE: This operation is kind of funny because it is heterogeneous in its
// result type. If we try to concept check Matrix<R, R> (where R is a matrix
// ref type), we would normally be asking for an operation a homogeneous
// operator+(R,R)->R. That's not what we have.
//
// In order to check this concept, we have to weaken the result type. The
// C++0x concepts required that the result be convertible to the argument
// types. That doesn't work here because matrix is not convertible to matrix
// ref. It's the other way around.
//
// The correct way to check this is to say that the result type must share
// a common type with the domain type. That is, if U is the result type of the
// expression t + t (with t having type T), then Common<T, U> must be true.
//
// NOTE: We may be able to generalize and differentiate the addition of 
// matrices vs. matrix references in the the future. A fully general matrix
// slice would have the same properties as matrix_ref w.r.t. to addition
// and subtraction.


// Matrix addition
//
// Adding two matrices with the same shape adds corresponding elements in
// each operatand.

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator+(const matrix<T, N>& a, const matrix<T, N>& b)
  {
    assert(same_extents(a, b));
    matrix<T, N> result = a;
    return result += b;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator+(const matrix_ref<T, N>& a, const matrix_ref<T, N>& b)
  {
    assert(same_extents(a, b));
    matrix<T, N> result = a;
    return result += b;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator+(const matrix<T, N>& a, const matrix_ref<T, N>& b)
  {
    assert(same_extents(a, b));
    matrix<T, N> result = a;
    return result += b;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator+(const matrix_ref<T, N>& a, const matrix<T, N>& b)
  {
    assert(same_extents(a, b));
    matrix<T, N> result = a;
    return result += b;
  }


//////////////////////////////////////////////////////////////////////////////
// Matrix subtraction
//
// Subtracting one matrix from another with the same shape subtracts
// corresponding elements in each operatand.
template <typename T, std::size_t N>
  inline matrix<T, N>
  operator-(const matrix<T, N>& a, const matrix<T, N>& b)
  {
    assert(same_extents(a, b));
    matrix<T, N> result = a;
    return a -= b;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator-(const matrix_ref<T, N>& a, const matrix_ref<T, N>& b)
  {
    assert(same_extents(a, b));
    matrix<T, N> result = a;
    return result -= b;
  }

// Cross-type subtraction
template <typename T, std::size_t N>
  inline matrix<T, N>
  operator-(const matrix<T, N>& a, const matrix_ref<T, N>& b)
  {
    assert(same_extents(a, b));
    matrix<T, N> result = a;
    return result -= b;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator-(const matrix_ref<T, N>& a, const matrix<T, N>& b)
  {
    assert(same_extents(a, b));
    matrix<T, N> result = a;
    return result -= b;
  }



//////////////////////////////////////////////////////////////////////////////
// Scalar addition
//
// Adding a scalar to a matrix adds that value to each element in the matrix.
// The following operations are supported:
//
//    a + n
//    n + a
template <typename T, std::size_t N>
  inline matrix<T, N> 
  operator+(const matrix<T, N>& x, const T& n)
  {
    matrix<T, N> result = x;
    return result += n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N> 
  operator+(const T& n, const matrix<T, N>& x)
  {
    matrix<T, N> result = x;
    return result += n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator+(const matrix_ref<T, N>& x, const T& n)
  {
    matrix<T, N> result = x;
    return result += n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator+(const T& n, const matrix_ref<T, N>& x)
  {
    matrix<T, N> result = x;
    return result += n;
  }


//////////////////////////////////////////////////////////////////////////////
// Scalar subtraction
//
// A scalar can be subtracted from a matrix. It is equivalent to adding the
// negation of the scalar. That is:
//
//    a - n <=> a + -n;
//
// It is not possible to subtract a matrix from a scalar.
template <typename T, std::size_t N>
  inline matrix<T, N> 
  operator-(const matrix<T, N>& x, const T& n)
  {
    matrix<T, N> result = x;
    return result += n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator-(const matrix_ref<T, N>& x, const T& n)
  {
    matrix<T, N> result = x;
    return result -= n;
  }



//////////////////////////////////////////////////////////////////////////////
// Scalar multiplication
//
// Multiplying a matrix by a scalar multiplies each element by the scalar
// value. The following operations are supported:
//
//    a * n
//    n * a
//
template <typename T, std::size_t N>
  inline matrix<T, N>
  operator*(const matrix<T, N>& x, const T& n)
  {
    matrix<T, N> result = x;
    return result *= n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator*(const T& n, const matrix<T, N>& x)
  {
    matrix<T, N> result = x;
    return result *= n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator*(const matrix_ref<T, N>& x, const T& n)
  {
    matrix<T, N> result = x;
    return result *= n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator*(const T& n, const matrix_ref<T, N>& x)
  {
    matrix<T, N> result = x;
    return result *= n;
  }


//////////////////////////////////////////////////////////////////////////////
// Scalar division
//
// A matrix can be divided by a scalar value. It is equivalent to multiplying
// the matrix by the reciprocal of the scalar.
//
//    a / n <=> a * 1/n
//
// It is not possible to divide a scalar by a matrix.
template <typename T, std::size_t N>
  inline matrix<T, N>
  operator/(const matrix<T, N>& a, const T& n)
  {
    matrix<T, N> result = a;
    return result /= n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator/(const matrix_ref<T, N>& x, const T& n)
  {
    matrix<T, N> result = x;
    return result /= n;
  }



//////////////////////////////////////////////////////////////////////////////
// Scalar modulus
//
// Returns a matrix containing the remainder of each element divided by the
// given scalar value.
//
// This operation is only available when T is an Integer type.
template <typename T, std::size_t N>
  inline matrix<T, N>
  operator%(const matrix<T, N>& a, const T& n)
  {
    matrix<T, N> result = a;
    return result %= n;
  }

template <typename T, std::size_t N>
  inline matrix<T, N>
  operator%(const matrix_ref<T, N>& x, const T& n)
  {
    matrix<T, N> result = x;
    return result %= n;
  }


// Declarations
template <typename M1, typename M2, typename M3>
  void matrix_product(const M1&, const M2&, M3&);

template <typename M1, typename M2, typename M3>
  void hadamard_product(const M1&, const M2&, M3&);



//////////////////////////////////////////////////////////////////////////////
// Matrix Multiplication
//
// Two 2D matrices a (m x p) and b (p x n) can be multiplied, resulting in a
// matrix c (m x n). Note that the "inner" dimension of the operands must
// be the same.
template <typename T>
  inline matrix<T, 2>
  operator*(const matrix<T, 2>& a, const matrix<T, 2>& b) 
  {
    matrix<T, 2> result (a.rows(), b.cols());
    matrix_product(a, b, result);
    return result;
  }

template <typename T>
  inline matrix<T, 2>
  operator*(const matrix_ref<T, 2>& a, const matrix_ref<T, 2>& b) 
  {
    matrix<T, 2> result (a.dims());
    matrix_product(a, b, result);
    return result;
  }

// Cross product multiplication.
template <typename T>
  inline matrix<T, 2>
  operator*(const matrix<T, 2>& a, const matrix_ref<T, 2>& b) 
  {
    matrix<T, 2> result (a.dims());
    matrix_product(a, b, result);
    return result;
  }

template <typename T>
  inline matrix<T, 2>
  operator*(const matrix_ref<T, 2>& a, const matrix<T, 2>& b) 
  {
    matrix<T, 2> result (a.dims());
    matrix_product(a, b, result);
    return result;
  }



//////////////////////////////////////////////////////////////////////////////
// Matrix Product
//
// The usual meaning of the operation.
//
// FIXME: This is not terribly efficient. Implement a more efficient algorithm
// than this brute force version.
//
// FIXME: I'm not at all sure that this generalizes to n dimensions. It might
// be the case that we want all M's to be 2 dimensions (as they are now!).
template <typename M1, typename M2, typename M3>
  void 
  matrix_product(const M1& a, const M2& b, M3& out)
  {
    static_assert(M1::order == 2, "");
    static_assert(M2::order == 2, "");
    static_assert(M3::order == 2, "");
    assert(cols(a) == rows(b));
    assert(rows(a) == rows(out));
    assert(cols(b) == cols(out));

    using Size = Size_type<M3>;

    for (Size i = 0; i != rows(a); ++i) {
      for (Size j = 0; j < cols(b); ++j) {
        for (Size k = 0; k < rows(b); ++k)
          out(i, j) += a(i, k) * b(k, j);
      }
    }
  }


//////////////////////////////////////////////////////////////////////////////
// Hadamard Product
//
// The hadamard product can be easly generalized to N-dimensional matrices 
// since the operation is performed elementwise. The operands only need the
// same shape.
template <typename M1, typename M2, typename M3>
  void
  hadamard_product(const M1& a, const M2& b, M3& out)
  {
    assert(a.shape() == b.shape());
    assert(a.shape() == out.shape());

    using Mul = std::multiplies<Value_type<M1>>;
    std::transform(a.begin(), a.end(), b.begin(), out.begin(), Mul{});
  }



// -------------------------------------------------------------------------- //
// Output
//
// Write the matrix to the the given output stream.
//
// TODO: Write extensions that allow for pretty printing of matrices.

template <typename C, typename T, typename M>
  inline Requires<Matrix<M>(), std::basic_ostream<C, T>&>
  operator<<(std::basic_ostream<C, T>& os, const M& m)
  {
    os << '[';
    for (std::size_t i = 0; i < rows(m); ++i) {
      os << m[i];
      if (i + 1 != rows(m))
        os << ',';
    }
    os << ']';
    return os;
  }


template <typename M, std::size_t D = M::order>
  struct pretty_matrix
  {
    pretty_matrix(const M& m) : matrix(m), depth(0) { }
    pretty_matrix(const M& m, std::size_t d) : matrix(m), depth(d) { }

    const M& matrix;
    std::size_t depth;
  };

template <typename M>
  Requires<Matrix<M>(), pretty_matrix<M>>
  pretty(const M& m) { return pretty_matrix<M> {m}; }

template <typename M>
  Requires<Matrix<M>(), pretty_matrix<M>>
  pretty(const M& m, std::size_t d) { return pretty_matrix<M> {m, d}; }


template <typename C, typename T, typename M, std::size_t D>
  inline Requires<Matrix<M>(), std::basic_ostream<C, T>&>
  operator<<(std::basic_ostream<C, T>& os, pretty_matrix<M, D> p)
  {
    const auto& m = p.matrix;
    const std::size_t n = m.rows();
    
    // Indent
    for (std::size_t i = 0; i < p.depth; ++i)
      os << ' ';

    // Write this level of matrix.
    os << '[' << '\n';
    for (std::size_t i = 0; i < n; ++i) {
      os << pretty(m[i], p.depth + 1);
      if (i + 1 != n) os << ',';
      os << '\n';
    }
    os << ']';
    return os;
  }

template <typename C, typename T, typename M>
  inline Requires<Matrix<M>(), std::basic_ostream<C, T>&>
  operator<<(std::basic_ostream<C, T>& os, const pretty_matrix<M, 1>& p)
  {
    const auto& m = p.matrix;
    const std::size_t n = m.size();

    // Indent
    for (std::size_t i = 0; i < p.depth; ++i)
      os << ' ';

    // Write the 1d matrix 
    os << '[';
    for (std::size_t i = 0; i < n; ++i) {
      os << m(i);
      if (i + 1 != n) os << ',';
    }
    os << ']';
    return os;
  }
