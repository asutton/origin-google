// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_TRAITS_HPP
#define ORIGIN_TYPE_TRAITS_HPP

#include <type_traits>
#include <utility>

#include "default.hpp"

namespace origin
{

  //////////////////////////////////////////////////////////////////////////////
  // Metaprogramming Support                                                meta
  //
  // The following construccts extend the std type traits, providing new 
  // features, aliases, and functiohns.
  //////////////////////////////////////////////////////////////////////////////



  // ------------------------------------------------------------------------ //
  // Boolean Constant                                                [meta.bool]
  //
  // An alias for the type integral_constant<bool, X>. This type is provided
  // purely for convenience. This simplifies the writing of some type traits.
  // For example:
  //
  //    template <std::size_t N>
  //      struct equal_zero : boolean_constant<N == 0> { };
  //
  // Origin also includes the aliases True_type and False_type, which provide
  // uniform naming for the associated Boolean constants.
  template <bool B>
    using boolean_constant = std::integral_constant<bool, B>;


  // The True_type and False_type aliases provide alternative names for the
  // true_type and false_type traits in the C++ standard library. These
  // exist solely for the purose of adapting those names to the Origin style.
  using True_type = std::true_type;
  using False_type = std::false_type;



  // ------------------------------------------------------------------------ //
  // Size Constant                                                   [meta.size]
  //
  // An alias for integral_constant<size_t, N>.
  template <std::size_t N>
    using size_constant = std::integral_constant<std::size_t, N>;



  //////////////////////////////////////////////////////////////////////////////
  // Type List
  //
  // The type_list class template describes a sequence of types, much like a a
  // tuple. The similarities end there; type_list is an empty class that only
  // serves to carry type information.Also, std::tuple<> does not seem to be a
  // complete type whereas type_list<> is.
  template <typename... Ts>
    struct type_list { };



  //////////////////////////////////////////////////////////////////////////////
  // Expand
  //
  // The expand class template is a helper class that can be used to indicate
  // the explicit expansion of variadic class templates such as type_list and
  // tuple in the context of variadic type functions such as Same or Common.
  //
  // Example:
  //
  //    using T1 = tuple<char, short, int>;
  //    using T2 = type_list<char, short, int>;
  //    Same<Expand<T1>>() // same as Same<char, short, int>()
  //    Same<Expand<T2>>() // same as Same<char, short, int>()
  //
  // Note that the Expand facility works with any type define over a sequence
  // of types. We call such types (e.g., type_list and tuple), "expandable 
  // types".
  //
  // Note that the implementation of Same must be explicitly adapted to use
  // the Expand wrapper. As an example, here is the specialization of are_same,
  // the implementation of the Same type predicate, that adapts the type trait
  // for use with an expandable sequence.
  //
  //    template <template <typename...> class Template, typename... Args>
  //      struct are_same<Expand<Template<Args...>>>
  //        : are_same<Args...>
  //      { };
  //
  // The specialization includes both the template name (either type_list or
  // tuple from the example above), and the sequence of types that being
  // expanded in the type trait.
  template <typename T>
    struct Expand { };


// Include metaprogramming support.
#include "traits.impl/meta.hpp"


  //////////////////////////////////////////////////////////////////////////////
  // Requires                                                      meta.requires
  //
  // Refers to T, only if B is true. If B is false, the alias results in a
  // substitution failure, eliminating an overload or specialization from a
  // candidate set. For example:
  //
  //    template <typename T>
  //      Requires<Integer<T>(), T> odd(T n) { return T & 1; }
  //
  // Here, the operation, odd, is only a viable candidate when the expression
  // Integer<T>() evaluates to true.
  //
  // Template Parameters:
  //    B -- A Boolean constant
  //    T -- The aliased type when B is true
  //
  // Refers To:
  //    T if B is true, otherwise the alias results in a substitution failure.
  template <bool B, typename T = void>
    using Requires = typename std::enable_if<B, T>::type;




  //////////////////////////////////////////////////////////////////////////////
  // If                                                                  meta.if
  //
  // The If alias refers to T when B is true and F otherwise. For example:
  //
  //    template<typename T>
  //      using Ptr = If<Const<T>(), const T*, T*>;
  //
  //    Ptr<int>       // is int*
  //    Ptr<const int> // is const int*
  //
  // Note that this is an alias to std::conditional.
  //
  // Template Parameters:
  //    B -- A Boolean constant
  //    T -- The aliased type when B is true
  //    F -- The aliased type when B is false
  //
  // Refers To:
  //    T if B is true, F otherwise.
  template <bool B, typename T, typename F>
    using If = typename std::conditional<B, T, F>::type;
  


  // FIXME: Move into impl.
  template <std::size_t N, typename... Args>
    struct selection;

  template <std::size_t N, typename T, typename... Args>
    struct selection<N, T, Args...>
      : selection<N - 1, Args...>
    { };
  
  template <typename T, typename... Args>
    struct selection<0, T, Args...>
    {
      using type = T;
    };

  //////////////////////////////////////////////////////////////////////////////
  // Select                                                          meta.select
  //
  // The Select alias refers to the Nth type is a sequence of Args. It is a
  // generalization of if If, to N values. For example:
  //
  //    Select<0, char, short, int, long> // is char
  //    Select<1, char, short, int, long> // is short
  //
  // Note that the value of N must be less than the number of types in Args.
  // If not, the program is ill-formed.
  //
  // Template Parameters:
  //    N -- The nth type
  //    Args -- A list of types
  //
  // Refers To:
  //    The Nth type in Args
  //
  // Requires:
  //    N < sizeof...(Args)
  template <std::size_t N, typename... Args>
    using Select = typename selection<N, Args...>::type;




  //////////////////////////////////////////////////////////////////////////////
  // Structural Type
  //
  // The Structural_type alias identifies the structure or value type in a
  // cv-qualified reference type T. The structural type of a cv-qualified or
  // reference type is the underlying type of the cv-qualifiers or reference.
  // That is, it describes the structure of the object being constrained. For
  // example:
  //
  //    Structural_type<int>        // is int
  //    Structural_type<const int>  // is int
  //    Structural_type<const int&> // is int
  //    Structural_type<int*>       // is int*
  //    Structural_type<int[3]>     // is int[3]
  //
  // Many type functions in this library operate only on or return only
  // structural types. This alias is provided specifically to derive structural
  // types from cv-qualified or reference type expressions.
  //
  // Note that the Value_type alias has similar meaning. It refers to a 
  // structural type associated with an iterator or range.
  //
  // Template Parameters:
  //    T -- A type
  //
  // Refers To:
  //    The structural type underlying the type T.
  template <typename T>
    using Structural_type
      = typename std::remove_cv<
          typename std::remove_reference<T>::type
        >::type;



  //////////////////////////////////////////////////////////////////////////////
  // Front Type                                                       meta.front
  //
  // An alias to the first type in a non-empty sequence of type arguments. For
  // example:
  //
  //    Front_type<char, short, int> // is char
  template <typename... Args>
    using Front_type = typename type_impl::front_type<Args...>::type;



  //////////////////////////////////////////////////////////////////////////////
  // Back Type                                                         meta.back
  //
  // An alias to the last type in a non-empty sequence of type arguments. For
  // example:
  //
  //    Back_type<char, short, int> // is int
  template <typename T, typename... Args>
    using Back_type = typename type_impl::back_type<T, Args...>::type;
    



  //////////////////////////////////////////////////////////////////////////////
  // SFINAE Support                                                  meta.sfinae
  //
  // Origin provides a small set of types and type functions that are used to
  // provide a framwork for managing substitution failure in lookup. The
  // Primary feature of this component is the subst_failure class, a type that
  // indicates when a lookup has failed.
  //
  // There are two associated type predicates that can be used to determine
  // whether a result type indicates substitution failure.
  //
  //    Subst_failed<T>()    -- True if T indicates substitution failure
  //    Subst_succeeded<T>() -- True when T is not a substitution failure


  // The subst_failure class is an empty class that is returned by type traits
  // to indicate substitution failure.
  struct subst_failure { };


