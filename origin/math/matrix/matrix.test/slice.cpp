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


template <typename R>
  void print(R&& range) 
  {
    for (auto x : range)
      cout << x << ' ';
    cout << '\n';
  }

template <typename R, typename T>
  bool eq(R&& range, std::initializer_list<T> list)
  {
    return equal(begin(range), end(range), begin(list));
  }



template <std::size_t N>
  struct slice_enumerator
  {
    // Create an enumerator over the slice. If limit is false, then set the
    // enumerator will start at the first element in the slice. Otherwise, the
    // enumerator will refer to a position past the end of the slice.
    slice_enumerator(const matrix_slice<N>& s, bool limit = false)
      : slice(s), offset(s.start)
    {
      fill_n(indexes, N, 0);
      if (limit) {
        indexes[0] = slice.extents[0];
        offset = slice.offset(indexes);
      } else {
        offset = s.start;
      }
    }


    // Create an enumerator referring to the element at dims.
    template <typename... Dims>
    slice_enumerator(const matrix_slice<N>& s, Dims... dims)
      : slice(s), indexes {std::size_t(dims)...}
    {
      static_assert(sizeof...(Dims) == N, "");
      offset = slice(dims...);
    }


    std::size_t operator*() const { return offset; }

    slice_enumerator& operator++() { advance(); return *this; }

    // Two enumerators compare equal when they are defined over the same
    // slice, and refer to the same offset.
    bool operator==(const slice_enumerator& x) const 
    {
      return offset == x.offset;
    }

    bool operator!=(const slice_enumerator& x) const
    {
      return !(*this == x);
    }

    void advance();

    // NOTE: I could use slice.start to count track the current offset in order
    // to reduce the overhead of an extra size_t, but it's a little more clear
    // do separate those.
    const matrix_slice<N>& slice; // The enumerated slice
    std::size_t indexes[N];      // An odometer-like array of indexes
    std::size_t offset;           // The current offset
  };

template <std::size_t N>
  void
  slice_enumerator<N>::advance()
  {
    std::size_t d = N - 1;
    while (true) {
      offset += slice.strides[d];
      ++indexes[d];

      // If have not yet counted to the extent of the current dimension, then
      // we will continue to do so in the next iteration.
      if (indexes[d] != slice.extents[d])
        break;

      // Otherwise, if we have not counted to the extent in the outermost
      // dimension, move to the next dimension and try again. If d is 0, then
      // we have counted through the entire slice.
      if (d != 0) {
        offset -= slice.strides[d] * slice.extents[d];
        indexes[d] = 0;
        --d;
      } else {
        break;
      }
    }
  }

template <std::size_t N>
  struct slice_enumerator_range
  {
    slice_enumerator_range(const matrix_slice<N>& s)
      : slice(s)
    { }

    slice_enumerator<N> begin() const { return {slice}; }
    slice_enumerator<N> end() const { return {slice, true}; }

    const matrix_slice<N>& slice;
  };

template <std::size_t N>
  inline slice_enumerator_range<N>
  enumerate(const matrix_slice<N>& slice) { return {slice}; }


int main()
{
  matrix_slice<3> s(0, {2, 4, 3});
  assert(eq(s.extents, {2, 4, 3}));
  assert(eq(s.strides, {12, 3, 1}));
  assert(s.size == 24);
  print(enumerate(s)); // [0, 24)

  // Slices of a row (in the 0th dimension)
  matrix_slice<2> r = s.row<0>(1);
  assert(eq(r.extents, {4, 3}));
  assert(eq(r.strides, {3, 1}));
  assert(r.size == 12);
  assert(r.start == 12);
  print(enumerate(r)); // 12 13 14
                       // 15 16 17
                       // 18 19 20
                       // 21 22 23

  matrix_slice<1> rr = r.row<0>(2);
  assert(eq(rr.extents, {3}));
  assert(eq(rr.strides, {1}));
  assert(rr.size == 3);
  assert(rr.start == 18);
  print(enumerate(rr)); // 18, 19, 20

  matrix_slice<1> rc = r.row<1>(1);
  assert(eq(rc.extents, {4}));
  assert(eq(rc.strides, {3}));
  assert(rc.size == 4);
  assert(rc.start == 13);
  print(enumerate(rc)); // 13, 16, 19, 22

  // Slices along a column (in the 1st dimension)
  matrix_slice<2> c = s.row<1>(2);
  assert(eq(c.extents, {2, 3}));
  assert(eq(c.strides, {12, 1}));
  assert(c.size == 6);
  assert(c.start == 6);
  print(enumerate(c));  // 6  7  8
                        // 18 19 20

  matrix_slice<1> cr = c.row<0>(1);
  assert(eq(cr.extents, {3}));
  assert(eq(cr.strides, {1}));
  assert(cr.size == 3);
  assert(cr.start == 18);
  print(enumerate(cr)); // 18 19 20

  matrix_slice<1> cc = c.row<1>(1);
  assert(eq(cc.extents, {2}));
  assert(eq(cc.strides, {12}));
  assert(cc.size == 2);
  assert(cc.start == 7);
  print(enumerate(cc)); // 7 19

  // Slice along the z-plane (in the 2nd dimension)
  matrix_slice<2> z = s.row<2>(1);
  assert(eq(z.extents, {2, 4}));
  assert(eq(z.strides, {12, 3}));
  assert(z.size == 8);
  assert(z.start == 1);
  print(enumerate(z)); // 1  4  7  10
                       // 13 16 19 22

  matrix_slice<1> zr = z.row<0>(1);
  assert(eq(zr.extents, {4}));
  assert(eq(zr.strides, {3}));
  assert(zr.size == 4);
  assert(zr.start == 13);
  print(enumerate(zr)); // 13 16 19 22

  matrix_slice<1> zc = z.row<1>(2);
  assert(eq(zc.extents, {2}));
  assert(eq(zc.strides, {12}));
  assert(zc.size == 2);
  assert(zc.start == 7);
  print(enumerate(zc)); // 7 19
}
