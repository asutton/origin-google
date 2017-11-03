// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_CONCEPTS_HPP
#define ORIGIN_TYPE_CONCEPTS_HPP

#include <iosfwd>

#include "traits.hpp"

namespace origin
{
  // Declarations
  namespace type_impl
  {
    template <typename T, typename U> struct is_equality_comparable;
    template <typename T, typename U> struct is_weakly_ordered;
    template <typename R, typename T, typename U> struct is_relation;
    template <typename O, typename T, typename U> struct is_binary_operation;

    template <typename S, typename T> struct is_input_streamable;
    template <typename S, typename T> struct is_output_streamable;
  }


  //////////////////////////////////////////////////////////////////////////////
  // Boolean                                                    concepts.boolean
  //
  // A type T is Boolean if it can be used in a Boolean context; That is, T
  // must be convertible to bool. For example:
  //
  //    Boolean<int>()    // returns true
  //    Boolean<void*>()  // returns true
  //    Boolean<string>() // returns false.
  // 
  // This concept is primarily defined as a more concise way of writing
  // Convertible<T, bool>.
  //
  // Template parameters:
  //    T -- The type being tested
  //
  // Returns:
  //    True if and only if T can be converted to bool.
  template <typename T>
    constexpr bool Boolean() { return Convertible<T, bool>(); }



  //////////////////////////////////////////////////////////////////////////////
  // Equality                                                        concepts.eq
  //
  // TODO: Give a semantic description of notions of equality.
  //
  // Note that it is inusfficient to define == as only an equivalence relation.
  // Two equivalent values may be substituted in programs, yielding different
  // results.



