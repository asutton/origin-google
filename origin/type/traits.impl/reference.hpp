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
  ////////////////////////////////////////////////////////////////////////////
  // Reference Traits
  //
  // Safely form lvalue and rvalue references to the given type, if they can
  // be formed. Fail substitution in all other cases.
  //
  // FIXME: Are there other types besides void to which we cannot form
  // references?
  ////////////////////////////////////////////////////////////////////////////


  // Form an lvalue reference to T.
  template <typename T>
    struct require_lvalue_reference
      : std::add_lvalue_reference<T>
    { };

  template <>
    struct require_lvalue_reference<void>
    {
      using type = subst_failure;
    };


  // Form an rvalue reference to T.
  template <typename T>
    struct require_rvalue_reference
      : std::add_rvalue_reference<T>
    { };

  template <>
    struct require_rvalue_reference<void>
    {
      using type = subst_failure;
    };


} // namespace type_impl