  // Returns true if T indicates a substitution failure.
  template <typename T>
    constexpr bool Subst_failed() 
    { 
      return std::is_same<T, subst_failure>::value;
    }


  // Returns true if T does not indicate a substitution failure.
  template <typename T>
    constexpr bool Subst_succeeded() 
    { 
      return !std::is_same<T, subst_failure>::value;
    }


// Include operator support.
#include "traits.impl/operators.hpp"


  //////////////////////////////////////////////////////////////////////////////
  // Type predicates
  //
  // The following type functions define predicates over types.
  //////////////////////////////////////////////////////////////////////////////


  // ------------------------------------------------------------------------ //
  //                                All
  //
  // Returns true if every argument is true or if no arguments are given.
  // This function can be used to evaluate a concept or type constraint for all
  // types in a template parameter pack. For example:
  //
  //    static_assert(All(Equality_comparable<Args>()...), "")
  //
  // where Equality_comparable is a concept taking a single argument.
  //
  // Note that all arguments must be convertible to bool.
  constexpr bool All() { return true; }

  template <typename... Args>
    constexpr bool All(bool b, Args... args)
    {
      return b && All(args...);
    }


  // ------------------------------------------------------------------------ //
  //                                Some
  //
  // Returns true if some (at least one) argument is true.

  constexpr bool Some() { return false; }

  template <typename... Args>
    constexpr bool Some(bool b, Args... args)
    {
      return b || Some(args...);
    }


  // ------------------------------------------------------------------------ //
  //                                None
  //
  // Returns true if every argument is false or if no arguments are given.
  //
  // Note that all arguments must be convertible to bool.

  constexpr bool None() { return true; }

  template <typename... Args>
    constexpr bool None(bool b, Args... args)
    {
      return !b && None(args...);
    }



  //////////////////////////////////////////////////////////////////////////////
  // Same
  //
  // The Same type predicate returns true when all types T in Args are the same
  // type, or when Args is an empty list. For example:
  //
  //    Same<int, int>()       // returns true
  //    Same<int, const int>() // returns false
  //    Same<int, int&>()      // returns false
  //    Same<char>()           // returns true
  //    Same<>()               // returns true
  //
  // This is called the "same type relation" when used with only two arguments.
  //
  // Template Parameters:
  //    Args -- A list of types
  //
  // Returns:
  //    True if and only if all types T in Args are the same type.
  template <typename... Args>
    constexpr bool Same()
    {
      return type_impl::are_same<Args...>::value;
    }
    


