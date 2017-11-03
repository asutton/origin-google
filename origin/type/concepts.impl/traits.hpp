// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_CONCEPTS_HPP
#  error This file cannot be included directly. Include type/concepts.hpp.
#endif

namespace type_impl
{
  //////////////////////////////////////////////////////////////////////////////
  // Overloaded Concept Implementations
  //
  // The followign traits implement overloaded concepts. Here, overloading is
  // done by specializaing on a template when the given types are the same
  // or one is given as default_t.
  //////////////////////////////////////////////////////////////////////////////


  // Returns true if T and U are cross-type equality comparable.
  template <typename T, typename U>
    struct is_equality_comparable
      : boolean_constant<
            Common<T, U>() 
         && Equality_comparable<T>()
         && Equality_comparable<U>()
         && Equality_comparable<Common_type<T, U>>()
         && Has_equal<T, U>()     && Boolean<Equal_result<T, U>>()
         && Has_equal<U, T>()     && Boolean<Equal_result<U, T>>()
         && Has_not_equal<T, U>() && Boolean<Equal_result<T, U>>()
         && Has_not_equal<U, T>() && Boolean<Equal_result<T, U>>()
        >
    { };
  
  // Returns true if T is equality comparable.
  template <typename T>
    struct is_equality_comparable<T, T>
      : boolean_constant<
            Has_equal<T>()     && Boolean<Equal_result<T>>()
         && Has_not_equal<T>() && Boolean<Not_equal_result<T>>()
        >
    { };



  // Returns true if T and U are cross-type weakly ordered.
  template <typename T, typename U>
    struct is_weakly_ordered
      : boolean_constant<
            Common<T, U>()
         && Totally_ordered<T>()
         && Totally_ordered<T>()
         && Totally_ordered<Common_type<T, U>>()
         && Has_less<T, U>()          && Boolean<Less_result<T, U>>()
         && Has_less<U, T>()          && Boolean<Less_result<U, T>>()
         && Has_greater<T, U>()       && Boolean<Greater_result<T, U>>()
         && Has_greater<U, T>()       && Boolean<Greater_result<U, T>>()
         && Has_less_equal<T, U>()    && Boolean<Less_equal_result<T, U>>()
         && Has_less_equal<U, T>()    && Boolean<Less_equal_result<U, T>>()
         && Has_greater_equal<T, U>() && Boolean<Greater_equal_result<T, U>>()
         && Has_greater_equal<U, T>() && Boolean<Greater_equal_result<U, T>>()
        >
    { };

  // Returns true if T is weakly ordered.
  template <typename T>
    struct is_weakly_ordered<T, T>
      : boolean_constant<
            Has_less<T>()          && Boolean<Less_result<T>>()
         && Has_greater<T>()       && Boolean<Greater_result<T>>()
         && Has_less_equal<T>()    && Boolean<Less_equal_result<T>>()
         && Has_greater_equal<T>() && Boolean<Greater_equal_result<T>>()
        >
    { };


  // Returns true if R is a cross-type relation over (T, U).
  template <typename R, typename T, typename U>
    struct is_relation
      : boolean_constant<
            Common<T, U>()
         && Relation<R, T>()
         && Relation<R, U>()
         && Relation<R, Common_type<T, U>>()
         && Predicate<R, T, U>()
         && Predicate<R, U, T>()
        >
    { };

  // Returns true if R is a relation over T.
  template <typename R, typename T>
    struct is_relation<R, T, T>
      : boolean_constant<Predicate<R, T, T>()>
    { };



  // Returns true if Op is a binary operation over T and U.
  template <typename R, typename T, typename U>
    struct is_binary_operation
      : boolean_constant<
            Common<T, U>()
         && Relation<R, T>()
         && Relation<R, U>()
         && Relation<R, Common_type<T, U>>()
         && Regular_function<R, T, U>() 
         && Same<Result_of<R(T, U)>, Common_type<T, U>>()
         && Regular_function<R, U, T>()
         && Same<Result_of<R(U, T)>, Common_type<U, T>>()
        >
    { };



  //////////////////////////////////////////////////////////////////////////////
  // Streaming
  //
  // Traits for streaming I/O concepts.
  //////////////////////////////////////////////////////////////////////////////


  // Safely deduce the result type of the expression s >> x, for any Stream s.
  template <typename S, typename T>
    struct get_input_stream_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y& y) -> decltype(x >> y);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<S&>(), std::declval<T&>()));
    };


  // A type trait that determines if s >> t is valid (or cin >> t).
  template <typename S, typename T>
    struct is_input_streamable
      : boolean_constant<
          Subst_succeeded<typename get_input_stream_result<S, T>::type>()
        >
    { };

  template <typename T>
    struct is_input_streamable<T, default_t>
      : is_input_streamable<std::istream, T>
    { };



  // Safely deduce the result type of the expression s << x, for any Stream s.
  template <typename S, typename T>
    struct get_output_stream_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y const& y) -> decltype(x << y);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<S&>(), std::declval<T const&>()));
    };


  // A type trait that determines if s << t is valid (or cin << t).
  template <typename S, typename T>
    struct is_output_streamable
      : boolean_constant<
          Subst_succeeded<typename get_output_stream_result<S, T>::type>()
        >
    { };

  template <typename T>
    struct is_output_streamable<T, default_t>
      : is_output_streamable<std::ostream, T>
    { };

} // namespace type_impl
