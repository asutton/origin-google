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
// Matrix reference                                                 [matrix.ref]
//
// A matrix_ref is a reference to memory in a matrix specified by a slice. A
// matrix_ref does not own its elements.
//
// Template parameters:
//    T -- The underlying value type of the matrix, possibly const.
//    N -- The order of the matrix (number of extents).
//
template <typename T, std::size_t N>
  class matrix_ref
  {
  public:
    static constexpr std::size_t order = N;

    using value_type          = Remove_const<T>;
    using iterator            = slice_iterator<T, N>;
    using const_iterator      = slice_iterator<const T, N>;

    // Default construction
    matrix_ref() = delete;


    // Move semantics
    // FIXME: The implementation is not exception-safe.
    matrix_ref(matrix_ref&& x);
    matrix_ref& operator=(matrix_ref&& x);

    // Copy semantics
    matrix_ref(const matrix_ref& x);
    matrix_ref& operator=(const matrix_ref& x);


    // Slice initialization
    //
    // Initialize the matrix_ref over the slice, s, starting at the element
    // pointed to by p.
    matrix_ref(const matrix_slice<N>& s, T* p);


    // Matrix initialization
    //
    // Initialize the matrix_ref so that it refers to another matrix x, or assign
    // the elements of that matrix into this sub-matrix. Note that we explicitly
    // prohibit the initialization of a sub-matrix from an rvalue matrix. That
    // is a recipe for leaking memory.
    //
    // Assigning from a sub-matrix copies the values from x.
    matrix_ref(matrix<value_type, N>& x);
    matrix_ref(const matrix<value_type, N>& x);
    matrix_ref(matrix<value_type, N>&&) = delete;

    matrix_ref& operator=(const matrix<value_type, N>& x);


    // Submatrix conversion
    //
    // Allow implicit conversion from a non-const matrix_ref to a const
    // matrix_ref.
    template <typename U>
      matrix_ref(const matrix_ref<U, N>& x);

    template <typename U>
      matrix_ref& operator=(const matrix_ref<U, N>& x);


    // Destruction
    ~matrix_ref() = default;


    // Properties

    // Returns the shape of the matrix.

    // Return the array of extents describing the shape of the matrix.
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
    // Return a matrix_ref referring to the nth row. This is equivalent to
    // calling m.row(n).
    matrix_ref<T, N-1>       operator[](std::size_t n)       { return row(n); }
    matrix_ref<const T, N-1> operator[](std::size_t n) const { return row(n); }

    // Row
    //
    // Return a matrix_ref referring to the nth row.
    matrix_ref<T, N-1>       row(std::size_t n);
    matrix_ref<const T, N-1> row(std::size_t n) const;

    // Column
    //
    // Return a matrix_ref referring to the nth column.
    matrix_ref<T, N-1>       col(std::size_t n);
    matrix_ref<const T, N-1> col(std::size_t n) const;


    // Data access
    T*       data()       { return ptr; }
    const T* data() const { return ptr; }


    // Apply
    template <typename F>
      matrix_ref& apply(F f);

    template <typename M, typename F>
      matrix_ref& apply(const M& m, F f);


    // Scalar arithmetic
    matrix_ref& operator=(const value_type& x);
    matrix_ref& operator+=(const value_type& x);
    matrix_ref& operator-=(const value_type& x);
    matrix_ref& operator*=(const value_type& x);
    matrix_ref& operator/=(const value_type& x);
    matrix_ref& operator%=(const value_type& x);

    // Matrix arithmetic
    template <typename M>
      matrix_ref& operator+=(const M& m);
    
    template <typename M>
      matrix_ref& operator-=(const M& m);


    // Iterators
    iterator begin() { return {desc, ptr}; }
    iterator end()   { return {desc, ptr, true}; }

    const_iterator begin() const { return {desc, ptr}; }
    const_iterator end() const   { return {desc, ptr, true}; }


    void swap(matrix_ref& x);
    void swap_rows(std::size_t m, std::size_t n);

  private:
    matrix_slice<N> desc; // Descirbes the matrix_ref
    T* ptr;                // Points to the first element of a matrix
  };


