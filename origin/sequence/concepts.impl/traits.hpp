// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_SEQUENCE_CONCEPTS_HPP
#  error Do not include this file directly. Include range/concepts.hpp.
#endif

namespace sequence_impl
{
  // This class is used to induce failures for non-existent overloads of
  // ADL lookups. Any operations for that take this class will succeed
  // insantiation, but return subst_failure, indicating (obviously) failure.
  // See begin/end below.
  //
  // TODO: So far, this is a one-time thing, so I don't need to generalize it.
  // However, it might be nice to integrate this with the adl_t that's floating
  // around in other lib implementations and make this into a real framework.
  struct adl_fail_t
  {
    adl_fail_t(...);
  };

  subst_failure begin(adl_fail_t);
  subst_failure end(adl_fail_t);


  // Deducing the existence of begin/end is not straightforward. The operations
  // can be defined in one of two ways.
  //
  //    1. std::begin(r) and std::end(r) are valid expression. This means that
  //      a. r.begin() is defined, or
  //      b. r is an array.
  //    2. xxx::begin(r) and xxx::end(r) are valid expressions and expected
  //      to be found by ADL.
  //
  // If we cannot find a conforming std version of these expressions, then we
  // must resort to using ADL. In other words, the traits that deduce the
  // existence and type of the begin/end expressions is a manual two-phase
  // lookup.
  //
  // It is probably reasonable to assume that begin/end return the same type.
  // However, there have been some rumblings that people think otherwise. This
  // implementation allows the results of those expressions to be different
  // types, but the Range concept requires them to be the same (for now).


  // Safely deduce the result of the expression std::begin(r).
  template <typename T>
    struct get_std_begin_result
    {
    private:
      template <typename X>
        static auto check(X&& x) -> decltype(std::begin(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    

  // Safely deduce the result of the expression std::end(r).
  template <typename T>
    struct get_std_end_result
    {
    private:
      template <typename X>
        static auto check(X&& x) -> decltype(std::end(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };


  // Safely deduce the result of the expression xxx::begin(r) where xxx is
  // a non-std namespace and the type of r is defined in that namespace.
  template <typename T>
    struct get_adl_begin_result
    {
    private:
      template <typename X>
        static auto check(X&& x) -> decltype(begin(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };


  // Safely deduce the result of the expression xxx::end(r) where xxx is a 
  // non-std namespace and the type of r is defined in that namespace.
  template <typename T>
    struct get_adl_end_result
    {
    private:
      template <typename X>
        static auto check(X&& x) -> decltype(end(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };


  // Helper traits

  // Returns true if std::begin(r) is a valid expression.
  template <typename T>
    constexpr bool Has_std_begin()
    {
      return Subst_succeeded<typename get_std_begin_result<T>::type>();
    }


  // Returns true if std::end(r) is a valid expression.
  template <typename T>
    constexpr bool Has_std_end()
    {
      return Subst_succeeded<typename get_std_end_result<T>::type>();
    }


  // Deduce the result of applying the following statements:
  //
  //    using std::begin;
  //    begin(r);
  //
  template <typename T, bool = Has_std_begin<T>()>
    struct get_begin_result;

  template <typename T>
    struct get_begin_result<T, true>
    {
      using type = typename get_std_begin_result<T>::type;
    };

  template <typename T>
    struct get_begin_result<T, false>
    {
      using type = typename get_adl_begin_result<T>::type;
    };


  // Deduce the result of applying the following statements:
  //
  //    using std::end;
  //    end(r);
  //
  template <typename T, bool = Has_std_end<T>()>
    struct get_end_result;

  template <typename T>
    struct get_end_result<T, true>
    {
      using type = typename get_std_end_result<T>::type;
    };

  template <typename T>
    struct get_end_result<T, false>
    {
      using type = typename get_adl_end_result<T>::type;
    };



  // Support for safely deducing the iterator category. This happens in
  // one of two ways: either the class defines it as an associate type, or
  // I define it. There are no other ways.
  template <typename T>
    struct get_associated_iterator_category
    {
    private:
      template <typename X>
        static typename X::iterator_category check(const X&);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };


  // NOTE: It should not be the case that there are specializations of
  // iterator traits in user code. Specializing elements in std is typically
  // forbidden (and may result in undefined behavior?).
  template <typename T>
    struct iterator_category_traits
    {
      using type = typename get_associated_iterator_category<T>::type;
    };

  // Specialization for pointers.
  template <typename T>
    struct iterator_category_traits<T*>
    {
      using type = std::random_access_iterator_tag;
    };

  // Make sure that we can get the associated traits class even through
  // reference types.
  template <typename T>
    struct iterator_category_traits<T&>
      : iterator_category_traits<T>
    { };

  template <typename T>
    struct iterator_category_traits<T&&>
      : iterator_category_traits<T>
    { };

} // namespace origin

