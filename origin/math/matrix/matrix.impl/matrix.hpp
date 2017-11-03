// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif


// -------------------------------------------------------------------------- //
// Matrix                                                          [matrix.decl]
//
// The matrix template an N-dimensional array of elements of type T. The
// matrix class provides support for indexing, slicing and basic arithmetic
// operations.
//
// Note that matrix<T, 0> is a valid type, but it is not a matrix. It contains
// a single scalar of type T.
//
// TODO: Don't use vector<T> as the underlying store; just use a dynamically
// allocated array. Certain operations will become faster (with fewer
// assignments) and the class will take less space (2 pointers less). This
// would also allow us to write an interesting version of reserve().
//
// Template Parameters:
//    T -- The eleemnt type stored by the matrix
//    N -- The matrix order (number of extents).
template <typename T, std::size_t N>
  class matrix
  {
  public:
    static constexpr std::size_t order = N;

    using value_type     = T;
    using iterator       = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;


    // Default construction
    matrix() = default;

    // Move semantics
    matrix(matrix&&) = default;
    matrix& operator=(matrix&&) = default;

    // Copy semantics
    matrix(const matrix&) = default;
    matrix& operator=(const matrix&) = default;

    // Destruction
    ~matrix() = default;


    // Matrix assignment.
    //
    // Initialize or assign this matrix by copying the matrix x. In the
    // case of assignment, the original matrix is destroyed and replaced by
    // the copy.
    //
    // The order of the two matrices must be the same, and the value type of
    // M must be convertible to this matrix's value type.
    template <typename M, typename = Requires<Matrix<M>()>>
      matrix(const M& x);
    
    template <typename M, typename = Requires<Matrix<M>()>>
      matrix& operator=(const M& x);


    // Slice initialization
    //
    // Initialize the matrix so that it has the same extents as the given
    // slice. Note that the strides are not copied. The resulting matrix
    // indexes it's elements in row-major order.
    explicit matrix(const matrix_slice<N>& slice);


    // Extent initialization
    //
    // Initialize the matrix with the dimensions given as a sequence of
    // arguments. All elements are default initialized. 
    //
    // NOTE: There is no corresponding assignment operator for this
    // constructor. The resize() operation to change the dimensions of
    // the matrix.
    // 
    // TODO: Create overloads that allow the specification of a default
    // value as the last argument and one that allows the specification of
    // a default value and an allocator as the last pairs of arguments.
    template <typename... Dims>
      explicit
      matrix(Dims... dims);


    // Value initialization
    //
    // Initialize the matrix over a nesting of initializer lists. The number
    // of nestings is determined by the order of the matrix, N.
    matrix(matrix_initializer<T, N> init);
    matrix& operator=(matrix_initializer<T, N> init);

    // The "flat" initializer list is disabled, preventing a user from
    // writing m {a, b, c, ...} for an arbitrarily nested type. This may be
    // confused with extent initialization.
    template <typename U>
      matrix(std::initializer_list<U> list) = delete;


    // Properties

    // Return the slice describing the matrix.
    const matrix_slice<N>& descriptor() const { return desc; }

    // Returns the extent of the matrix in the nth dimension. 
    std::size_t extent(std::size_t n) const { return desc.extents[n]; }

    // Returns the number of rows (0th extent) in the matrix.
    std::size_t rows() const { return extent(0); }

    // Returns the number of columns (1st extent) in the matrix.
    std::size_t cols() const { return extent(1); }

    // Returns the total number of elements contained in the matrix.
    std::size_t size() const { return desc.size; }


    // Subscripting
    //
    // Returns a reference to the element at the index given by the sequence
    // of indexes, args.

    template <typename... Args>
      Requires<matrix_impl::Index_sequence<Args...>(), T&>
      operator()(Args... args);

    template <typename... Args>
      Requires<matrix_impl::Index_sequence<Args...>(), const T&>
      operator()(Args... args) const;

    template <typename... Args>
      Requires<matrix_impl::Slice_sequence<Args...>(), matrix_ref<T, N>>
      operator()(const Args&... args);

    template <typename... Args>
      Requires<matrix_impl::Slice_sequence<Args...>(), matrix_ref<const T, N>>
      operator()(const Args&... args) const;


    // Row subscripting
    //
    // Returns a reference to the nth row of the matrix. This is the equivalent
    // to m.row(n).
    matrix_ref<T, N-1>       operator[](std::size_t n)       { return row(n); }
    matrix_ref<const T, N-1> operator[](std::size_t n) const { return row(n); }

    // Row
    //
    // Returns a matrix_ref referring to the nth row of the matrix.
    matrix_ref<T, N-1>       row(std::size_t n);
    matrix_ref<const T, N-1> row(std::size_t n) const;

    // Column
    //
    // Returns a matrix_ref referring to the nth column of the matrix.
    matrix_ref<T, N-1>       col(std::size_t n);
    matrix_ref<const T, N-1> col(std::size_t n) const;


    // Data access
    //
    // Returns a pointer to the underlying data.
    T*       data()       { return elems.data(); }
    const T* data() const { return elems.data(); }


    // Apply
    template <typename F>
      matrix& apply(F f);

    template <typename M, typename F>
      matrix& apply(const M& m, F f);

    // Scalar arithmetic
    matrix& operator=(const T& x);
    matrix& operator+=(const T& x);
    matrix& operator-=(const T& x);
    matrix& operator*=(const T& x);
    matrix& operator/=(const T& x);
    matrix& operator%=(const T& x);

    // Matrix arithmetic
    template <typename M>
      matrix& operator+=(const M& m);
    
    template <typename M>
      matrix& operator-=(const M& m);


    // Iterators
    //
    // The begin() and end() functions return iterators over the underlying
    // data in the matrix. It is not structured.
    //
    // TODO: Write iterators over rows and columns.
    iterator begin() { return elems.begin(); }
    iterator end()   { return elems.end(); }

    const_iterator begin() const { return elems.begin(); }
    const_iterator end() const   { return elems.end(); }


    // Mutators
    
    void swap(matrix& x);
    void swap_rows(std::size_t m, std::size_t n);
    void clear();

  private:
    void make_slice(matrix_slice<N>&, std::size_t);
    void make_slice(matrix_slice<N>&, std::size_t, std::size_t);
    void make_slice(matrix_slice<N>&, std::size_t, std::size_t, std::size_t);

  private:
    matrix_slice<N> desc; // Describing slice
    std::vector<T> elems; // Underlying elements
  };