template <typename T, std::size_t N>
  inline
  matrix_ref<T, N>::matrix_ref(matrix_ref&& x)
    : desc(x.desc), ptr(x.ptr)
  { }

template <typename T, std::size_t N>
  inline matrix_ref<T, N>&
  matrix_ref<T, N>::operator=(matrix_ref&& x)
  {
    assert(same_extents(desc, x.desc));
    // FIXME: This does not guarantee exception safety. If move throws, this
    // operator may leak resources. The algorithm must be specialized on
    // whether T is nothrow-movable.
    std::move(x.begin(), x.end(), begin());
    return *this;
  }


template <typename T, std::size_t N>
  inline
  matrix_ref<T, N>::matrix_ref(const matrix_ref& x)
    : desc(x.desc), ptr(x.ptr)
  { }

template <typename T, std::size_t N>
  inline matrix_ref<T, N>&
  matrix_ref<T, N>::operator=(const matrix_ref& x)
  {
    assert(same_extents(desc, x.desc));
    copy(x.begin(), x.end(), begin());
    return *this;
  }


template <typename T, std::size_t N>
  inline
  matrix_ref<T, N>::matrix_ref(matrix<value_type, N>& x)
    : desc(x.descriptor()), ptr(x.data())
  { }

template <typename T, std::size_t N>
  inline
  matrix_ref<T, N>::matrix_ref(const matrix<value_type, N>& x)
    : desc(x.descriptor()), ptr(x.data())
  { }

template <typename T, std::size_t N>
  inline matrix_ref<T, N>&
  matrix_ref<T, N>::operator=(const matrix<value_type, N>& x)
  {
      // FIXME: Is this right? Should we just assign values or resize the
      // vector based o what x is?
    assert(same_extents(desc, x.descriptor()));
    apply(x, [](T& a, const T& b) { a = b; });
    return *this;
  }

template <typename T, std::size_t N>
  template <typename U>
    inline
    matrix_ref<T, N>::matrix_ref(const matrix_ref<U, N>& x)
      : desc(x.descriptor()), ptr(x.ptr)
    { }

template <typename T, std::size_t N>
  template <typename U>
    inline matrix_ref<T, N>&
    matrix_ref<T, N>::operator=(const matrix_ref<U, N>& x)
    {
      // FIXME: Is this right? Should we just assign values or resize the
      // vector based o what x is?
      assert(same_extents(desc, x.descriptor()));
      apply(x, [](T& a, const U& b) { a = b; });
      return *this;
    }


template <typename T, std::size_t N>
  inline
  matrix_ref<T, N>::matrix_ref(const matrix_slice<N>& s, T* p)
    : desc(s), ptr(p)
  { }

template <typename T, std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Index_sequence<Args...>(), T&>
    matrix_ref<T, N>::operator()(Args... args)
    {
      assert(matrix_impl::check_bounds(desc, args...));
      return *(ptr + desc(args...));
    }

template <typename T, std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Index_sequence<Args...>(), const T&>
    matrix_ref<T, N>::operator()(Args... args) const
    {
      assert(matrix_impl::check_bounds(desc, args...));
      return *(ptr + desc(args...));
    }

template <typename T, std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Slice_sequence<Args...>(), matrix_ref<T, N>>
    matrix_ref<T, N>::operator()(const Args&... args)
    {
      return {{desc, args...}, ptr};
    }

template <typename T, std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Slice_sequence<Args...>(), matrix_ref<const T, N>>
    matrix_ref<T, N>::operator()(const Args&... args) const
    {
      return {{desc, args...}, ptr};
    }

template <typename T, std::size_t N>
  inline matrix_ref<T, N-1>
  matrix_ref<T, N>::row(std::size_t n)
  {
    assert(n < extent(0));
    matrix_slice<N-1> row(desc, size_constant<0>(), n);
    return {ptr, row};
  }

template <typename T, std::size_t N>
  inline matrix_ref<const T, N-1>
  matrix_ref<T, N>::row(std::size_t n) const
  {
    assert(n < extent(0));
    matrix_slice<N-1> row(desc, size_constant<0>(), n);
    return {row, ptr};
  }


