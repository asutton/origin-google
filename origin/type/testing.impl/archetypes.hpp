// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_TESTING_HPP
#  error This file cannot be included directly. Include type/testing.hpp.
#endif

namespace type_impl
{

  // All of the _archetype classes within this namespace are essentially policy-
  // based classes that are derived from the archetype class. The structure
  // of these classes mirrors that of the concepts that they personify (?).
  //
  // NOTE: Within the _*archtype classes, the derived archetype class, Arch,
  // is an incomplete type. The parameter after Arch (usually T) is the
  // archetype's underlying value type. It must be a complete type.



  template <typename Arch, typename T>
    struct default_constructible_archetype
    {
      static_assert(Default_constructible<T>(), "");
    };


  template <typename Arch, typename T>
    struct move_constructible_archetype
    {
      static_assert(Move_constructible<T>(), "");
    };


  template <typename Arch, typename T>
    struct copy_constructible_archetype
    {
      static_assert(Copy_constructible<T>(), "");
    };


  template <typename Arch, typename T>
    struct movable_archetype 
      : move_constructible_archetype<Arch, T>
    {
      static_assert(Movable<T>(), "");
    };
  

  template <typename Arch, typename T>
    struct copyable_archetype 
      : movable_archetype<Arch, T>
      , copy_constructible_archetype<Arch, T>
    {
      static_assert(Copyable<T>(), "");
    };


  // Equality comparable
  template <typename Arch, typename T, typename U = default_t>
    struct equality_comparable_archetype
    {
      static_assert(Equality_comparable<T, U>(), "");
      friend bool operator==(const Arch& a, const U& b) { return a.value == b; }
      friend bool operator==(const U& a, const Arch& b) { return a == b.value; }
      friend bool operator!=(const Arch& a, const U& b) { return a.value != b; }
      friend bool operator!=(const U& a, const Arch& b) { return a != b.value; }
    };

  template <typename Arch, typename T>
    struct equality_comparable_archetype<Arch, T, default_t>
    {
      static_assert(Equality_comparable<T>(), "");
      friend bool operator==(const Arch& a, const Arch& b) { return a.value == b.value; }
      friend bool operator!=(const Arch& a, const Arch& b) { return a.value != b.value; }
    };


  // Weakly ordered (facade)
  template <typename Arch, typename T, typename U = default_t>
    struct weakly_ordered_archetype
    {
      static_assert(Equality_comparable<T, U>(), "");
      friend bool operator<(const Arch& a, const U& b) { return a.value < b; }
      friend bool operator<(const U& a, const Arch& b) { return a < b.value; }
      friend bool operator>(const Arch& a, const U& b) { return a.value > b; }
      friend bool operator>(const U& a, const Arch& b) { return a > b.value; }
      friend bool operator<=(const Arch& a, const U& b) { return a.value <= b; }
      friend bool operator<=(const U& a, const Arch& b) { return a <= b.value; }
      friend bool operator>=(const Arch& a, const U& b) { return a.value >= b; }
      friend bool operator>=(const U& a, const Arch& b) { return a >= b.value; }
    };

  template <typename Arch, typename T>
    struct weakly_ordered_archetype<Arch, T, default_t>
    {
      static_assert(Equality_comparable<T>(), "");
      friend bool operator<(const Arch& a, const Arch& b) { return a.value < b.value; }
      friend bool operator>(const Arch& a, const Arch& b) { return a.value > b.value; }
      friend bool operator<=(const Arch& a, const Arch& b) { return a.value <= b.value; }
      friend bool operator>=(const Arch& a, const Arch& b) { return a.value >= b.value; }
    };


  // Totally ordered
  template <typename Arch, typename T, typename U = default_t>
    struct totally_ordered_archetype 
      : equality_comparable_archetype<Arch, T, U> 
      , weakly_ordered_archetype<Arch, T, U>
    {  
      static_assert(Totally_ordered<T, U>(), "");
    };

  template <typename Arch, typename T>
    struct totally_ordered_archetype<Arch, T, default_t>
      : equality_comparable_archetype<Arch, T> 
      , weakly_ordered_archetype<Arch, T>
    {  
      static_assert(Totally_ordered<T>(), "");
    };


  // Semiregular
  template <typename Arch, typename T>
    struct semiregulr_archetype
      : copyable_archetype<Arch, T>
      , default_constructible_archetype<Arch, T>
    {
      static_assert(Semiregular<T>(), "");
    }


  // Regular
  template <typename Arch, typename T>
    struct regular_archetype 
      : semiregulr_archetype<Arch, T> 
      , equality_comparable_archetype<Arch, T>
    { 
      static_assert(Regular<T>(), "");
    };


