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
  // Overloadable operators

  // Deduce the result of the expression t == u.
  template <typename T, typename U>
    struct get_equal_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x == y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t != u.
  template <typename T, typename U>
    struct get_not_equal_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x != y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t < u.
  template <typename T, typename U>
    struct get_less_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x < y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t > u.
  template <typename T, typename U>
    struct get_greater_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x > y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t <= u.
  template <typename T, typename U>
    struct get_less_equal_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x <= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t >= u.
  template <typename T, typename U>
    struct get_greater_equal_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x >= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t + u.
  template <typename T, typename U>
    struct get_plus_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x + y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t - u.
  template <typename T, typename U>
    struct get_minus_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x - y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t * u.
  template <typename T, typename U>
    struct get_multiplies_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x * y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t / u.
  template <typename T, typename U>
    struct get_divides_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x / y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };
  
  // Deduce the result type of the expression t % u.
  template <typename T, typename U>
    struct get_modulus_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x % y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result of +t.
  template <typename T>
    struct get_unary_plus_result
    {
    private:
      template <typename X>
        static auto check(X&& a) -> decltype(+a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Deduce the result of -t.
  template <typename T>
    struct get_unary_minus_result
    {
    private:
      template <typename X>
        static auto check(X&& a) -> decltype(-a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Deduce the result type of the expression t += u.
  template <typename T, typename U>
    struct get_plus_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x += y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t -= u.
  template <typename T, typename U>
    struct get_minus_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x -= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t *= u.
  template <typename T, typename U>
    struct get_multiplies_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x *= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t /= u.
  template <typename T, typename U>
    struct get_divides_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x /= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t %= u.
  template <typename T, typename U>
    struct get_modulus_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x %= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result of ++t;
  template <typename T>
    struct get_pre_increment_result
    {
    private:
      template <typename X>
        static auto check(X& x) -> decltype(++x);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>()));
    };

  // Deduce the result of t++;
  template <typename T>
    struct get_post_increment_result
    {
    private:
      template <typename X>
        static auto check(X& x) -> decltype(x++);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>()));
    };

  // Deduce the result of --t;
  template <typename T>
    struct get_pre_decrement_result
    {
    private:
      template <typename X>
        static auto check(X& x) -> decltype(--x);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>()));
    };
    
  
  // Deduce the result of t--;
  template <typename T>
    struct get_post_decrement_result
    {
    private:
      template <typename X>
        static auto check(X& x) -> decltype(x--);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>()));
    };

  // Deduce the result type of the expression t && u.
  template <typename T, typename U>
    struct get_and_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x && y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };


  // Deduce the result type of the expression t || u.
  template <typename T, typename U>
    struct get_or_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x || y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result of !t.
  template <typename T>
    struct get_not_result
    {
    private:
      template <typename X>
        static auto check(X&& a) -> decltype(!a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Deduce the result type of the expression t & u.
  template <typename T, typename U>
    struct get_bit_and_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x & y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t | u.
  template <typename T, typename U>
    struct get_bit_or_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x | y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };


  // Deduce the result type of the expression t ^ u.
  template <typename T, typename U>
    struct get_bit_xor_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x ^ y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };
  
  // Deduce the result type of the expression t << u.
  template <typename T, typename U>
    struct get_left_shift_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x << y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t >> u.
  template <typename T, typename U>
    struct get_right_shift_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x >> y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // Deduce the result of ~t.
  template <typename T>
    struct get_complement_result
    {
    private:
      template <typename X>
        static auto check(X&& a) -> decltype(~a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Deduce the result type of the expression t &= u.
  template <typename T, typename U>
    struct get_bit_and_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x &= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t |= u.
  template <typename T, typename U>
    struct get_bit_or_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x |= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t ^= u.
  template <typename T, typename U>
    struct get_bit_xor_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x ^= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t <<= u.
  template <typename T, typename U>
    struct get_left_shift_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x <<= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result type of the expression t >>= u.
  template <typename T, typename U>
    struct get_right_shift_assign_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x >>= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // Deduce the result of the expression &t.
  template <typename T>
    struct get_address_result
    {
    private:
      template <typename X>
        static auto check(X&& a) -> decltype(&a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Deduce the result of the expression *t.
  template <typename T>
    struct get_dereference_result
    {
    private:
      template <typename X>
        static auto check(X&& a) -> decltype(*a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // Deduce the result of the exprssion t[u].
  template <typename T, typename U>
    struct get_subscript_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x[y]);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

} // namespace type_impl
