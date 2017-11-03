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
//                             Matrix Base
//
// Provides support for features common to both matrices and matrix
// references (with 0 < N). 

// Note that matrix initialization is always in row-major order. This means
// that strides[0] will contain the computed size of the matrix immediately
// after value or dimensional initialization.
//
// Implementation Notes:
// The strides array is cached by the matrix base to provide efficient
// computation of element indexes in row-major and column-major order.
template <typename T, std::size_t N>
  struct matrix_base
  {
    using array_type = std::array<std::size_t, N>;

    static_assert(0 < N, "");

    static std::size_t constexpr order = N;

    // Default construction
    matrix_base() = default;

    // Move semantics
    matrix_base(matrix_base&&) = default;
    matrix_base& operator=(matrix_base&&) = default;

    // Copy semantics
    matrix_base(const matrix_base&) = default;
    matrix_base& operator=(const matrix_base&) = default;

    // Destruction
    ~matrix_base() = default;

    // Dimension initialization
    template <typename... Dims>
      matrix_base(Dims... dims);

    // Value initialization
    matrix_base(Matrix_initializer<T, N> list);

    // Slice initialization
    matrix_base(const array_type& sz, const array_type& str);

    // Index computation
    template <typename... Dims>
      std::size_t operator()(Dims... dims) const;

    // Return a matrix base describing the inner row.
    matrix_base<T, N - 1> row() const;

    template <typename Act>
      void apply_scalar(T* self, const T& value, Act act);

    template <typename Act>
      void apply_matrix(T* self, const T* data, Act act);

    // Base swap
    void swap(matrix_base& base);

    // FIXME: Wrap these in a slice class.
    std::size_t start;  // The starting index of the slice
    array_type extents; // The dimensions of the matrix
    array_type strides; // The partial product of dimensions
    std::size_t size;   // The total size of the matrix
  };


template <typename T, std::size_t N>
  template <typename... Dims>
    inline
    matrix_base<T, N>::matrix_base(Dims... dims)
      // : extents{dims...}
    {
      static_assert(N == sizeof...(Dims), "");

      // TODO: GCC is complaing about the member-initializer above. That
      // should be completely legal, but somehow GCC is getting confused.
      // There are no narrowing problems, either. The error is:
      //
      //    array must be initialized with a brace-enclosed initializer
      //
      // Clearly, extents is being bing initialized correctly. As a work
      // around, we have this:
      array_type tmp { dims... };
      copy(tmp, extents);

      // Compute the row major ordering vector.
      size = matrix_impl::reverse_partial_product(extents, strides);
    }

template <typename T, std::size_t N>
  inline
  matrix_base<T, N>::matrix_base(Matrix_initializer<T, N> list)
  {
    // Derive the extents from nested list.
    matrix_impl::derive_extents(extents, list);

    // Compute the row major ordering vector.
    size = matrix_impl::reverse_partial_product(extents, strides);
  }

template <typename T, std::size_t N>
  inline
  matrix_base<T, N>::matrix_base(const array_type& sz, const array_type& str)
    : extents(sz), strides(str)
  { }


template <typename T, std::size_t N>
  template <typename... Dims>
    inline std::size_t
    matrix_base<T, N>::operator()(Dims... dims) const
    {
      static_assert(sizeof...(Dims) == N, "");

      // TODO: Assert that dims are in bounds.

      // Copy arguments into a vector.
      std::size_t args[N] { std::size_t(dims)... };

      // The index is the inner product of given indexes and strides.
      constexpr std::size_t zero = 0;
      return start + std::inner_product(args, args + N, strides.begin(), zero);
    }

template <typename T, std::size_t N>
  inline matrix_base<T, N - 1>
  matrix_base<T, N>::row() const
  {
    static_assert(N != 0, "");

    // We can construct the the lower-dimension base by simply chopping off
    // the first element of the array.
    matrix_base<T, N - 1> res;
    std::copy_n(extents.begin() + 1, N, res.extents.begin());
    std::copy_n(strides.begin() + 1, N, res.strides.begin());
    res.size = strides[0];
    return res;
  }


template <typename T, std::size_t N>
  template <typename Act>
  inline void
  matrix_base<T, N>::apply_scalar(T* self, const T& value, Act act)
  {
    apply(self, self + size, value, act);
  }

template <typename T, std::size_t N>
  template <typename Act>
  inline void
  matrix_base<T, N>::apply_matrix(T* self, const T* data, Act act)
  {
    apply_each(self, self + size, data, act);
  }

template <typename T, std::size_t N>
  inline void
  matrix_base<T, N>::swap(matrix_base& x)
  {
    using std::swap;
    std::swap(extents, x.extents);
    std::swap(strides, x.strides);
    std::swap(size, x.size);
  }
