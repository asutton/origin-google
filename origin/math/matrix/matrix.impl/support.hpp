// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif

// NOTE: Facilities in this header could be moved into more general libraries.

namespace matrix_impl
{

  // ------------------------------------------------------------------------ //
  //                          Insert Flattened
  //
  // Insert the elements of a initializer list nesting into a vector such that
  // each subsequent set of "leaf" values are copied into a contiguous memory.

  // TODO: This algorithm could be generalized to flatten an arbitrary
  // initializer list structure.

  // For iterators over the leaf nodes, insert elements into the back of the
  // vector. We generally assume that the vector has sufficient capacity for
  // all such insertions, but insert guarantees that it will resize if needed.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const T* first, const T* last, Vec& vec)
    {
      vec.insert(vec.end(), first, last);
    }

  // For iterators into nested initializer lists, recursively intiailize each
  // sub-initializeer.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const std::initializer_list<T>* first,
                    const std::initializer_list<T>* last,
                    Vec& vec)
    {
      while (first != last) {
        insert_flattened(first->begin(), first->end(), vec);
        ++first;
      }
    }

  // Copy the elements from the initializer list nesting into contiguous
  // elements in the vector.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const std::initializer_list<T>& list, Vec& vec)
    {
      insert_flattened(list.begin(), list.end(), vec);
    }


  // ------------------------------------------------------------------------ //
  //                              Algorithms

  // Compute the a variant of partial product of the in array, storing the
  // results in the out array. This implementation stores the multiplicative
  // identinity in the first element
  //
  // The resulting vector is used to compute indexes in column-major order.
  template <typename Array>
    inline std::size_t
    forward_partial_product(const Array& in, Array& out)
    {
      using T = Value_type<Array>;
      std::multiplies<T> mul;
      auto f = in.begin();
      auto l = --in.end();
      auto o = out.begin();
      *o++ = T(1);
      std::partial_sum(f, l, o, mul);
      return *(--o) * *l;
    }

  // Compute the partial product, in reverse order, of the in array, storing
  // the results in the out array.
  template <typename Array>
    inline std::size_t
    reverse_partial_product(const Array& in, Array& out)
    {
      using T = Value_type<Array>;
      std::multiplies<T> mul;
      auto f = in.rbegin();
      auto l = --in.rend();
      auto o = out.rbegin();
      *o++ = T(1);
      o = std::partial_sum(f, l, o, mul);
      return *(--o) * *l;
    }


  // ------------------------------------------------------------------------ //
  //                          Derive Extents


  template <std::size_t N, typename List>
    inline std::array<std::size_t, N> derive_extents(const List& list);


  // An matrix is non-jagged if the extents of each submatrix compare equal.
  template <std::size_t N, typename List>
    inline bool
    check_non_jagged(const List& list)
    {
      auto i = list.begin();
      for (auto j = i + 1; j != list.end(); ++j) {
        if (derive_extents<N - 1>(*i) != derive_extents<N - 1>(*j))
          return false;
      }
      return true;
    }


  // Derive extents implementation, matches when N == 1.
  template <std::size_t N, typename I, typename List>
    inline Requires<(N == 1), void>
    derive_extents(I& first, I last, const List& list)
    {
      assert(first != last);
      *first++ = list.size();
    }

  // Derive extents implementation, matches when N > 1.
  template <std::size_t N, typename I, typename List>
    inline Requires<(N > 1), void>
    derive_extents(I& first, I last, const List& list)
    {
      assert(check_non_jagged<N>(list));
      assert(first != last);
      *first++ = list.size();
      derive_extents<N - 1>(first, last, *list.begin());
    }


  template <std::size_t N, typename List>
    inline std::array<std::size_t, N>
    derive_extents(const List& list)
    {
      std::array<std::size_t, N> a;
      auto f = a.begin();
      auto l = a.end();
      derive_extents<N>(f, l, list);
      assert(f == l);

      return a;
    }


  // Returns true if each element in range is within the bounds of the
  // corresponding extent.
  template <std::size_t N, typename... Dims>
    inline bool
    check_bounds(const matrix_slice<N>& slice, Dims... dims)
    {
      std::size_t indexes[N] {std::size_t(dims)...};
      std::less<std::size_t> lt;
      return std::equal(indexes, indexes + N, slice.extents, lt);
    }


} // namespace matrix_impl

