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
  // Integer Traits
  //
  // The std::make_signed and std::make_unsigned will fail when the argument
  // type is not integral. We must take care that this does not happen.
  ////////////////////////////////////////////////////////////////////////////


  // Returns the signed version of T if T is an Integer type. Otherwise, return
  // subst_failure.
  template <typename T, bool = Integer<T>()>
    struct make_signed;

  template <typename T>
    struct make_signed<T, true> : std::make_signed<T> { };
  
  template <typename T>
    struct make_signed<T, false> { using type = subst_failure; };


  // Return the unsgiend version of T if T is an Integer type. Otherwise returns
  // sbust_failure.
  template <typename T, bool = Integer<T>()>
    struct make_unsigned;

  template <typename T>
    struct make_unsigned<T, true> : std::make_unsigned<T> { };

  template <typename T>
    struct make_unsigned<T, false> { using type = subst_failure; };

} // namespace type_impl