  // Function
  //
  // The function call operator is defined within this class.  Note that the
  // class defines a non-const overload, allowing calls to be delegated to a
  // similar operation on the deriving class.
  template <typename Arch, typename F, typename... Args>
    class function_archetype : public copy_constructible_archetype<Arch, F>
    {
      static_assert(Function<F, Args...>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }
    
    public:
      auto 
      operator()(Args... args) -> Result_of<F(Args...)>
      {
        return self().value(args...);
      }

      auto 
      operator()(Args... args) const -> Result_of<F(Args...)>
      {
        return self().value(args...);
      }
    };

  
  // Predicate
  //
  // Note that this does not refine function, only copy constructible. The
  // purpose for doing this is so that we don't inherit and hide function's 
  // operator() declarations. Those simply should not be accessible.
  //
  // Function call operators on predicates should never be non-const, like they
  // may be with Function models.
  template <typename Arch, typename P, typename... Args>
    class predicate_archetype : public copy_constructible_archetype<Arch, P>
    { 
      static_assert(Predicate<P, Args...>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }
    
    public:
      bool
      operator()(Args... args) const
      {
        return self().value(args...);
      }
    };



  // Relation
  //
  // This class is specialized for homogenous and cross-type relations.
  //
  // See the comment on Predicate regarding the redefinition of function call
  // operators.
  template <typename Arch, typename R, typename T, typename U = default_t>
    class relation_archetype : public copy_constructible_archetype<Arch, R>
    {
      static_assert(Relation<R, T, U>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }
    
    public:
      bool 
      operator()(const T& t, const U& u) const
      {
        return self().value(t, u);
      }

      bool 
      operator()(const U& u, const T& t) const
      {
        return self().value(u, t);
      }
    };

  template <typename Arch, typename R, typename T>
    class relation_archetype<Arch, R, T, default_t> 
      : public copy_constructible_archetype<Arch, R>
    { 
      static_assert(Relation<R, T>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }
    
    public:
      bool operator()(const T& a, const T& b) const
      {
        return self().value(a, b);
      }
    };


  // TODO: Write operation archetypes


  // Selectors
  //
  // A selector is a small class that helps the archetype class enumerate and
  // inherit the archetypes above.
  //
  // FIXME: I could do away with the refines tuple if I had access to a list
  // of base types. GCC has an extension that allows such queries, but it's
  // very much internal. I should wait until there's a more standard method.



  // Default constructible (archetype selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct use_default_constructible
  {
    using refines = std::tuple<>;

    template <typename Arch, typename T>
      using impl = default_constructible_archetype<Arch, T>;
  };



  // Move constructible (selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct use_move_constructible
  {
    using refines = std::tuple<>;

    template <typename Arch, typename T>
      using impl = move_constructible_archetype<Arch, T>;
  };



  // Copy constructible (selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct use_copy_constructible
  {
    using refines = std::tuple<>;

    template <typename Arch, typename T>
      using impl = copy_constructible_archetype<Arch, T>;
  };



  // Movable (selector)
  // The movable archetype is a placeholder for copy semantics.
  struct use_movable
  { 
    using refines = std::tuple<use_move_constructible>;

    template <typename Arch, typename T>
      using impl = movable_archetype<Arch, T>;
  };



  // Copyable (selector)
  // The copyable archetype is a placeholder for copy semantics
  struct use_copyable
  { 
    using refines = std::tuple<use_movable, use_copy_constructible>;

    template <typename Arch, typename T>
      using impl = copyable_archetype<Arch, T>;
  };

 

 // Equality comparable (selector)
  template <typename U = default_t>
    struct use_equality_comparable
    {
      using refines = std::tuple<>;

      template <typename Arch, typename T>
        using impl = equality_comparable_archetype<Arch, T, U>;
    };

  // FIXME: This should be merged with the primary template, but GCC was
  // giving an ICE when that happened.
  template <>
    struct use_equality_comparable<default_t>
    {
      using refines = std::tuple<>;

      template <typename Arch, typename T>
        using impl = equality_comparable_archetype<Arch, T>;
    };



  // Weakly ordered (selector)
  template <typename U = default_t>
    struct use_weakly_ordered
    {
      using refines = std::tuple<>;

      template <typename Arch, typename T>
        using impl = weakly_ordered_archetype<Arch, T, U>;
    };

  // FIXME: See the comments above about ICEs.
  template <>
    struct use_weakly_ordered<default_t>
    {
      using refines = std::tuple<>;

      template <typename Arch, typename T>
        using impl = weakly_ordered_archetype<Arch, T>;
    };



