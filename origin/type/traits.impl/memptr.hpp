// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_TRAITS_HPP
#  error This file cannot be included directly. Include type/traits.hpp.
#endif

namespace type_impl
{
  //////////////////////////////////////////////////////////////////////////////
  // Member pointers

  // Type traits for pointers-to-member data and funtions. This class is
  // specialized for different kinds of member pointers. Note that the class
  // type is a value type; it is not cv-qualified as if it were a this 
  // pointer.
  //
  // TODO: Include "this" type as cv-qualified?
  template <typename Memptr>
    struct member_pointer_traits
    {
      using result_type = subst_failure;
      using class_type = subst_failure;
    };

  template <typename T, typename C>
    struct member_pointer_traits<T(C::*)>
    {
      using result_type = T;
      using class_type = C;
    };

  template <typename T, typename C, typename... Args>
    struct member_pointer_traits<T(C::*)(Args...)>
    {
      using result_type = T;
      using class_type = C;
    };

  template <typename T, typename C, typename... Args>
    struct member_pointer_traits<T(C::*)(Args...) const>
    {
      using result_type = T;
      using class_type = C;
    };

  template <typename T, typename C, typename... Args>
    struct member_pointer_traits<T(C::*)(Args...) volatile>
    {
      using result_type = T;
      using class_type = C;
    };

  template <typename T, typename C, typename... Args>
    struct member_pointer_traits<T(C::*)(Args...) const volatile>
    {
      using result_type = T;
      using class_type = C;
    };

} // namespace type_impl
