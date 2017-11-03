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
  // Extended Function Traits
  //
  // The traits in this section provide support for querying properties of
  // functions and function arguments.


  //////////////////////////////////////////////////////////////////////////////
  // Invoke
  //
  // The following overloads provide a facility for evaluating the result of
  // the INVOKE definition in the C++ standard [func.require] (20.8.2/1). The
  // general syntax of the operation is
  //
  //    invoke::fn(f, args...)
  //
  // where f is a callable object and args is a list of arguments over f is 
  // called.
  //
  // If f is not callable, or f cannot be called with args, the result type of
  // the operation indicates substitution failure. If f is a member function
  // or member object pointer, then the first argument must be a an object or
  // reference to an object of f's defining class (or a class derived from
  // that).
  //
  // NOTE: This is not entirely dissimilar from the deduction systems used for
  // value type and size type, except that this definition is closed; it is
  // not possible to extend it. Wrapping the facility in a class guarantees
  // that it is the final definition and that uses of invoke do not participate
  // in ADL.

  struct invoke
  {
    // template <typename Ptr, typename... Args>
      // auto memfun(Ptr)


    // Matches none of the invocations.
    static subst_failure fn(...);

    // Invoke the member function pointer f.
    template <typename F, typename C, typename... Args>
      static auto 
      fn(F f, C&& c, Args&&... args)
        -> decltype((std::forward<C>(c).*f)(std::forward<Args>(args)...));

    // Invoke a pionter to a member function pointer f.
    template <typename F, typename C, typename... Args>
      static auto 
      fn(F f, C&& c, Args&&... args)
        -> decltype(((*std::forward<C>(c)).*f)(std::forward<Args>(args)...));

    // Invoke a member object pointer.
    template <typename P, typename C>
      static auto 
      fn(P p, C&& c) -> decltype(std::forward<C>(c).*p);

    // Invoke a pointer to a member object pointer.
    template <typename P, typename C>
      static auto 
      fn(P p, C&& c) -> decltype((*std::forward<C>(c)).*p);

    // Invoke function pointers and functors,
    template <typename F, typename... Args>
      static auto 
      fn(F&& f, Args&&... args)
        -> decltype(std::forward<F>(f)(std::forward<Args>(args)...));
  };


  // The invoke_result trait is the same as result_of, except that the callable
  // type and function arguments are given separately. It does not require the
  // type argument to be written as a call expression type.
  template <typename F, typename... Args>
    struct invoke_result
    {
      using type = decltype(invoke::fn(std::declval<F>(), std::declval<Args>()...));
    };

  // Specialization when no arguments are given. We have to avoid trying to
  // expand an empty argument list into declval!
  template <typename F>
    struct invoke_result<F>
    {
      using type = decltype(invoke::fn(std::declval<F>()));
    };

  // A wrapper on the invoke facility for convenience.
  template <typename F, typename... Args>
    using Invoke = typename invoke_result<F, Args...>::type;



  // The implementation of result_of type trait exists solely to enforce the
  // requirement that Result_of be given a call expression type. It also catches
  // some other errors that crop up from time to time.
  template <typename T>
    struct result_of
    {
      using type = subst_failure;
    };
    
  template <typename F, typename... Args>
    struct result_of<F(Args...)>
    {
      using type = Invoke<F, Args...>;
    };

  // This turned out to be a weird case that was generating a compilation
  // failure when none was expected. It's probably not the last of them.
  template <typename... Args>
    struct result_of<void(Args...)>
    {
      using type = subst_failure;
    };



  // Deduces the type of the expression &T::operator(), the address of the
  // T's function call operator. This will fail when T is a polymorphic
  // functor.
  template <typename T>
    struct get_call_operator_type
    {
    private:
      template <typename X>
        static auto check(X) -> decltype(&X::operator());

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };



  // Deduce the arity of a function type, function pointer, or member to
  // function pointer. If F is not one of those types, the resulting arity is
  // -1.
  template <typename F, bool = Class<F>()>
    struct function_arity
      : std::integral_constant<int, -1>
    { };

  // Specialization table for function_arity.
  template <typename R, typename... Args>
    struct function_arity<R(Args...), false>
      : std::integral_constant<int, sizeof...(Args)>
    { };

  // Specializations for callable types.
  template <typename R, typename... Args>
    struct function_arity<R(*)(Args...), false>
      : std::integral_constant<int, sizeof...(Args)>
    { };

  template <typename R, typename C, typename... Args>
    struct function_arity<R(C::*)(Args...), false>
      : std::integral_constant<int, sizeof...(Args)>
    { };

  template <typename R, typename C, typename... Args>
    struct function_arity<R(C::*)(Args...) const, false>
      : std::integral_constant<int, sizeof...(Args)>
    { };

  // For class types, this is defined recursively in terms of the member
  // call operator. Note that if the functor is polymorphic, this is going
  // to fail.
  template <typename F>
    struct function_arity<F, true>
      : function_arity<typename get_call_operator_type<F>::type>
    { };




  // Deduce the argument types from a function type.
  template <typename F, bool = Class<F>()>
    struct get_parameter_types
    {
      using type = subst_failure;
    };

  // There are some types for which the argument types are fairly easily
  // deduced., These are: function types, function pointer types, and
  // member function pointer types.
  template <typename R, typename... Args>
    struct get_parameter_types<R(Args...), false>
    {
      using type = type_list<Args...>;
    };

  // Specialization for callable types.
  template <typename R, typename... Args>
    struct get_parameter_types<R(*)(Args...), false>
    {
      using type = type_list<Args...>;
    };

  template <typename R, typename C, typename... Args>
    struct get_parameter_types<R(C::*)(Args...), false>
    {
      using type = type_list<Args...>;
    };

  template <typename R, typename C, typename... Args>
    struct get_parameter_types<R(C::*)(Args...) const, false>
    {
      using type = type_list<Args...>;
    };

  // For class types, the argument types are deduced from the member call
  // operator, if present.
  template <typename F>
    struct get_parameter_types<F, true>
      : get_parameter_types<typename get_call_operator_type<F>::type>
    { };



  // Get the result type of a function or callable type F. This is different
  // than result_of in that it does F is not a call expression type. This means
  // that the result type is not defined for polymorphic functors.
  template <typename F, bool = Class<F>()>
    struct get_result_type
    {
      using type = subst_failure;
    };

  template <typename R, typename... Args>
    struct get_result_type<R(Args...), false>
    {
      using type = R;
    };

  template <typename R, typename... Args>
    struct get_result_type<R(*)(Args...), false>
    {
      using type = R;
    };

  template <typename R, typename C, typename... Args>
    struct get_result_type<R(C::*)(Args...), false>
    {
      using type = R;
    };

  template <typename R, typename C, typename... Args>
    struct get_result_type<R(C::*)(Args...) const, false>
    {
      using type = R;
    };

  // For class types, the argument types are deduced from the member call
  // operator, if present.
  template <typename F>
    struct get_result_type<F, true>
      : get_result_type<typename get_call_operator_type<F>::type>
    { };

} // namespace type_impl
