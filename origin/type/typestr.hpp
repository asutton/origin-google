// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_TYPESTR_HPP
#define ORIGIN_TYPE_TYPESTR_HPP

#if defined(__GNUC__)
#include <cxxabi.h>
#endif

#include <iostream>
#include <string>
#include <typeinfo>


namespace origin
{
  // Declarations.
  namespace type_impl
  {
    template <typename... Args> 
      struct typestr_dispatch;
    
    template <typename T> 
      struct type_to_string;
    
    template <typename T>
      struct type_to_string;

    template <typename... Args> 
      struct typelist_to_string;
    
    std::string to_string(const std::type_info& info);
  } // namespace type_impl



  //////////////////////////////////////////////////////////////////////////////
  // Typestr                                                        type.typestr
  //
  // Return the textual representation of a type. There are two overloads of
  // this function:
  //
  //    typestr<Args...>()
  //    typestr(args...);
  //
  // The first returns a string containing the names of the explicitly specified
  // type arguments. 
  //
  // The second returns the types of the given arguments. Note that the string
  // returned from second overload will reflect the type deduction rules for
  // perfect forwarding; lvalues arguments will be result in references, and
  // rvalues in unqualified value types.
  //
  // When multiple arguments are given, the resulting string is written in
  // initializer list format: "{T1, T2, ...}".
  template <typename... Args>
    inline std::string typestr()
    {
      return type_impl::typestr_dispatch<Args...>{}();
    }
    
  // Return a textual representation of the type name of the given argument.
  //
  template <typename... Args>
    inline std::string typestr(Args&&...)
    {
      return typestr<Args...>();
    }


#include "typestr.impl/typestr.hpp"

} // namespace origin

#endif
