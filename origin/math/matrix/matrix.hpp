// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#define ORIGIN_MATH_MATRIX_HPP


#include <cassert>
#include <algorithm>
#include <array>
#include <numeric>

#include <origin/type/concepts.hpp>
#include <origin/type/typestr.hpp>
#include <origin/sequence/algorithm.hpp>

namespace origin
{
  // Declarations
  struct slice;
  template <std::size_t N> class matrix_slice;
  template <typename T, std::size_t N> class matrix;
  template <typename T, std::size_t N> class matrix_ref;


// Type traits implementations
#include "matrix.impl/traits.hpp"


  // Returns true if T is a matrix.
  //
  // FIXME: A matrix is substantially more complex than this. Finish defining
  // and implementing the concept. There is an interesting design question
  // here. What are the minimum requirements of a Matrix? Surely we can't 
  // require all arithmetic operations.
  template <typename M>
    constexpr bool Matrix()
    {
      return matrix_impl::Has_order<M>();
    }


  // The matrix initializer is a sequence of nested initializer lists that
  // describes the initailization structure of an N dimensional matrix. For
  // example, an initializer for a 2D matrix of ints allows us to construct 
  // the following initializer list:
  //
  //    {{0, 1},
  //     {2, 3},
  //     {4, 5}}
  //
  // Assuming we have a 3x2 matrix.
  template <typename T, std::size_t N>
    using matrix_initializer = typename matrix_impl::matrix_init<T, N>::type;


// Support classes and algorithms
#include "matrix.impl/slice.hpp"
#include "matrix.impl/iterator.hpp"
#include "matrix.impl/support.hpp"

// Matrix classes
#include "matrix.impl/matrix.hpp"
#include "matrix.impl/matrix_ref.hpp"

// Arithmetic and linear operations
#include "matrix.impl/operations.hpp"


} // namespace origin

#endif