template <typename T, std::size_t N>
  template <typename M, typename X>
  inline
  matrix<T, N>::matrix(const M& x)
    : desc(x.descriptor()), elems(x.begin(), x.end())
  {
    static_assert(Convertible<Value_type<M>, T>(), "");
  }

template <typename T, std::size_t N>
  template <typename M, typename X>
  inline matrix<T, N>&
  matrix<T, N>::operator=(const M& x)
  {
    desc = x.descriptor();
    elems.assign(x.begin(), x.end());
    return*this;
  }


template <typename T, std::size_t N>
  inline
  matrix<T, N>::matrix(const matrix_slice<N>& slice)
    : desc(0, slice.extents), elems(desc.size)
  { }


template <typename T, std::size_t N>
  template <typename... Dims>
    inline
    matrix<T, N>::matrix(Dims... dims)
      : desc(0, {std::size_t(dims)...}), elems(desc.size)
    { }

template <typename T, std::size_t N>
  inline
  matrix<T, N>::matrix(matrix_initializer<T, N> init)
    : desc(0, matrix_impl::derive_extents<N>(init))
  {
    // matrix_impl::derive_extents(desc.extents, init);
    elems.reserve(desc.size);
    matrix_impl::insert_flattened(init, elems);
    assert(elems.size() == desc.size);
  }

template <typename T, std::size_t N>
  inline matrix<T, N>&
  matrix<T, N>::operator=(matrix_initializer<T, N> init)
  {
    matrix tmp(init);
    swap(tmp);
    return *this;
  }


// Subscripting

template <typename T, std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Index_sequence<Args...>(), T&>
    matrix<T, N>::operator()(Args... args)
    {
      assert(matrix_impl::check_bounds(desc, args...));
      return *(data() + desc(args...));
    }

template <typename T, std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Index_sequence<Args...>(), const T&>
    matrix<T, N>::operator()(Args... args) const
    {
      assert(matrix_impl::check_bounds(desc, args...));
      return *(data() + desc(args...));
    }

template <typename T, std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Slice_sequence<Args...>(), matrix_ref<T, N>>
    matrix<T, N>::operator()(const Args&... args)
    {
      matrix_slice<N> d {desc, args...};
      return {d, data()};
    }

template <typename T, std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Slice_sequence<Args...>(), matrix_ref<const T, N>>
    matrix<T, N>::operator()(const Args&... args) const
    {
      matrix_slice<N> d {desc, args...};
      return {d, data()};
    }

// Row

template <typename T, std::size_t N>
  inline matrix_ref<T, N-1>
  matrix<T, N>::row(std::size_t n)
  {
    assert(n < rows());
    matrix_slice<N-1> row(desc, size_constant<0>(), n);
    return {row, data()};
  }