template <typename T, std::size_t N>
  inline matrix_ref<T, N-1>
  matrix_ref<T, N>::col(std::size_t n)
  {
    assert(n < extent(1));
    matrix_slice<N-1> col(desc, size_constant<1>(), n);
    return {ptr, col};
  }

template <typename T, std::size_t N>
  inline matrix_ref<const T, N-1>
  matrix_ref<T, N>::col(std::size_t n) const
  {
    assert(n < extent(1));
    matrix_slice<N-1> col(desc, size_constant<1>(), n);
    return {ptr, col};
  }



template <typename T, std::size_t N>
  template <typename F>
    inline matrix_ref<T, N>&
    matrix_ref<T, N>::apply(F f)
    {
      for (auto i = begin(); i != end(); ++i)
        f(*i);
      return *this;
    }

template <typename T, std::size_t N>
  template <typename M, typename F>
    inline matrix_ref<T, N>&
    matrix_ref<T, N>::apply(const M& m, F f)
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
  inline matrix_ref<T, N>& 
  matrix_ref<T, N>::operator=(const value_type& value)  
  {
    return apply([&](T& x) { x = value; });
  }
    
// Scalar addition.
template <typename T, std::size_t N>
  inline matrix_ref<T, N>& 
  matrix_ref<T, N>::operator+=(const value_type& value) 
  { 
    return apply([&](T& x) { x += value; });
  }

// Scalar subtraction
template <typename T, std::size_t N>
  inline matrix_ref<T, N>& 
  matrix_ref<T, N>::operator-=(value_type const& value) 
  { 
    return apply([&](T& x) { x -= value; });
  }

// Scalar multiplication
template <typename T, std::size_t N>
  inline matrix_ref<T, N>& 
  matrix_ref<T, N>::operator*=(value_type const& value) 
  { 
    return apply([&](T& x) { x *= value; });
  }

// Scalar division
template <typename T, std::size_t N>
  inline matrix_ref<T, N>& 
  matrix_ref<T, N>::operator/=(value_type const& value) 
  { 
    return apply([&](T& x) { x /= value; });
  }

// Scalar modulus
// This operation is only defined when T is a model of Euclidean domain.
template <typename T, std::size_t N>
  inline matrix_ref<T, N>& 
  matrix_ref<T, N>::operator%=(value_type const& value) 
  { 
    return apply([&](T& x) { x %= value; });
  }

// Matrix addition
template <typename T, std::size_t N>
  template <typename M>
    inline matrix_ref<T, N>&
    matrix_ref<T, N>::operator+=(const M& m)
    {
      using U = Value_type<M>;
      return apply(m, [&](T& t, const U& u) { t += u; });
    }

// Matrix subtraction
template <typename T, std::size_t N>
  template <typename M>
    inline matrix_ref<T, N>&
    matrix_ref<T, N>::operator-=(const M& m)
    {
      using U = Value_type<M>;
      return apply(m, [&](T& t, const U& u) { t -= u; });
    }


  // Swap
  template <typename T, std::size_t N>
    inline void
    matrix_ref<T, N>::swap(matrix_ref& x)
    {
      using std::swap;
      swap(desc, x.desc);
      swap(ptr, x.ptr);
    }

  // Swap rows
  template <typename T, std::size_t N>
    inline void
    matrix_ref<T, N>::swap_rows(std::size_t m, std::size_t n)
    {
      auto a = (*this)[m];
      auto b = (*this)[n];
      std::swap_ranges(a.begin(). a.end(), b.begin());
    }


  // ------------------------------------------------------------------------ //
  //                        Zero-Dimension Submatrix
  //
  // The type matrix_ref<T, 0> is not really a matrix. It contains a pointer
  // to an element in a matrix_ref.

  template <typename T>
    class matrix_ref<T, 0>
    {
    public:
      matrix_ref() = delete;

      matrix_ref(const matrix_slice<0>& s, T* p) : ptr(p + s.start) { }

      // Modifying operators
      matrix_ref& operator=(const T& x) { *ptr = x; return *this; }

      T& operator()()       { return *ptr; }
      T& operator()() const { return *ptr; }

      operator T&() { return *ptr; }

    private:
      T* ptr;
    };