  //////////////////////////////////////////////////////////////////////////////
  // Different
  //
  // Returns true if any type T in Args is not the same as any other type U in
  // Args. The predicate returns false when Args is empty or contains only a
  // single type. For example:
  //
  //    Different<int, int>()       // returns false
  //    Different<char, int>()      // returns true
  //    Different<int, const int>() // returns true
  //    Different<T>()              // returns false
  //    Different<>()               // returns false
  //
  // Note that Different is the complement of Same. That is:
  //
  //    Different<T, U>() <=> !Same<Args...>()
  //
  // Template Parameters:
  //    Args -- A list of types
  //
  // Returns:
  //    True if, for any pair of types, T and U in Args, Same<T, U> is false.
  template <typename... Args>
    constexpr bool Different()
    {
      return !Same<Args...>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Common Type Trait
  //
  // The common_type trait defines the basic mechanism by which the common type
  // lattice is defined. The type trait is part of the public interface because
  // it can be specialized by a user to disambiguate conversions or to derive a
  // new type that is not one of the type arguments. For example, if T and U are
  // mutually convertible, common_type must be specialized to select on of those
  // conversions as being dominant. If the common type C of T and U is neither
  // of those types, the common_type must be specialized to derive C. An example
  // of this is std::duration.
  //
  // In general, it is preferable to avoid explicit specialization of this 
  // relation. The first approach to defining an appropriate conversion lattice
  // is to use the standard mechanisms for conversion.
  //
  // Notes: This supercedes the std implementation of common type, which will
  // can result in compilation failures when the common type of T and U is not
  // defined. Also note tht this is not in the traits namespace to allow for
  // easier customization.
  //
  // FIXME: This needs to be explicitly specialized for std::duration.

  template <typename... Args>
    struct common_type;

  // The common type of a single type is obviously that type.
  template <typename T>
    struct common_type<T>
    {
      using type = T;
    };

  // The common type relation. This specialization is the primary mechanism
  // by which common type is defined, and by which it is extended. The default
  // definition of common type is based on the result type of the ?: operator.
  template <typename T, typename U>
    struct common_type<T, U>
    {
    private:
      template <typename X, typename Y>
        static auto check(X&& a, Y&& b) -> decltype(true ? a : b);

      static subst_failure check(...);

      using C = decltype(check(std::declval<T>(), std::declval<U>()));
    public:
      using type = typename std::remove_const<
        typename std::remove_reference<C>::type
      >::type;
    };

  // When the type arguments are the same, we can bypass the deduction on
  // the conditional operator. This should make compilation slightly faster
  // in those cases.
  template <typename T>
    struct common_type<T, T>
    {
      using type = typename std::remove_const<
        typename std::remove_reference<T>::type
      >::type;
    };

  // The recursive definition of common type simply applies the common type
  // to each pair of types in turn. The computation is similar to that of
  // max_elements where the "max" type is the common type of each consecutive
  // pair of elements.
  template <typename T, typename U, typename... Args>
    struct common_type<T, U, Args...>
      : common_type<typename common_type<T, U>::type, Args...>
    { };

  // Adaptation for expandable types
  template <template <typename...> class Template, typename... Args>
    struct common_type<Expand<Template<Args...>>>
      : origin::common_type<Args...>
    { };



  //////////////////////////////////////////////////////////////////////////////
  // Commmon Type
  //
  // The common type alias yields a type, C, that is common to each of the
  // argument types in the list Args. By "common" we mean that each type in
  // Args can be embedded in the universe of values represented by the common
  // type C. For example, the common type of char, short, and int is int because
  // the values of each argument type can be represented by int.
  //
  // More specifically, each argument Args_i must be unambiguously converted to
  // C. That is, an implicit conversion from Args_i to C is possible, but not
  // from C to Args_i. Note that the common_type specialization table may be
  // used disambiguate conversions (in some cases the C++ language
  // disambiguates).
  //
  // Note that the common type of Args_n may not be any of the types in the list
  // Args. In such cases, the common type must be computed from the arguments.
  // This is only possible using the common_type specialization table.
  //
  // Template Parameters:
  //    Args -- A list of types
  //
  // Refers To:
  //    A type C that is common tyoe all types T in Args.
  template <typename... Args>
    using Common_type = typename common_type<Args...>::type;


  //////////////////////////////////////////////////////////////////////////////
  // Commmon
  //
  // The Common type predicate returns true when there exists a type C that
  // is common to all types T in Args. We refer to this as the "common type
  // relation" when used with only two types.
  //
  // Template Parameters:
  //    Args -- A list of types
  //
  // Returns:
  //    True if and only there is a type C common to each type T in Args.
  template <typename... Args>
    constexpr bool Common() 
    { 
      return Subst_succeeded<Common_type<Args...>>(); 
    }

    

  // Returns true if T is convertible to U.
  //
  // FIXME: Convertibility is not particularly well-defined in the abstract
  // sense. It convers some forms of construction and casting, but does not
  // include the notion of assignment. Convertible should be analogous to
  // Copyable, in that the a value of type U can be constructed over or assigned
  // to a value of type T and the result of that operation should preserve the
  // value of the original.
  template <typename T, typename U>
    constexpr bool Convertible() { return std::is_convertible<T, U>::value; }



  // Returns true if T is derived from U.
  template <typename T, typename U>
    constexpr bool Derived() { return std::is_base_of<U, T>::value; }    

    
    
  //////////////////////////////////////////////////////////////////////////////   
  // Type queries and transformations
  //
  // The following type traits query a type for classification or property.
  // Type transformations return a type name.
  //////////////////////////////////////////////////////////////////////////////


  // An alias to the type argument. One use of the Identity alias is inhibit
  // perfect forwarding for rvalue parameters. For example:
  //
  //    template <typename T>
  //      void f(Identity<T>&& x) { ... }
  //
  //    A a = ... // some value
  //    f(a)
  //
  // The compiler will deduce T as the type A (resulting in an rvalue reference
  // to A) instead of forwarding the function argument x as A&.
  template <typename T>
    using Identity = typename type_impl::identity<T>::type;


    
  // Returns true if T is void, and false otherwise.
  template <typename T>
    constexpr bool Void() { return std::is_void<T>::value; }



  //////////////////////////////////////////////////////////////////////////////
  // Integer types
  //
  // The following traits apply to integer types.
  //  
  //
  // FIXME: There is an issue with the classification of bool as an integer type
  // in that it is neither signed nor unsigned. This can lead to compilation
  // failures when using the Make_unsigned or Make_signed type traits when bool
  // is accepted as an argument. A reasonable solution would be to abstract the
  // language's characterization of Integer types and exclude bool.
  //////////////////////////////////////////////////////////////////////////////

  // Returns true if T is a signed or unsigned, possibly cv-qualified, bool,
  // char, short, int, long or long long.
  template <typename T>
    constexpr bool Integer() { return std::is_integral<T>::value; }
  
  // Returns true if T is a signed type. Note that floating point values
  // are signed.
  template <typename T>
    constexpr bool Signed() { return std::is_signed<T>::value; }

  // Returns true if T is an unsigned type.
  template <typename T>
    constexpr bool Unsigned() { return std::is_unsigned<T>::value; }


// Include alternative implementations for make_unsigned, make_signed.
#include "traits.impl/integer.hpp"


  // An alias for the unsigned integral type with the same width as T.
  template <typename T>
    using Make_unsigned = typename type_impl::make_unsigned<T>::type;

  // An alias for the signed integral type with the same width as T.
  template <typename T>
    using Make_signed = typename type_impl::make_signed<T>::type;
    
  

  //////////////////////////////////////////////////////////////////////////////
  // Integer types
  //
  // The following traits apply to floating types.
  //////////////////////////////////////////////////////////////////////////////
    
  
  // Returns true if T is a float, double, or long double.
  template <typename T>
    constexpr bool Floating_point() { return std::is_floating_point<T>::value; }



  //////////////////////////////////////////////////////////////////////////////
  // Array types
  //
  // The following traits apply to array types.
  //////////////////////////////////////////////////////////////////////////////
  

  // Return true if T is an array type of type U[N].
  template <typename T>
    constexpr bool Array() { return std::is_array<T>::value; }

  // Returns the number of dimensions of the array type T. If T is not an
  // Array, it has 0 dimensions. For example:
  //
  //    using A1 = int[3][2];
  //    static_assert(Rank<A1>() == 2);
  //  
  //    using A2 = int[5];
  //    static_assert(Rank<A2>() == 1);
  //
  //    using A3 = int;
  //    static_assert(Rank<A3>() == 0);
  //
  template <typename T>
    constexpr unsigned Rank() { return std::rank<T>::value; }

  // Returns the extent of the array type T in the Ith dimension. The extent
  // of an array is the number of subobjects allocated in that dimensions. For
  // example:
  //
  //    using A = int[3][2];
  //    static_assert(Extent<A, 0> == 3, "");
  //    static_assert(Extent<A, 1> == 2, "");
  //
  // If the dimension I is not given, it defaults to 0.
  template <typename T, unsigned I = 0>
    constexpr std::size_t Extent() { return std::extent<T, I>::value; }

  // An alias to the array type T with its outermost extent removed. For
  // example:
  //
  //    using A = int[3][2];
  //    using B = Remove_extent<A>;
  //    static_assert(Same<B, int[3]>(), "");
  template <typename T>
    using Remove_extent = typename std::remove_extent<T>::type;
    
  // An alias to the underlying value type of the array type T. For example:
  //
  //    using A = int[3][2];
  //    using B = Remove_all_extents<A>;
  //    static_assert(Same<B, int>(), "");
  template <typename T>
    using Remove_all_extents = typename std::remove_all_extents<T>::type;



  //////////////////////////////////////////////////////////////////////////////
  // Pointer types
  //
  // The following traits apply to pointer types.
  //////////////////////////////////////////////////////////////////////////////
    
  // Returns true if T is a pointer.
  template <typename T>
    constexpr bool Pointer() { return std::is_pointer<T>::value; }
  
  // An alias to a pointer-to-T.
  template <typename T>
    using Add_pointer = typename std::add_pointer<T>::type;

  // An alias to the underlying type U if T is a pointer-to-U.
  template <typename T>
    using Remove_pointer = typename std::remove_pointer<T>::type;
    

  
  //////////////////////////////////////////////////////////////////////////////
  // Reference types
  //
  // The following traits apply to pointer types.
  //////////////////////////////////////////////////////////////////////////////

// Additional traits needed for forming references.
#include "traits.impl/reference.hpp"

  // Returns true if T is an lvalue reference to some type U.
  template <typename T>
    constexpr bool Lvalue_reference() { return std::is_lvalue_reference<T>::value; }

  // Returns true if T is rvalue reference to some type U.
  template <typename T>
    constexpr bool Rvalue_reference() { return std::is_rvalue_reference<T>::value; }
    
  // Returns true if T is either an lvalue or rvalue reference to some type U.
  template <typename T>
    constexpr bool Reference() { return std::is_reference<T>::value; }
    
  // An alias to an lvalue-reference-to-T.
  template <typename T>
    using Add_lvalue_reference = typename std::add_lvalue_reference<T>::type;

  // An alias to an rvalue-reference-to-T.
  template <typename T>
    using Add_rvalue_reference = typename std::add_rvalue_reference<T>::type;

  // An alias to a type U if T is a reference-to-T.
  template <typename T>
    using Remove_reference = typename std::remove_reference<T>::type;


  // Refers to an lvalue-reference-to-T if it can be formed. Otherwise, the type
  // function indicates substitution failure.
  template <typename T>
    using Require_lvalue_reference = 
      typename type_impl::require_lvalue_reference<T>::type;


  // Refers to an lvalue-reference-to-T if it can be formed. Otherwise, the type
  // function indicates substitution failure.
  template <typename T>
    using Require_rvalue_reference = 
      typename type_impl::require_rvalue_reference<T>::type;



    
  //////////////////////////////////////////////////////////////////////////////
  // User-defined Types
  //
  // These traits describe properties of user-defined types: class types, union
  // types, and enumerated types.
  //////////////////////////////////////////////////////////////////////////////

  // Returns true if T is a class or struct.
  template <typename T>
    constexpr bool Class() { return std::is_class<T>::value; };

  // Returns true if T is a union.
  template <typename T>
    constexpr bool Union() { return std::is_union<T>::value; }
    
  // Returns true if T has no member variables.
  template <typename T>
    constexpr bool Empty() { return std::is_empty<T>::value; }

  // Returns true if T has at least one virtual method.
  template <typename T>
    constexpr bool Polymorphic() { return std::is_polymorphic<T>::value; }
    
  // Returns true if T has at least one pure virtual method.
  template <typename T>
    constexpr bool Abstract() { return std::is_abstract<T>::value; }
 


  // Returns true if T is an enum or enum class.
  template <typename T>
    constexpr bool Enum() { return std::is_enum<T>::value; }

  // An alias to the integer type that stores the values of the enum T.
  template <typename T>
    using Underlying_type = typename std::underlying_type<T>::type;



  //////////////////////////////////////////////////////////////////////////////
  // Functions                                                       traits.func
  //
  // There are number of different ways that a function can be represented in
  // C++. The most obvious is the definition of a function or member function.
  // We can also make objects of class type act like functions by overloading
  // their function call operator. Lambda expressions are examples of this
  // representation.
  //
  // The number of ways in which functions are represented can lead to some
  // confusing terminology. Here, we use the following definitions:
  //
  //    function type -- The type of a declared function. A function type has
  //    the form R(Args...) where R is the result type and Args... is a list
  //    of function argument types.
  //
  //    callable type -- The type of an object, f, that can be called with the
  //    syntax f(args...). Here, f can be a function pointer, a member function
  //    pointer, a functor, or a lambda.
  //
  //    call expression type -- A function type of the form F(Args...) where
  //    F is a callable type and Args is a list of arguments over which F
  //    is being invoked. The distinction between call expression types and
  //    function types is purely nominal; the C++ language does not actually
  //    differentiate. We characterize these separately because some typing
  //    facilities make use of types of this form, but will not work for
  //    function types.
  //
  // It is important to note that the set of callable types is disjoint from
  // the set of function types. 
  //
  // It is also important to note that we cannot generally determine if a type
  // T is a callable type (without extended compiler support). We cannot
  // effectively determine the presence of operator() in functors if there are
  // overloads or if the operator is a member function template. As such, there
  // is no concept for the notion of Callable types.
  //
  // Finally, note that some properties in this component cannot be deduced for
  // polymorphic functors. If the function call operator is overloaded on
  // different arguments, or if it is a member function template, then we cannot
  // deduce its arity or set of arguments. That impacts several useful type
  // functions.
  //
  // TODO: Should the Parameters_of, Function_arity, Domain, and Codomain 
  // traits accept function types? I think it might be more uniform to restrict
  // their operation to only callable types.
  //////////////////////////////////////////////////////////////////////////////
    
#include "traits.impl/function.hpp"

  //////////////////////////////////////////////////////////////////////////////
  // Function Type
  //
  // A type T is a function type when it has the form R(Args...) where R is
  // a result type and Args is a list of types representing the argument types
  // of the function.
  //
  // Template Parameters:
  //    T -- The type being tested
  //
  // Returns:
  //    True if and only if T is a function type.
  template <typename T>
    constexpr bool Function_type()
    {
      return std::is_function<T>();
    }
    


  //////////////////////////////////////////////////////////////////////////////
  // Result Of
  //
  // The Result_of alias denotes the result type of a call expression type, C,
  // having the form F(Args...) where F is a callable type and Args is a list of
  // types representing the arguments with which F is called.
  //
  // If T is given as a function type, the referred to type will indicate
  // substitution failure.
  //
  // Template Parameters:
  //    T -- A call expression type
  //
  // Refers To:
  //    The result of the call expression type T.
  //
  // WARNING: Result_of does not fail gracefully on substitution failures 
  // involving member function pointers. In p
  template <typename C>
    using Result_of = typename type_impl::result_of<C>::type;



  //////////////////////////////////////////////////////////////////////////////
  // Parameters Of
  //
  // The Parameters_of alias denotes the list of types that are the function
  // parameters of the function type or callable type F.
  //
  // Template parameters:
  //    F -- A function type or callable type
  //
  // Refers To:
  //    A type list containing the function argument types of F.
  //
  // WARNING: This property cannot be deduced for polymorphic functors.
  //
  // FIXME: The definition of this function may need to be reconsidered for
  // member function pointers. When used as an expression, the class object
  // must be explicitly given as an argument. The total argument list should
  // be that object and the parameters of the pointed-at function.
  template <typename F>
    using Parameters_of = typename type_impl::get_parameter_types<F>::type;



  //////////////////////////////////////////////////////////////////////////////
  // Function Arity
  //
  // The arity of a function, F, is the size of its Parameter list. Note that
  // this is not the same as the number of arguments over which the function
  // may be called. Parameters with default arguments are included in the arity
  // of the function.
  //
  // Template parameters:
  //    F -- A function type or callable type
  //
  // Returns: 
  //    The arity of the given callable type or function type. 
  //
  // WARNING: This property cannot be deduced for polymorphic functors.
  //
  // FIXME: The definition of this function may need to be reconsidered for
  // member function pointers. When used as an expression, the number of
  // arguments required to invoke the expression is 1 more than the number of
  // parameters of the pointed-at function.
  template <typename F>
    constexpr std::size_t Function_arity()
    {
      return type_impl::function_arity<F>::value;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Homogeneous function
  //
  // A homogeneous function, F, is one whose argument types are all the same.
  // Here, F can be either a function type or callable type.
  //
  // Template Parameters:
  //    F -- A function or callable type
  //
  // Returns:
  //    True if and only if the arguments of F are all the same.
  //
  // WARNING: This property cannot be deduced for polymorphic functors.
  template <typename F>
    constexpr bool Homogeneous_function()
    {
      return Same<Expand<Parameters_of<F>>>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Domain
  //
  // The domain of a (homogeneous) function or callable type, F, is the type
  // shared by its arguments. The domain type of a function is a structural
  // type; it is neither a reference nor cv-qualified. If F is non-homogeneous,
  // then this alias refers to a type indicating substitution failure.
  //
  // Note that requesting the domain of a nullary function results in an
  // ill-formed program.
  //
  // Template Parameters:
  //    F -- A function or callable type
  //
  // Refers To:
  //    The domain type of the function or callable type F.
  //
  // WARNING: This property cannot be deduced for polymorphic functors.
  template <typename F>
    using Domain = 
      If<Homogeneous_function<F>(), 
         Structural_type<Front_type<Expand<Parameters_of<F>>>>, 
         subst_failure>;



  //////////////////////////////////////////////////////////////////////////////
  // Codomain
  //
  // The Codomain of a function or callable type, F, is the structural type
  // of its result. The codomain type is neither a reference nor cv-qualified
  // type.
  //
  // The Codomain alias differs from Result_of in that the argument type F is a
  // function type or callable type. In contrast, Result_of requires its
  // argument type to be a call expression type. Moreover, Result_of refers to
  // the exact result type of an operation. The Codomain alias refers to the
  // structural type returned by F. In that sense, the following would be
  // equivalent programs:
  //
  //    auto x = f(args...);
  //    Codomain<decltype(f)> x = f(args...);
  //
  // Assuming of course, that decltype(f) can be computed (i.e., f must not
  // name an overload set).
  //
  // Template Parameters:
  //    F -- A function or callable type
  //
  // Refers To:
  //    The domain type of the function or callable type F.
  //
  // WARNING: This property cannot be deduced for polymorphic functors.
  template <typename F>
    using Codomain = 
      Structural_type<typename type_impl::get_result_type<F>::type>;



  //////////////////////////////////////////////////////////////////////////////
  // Member Pointers                                               traits.memptr
  //
  // The following traits implement queries for pointers to member objects
  // and member functions.
  //////////////////////////////////////////////////////////////////////////////

#include "traits.impl/memptr.hpp"

  // A type T is a member-obect-pointer if it is of the form R C::* where R is
  // the type of the pointed-at object and C is the class containing the member.
  //
  // Template Parameters:
  //    T -- The type being tested
  //
  // Returns:
  //     True if T is a pointer to a member object.
  template <typename T>
    constexpr bool Member_object_pointer()
    {
      return std::is_member_object_pointer<T>::value;
    }

  // A type T is a member-function-pointer if it is of the form 
  // R (C::*)(Args...) where R is the result type of the function, C is the
  // class containing the member, and Args is the sequence of function 
  // arguments.
  //
  // Template Parameters:
  //    T -- The type being tested
  //
  // Returns:
  //     True if T is a pointer to a member object.
  template <typename T>
    constexpr bool Member_function_pointer()
    {
      return std::is_member_function_pointer<T>::value;
    }



  // The member result type is the result of an expression using a member
  // pointer. For a member object pointer, this is the type of the dereferenced
  // object. For a member function pointer, it is the result type of the
  // the function.
  //
  // FIXME: I don't think that this is very well defined. It's kind of hard to
  // say what it points at.

  // template <typename T>
  //   using Member_result_type = 
  //     typename type_impl::member_pointer_traits<T>::result_type;

  // An alias to a member pointer's class type.
  template <typename T>
    using Member_class_type = 
      typename type_impl::member_pointer_traits<T>::class_type;



  //////////////////////////////////////////////////////////////////////////////
  // Composite categories
  //
  // These traits query the composite categories of primary types. Note that
  // there are other categories (e.g. Reference), but these do not belong to
  // any more obvious grouping.
  //
  //////////////////////////////////////////////////////////////////////////////
    
  // Returns true if T is an object type. An object type is any type that can
  // be placed in a region of memory. Void, reference, and function types are
  // not object types.
  template <typename T>
    constexpr bool Object() { return std::is_object<T>::value; }

  // Returns true if T is a scalar type. A scalar type is an object type that is
  // an integer type, floating point type, or pointer. Compound types (classes,
  // unions, and arrays) are not scalar.
  template <typename T>
    constexpr bool Scalar() { return std::is_scalar<T>::value; }

  // Returns true if T is a compound type. Compound types are class types, 
  // union types, and arrays.
  template <typename T>
    constexpr bool Compound() { return std::is_compound<T>::value; }

  // Returns true if T is a built-in type. 
  template <typename T>
    constexpr bool Fundamental() { return std::is_fundamental<T>::value; }

  // Returns true if T is an arithmetic type. Integer and floating point types
  // are arithmetic types.
  template <typename T>
    constexpr bool Arithmetic() { return std::is_arithmetic<T>::value; }
    


  //////////////////////////////////////////////////////////////////////////////
  // Qualifiers
  //
  // The following type traits implement queries and transformations for
  // const and volatile qualifiers.
  //////////////////////////////////////////////////////////////////////////////

  // Returns true if T is const-qualified
  template <typename T>
    constexpr bool Const() { return std::is_const<T>::value; }

  // Returns true if T is volatile-qualified
  template <typename T>
    constexpr bool Volatile() { return std::is_volatile<T>::value; }

  // Returns true if T is cv-qualified.
  template <typename T>
    constexpr bool Qualified() { return Const<T>() || Volatile<T>(); }

  // An alias to const-qualified T (i.e., const T).
  template <typename T>
    using Add_const = typename std::add_const<T>::type;

  // An alias to volatile-qualified T (i.e., volatile T).    
  template <typename T>
    using Add_volatile = typename std::add_volatile<T>::type;
    
  // An alias to const volatile qualified T (i.e., const volatile T).
  template <typename T>
    using Add_cv = typename std::add_cv<T>::type;

  // An alias to U if T has type const U, otherwise T.
  template <typename T>
    using Remove_const = typename std::remove_const<T>::type;
    
  // An alias to U if T has type volatile U, otherwise T.
  template <typename T>
    using Remove_volatile = typename std::remove_volatile<T>::type;

  // An alias to U if T is cv-qualified or T otherwise.
  template <typename T>
    using Remove_cv = typename std::remove_cv<T>::type;

  // Decay an array into a pointer or a function into a function pointer.
  template <typename T>
    using Decay = typename std::decay<T>::type;


  //////////////////////////////////////////////////////////////////////////////
  // Layout, initialization, and assignment
  //
  // These traits describe properties of a type's layout, properties of its
  // destructor, constructors, and assignement operators.
  //
  // Many of the traits in this section deal with triviality of an operation.
  // A constructor or destructor is trivial if:
  //
  //    - It is generated by the compiler, either implicitly or because of an 
  //      '= default' annotation, and:
  //    - The class has no virtual functions or virtual bases.
  //    - All of the corresponding operations in the direct bases of the
  //      class are also trivial.
  //    - The corresponding operations in all non-static data members of
  //      the class are also trivial.
  //
  // Many of the other traits involve the nothrow exception guarantee. An
  // operation satisifes this property if it is annotated with noexcept or
  // noexcept(true).
  //////////////////////////////////////////////////////////////////////////////



  // Returns true if T is a standard layout type
  template <typename T>
    constexpr bool Standard_layout() { return std::is_standard_layout<T>::value; }
    


  //////////////////////////////////////////////////////////////////////////////
  // Destruction


  // Return true if T has a destructor.
  template <typename T>
    constexpr bool Destructible() { return std::is_destructible<T>::value; }
    
  // Returns true if T has a trivial destructor.
  //
  // FIXME: GCC does not implement the corresponding type trait. The
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T>
    constexpr bool Trivially_destructible()
    {
      // return std::is_trivially_destructible<T>::value;
      return false;
    }

  // Returns true if T has a destructor that does not propagate exceptions.
  template <typename T>
    constexpr bool Nothrow_destructible()
    {
      return std::is_nothrow_destructible<T>::value;
    }
    
  // Returns true if T has a virtual destructor
  template <typename T>
    constexpr bool Has_virtual_destructor() 
    { 
      return std::has_virtual_destructor<T>::value; 
    }



  //////////////////////////////////////////////////////////////////////////////
  // Construction

  // Returns true if T has a constructor for Args.
  template <typename T, typename... Args>
    constexpr bool Constructible() 
    { 
      return std::is_constructible<T, Args...>::value; 
    }
    
  // Returns true if T has a trivial constructor for Args. 
  //
  // FIXME: GCC does not implement the corresponding type trait. The
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T, typename... Args>
    constexpr bool Trivially_constructible()
    {
      // return std::is_trivially_constructible<T, Args...>::value;
      return false;
    }

  // Returns true if T has a constructor for Args that does not propagate
  // exceptions.
  template <typename T, typename... Args>
    constexpr bool Nothrow_constructible() 
    { 
      return std::is_nothrow_constructible<T, Args...>::vlaue; 
    }



  //////////////////////////////////////////////////////////////////////////////
  // Default construction

  // Returns true if T has a default constructor.
  template <typename T>
    constexpr bool Default_constructible()
    {
      return std::is_default_constructible<T>::value;
    }
    
  // Returns true if T has a trivial default constructor.
  //
  // FIXME: GCC does not implement the corresponding type trait. The 
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T>
    constexpr bool Trivially_default_constructible()
    {
      // return std::is_trivially_default_constructible<T>::value;
      return false;
    }

  // Returns true if T can be default constructed, and the constructor does not
  // propagate exceptions.
  template <typename T>
    constexpr bool Nothrow_default_constructible()
    {
      return std::is_nothrow_default_constructible<T>::value;
    }
    


  //////////////////////////////////////////////////////////////////////////////
  // Copy construction

  // Returns true if T has a copy constructor.
  template <typename T>
    constexpr bool Copy_constructible()
    {
      return std::is_copy_constructible<T>::value;
    }
    
  // Returns true if T has a trivial copy constructor.
  //
  // FIXME: GCC does not implement the corresponding type trait. The 
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T>
    constexpr bool Trivially_copy_constructible()
    {
      // return std::is_trivially_copy_constructible<T>::value;
      return false;
    }

  // Returns true if T has a copy constructor that does not propagate 
  // exceptions.
  template <typename T>
    constexpr bool Nothrow_copy_constructible()
    {
      return std::is_nothrow_copy_constructible<T>::value;
    }
    


  //////////////////////////////////////////////////////////////////////////////
  // Move construction

  // Returns true if T has a move constructor.
  template <typename T>
    constexpr bool Move_constructible()
    {
      return std::is_move_constructible<T>::value;
    }

  // Returns true if T has a trivial move constructor.
  //
  // FIXME: GCC does not implement the corresponding type trait. The 
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T>
    constexpr bool Trivially_move_constructible()
    {
      // return std::is_trivially_move_constructible<T>::value;
      return false;
    }

  // Returns true if T has a move constructor that does not propagate 
  // exceptions.
  template <typename T>
    constexpr bool Nothrow_move_constructible()
    {
      return std::is_nothrow_move_constructible<T>::value;
    }
    


  //////////////////////////////////////////////////////////////////////////////
  // Assignment


  //////////////////////////////////////////////////////////////////////////////
  // Assignable
  //
  // The Assignable trait determines if a type T supports assignment to a
  // value of type U. U is generally expected to be a compound type. This trait
  // can be used to check for copy and move assignment by using reference
  // types. For example:
  //
  //    Assignable<T, const T&>() // Check for copy assignment
  //    Assignable<T, T&&>()      // Check for move assignment
  //
  // A positive response when U is an rvalue reference does not mean that T
  // defines an assignment operator that specifically takes an rvalue reference
  // to U (i.e., U&&). A copying assignment operator (taking const U&) will
  // also satisfy the requirments of rvalue assignment.
  //
  // Note that the the first argument can also be given as an lvalue reference
  // with the same meaning. That is:
  //
  //    Assignable<T&, U>() <=> Assignable<T, U>()
  //
  // Template Parameters:
  //    T -- The type of object being assigned to
  //    U -- The type of value being assigned
  //
  // Returns:
  //    True if and only if an object of type T can be assigned a value of type
  //    U.
  template <typename T, typename U>
    constexpr bool Assignable()
    {
      return std::is_assignable<T, U>::value;
    }

  // Returns true if T has a trivial assignment operator for values of type U.    
  //
  // FIXME: GCC does not implement the corresponding type trait. The 
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T, typename U>
    constexpr bool Trivially_assignable()
    {
      // return std::is_trivially_assignable<T>::value;
      return false;
    }

  // Returns true if T has an  assignment operator for values of type U that
  // does not propagate exceptions.
  template <typename T, typename U>
    constexpr bool Nothrow_assignable()
    {
      return std::is_nothrow_assignable<T, U>::value;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Move assignement

  // Returns true if T has a move assignment operator.
  template <typename T>
    constexpr bool Move_assignable()
    {
      return std::is_move_assignable<T>::value;
    }

  // Returns true if T has a trivial move assignment oeprator.
  //
  // FIXME: GCC does not implement the corresponding type trait. The 
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T>
    constexpr bool Trivially_move_assignable()
    {
      // return std::is_trivially_move_assignable<T>::value;
      return false;
    }
  
  // Returns true if T has a move assignment operator that does not propagate
  // exceptions.
  template <typename T>
    constexpr bool Nothrow_move_assignable()
    {
      return std::is_nothrow_move_assignable<T>::value;
    }
    


  //////////////////////////////////////////////////////////////////////////////
  // Copy assignement

  // Returns true if T has a copy assignment operator.
  template <typename T>
    constexpr bool Copy_assignable()
    {
      return std::is_copy_assignable<T>::value;
    }

  // Returns true if T has a trivial copy assignment operator.
  //
  // FIXME: GCC does not implement the corresponding type trait. The 
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T>
    constexpr bool Trivially_copy_assignable()
    {
      // return std::is_trivially_copy_assignable<T>::value;
      return false;
    }

  // Returns true if T has a copy assignment operator that does not propagate
  // exceptions.    
  template <typename T>
    constexpr bool Nothrow_copy_assignable()
    {
      return std::is_nothrow_copy_assignable<T>::value;
    }
    


  //////////////////////////////////////////////////////////////////////////////
  // Composites
  //
  // FIXME: We can't implemnt Trivially_movable and trivially copyable because
  // GCC does not implement the corresponding traits yet.


  // Returns true if T is a trivially movable type. A trivially movable type
  // is both trivially move constructible and assignable.
  //
  // FIXME: GCC does not implement the corresponding type trait. The 
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T>
    constexpr bool Trivially_movable()
    {
      // return Trivially_move_constructible<T>()
      //     && Trivially_move_assignable<T>();
      return false;
    }

  // Returns true if T is a trivially copyable type. A trivially copyable type
  // is trivially movable, trivially copy constructible, and trivially copy
  // assignable.
  //
  // FIXME: GCC does not implement the corresponding type trait. The 
  // implementation is designed to fail. Use Trivial as an alternative.
  template <typename T>
    constexpr bool Trivially_copyable() 
    { 
      // return Trivially_movable<T>()
      //     && Trivially_copy_constructible<T>() 
      //     && Trivially_copy_assignable<T>();
      return false;
    }

  // Returns true if T is a trivial type. A trivialy type is trivially
  // destructible, trivially default constructible, and trivally copyable.
  //
  // The Trivial type trait could be seen as a concept refining the Regular
  // concept except for the missing requirement for equality comparability.
  // Otherwise, this trait strengthens the semantic requirements of the 
  // required constructors and destructors (i.e., they must be trivial).
 template <typename T>
    constexpr bool Trivial() { return std::is_trivial<T>::value; }
    


  //////////////////////////////////////////////////////////////////////////////
  // Miscellaneous traits
  //////////////////////////////////////////////////////////////////////////////

  // An alias to a data type with that can be used as uninitialized storage
  // for an object of size Size and whose alignment requirement is a divisor
  // of Align.
  template <std::size_t Size, std::size_t Align>
    using Aligned_storage = typename std::aligned_storage<Size, Align>::type;

    

  //////////////////////////////////////////////////////////////////////////////
  // Operators
  //
  // The following traits implement queries for expressions involving C++
  // operators. Each trait is comprised of two (public) components:
  //
  //    - An alias to the result of an expression using the operator or
  //      subst_failure if the operator is not defined for the given argument
  //      types.
  //    - A predicate that returns true when the operator is defined for the
  //      given argument type.
  //
  // Note the general convention that t is an object of type T and u is an
  // object of type U.
  //
  // Predicates on operators are evaluated in terms of the value type.
  // Evaluating e.g., Has_plus<T&> will yield the same result as Has_plus<T>.
  //////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////
  // Relational operators


  // An alias for the result type of the expression t == u.
  template <typename T, typename U = T>
    using Equal_result = typename type_impl::get_equal_result<T, U>::type;

  // Returns true if the expression t == u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_equal()
    {
      return Subst_succeeded<Equal_result<T, U>>();
    }
    


  // An alias for the result type of the expression t != u.
  template <typename T, typename U = T>
    using Not_equal_result = typename type_impl::get_not_equal_result<T, U>::type;

  // Returns true if the expression t != u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_not_equal()
    {
      return Subst_succeeded<Not_equal_result<T, U>>();
    }



  // An alias for the result type of the expressoin t < u.
  template <typename T, typename U = T>
    using Less_result = typename type_impl::get_less_result<T, U>::type;

  // Returns true if the expression t < u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_less()
    {
      return Subst_succeeded<Less_result<T, U>>();
    }



  // An alias for the result type of the expression t > u.
  template <typename T, typename U = T>
    using Greater_result = typename type_impl::get_greater_result<T, U>::type;

  // Returns true if the expression t > u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_greater()
    {
      return Subst_succeeded<Greater_result<T, U>>();
    }
    


  // An alias for the result type of the expression  <= u.
  template <typename T, typename U = T>
    using Less_equal_result = 
      typename type_impl::get_less_equal_result<T, U>::type;

  // Returns true if the expression t <= u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_less_equal()
    {
      return Subst_succeeded<Less_equal_result<T, U>>();
    }



  // An alias for the result of t >= u.
  template <typename T, typename U = T>
    using Greater_equal_result = 
      typename type_impl::get_greater_equal_result<T, U>::type;
    
  // Return true if the expression t >= u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_greater_equal()
    {
      return Subst_succeeded<Greater_equal_result<T, U>>();
    }    
    


  //////////////////////////////////////////////////////////////////////////////
  // Arithmetic operators

  namespace traits
  {
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

  } // namespace traits



  // An alias for the result type of the expression t + u.
  template <typename T, typename U = T>
    using Plus_result = typename type_impl::get_plus_result<T, U>::type;
    
  // Returns true if the expression t + u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_plus()
    {
      return Subst_succeeded<Plus_result<T, U>>();
    }



  // An alias for the result type of the expression t - u.
  template <typename T, typename U = T>
    using Minus_result = typename type_impl::get_minus_result<T, U>::type;
    
  // Returns true if the expression t - u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_minus()
    {
      return Subst_succeeded<Minus_result<T, U>>();
    }
  


  // An alias for the result type of the expression t * u.
  template <typename T, typename U = T>
    using Multiplies_result = typename type_impl::get_multiplies_result<T, U>::type;
    
  // Returns true if the expression t * u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_multiplies()
    {
      return Subst_succeeded<Multiplies_result<T, U>>();
    }
    


  // An alias for the result type of the expression t / u.
  template <typename T, typename U = T>
    using Divides_result = typename type_impl::get_divides_result<T, U>::type;
    
  // Return true if the expression t / u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_divides()
    {
      return Subst_succeeded<Divides_result<T, U>>();
    }
  


  // An alias for the result type of the expression t % u.
  template <typename T, typename U = T>
    using Modulus_result = typename type_impl::get_modulus_result<T, U>::type;
    
  // Return true if the expression t % u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_modulus()
    {
      return Subst_succeeded<Modulus_result<T, U>>();
    }
    


  // An alias for the result type of the expression +t.
  template <typename T>
    using Unary_plus_result = typename type_impl::get_unary_plus_result<T>::type;
    
  // Returns true if the expression +t is valid.
  template <typename T>
    constexpr bool Has_unary_plus()
    {
      return Subst_succeeded<Unary_plus_result<T>>();
    }



  // An alias for the result type of the expression -t.
  template <typename T>
    using Unary_minus_result = typename type_impl::get_unary_minus_result<T>::type;
    
  // Returns true if the expression -t is valid.
  template <typename T>
    constexpr bool Has_unary_minus()
    {
      return Subst_succeeded<Unary_plus_result<T>>();
    }
    


  //////////////////////////////////////////////////////////////////////////////    
  // Compound arithmetic assignment operators


  // An alias for the result type of the exprssion t += u.
  template <typename T, typename U>
    using Plus_assign_result = typename 
      type_impl::get_plus_assign_result<T, U>::type;
    
  // Returns true if the expression t += u is valid.
  template <typename T, typename U>
    constexpr bool Has_plus_assign()
    {
      return Subst_succeeded<Plus_assign_result<T, U>>();
    }    

    

  // An alias for the result type of the expression t -= u.
  template <typename T, typename U>
    using Minus_assign_result = 
      typename type_impl::get_minus_assign_result<T, U>::type;
    
  // Returns true if the expression t -= u is valid.
  template <typename T, typename U>
    constexpr bool Has_minus_assign()
    {
      return Subst_succeeded<Minus_assign_result<T, U>>();
    }    
    
    

  // An alias for the result type of the expression t *= u.
  template <typename T, typename U>
    using Multiplies_assign_result = 
      typename type_impl::get_multiplies_assign_result<T, U>::type;
    
  // Returns true if the expression t *= u is valid.
  template <typename T, typename U>
    constexpr bool Has_multiplies_assign()
    {
      return Subst_succeeded<Multiplies_assign_result<T, U>>();
    }    
    
    

  // An alias for the result type of the expression t /= u.
  template <typename T, typename U>
    using Divides_assign_result = 
      typename type_impl::get_divides_assign_result<T, U>::type;
    
  // Returns true if the expression t /= u is valid.
  template <typename T, typename U>
    constexpr bool Has_divides_assign()
    {
      return Subst_succeeded<Divides_assign_result<T, U>>();
    }    
    
    
  // An alias for the result type of the expression t %= u.
  template <typename T, typename U>
    using Modulus_assign_result = 
      typename type_impl::get_modulus_assign_result<T, U>::type;
    
  // Returns true if the expression t %= u is valid.
  template <typename T, typename U>
    constexpr bool Has_modulus_assign()
    {
      return Subst_succeeded<Modulus_assign_result<T, U>>();
    }        
    


  //////////////////////////////////////////////////////////////////////////////
  // Increment and decrement

  namespace traits
  {
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
  } // namespace traits



  // An alias to the result type of the expression ++t.
  template <typename T>
    using Pre_increment_result = 
      typename type_impl::get_pre_increment_result<T>::type;

  // Returns true if the expression ++t is valid.
  template <typename T>
    constexpr bool Has_pre_increment()
    {
      return Subst_succeeded<Pre_increment_result<T>>();
    }



  // An alias for the result type of the expression t++.
  template <typename T>
    using Post_increment_result = 
      typename type_impl::get_post_increment_result<T>::type;

  // Returns true if the expostssion t++ is valid.
  template <typename T>
    constexpr bool Has_post_increment()
    {
      return Subst_succeeded<Post_increment_result<T>>();
    }

    
    
  // An alias for the result type of the expression --t.
  template <typename T>
    using Pre_decrement_result = 
      typename type_impl::get_pre_decrement_result<T>::type;

  // Returns true if the expression --t is valid.
  template <typename T>
    constexpr bool Has_pre_decrement()
    {
      return Subst_succeeded<Pre_decrement_result<T>>();
    }



  // An alias for the result type of the expression t--.
  template <typename T>
    using Post_decrement_result 
      = typename type_impl::get_post_decrement_result<T>::type;

  // Returns true if the expostssion t-- is valid.
  template <typename T>
    constexpr bool Has_post_decrement()
    {
      return Subst_succeeded<Post_decrement_result<T>>();
    }




  //////////////////////////////////////////////////////////////////////////////    
  // Logical operators
  
  namespace traits
  {
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

  } // namespace traits



  // An alias for the result type of the expression t && u.
  template <typename T, typename U = T>
    using And_result = typename type_impl::get_and_result<T, U>::type;
    
  // Returns true if the expression t && u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_and()
    {
      return Subst_succeeded<And_result<T, U>>();
    }
    


  // An alias for the result type of the experssion t || u.
  template <typename T, typename U = T>
    using Or_result = typename type_impl::get_or_result<T, U>::type;
    
  // Return true if the expression t || u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_or()
    {
      return Subst_succeeded<Or_result<T, U>>();
    }
    

    
  // An alias for the result type of the expression !t.
  template <typename T>
    using Not_result = typename type_impl::get_not_result<T>::type;
    
  // Return true if the expression !t is valid.
  template <typename T>
    constexpr bool Has_not()
    {
      return Subst_succeeded<Not_result<T>>();
    }   


  //////////////////////////////////////////////////////////////////////////////
  // Binary oeprators
  
  namespace traits
  {
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

  } // namespace traits



  // An alias for the result type of the expression t & u.
  template <typename T, typename U = T>
    using Bit_and_result = typename type_impl::get_bit_and_result<T, U>::type;
    
  // Returns true if the expression t & u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_bit_and()
    {
      return Subst_succeeded<Bit_and_result<T, U>>();
    }
    


  // An alias for the result type of the expression t | u.
  template <typename T, typename U = T>
    using Bit_or_result = typename type_impl::get_bit_or_result<T, U>::type;
    
  // Returns true if the expression t | u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_bit_or()
    {
      return Subst_succeeded<Bit_or_result<T, U>>();
    }
    


  // An alias for the result type of the expression t ^ u.
  template <typename T, typename U = T>
    using Bit_xor_result = typename type_impl::get_bit_xor_result<T, U>::type;
    
  // Returns true if the expression t ^ u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_bit_xor()
    {
      return Subst_succeeded<Bit_xor_result<T, U>>();
    }
    
    

  // An alias for the result type of the expression t << u.
  template <typename T, typename U = T>
    using Left_shift_result = typename type_impl::get_left_shift_result<T, U>::type;
    
  // Returns true if the expression t << u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_left_shift()
    {
      return Subst_succeeded<Left_shift_result<T, U>>();
    }
    
    

  // An alias for the result type of the expression t >> u.
  template <typename T, typename U = T>
    using Right_shift_result = typename type_impl::get_right_shift_result<T, U>::type;
    
  // Returns true if the expression t >> u is valid.
  template <typename T, typename U = T>
    constexpr bool Has_right_shift()
    {
      return Subst_succeeded<Right_shift_result<T, U>>();
    }
    
    

  // An alias for the result type of the expression ~t.
  template <typename T>
    using Complement_result = typename type_impl::get_complement_result<T>::type;
    
  // Return true if the expression ~t is valid.
  template <typename T>
    constexpr bool Has_complement()
    {
      return Subst_succeeded<Complement_result<T>>();
    }   
  

  //////////////////////////////////////////////////////////////////////////////
  // Binary assignment operators

  namespace traits
  {
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

  } // namespace traits



  // An alias for the result type of the expression t &= u.
  template <typename T, typename U>
    using Bit_and_assign_result = 
      typename type_impl::get_bit_and_assign_result<T, U>::type;
    
  // Returns true if the expression t &= u is valid.
  template <typename T, typename U>
    constexpr bool Has_bit_and_assign()
    {
      return Subst_succeeded<Bit_and_assign_result<T, U>>();
    }        

    

  // An alias for the result type of the expression t |= u.
  template <typename T, typename U>
    using Bit_or_assign_result = 
      typename type_impl::get_bit_or_assign_result<T, U>::type;
    
  // Returns true if the expression t |= u is valid.
  template <typename T, typename U>
    constexpr bool Has_bit_or_assign()
    {
      return Subst_succeeded<Bit_or_assign_result<T, U>>();
    }        

    

  // An alias for the result type of the expression t ^= u.
  template <typename T, typename U>
    using Bit_xor_assign_result = 
      typename type_impl::get_bit_xor_assign_result<T, U>::type;
    
  // Returns true if the expression t ^= u is valid.
  template <typename T, typename U>
    constexpr bool Has_bit_xor_assign()
    {
      return Subst_succeeded<Bit_xor_assign_result<T, U>>();
    }        

    

  // An alias for the result type of the expression t <<= u.
  template <typename T, typename U>
    using Left_shift_assign_result = 
      typename type_impl::get_left_shift_assign_result<T, U>::type;
    
  // Returns true if the expression t <<= u is valid.
  template <typename T, typename U>
    constexpr bool Has_left_shift_assign()
    {
      return Subst_succeeded<Left_shift_assign_result<T, U>>();
    }        

    

  // An alias for the result type of the expression t >>= u.
  template <typename T, typename U>
    using Right_shift_assign_result = 
      typename type_impl::get_right_shift_assign_result<T, U>::type;
    
  // Return true if the expression t >>= u is valid.
  template <typename T, typename U>
    constexpr bool Has_right_shift_assign()
    {
      return Subst_succeeded<Right_shift_assign_result<T, U>>();
    }        



  //////////////////////////////////////////////////////////////////////////////
  // Address, dereference, subscript, and function call

  namespace traits
  {
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
  } // namespace traits



  // An alias for the result type of the expression &t.
  template <typename T>
    using Address_result = typename type_impl::get_address_result<T>::type;
    
  // Returns true if the expression &t is valid.
  template <typename T>
    constexpr bool Has_address()
    {
      return Subst_succeeded<Address_result<T>>();
    }   
    


  // An alias for the result type of the expression *t.
  template <typename T>
    using Dereference_result = typename type_impl::get_dereference_result<T>::type;
    
  // Returns true if the expression *t is valid.
  template <typename T>
    constexpr bool Has_dereference()
    {
      return Subst_succeeded<Dereference_result<T>>();
    }
    


  // An alias for the result type of the expression t[u].
  template <typename T, typename U>
    using Subscript_result = typename type_impl::get_subscript_result<T, U>::type;
    
  // Returns true if the expression t[u] is valid.
  template <typename T, typename U>
    constexpr bool Has_subscript()
    {
      return Subst_succeeded<Subscript_result<T, U>>();
    }

    

  // An alias for the result type of the expression f(args...). Note that this
  // is equivalent to Result_of<F(Args...)> and is provided purely for symmetry
  // with the other operator traits.
  template <typename F, typename... Args>
    using Call_result = Result_of<F(Args...)>;

  // Returns true if the expression f(args...) is valid.
  template <typename F, typename... Args>
    constexpr bool Has_call()
    {
      return Subst_succeeded<Result_of<F(Args...)>>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Common Member Traits
  //
  // There are number of common member types and functions found in data
  // structures in the STL such as value_type, difference_type, size(), empty(),
  // find(), and swap(). We provide traits for dedecting these operations in
  // this library because they are sometimes needed for describing concepts or
  // constraining templates.
  //
  // In particular, we provide traits for:
  //    - Associated value types (T::value_type)
  //    - Associated difference types (T::difference_type)
  //    - Associated size types (T::size_type)
  //
  //////////////////////////////////////////////////////////////////////////////


// Implementation supprot for member traits.
#include "traits.impl/member.hpp"


  // An alias for T::value_type.
  template <typename T>
    using Associated_value_type = 
      typename type_impl::get_associated_value_type<T>::type;


  // Returns true if T::value_type is a valid type expression.
  template <typename T>
    constexpr bool Has_associated_value_type()
    {
      return Subst_succeeded<Associated_value_type<T>>();
    }



  // An alias for T::difference_type.
  template <typename T>
    using Associated_difference_type = 
      typename type_impl::get_associated_difference_type<T>::type;

  // Returns true if T::difference_type is a valid type expression.
  template <typename T>
    constexpr bool Has_associated_difference_type()
    {
      return Subst_succeeded<Associated_difference_type<T>>();
    }



  // An alias for T::size_type.
  template <typename T>
    using Associated_size_type = typename type_impl::get_associated_size_type<T>::type;

  // Returns true if T::size_type is a valid type expression.
  template <typename T>
    constexpr bool Has_associated_size_type()
    {
      return Subst_succeeded<Associated_size_type<T>>();
    }



  // An alias to the associated reference type.
  template <typename T>
    using Associated_reference = 
      typename type_impl::get_associated_reference<T>::type;

  // Returns true if T::reference is a valid type name.
  template <typename T>
    constexpr bool Has_associated_reference()
    {
      return Subst_succeeded<Associated_reference<T>>();
    }



  // An alias to the associated const_reference type.
  template <typename T>
    using Associated_const_reference = 
      typename type_impl::get_associated_const_reference<T>::type;

  // Returns true if T::reference is a valid type name.
  template <typename T>
    constexpr bool Has_associated_const_reference()
    {
      return Subst_succeeded<Associated_const_reference<T>>();
    }



  // An alias to the associated pointer type.
  template <typename T>
    using Associated_pointer = 
      typename type_impl::get_associated_pointer<T>::type;

  // Return true if T::cosnt_reference is a valid type name.
  template <typename T>
    constexpr bool Has_associated_pointer()
    {
      return Subst_succeeded<Associated_pointer<T>>();
    }


  // An alias to the associated const_pointer type.
  template <typename T>
    using Associated_const_pointer = 
      typename type_impl::get_associated_const_pointer<T>::type;

  // Return true if T::const_pointer is a valid type name.
  template <typename T>
    constexpr bool Has_associated_const_pointer()
    {
      return Subst_succeeded<Associated_const_pointer<T>>();
    }



  // Member size
  // These traits determine if x.size() is a valid expression. Classes 
  // implementing this member typically cache their size rather than compute
  // it (e.g., Containers).

  // An alias for the result type of the expression t.size().
  template <typename T>
    using Member_size_result = typename type_impl::get_member_size_result<T>::type;
    
  // Returns true if t.size() is a valid expression.
  template <typename T>
    bool constexpr Has_member_size()
    {
      return Subst_succeeded<Member_size_result<T>>();
    }



  // An alias for the result type of the expression t.empty().
  template <typename T>
    using Member_empty_result = typename type_impl::get_member_empty_result<T>::type;
    
  // Returns true if t.empty() is a valid expression.
  template <typename T>
    bool constexpr Has_member_empty()
    {
      return Subst_succeeded<Member_empty_result<T>>();
    }
  

    
  // An alias for the result type of the expression t.find(u).
  template <typename C, typename K>
    using Member_find_result = typename type_impl::get_member_find_result<C, K>::type;
    
  // Returns true if t.find(u) is a valid expression.
  template <typename C, typename K>
    bool constexpr Has_member_find()
    {
      return Subst_succeeded<Member_find_result <C, K>>();
    }


} // namespace origin

#endif