  // Totally ordered (selector)
  template <typename U = default_t>
    struct use_totally_ordered
    {
      using refines = std::tuple<use_equality_comparable<U>, use_weakly_ordered<U>>;
      
      template <typename Arch, typename T>
        using impl = totally_ordered_archetype<Arch, T, U>;
    };

  // FIXME: See the comment above about ICEs.
  template <>
    struct use_totally_ordered<default_t>
    {
      using refines = 
        std::tuple<use_equality_comparable<>, use_weakly_ordered<>>;
      
      template <typename Arch, typename T>
        using impl = totally_ordered_archetype<Arch, T>;
    };



  // Regular type (selector)
  struct use_regular
  {
    using refines = 
      std::tuple<use_default_constructible, use_copyable, use_equality_comparable<>>;

    template <typename Arch, typename T>
      using impl = regular_archetype<Arch, T>;
  };



  // Function (selector)
  template <typename... Args>
    struct use_function
    {
      using refines = std::tuple<use_copy_constructible>;

      template <typename Arch, typename F>
        using impl = function_archetype<Arch, F, Args...>;
    };



  // Predicate (selector)
  template <typename... Args>
    struct use_predicate
    {
      using refines = std::tuple<use_copy_constructible>;

      template <typename Arch, typename F>
        using impl = predicate_archetype<Arch, F, Args...>;
    };



   // Relation (selector)
  template <typename T, typename U = T>
    struct use_relation
    {
      using refines = std::tuple<use_copy_constructible>;

      template <typename Arch, typename R>
        using impl = relation_archetype<Arch, R, T, U>;
    }; 



  // Input iterator selector
  struct use_input_iterator
  {
    using refines = std::tuple<use_copyable, use_equality_comparable<>>;

    template <typename Arch, typename I>
      using impl = input_iterator_archetype<Arch, I>;
  };



  // Output iterator (selector)
  template <typename T>
    struct use_output_iterator
    {
      using refines = std::tuple<use_copyable>;

      template <typename Arch, typename I>
        using impl = output_iterator_archetype<Arch, I, T>;
    };



  // Forward iterator (selector)
  struct use_forward_iterator
  {
    using refines = std::tuple<use_input_iterator>;

    template <typename Arch, typename I>
      using impl = forward_iterator_archetype<Arch, I>;
  };



  // Bidirectional iterator (selector)
  struct use_bidirectional_iterator
  {
    using refines = std::tuple<use_forward_iterator>;

    template <typename Arch, typename I>
      using impl = bidirectional_iterator_archetype<Arch, I>;
  };



  // Random access iterator (selector)
  struct use_random_access_iterator
  {
    using refines = std::tuple<use_bidirectional_iterator>;

    template <typename Arch, typename I>
      using impl = random_access_iterator_archetype<Arch, I>;
  };



  // Archetype framework
  // The archetype framework is a single class that constructs a single type
  // based on a number of selectors.



  template <typename Arch, typename T> struct inspect_selector;
  template <typename Arch, typename... Args> struct has_selector;

  // Inspect archetype
  // Returns true if true if Arch is contained in the list of T's refinements.
  //
  // FIXME: This is a terrible name...
  template <typename Arch, typename T>
    static constexpr bool Inspect_selector()
    {
      return inspect_selector<Arch, T>::value;
    }


  // Has archetype
  // Returns true if Arch is found in the list of archetypes.
  template <typename Arch, typename... Args>
    static constexpr bool Has_selector()
    {
      return has_selector<Arch, Args...>::value;
    }



  // Search for the selector Sel in the selector list [T, Args...]. Note that we
  // recursively search the set of refinements of each T in the list.
  template <typename Sel, typename T, typename... Args>
    struct has_selector<Sel, T, Args...>
      : boolean_constant<
          Same<Sel, T>() 
          ? true 
          : (Inspect_selector<Sel, T>() 
             ? true
             : Has_selector<Sel, Args...>())
        >
    { };

  template <typename Sel>
    struct has_selector<Sel> : std::false_type { };


  // Inspect the list of refinements, rerpresented by T for the the given
  // Archetype. T is intended to be a tuple. This simply delegates the search
  // back to has_selector, in order to flatten the entire search.
  template <typename Sel, typename T>
    struct inspect_refinements : std::false_type { };

  template <typename Sel, typename... Args>
    struct inspect_refinements<Sel, std::tuple<Args...>>
      : has_selector<Sel, Args...>
    { };

  // Search the refinements of the T for the selector Sel.
  template <typename Sel, typename T>
    struct inspect_selector 
      : inspect_refinements<Sel, typename T::refines>
    { };




} // namespace type_impl
