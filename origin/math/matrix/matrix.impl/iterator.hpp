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
//                             Slice Iterator
//
// A slice iterator ranges over the elements of a submatrix specified by a
// slice.
//
// A slice iterator is a forward iterator. Note that it may be possible to make
// this bidirectional or random access, but we reserve that for future work.
template <typename T, std::size_t N>
  struct slice_iterator
  {
    using value_type = Remove_const<T>;
    using reference = T&;
    using pointer = T*;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    slice_iterator(const matrix_slice<N>& s, T* base, bool limit = false);

    // Returns the iterators describing slice.
    const matrix_slice<N>& descriptor() const { return desc; }

    // Readable
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }

    // Forward Iterator
    slice_iterator& operator++();
    slice_iterator operator++(int);

  private:
    void increment();

  private:
    const matrix_slice<N>& desc; // Describes the iterator range
    std::size_t indexes[N];       // Counting indexes
    T* ptr;                       // The current element
  };


template <typename T, std::size_t N>
  slice_iterator<T, N>::slice_iterator(const matrix_slice<N>& s, 
                                       T* base, 
                                       bool limit)
    : desc(s)
  {
    std::fill_n(indexes, N, 0);
    if (limit) {
      indexes[0] = desc.extents[0];
      ptr = base + desc.offset(indexes);
    } else {
      ptr = base + s.start;
    }
  }

template <typename T, std::size_t N>
  inline slice_iterator<T, N>&
  slice_iterator<T, N>::operator++()
  {
    increment();
    return *this;
  }

template <typename T, std::size_t N>
  inline slice_iterator<T, N>
  slice_iterator<T, N>::operator++(int)
  {
    slice_iterator x = *this;
    increment();
    return x;
  }


// Move to the next element in the range.
template <typename T, std::size_t N>
  void
  slice_iterator<T, N>::increment()
  {
    std::size_t d = N - 1;
    while (true) {
      ptr += desc.strides[d];
      ++indexes[d];

      // If have not yet counted to the extent of the current dimension, then
      // we will continue to do so in the next iteration.
      if (indexes[d] != desc.extents[d])
        break;

      // Otherwise, if we have not counted to the extent in the outermost
      // dimension, move to the next dimension and try again. If d is 0, then
      // we have counted through the entire slice.
      if (d != 0) {
        ptr -= desc.strides[d] * desc.extents[d];
        indexes[d] = 0;
        --d;
      } else {
        break;
      }
    }
  }


// Equality_comparable
//
// Two slice iterators are equality comparable when their slices compute the
// same sequence of elements, and the iterators refer to the same element.
//
// For efficiency, we assume the first requirement. It is undefined behavior
// to compare slice iterators from different slices.
template <typename T, std::size_t N>
  inline bool
  operator==(const slice_iterator<T, N>& a, const slice_iterator<T, N>& b)
  {
    assert(a.descriptor() == b.descriptor());
    return &*a == &*b;
  }

template <typename T, std::size_t N>
  inline bool
  operator!=(const slice_iterator<T, N>& a, const slice_iterator<T, N>& b)
  {
    return !(a == b);
  }
