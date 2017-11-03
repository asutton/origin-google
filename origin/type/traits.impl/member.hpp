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
  // Associated Member Types
  //
  // The following traits provide support for deducing (or not!) the explicitly
  // associated type names (e.g., T::foo_type).
  //////////////////////////////////////////////////////////////////////////////


  // Deduce the type expression T::size_type.
  template <typename T>
    struct get_associated_value_type
    {
    private:
      template <typename X>
        static typename X::value_type check(const X&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Deduce the type expression T::size_type.
  template <typename T>
    struct get_associated_difference_type
    {
    private:
      template <typename X>
        static typename X::difference_type check(const X&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Safely deduce the type expression T::size_type.
  template <typename T>
    struct get_associated_size_type
    {
    private:
      template <typename X>
        static typename X::size_type check(const X&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };


  // Safely deduce the associated type name T::reference.
  template <typename T>
    struct get_associated_reference
    {
    private:
      template <typename X>
        static typename X::reference check(const X&);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  
  // Safely deduce the associatd type name T::const_reference.
  template <typename T>
    struct get_associated_const_reference
    {
    private:
      template <typename X>
        static typename X::const_reference check(const X&);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    }; 


  // Safely deduce the associated type name T::pointer.
  template <typename T>
    struct get_associated_pointer
    {
    private:
      template <typename X>
        static typename X::pointer check(const X&);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  
  // Safely deduce the associatd type name T::const_pointer.
  template <typename T>
    struct get_associated_const_pointer
    {
    private:
      template <typename X>
        static typename X::const_pointer check(const X&);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    }; 



  //////////////////////////////////////////////////////////////////////////////
  // Common member functions
  //
  //
  //////////////////////////////////////////////////////////////////////////////

  // Safely deduce get the result type of the expression t.size().
  template <typename T>
    struct get_member_size_result
    {
    private:
      template <typename X>
        static auto check(const X& x) -> decltype(x.size());
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Safely deduce the result type of the expression t.empty().
  template <typename T>
    struct get_member_empty_result
    {
    private:
      template <typename X>
        static auto check(const X& x) -> decltype(x.empty());
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Safely deducethe result type of the expression t.find(u).
  //
  // NOTE: This was added here so that we can dispatch the find algorithm
  // to classes implementing associative data structures.
  template <typename T, typename U>
    struct get_member_find_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x.find(y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };



} // namespace type_impl