  //////////////////////////////////////////////////////////////////////////////
  // Equality Comparable                         concepts.eq.equality_comparable
  //
  // The equality comparable determines if objects of a given type (or set of
  // types) can be compared for equality (==) or differentiated (!=). There are
  // two overloads of this concept.
  //
  //    Equality_comparable<T>()
  //    Equality_comparable<T, U>()
  // 
  // The unary concept determines if objects of type T can be compared using
  // == and !=. The semantics of the == operator is that it implement the
  // equality relation (see [concepts.eq]); it must be an equivalence relation
  // that returns true when one argument can be substituted for the other in
  // regular programs. The != operator must be the complement of the equality
  // relation.
  //
  // The first overload returns true when T satisfies the syntactic requirements
  // of equality comparison. For example:
  //
  //    Equality_comparable<int>()           // returns true
  //    Equality_comparable<runtime_error>() // returns false
  //
  // The binary concept defines cross-type equality comparison. If T and U are
  // different types, then T and U must be individually equality comparable, T
  // and U must have a common type, C, that is also equality comparable, and
  // objects of type T and U must be symmetrically equality comparable using ==
  // and !=. The results of those comparisons must be the same as converting
  // each argument to the common type before comparing. See Common_type for more
  // information about cross-type concepts.
  //
  // This overload returns true when T and U satisfy the syntactic requirements
  // of cross-type equality comparison. For example:
  //
  //    Equality_comparable<int, long>()           // returns true
  //    Equality_comparable<string, const char*>() // returns true
  //    Equality_comparable<string, int>()         // returns false
  //
  // When the arguments T and U are the same, the binary concept is equivalent
  // to the unary concept. That is:
  //
  //    Equality_comparable<T, T>() <=> Equality_comparable<T>()
  //
  template <typename T, typename U = T>
    constexpr bool Equality_comparable()
    {
      return type_impl::is_equality_comparable<T, U>::value;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Ordering                                                       concepts.ord
  //
  // There are two ordering concepts in Origin: weak ordering and total
  // ordering. Both require the definition of the relational operators <, >,
  // <=, and >=. Additionally, the incomparability relation is also associated
  // with ordering. Two ordered values a and b are incomparable when
  //
  //    !(a < b) && !(a < b)
  //
  // The semantics of the ordering are primarily associated with the < operator.
  // For specifics, refer to [concepts.weakly_ordered] and
  // [concepts.totally_ordered]. The definitions of >, <=, and >= can be derived
  // from < in the usual way:
  //
  //    a > b  <=> b < a
  //    a <= b <=> !(b < a)
  //    a >= b <=> !(a < b)
  //
  // Note that there is no partially ordered concept for types. The reason for
  // this is that we expect all relational operators to be total operations in
  // the sense that all well-formed values of the type can be compared. There
  // is, however, a partial ordering property that can be used to describe 
  // specific relations.



  //////////////////////////////////////////////////////////////////////////////
  // Weakly Ordered                                            concepts.ord.weak
  //
  // The weakly ordered concept determines if a type (or pair of types) can be
  // compared using the relational operators <, >, <=, and >=. There are two
  // overloads of the weakly ordered concept.
  //
  //    Weakly_ordered<T>()
  //    Weakly_ordered<T, U>()
  //
  // The unary overload determines if a type T is is weakly ordered. A weakly
  // ordered type defines the standard relational operators <, >, <=, and >=
  // where < defines a strict weak order and the other operators have the usual
  // meaning. A strict weak order is a relation that is irreflexive, asymmetric,
  // and transitive, and incomparability is an equivalence relation.
  //
  // This overload returns true when T satisfies the syntactic requirements of
  // weakly ordered types. For example:
  //
  //    Weakly_ordered<int>()       // returns true
  //    Weakly_ordered<type_info>() // returns false (no operator <)
  //
  // Note that weakly ordered types are rarely defined in practice. If one can
  // define an ordering over values, then one can generally define a reasonable
  // equality operator. The weakly ordered concept is 
  //
  // The binary overload checks if T and U are cross-type weakly ordered. That
  // is, T and U must be individually weakly or totally ordered, T and U must
  // share a common type C, which is also weakly or totally ordered, and objects
  // of type T and U must be symmetrically comparable using <, >, <= and >=. See
  // Common_type for more information cross-type concepts.
  //
  // This overload returns true when the syntactic requirements for cross-type
  // weak ordering are satisfied. For example:
  //
  //    Weakly_ordered<int, long>()           // returns true
  //    Weakly_ordered<string, const char*>() // returns true
  //    Weakly_ordered<string, int>()         // returns false
  //
  // When the type areguments T and U are the same, the binary concept is
  // equivalent to the unary concept:
  //
  //    Weakly_ordered<T, T>() <=> Weakly_ordered<T>()
  //
  template <typename T, typename U = T>
    constexpr bool Weakly_ordered()
    {
      return type_impl::is_weakly_ordered<T, U>::value;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Totally Ordered                                          concepts.ord.total
  //
  // There are two overloads of the totally ordered concept.
  //
  //    Totally_ordered<T>
  //    Totally_ordered<T, U>
  //
  // The unary overload determines if a type T is is totally ordered. A totally
  // ordered type is both weakly ordered and equality comparable where the
  // incomparability relation is the same as the equality operator. That is:
  //
  //    !(a < b) && !(b < a) <=> a == b
  //
  // This overload returns true when T satisfies the syntactic requirements of
  // totally ordered types. For example:
  //
  //    Totally_ordered<int>()       // returns true
  //    Totally_ordered<type_info>() // returns false (no operator <)
  //
  // The binary overload checks if T and U are cross-type weakly ordered. That
  // is, T and U must be individually weakly or totally ordered, T and U must
  // share a common type C, which is also weakly or totally ordered, and objects
  // of type T and U must be symmetrically comparable using <, >, <= and >=. See
  // Common_type for more information cross-type concepts.
  //
  // This overload returns true when the syntactic requirements for cross-type
  // weak ordering are satisfied. For example:
  //
  //    Weakly_ordered<int, long>()           // returns true
  //    Weakly_ordered<string, const char*>() // returns true
  //    Weakly_ordered<string, int>()         // returns false
  template <typename T, typename U = T>
    constexpr bool Totally_ordered()
    {
      return Equality_comparable<T, U>() && Weakly_ordered<T, U>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Object concepts                                                concepts.obj
  //
  // The concepts in this section describe properties of types that define
  // objects. This is a small hierarchy of concepts that builds on a number of
  // initialization and assignment type traits. It includes:
  //
  //    Movable
  //    Copyable
  //    Semiregular
  //    Regular
  //
  // The reason that we refer to these as "object" concepts is that they 
  // describe properties of types used to initialize (and destroy and compare)
  // objects.
  //
  // Note that the "trivial" type traits can be seen as semantic refinements
  // of these concepts. That is, a trivially movable is a refinement of movable,
  // trivially copyable is a refinement of copyable, and trivial is a refinement
  // of semiregular.



  //////////////////////////////////////////////////////////////////////////////
  // Movable                                                concepts.obj.movable
  //
  // A type T is movable if it is desctructible and can be both move constructed
  // and move assigned. Note that a type is still movable even if the
  // constructor and assignment operator actually perform copies. 
  //
  // The function returns true if T satisfies the syntactic requirements of
  // movability.
  //
  //    Movable<int>()          // returns true
  //    Movable<string>()       // returns true 
  //    Movable<decltype([]{})> // returns false (not move assignable)
  //
  template <typename T>
    constexpr bool Movable()
    {
      return Destructible<T>() 
          && Move_constructible<T>() 
          && Move_assignable<T>();
    };
    
  
    
  //////////////////////////////////////////////////////////////////////////////
  // Copyable                                              concepts.obj.copyable
  //
  // A type T is copyable if it is movable and can be both copy constructed and 
  // copy assigned.
  //
  // The function returns true if T satisfies the syntactic requirements of
  // copyability.
  //
  //    Copyable<int>()          // returns true
  //    Copyable<string>()       // returns true 
  //    Copyable<decltype([]{})> // returns false (not copy assignable)
  //
  // Template parameters:
  //    T -- The type being tested
  //
  // Returns:
  //    True if and only if T satisfies the syntactic requirements of the
  //    Copyable concept.
  template <typename T>
    constexpr bool Copyable()
    {
      return Movable<T>() 
          && Copy_constructible<T>() 
          && Copy_assignable<T>();
    }
    
    
  
  //////////////////////////////////////////////////////////////////////////////
  // Semiregular                                        concepts.obj.semiregular
  //
  // A semiregular type T is copyable and default constructible. Semiregular
  // types are those that can be used in general ways: we can use them to 
  // declare variables or arrays and pass them by value (i.e., copy) or move
  // them. For example, the following class is semiregular:
  //
  //    struct Person 
  //    {
  //      string first;
  //      string last;
  //      int id;
  //    };
  //
  // The destructor, default constructor, move constructor, copy constructor,
  // and move assignment oeprator, and copy assignment operator are all
  // implicitly generated, but are non-trivial. Note that the class would still
  // be Semiregular if the members were private, as long as the required
  // operations were still publicly accessible.
  //
  // The notion of semiregularity of a type generalizes the notion of trivial
  // types in the language. A trivial type is one that trivially copyable (has
  // a trivial copy constructor and copy assignment operator) and trivially
  // default constructible.
  //
  // Template parameters:
  //    T -- the type being tested for semiregularity.
  //
  // Returns:
  //    True if and only if T satisfies the syntactic requirements of the
  //    Semiregular concept.
  template <typename T>
    constexpr bool Semiregular()
    {
      return Copyable<T>() && Default_constructible<T>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Regular                                                concepts.obj.regular
  //
  // A regular type T is semiregular and equality comparable. Regular types
  // are the basis of a large number of abstractions - in particular, those that
  // can be used as (if they represented) values. For example, every built-in
  // type is regular, and so are all standard container types (assuming that
  // their contained value types are also regular).
  //
  // Template parameters:
  //    T -- the type being tested for regularity.
  //
  // Returns:
  //    True if and only if T satisfies the syntactic requirements of the
  //    Regular concept.
  template <typename T>
    constexpr bool Regular()
    {
      return Semiregular<T>() && Equality_comparable<T>();
    }
    
  
  //////////////////////////////////////////////////////////////////////////////
  // Ordered                                                concepts.obj.ordered
  //
  // A type T is ordered if it is regular and totally ordered. This concept
  // is primarily provided as a convenience for describing higher level 
  // abstractions. 
  //
  // Template parameters:
  //    T -- The type being tested
  //
  // Returns:
  //    True if and only if T satisfies the syntactic requirements of the
  //    Ordered concept.
  template <typename T>
    constexpr bool Ordered()
    {
      return Regular<T>() && Totally_ordered<T>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Functions                                                     concepts.func
  //
  // A function is an object that can be called, taking a sequence of arguments
  // and returning some result (possibly none). There are a number of concepts
  // describing the kinds of functions in the STL.
  //
  // - Function
  // - Regular function
  // - Predicate
  // - Relation
  // - Unary operation
  // - Binary operation
      


  //////////////////////////////////////////////////////////////////////////////
  // Function                                             concepts.func.function
  //
  // A function type, F, is one that can be called with a sequence of arguments,
  // Args. Every function has an associated result type that can be deduced
  // using Result_of<F(Args...)>.
  //
  // Note that functions are not required to be equality preserving. There are
  // no constraints on the effects of the function.
  //
  // Template parameters:
  //    F -- The function being tested
  //    Args -- A sequence of argument types over which F might be called
  //
  // Returns:
  //    True if and only if F can be called with a sequence of arguments whose
  //    types are given by the type parameter pack Args.
  template <typename F, typename... Args>
    constexpr bool Function()
    {
      return Copy_constructible<F>() && Has_call<F, Args...>();
    }
    


  //////////////////////////////////////////////////////////////////////////////
  // Regular Function                                      concepts.func.regular
  //
  // A regular function is an equality preserving function. That is, equal
  // inputs reliably yield equal outputs. More formally: if a and b are
  // equivalent expressions (have the same observable effects), then:
  //
  //    a <=> b => f(a) <=> f(b)
  //
  // Evaluating a regular function, f, over the equivalent expressions, a and b,
  // will yield equivalent computations.
  //
  // The result of a regular function cannot be void.
  //
  // Template parameters:
  //    - F    -- The function being tested
  //    - Args -- A sequence of argument types over which F might be called
  //
  // Returns:
  //    True if and only if F can be called with a sequence of arguments whose
  //    types are given by the type parameter pack Args.
  template <typename F, typename... Args>
    constexpr bool Regular_function()
    {
      return Function<F, Args...>() && !Same<Result_of<F(Args...)>, void>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Predicate                                           concepts.func.predicate
  //
  // A predicate is a regular function whose result type is convertible to
  // bool.
  //
  // Template parameters:
  //    - P    -- The predicate being tested
  //    - Args -- A sequence of argument types over which P might be called
  //
  // Returns:
  //    True if and only if P can be called with a sequence of arguments whose
  //    types are given by the type parameter pack Args, and if the result of 
  //    of the function is Boolean.
  template <typename P, typename... Args>
    constexpr bool Predicate()
    {
      return Function<P, Args...>() && Boolean<Result_of<P(Args...)>>();
    }
    
    
    
  //////////////////////////////////////////////////////////////////////////////
  // Relation                                             concepts.func.relation
  //
  // A relation is a binary bredicate over a set of values. There are two
  // overloads of this concept:
  //
  //    Relation<R, T>()
  //    Relation<R, T, U>()
  //
  // The first overload determines if R is a relation with the homogenous
  // domain T. The second is a generalization, allowing the domain to be
  // heterogenous. However, when the domain is heterogeneous, the types t and
  // U must share a common type.
  //
  // Template parameters:
  //    R -- The relation being tested
  //    T -- The domain of the relation
  //    U -- If given, the heterogeneous domain type along with T
  //
  // Returns:
  //    True if R is a relation taking arguments arguments of type T (and U, if
  //    specified).
  template <typename R, typename T, typename U = T>
    constexpr bool Relation()
    {
      return type_impl::is_relation<R, T, U>::value;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Relational Properties                              properties.func.relation
  //
  // The followwing predicates are used to write preconditions in generic
  // algorithms or other data structures. Note that because these properties
  // cannot be evaluated at runtime, they simply return true. They are primarily
  // used to indicate a symbolic property.

  // Evaluates whether r is an equivalence relation. Always returns true.
  template <typename R>
    constexpr bool is_equivalence_relation(R r) { return true; }

  // Determines whether r is a strict partial order. Always returns true.
  template <typename R>
    constexpr bool is_strict_partial_order(R r) { return true; }

  // Determines whether r is a strict weak order. Always returns true.
  template <typename R>
    constexpr bool is_strict_weak_order(R r) { return true; }

  // Determines whether r is a total order. Always returns true.
  template <typename R>
    constexpr bool is_total_order(R r) { return true; }



  //////////////////////////////////////////////////////////////////////////////
  // Generator                                           concepts.func.generator
  //
  // a generator is a nullary function (taking no arguments) that returns values
  // of a specified type. Generators are typically non-regular functions, since
  // they are often used to generate different values on subsequent calls.
  // Random value generators are examples of generating functions. 
  //
  // Template parameters:
  //    G -- The function being tested
  //
  // Returns:
  //    True if and only if G can be called with no arguments, and has a 
  //    non-void result type.
  template <typename G>
    constexpr bool Generator()
    {
      return Function<G>() && !Void<Result_of<G()>>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Operations                                                     concepts.ops
  //
  // An operation is a regular function that takes one or more arguments of
  // its domain type and returns a value of the same type. The domain and
  // codomain types are the same.
  //
  // Operation concepts are prefixed with their arity because of overloads of
  // the binary operation concept.



  //////////////////////////////////////////////////////////////////////////////
  // Unary Operation                                          concepts.ops.unary
  //
  // Template parameters:
  //    Op -- The operation being tested
  //    T  -- The domain type of Op
  //
  // Returns:
  //    True if and only if Op can be called with a single argument of type T
  //    and the result type of Op is also T.
  //
  // TODO: Describe this concept.
  template <typename Op, typename T>
    constexpr bool Unary_operation()
    {
      return Regular_function<Op, T>() && Same<Result_of<Op(T)>, T>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Binary Operations                                       concepts.ops.binary
  //
  // Template parameters:
  //    Op -- The operation being tested
  //    T  -- The domain type of Op
  //    U  -- If given, the heteregenous domain type (along with T) of Op
  //
  // Returns:
  //    True if and only if Op can be called with arguments of type T and U,
  //    and the result type of Op is Common_type<T, U>.
  //
  // TODO: Describe this concept.
  template <typename Op, typename T, typename U = T>
    constexpr bool Binary_operation()
    {
      return type_impl::is_binary_operation<Op, T, U>::value;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Streaming                                                   concepts.stream
  //
  // There are 3 concepts describing streaming operations.
  //
  //     1. Input_streamable<T>
  //     2. Output_streamable<T>
  //     3. Streamable<T>
  //
  // TODO: Write about streaming...
  //////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////
  // Input Streamable                                         concepts.stream.in
  //
  // This concept ddescribes types that can be read from a stream using the
  // input stream operator (>>). There are two overloads of this concept:
  //  
  //     Input_streamable<T>
  //     Input_streamable<S, T>
  //
  // The first determines if T can be read from a stream derived from either 
  // istream or wistream. Note that the two are equivalent are equivalent if 
  // the >> overloads are defined in terms of basic_istream.
  //
  // The concept returns true if values of type T can be read from an input
  // stream.
  template <typename T, typename U = default_t>
    constexpr bool Input_streamable()
    {
      return type_impl::is_input_streamable<T, U>::value;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Output Streamable                                       concepts.stream.out
  //
  // Describes types that can be written to a stream using output stream
  // operator (<<). There are two overloads of this concept:
  //
  //    Output_streamable<T>
  //    Output_streamable<S, T>
  //
  // The first determines if T can be written to a stream derived from either 
  // ostream or wostream. Note that the two are equivalent are equivalent if 
  // the >> overloads are defined in terms of basic_ostream.
  //
  // The second determines if T can be written to the stream S.
  //
  // Thje concept returns true if values of type T can be written to an output
  // stream.
  template <typename T, typename U = default_t>
    constexpr bool Output_streamable()
    {
      return type_impl::is_output_streamable<T, U>::value;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Output Streamable                                        concepts.stream.io
  //
  //
  // TODO: Document me!
  template <typename T, typename U = default_t>
    constexpr bool Streamable()
    {
      return Input_streamable<T, U>() && Output_streamable<T, U>();
    }


// Include afore-mentioned implementations.
#include "concepts.impl/traits.hpp"



  //////////////////////////////////////////////////////////////////////////////
  // Associated types                                             concepts.types
  //
  // The following type functions describe commonly associated types.
  //////////////////////////////////////////////////////////////////////////////


// Include deduction support.
#include "concepts.impl/deduction.hpp"


  //////////////////////////////////////////////////////////////////////////////
  // Value Type                                             concepts.types.value
  //
  // Every type that contains or refers to other objects has a notion of "value
  // type". The value type is the type of object being owned or referred to. The
  // value type is never cv-qualifed or a reference. For example, the value type
  // of a container is the type of objects it contains. The value type of an
  // iterator is the type of object it refers to.
  //
  // This alias refers to the value type associated with T. It can be used with
  // objects of any concept defining the notion. For example:
  //
  //    using V = vector<T>;
  //    Value_type<V>                 // is T
  //    Value_type<V::iterator>       // is T
  //    Value_type<V::const_iterator> // is T
  //
  // Note that the value type of the const iterator is not T, not const T.
  //
  // If T does not have an associated value type, then the result of this type
  // function will indicate substitution failure.
  template <typename T>
    using Value_type = typename type_impl::get_deduced_value_type<T>::type;


  // Returns true if a value type is associated with T.
  template <typename T>
    constexpr bool Has_value_type()
    {
      return Subst_succeeded<Value_type<T>>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Size Type                                               concepts.types.size
  //
  // An associated size type encodes the largest possible number of elements
  // that can be contained in a data structure.
  //
  // The size type alias refers to the associated size type of T.
  //
  // TODO: Finish documenting this type.
  //
  // If T does not have an associated size type, then the result of this type
  // function will indicate substitution failure.
  template <typename T>
    using Size_type = typename type_impl::get_deduced_size_type<T>::type;


  // Has size type (trait)
  // Returns true if T hwas an associated size type.
  template<typename T>
    constexpr bool Has_size_type()
    {
      return Subst_succeeded<Size_type<T>>();
    }



  // This difference traits can be specialized to override the default
  // deduction and lookup mechanisms. This should only be used when a type
  // describes itself incorrectly (e.g., std::ostream_iterator).
  template <typename T>
    struct difference_type_traits
    {
      using type = typename type_impl::get_deduced_difference_type<T>::type;
    };


  //////////////////////////////////////////////////////////////////////////////
  // Difference Type                                   concepts.types.difference
  //
  // The difference type is a type that encodes the largest possible difference
  // between two objects in memory. This is virtually always the same as
  // std::ptrdiff_t. In general, any kind of type that abstracts over pointers
  // (e.g., iterators and containers) will provide an associated dfiference 
  // type.
  //
  // Note that the difference type is always the same as the signed 
  // representation of the size type.
  //
  // The origin of this abstraction is rooted in the different memory models
  // and their different pointer types (i.e., near, far, and huge pointers).
  //
  // The difference type alias refers to the associated difference type. For
  // example:
  //
  //    Difference_type<T*> // is std::ptrdiff_t
  //
  // If T does not have an associated difference type, then the result of this
  // type function will indicate substitution failure.
  template <typename T>
    using Difference_type = 
      typename difference_type_traits<T>::type;
    

  // Returns true if a difference type is associated with T.
  template <typename T>
    constexpr bool Has_difference_type()
    {
      return Subst_succeeded<Difference_type<T>>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Reference Of                                       concepts.types.reference
  //
  // A type T may have an associated reference type, defined either as the 
  // result of an operation or as a nested type. For example, an iterator's
  // reference type is the result of its dereference operator. A container's
  // associated reference type is explicitly declared as C::reference or
  // C::const_reference (with C being a Container).
  //
  // The associated reference type may depend on whether T is const qualified.
  //
  // Aliases:
  //    The associated reference type of T.
  template <typename T>
    using Reference_of = typename type_impl::get_deduced_reference<T>::type;

  //////////////////////////////////////////////////////////////////////////////
  // Pointer Of                                           concepts.types.pointer
  //
  // A type T may have an associated pointer type, defined either as the  result
  // of an operation or as a nested type. For example, an iterator's pointer
  // type is the result of its arrow operator. A container's associated pointer
  // type is explicitly declared as C::pointer or C::const_pointer (with C being
  // a Container).
  //
  // The associated pointer type may depend on whether T is const qualified.
  //
  // Aliases:
  //    The associated pointer type of T.
  template <typename T>
    using Pointer_of = typename type_impl::get_deduced_pointer<T>::type;

} // namespace origin


#endif