template <typename T, std::size_t N>
  inline matrix_ref<const T, N-1>
  matrix<T, N>::row(std::size_t n) const
  {
    assert(n < rows());
    matrix_slice<N-1> row(desc, size_constant<0>(), n);
    return {row, data()};
  }

// Column

template <typename T, std::size_t N>
  inline matrix_ref<T, N-1>
  matrix<T, N>::col(std::size_t n)
  {
    assert(n < cols());
    matrix_slice<N-1> col(desc, size_constant<1>(), n);
    return {col, data()};
  }

template <typename T, std::size_t N>
  inline matrix_ref<const T, N-1>
  matrix<T, N>::col(std::size_t n) const
  {
    assert(n < cols());
    matrix_slice<N-1> col(desc, size_constant<1>(), n);
    return {col, data()};
  }


// Scalar applicateion
template <typename T, std::size_t N>
  template <typename F>
    inline matrix<T, N>&
    matrix<T, N>::apply(F f)
    {
      for (auto& x : elems)
        f(x);
      return *this;
    }

template <typename T, std::size_t N>
  template <typename M, typename F>
    inline matrix<T, N>&
    matrix<T, N>::apply(const M& m, F f)
    {
      assert(same_extents(desc, m.descriptor()));
      auto i = begin();
      auto j = m.begin();
      while (i != end()) {
        f(*i, *j);
        ++i;
        ++j;
      }
      return *this;
    }

// Scalar assignment
template <typename T, std::size_t N>
  inline matrix<T, N>& 
  matrix<T, N>::operator=(const T& x) 
  { 
    return apply([&](T& x) { x = x; });
  }

// Scalar addition
template <typename T, std::size_t N>
  inline matrix<T, N>& 
  matrix<T, N>::operator+=(const T& x) 
  { 
    return apply([&](T& y) { y += x; });
  }

// Scalar subtraction      
template <typename T, std::size_t N>
  inline matrix<T, N>& 
  matrix<T, N>::operator-=(const T& x) 
  {
    return apply([&](T& y) { y -= x; });
  }

// Scalar multiplication
template <typename T, std::size_t N>
  inline matrix<T, N>& 
  matrix<T, N>::operator*=(const T& x) 
  { 
    return apply([&](T& y) { y *= x; });
  }

// Scalar division
template <typename T, std::size_t N>
  inline matrix<T, N>& 
  matrix<T, N>::operator/=(const T& x) 
  { 
    return apply([&](T& y) { y /= x; });
  }

// Scalar remainder    
template <typename T, std::size_t N>
  inline matrix<T, N>& 
  matrix<T, N>::operator%=(const T& x) 
  { 
    return apply([&](T& y) { y %= x; });
  }

// NOTE: Matrix addition and subtraction require the arguments to have the
// same order, dimensions, and size. It must be the case that if two matrices
// have the same dimensions, then they have the same size.

// Matrix addition
template <typename T, std::size_t N>
  template <typename M>
    inline matrix<T, N>&
    matrix<T, N>::operator+=(const M& m)
    {
      using U = Value_type<M>;
      return apply(m, [&](T& t, const U& u) { t += u; });
    }

// Matrix subtraction
template <typename T, std::size_t N>
  template <typename M>
    inline matrix<T, N>&
    matrix<T, N>::operator-=(const M& m)
    {
      using U = Value_type<M>;
      return apply(m, [&](T& t, const U& u) { t -= u; });
    }

template <typename T, std::size_t N>
  inline void
  matrix<T, N>::swap(matrix& x)
  {
    using std::swap;
    swap(desc, x.desc);
    elems.swap(x.elems);
  }

template <typename T, std::size_t N>
  inline void
  matrix<T, N>::swap_rows(std::size_t m, std::size_t n)
  {
    auto a = (*this)[m];
    auto b = (*this)[n];
    std::swap_ranges(a.begin(), a.end(), b.begin());
  }


// ------------------------------------------------------------------------ //
//                          Zero-Dimension Matrix
//
// The type matrix<T, 0> is not really a matrix. It stores a single scalar
// of type T and can only be converted to a reference to that type.

template <typename T>
  class matrix<T, 0>
  {
  public:
    matrix() = default;

    // Initialize the pseudo-matrix.
    matrix(const T& x) : elem(x) { }

    matrix& operator=(const T& value) { elem = value; return *this; }

    T&       operator()() { return elem; }
    const T& operator()() const { return elem; }

    operator T&() { return elem; }
    operator const T&() { return elem; }

  private:
    T elem;
  };
