// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif

namespace matrix_impl
{
  // Safely deduce the result type of the expression x.order.
  template <typename T>
    struct get_order_type
    {
    private:
      template <typename X>
        static auto check(const X& x) -> decltype(x.order);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Returns true if the experssion x.order is valid.
  template <typename T>
    constexpr bool Has_order()
    {
      return Subst_succeeded<typename get_order_type<T>::type>();
    }



  // The matrix init trait defines the matrix initializer list for any matrix
  // with order > 1.
  template <typename T, std::size_t N>
    struct matrix_init
    {
      using type = std::initializer_list<typename matrix_init<T, N - 1>::type>;
    };

  template <typename T>
    struct matrix_init<T, 1>
    {
      using type = std::initializer_list<T>;
    };

  // This is not defined on purpose!
  template <typename T>
    struct matrix_init<T, 0>;




  // Returns true if Args is an index sequence. An index sequence is a list
  // of integral types that can all be converted to size_t.
  template <typename... Args>
    constexpr bool Index_sequence()
    {
      return All(Convertible<Args, std::size_t>()...);
    }

  // Returns true if Args is a slice sequence. A slice sequence is a list of
  // slice requests, comprised of indexes (types convertible to size_t), and
  // at least one slice types. Note that a slice sequence comprised of all
  // indexes is an index sequence.
  template <typename... Args>
    constexpr bool Slice_sequence()
    {
      return All((Convertible<Args, std::size_t>() || Same<Args, slice>())...)
          && Some(Same<Args, slice>()...);
    }



  template <typename... Args>
    struct count_slices;

  template <>
    struct count_slices<>
    {
      static constexpr std::size_t value = 0;
    };

  template <typename T, typename... Args>
    struct count_slices<T, Args...>
    {
      static constexpr std::size_t value = 
        Same<T, slice>() + count_slices<Args...>::value;
    };

  template <typename... Args>
    constexpr std::size_t Count_slices()
    {
      return count_slices<Args...>::value;
    }

  template <typename T, std::size_t N, typename... Args>
    using Slice_result = matrix_ref<T, Count_slices<Args...>()>;


} // namespace matrix_impl

