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
  // Metaprogramming support
  //
  // The type functions in this module provide limited support for basic
  // metaprogramming tasks. None of the metafunctions in this module require
  // SFINAE support. These are purely type functions.
  ////////////////////////////////////////////////////////////////////////////



  // Return the first type in the given sequence of arguments. This is
  // undefined when Args... is empty.
  template <typename... Args> struct front_type;

  template <typename T, typename... Args>
    struct front_type<T, Args...>
    { 
      using type = T; 
    };

  // Adaptation for expandable types
  template <template <typename...> class Template, typename... Args>
    struct front_type<Expand<Template<Args...>>>
      : front_type<Args...>
    { };



  // Returns the last type in a sequence of type arguments. This is
  // undefined when Args... is empty.
  template <typename... Args> struct back_type;

  template <typename T>
    struct back_type<T>
    { 
      using type = T;
    };

  template <typename T, typename... Args>
    struct back_type<T, Args...> : back_type<Args...>
    { };

  // Adapatation for expandable types.
  template <template <typename...> class Template, typename... Args>
    struct back_type<Expand<Template<Args...>>>
      : back_type<Args...>
    { };



  // Returns true if all of the types are the same, or if Args... is an
  // empty sequence of types. Evaluation is of type equality is performed
  // left-to-right, and the operation will not instantiate arguments after
  // a non-equal pair is found.
  template <typename... Args> struct are_same;

  // True for an empty sequence
  template <>
    struct are_same<> : std::true_type  { };

  // For a single type, this is trivially true.
  template <typename T> 
    struct are_same<T> : std::true_type { };

  // Recursively apply are_same (is_same) to T, U and Args...
  template <typename T, typename U, typename... Args>
    struct are_same<T, U, Args...>
      : boolean_constant<
          std::is_same<T, U>::value &&
          are_same<U, Args...>::value
        >
    { };

  // Adapt are_same to the Expand wrapper. 
  template <template <typename... Args> class Template, typename... Args>
    struct are_same<Expand<Template<Args...>>>
      : are_same<Args...>
    { };


  // The identity metafunction for types.
  //
  // TODO: Move this into the origin namespace?
  template <typename T>
    struct identity
    {
      using type = T;
    };

} // namespace type_impl
