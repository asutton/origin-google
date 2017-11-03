// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_TESTING_HPP
#  error This file cannot be included directly. Include type/testing.hpp.
#endif

namespace testing
{
  //////////////////////////////////////////////////////////////////////////////
  // Randomized Testing
  //
  // This library provides basic support for randomized testing. To do this, the
  // library builds on and generalizes the features for random number generation
  // in the standard library. This is done with respect to two concepts:
  //
  //    - A random value pattern is a generalization of a random number
  //      distribution. It is a function object taking a pseudo-random number
  //      generator and computing a value according to some parameters. Note
  //      that a random number distribution is a random value pattern with
  //      strict semantic requirements. A random value pattern may have random
  //      number distributions as parameters, controlling the generation of
  //      values.
  //
  //    - Binding a random value pattern to a pseudo-random number distribution
  //      produces a random value generator. This analogous to binding a random
  //      number distribution to a PRNG to produce a random number generator,
  //      which is a kind of random value generator.
  //
  // Note that some of the requirements for random value patterns are relaxed
  // from random number distributions. In particular, there is no requirement
  // that models of that concept be Streamable.
  //
  // This library deals with the association of default patterns with specific
  // types. The default_* classes and class templates are function objects
  // that return pattern or distribution classes. The library can be extended
  // to new types by defining new patterns (e.g., for graphs), and associating
  // them with the their corresponding types.
  //
  // There are two methods of association: the default_pattern_traits class and
  // the deduce_default_pattern function. The first should be used when
  // associating a default pattern with a specific type or template (e.g., bool
  // and string<C, T>). The latter should be used when the association applies
  // to an entire concept (e.g., Integer).
  //
  // The primary interface to this library are the default_pattern function and
  // the Default_pattern_type alias. The former returns the default pattern
  // associated with a type, and the latter 



  // The singleton generator simply returns the same value every time it is
  // called.
  //
  // FIXME: This is also called a constant function. This class should probably
  // go away in favor of constant_function, whenever that shows up. However,
  // constant function may be in a library outside of the type library, which
  // would result in cyclic dependencies.
  template <typename T>
    class singleton_generator
    {
    public:
      singleton_generator(const T& val)
        : value(val)
      { }

      const T& operator()() const { return value; }
    
    private:
      T value;
    };

  // Returns a singleton generator for the given value.
  template <typename T>
    singleton_generator<T> single(const T& value) { return {value}; }


  template <typename T>
    struct single_value_distribution
    {
      single_value_distribution(const T& x) 
        : value(x) 
      { }

      template <typename Eng>
        const T& operator()(Eng&& eng)
        {
          return value;
        }

      T value;
    };


  // The default nullptr distribution returns null pointers.
  struct default_nullptr_distribution
  {
    single_value_distribution<std::nullptr_t> operator()() const 
    {
      return single_value_distribution<std::nullptr_t>(nullptr);
    }
  };

  // The default distribution for Boolean values is a fair Bernoulli trial.
  struct default_bool_distribution
  {
    std::bernoulli_distribution operator()() const
    {
      return std::bernoulli_distribution {};
    }
  };

  // The default distribution for Integer types is a uniform distribution over
  // the extent of encoded values, [min, max].
  template <typename T>
    struct default_int_distribution
    {
      std::uniform_int_distribution<T> operator()() const
      {
        return std::uniform_int_distribution<T> {
          std::numeric_limits<T>::min(),
          std::numeric_limits<T>::max()
        };
      }
    };

  // The default distribution for Floating_point types is a uniform distribution
  // the extent of encoded values [min, max), but excluding inf and -inf.
  //
  // FIXME: Can we make this generate values on the extended real line. That
  // is, can we include inf and -inf as values?
  template <typename T>
    struct default_float_distribution
    {
      std::uniform_real_distribution<T> operator()() const
      {
        return std::uniform_real_distribution<T> {
          std::numeric_limits<T>::min(),
          std::numeric_limits<T>::max()
        };
      }
    };



  // ADL-based association distributions and patterns with types in a given
  // concept.
  template <typename T>
    subst_failure 
    deduce_default_pattern(...);

  // Refinement for Integer types (excluding bool -- see below).
  template <typename T>
    Requires<Integer<T>(), default_int_distribution<T>>
    deduce_default_pattern(testing::adl_t, T);

  // Refinement for Floating_point types.
  template <typename T>
    Requires<Floating_point<T>(), default_float_distribution<T>>
    deduce_default_pattern(testing::adl_t, T);


  // Traits-based association of distributions and patterns with specific types.
  // Unless otherwise specified, the default association uses the concept-based
  // method above.
  template <typename T>
    struct default_pattern_traits
    {
      using type = 
        decltype(deduce_default_pattern(testing::adl_t {}, std::declval<T>()));
    };



  // Unwrap associations to reference types.
  template <typename T>
    struct default_pattern_traits<T&>
    {
      using type = typename default_pattern_traits<T>::type;
    };

  // Unwrap associations to const-qualified types.
  // 
  // TODO: We may need specializations for volatile and cv-qualified types.
  template <typename T>
    struct default_pattern_traits<const T>
    {
      using type = typename default_pattern_traits<T>::type;
    };

  // Specialization for bool. This prevents the default bool distribution
  // from being confused with the default integer distributions.
  template <>
    struct default_pattern_traits<bool>
    {
      using type = default_bool_distribution;
    };

  // Yes... this sometimes happens. The defualt nullptr distribution always
  // returns nulltprs.
  template <>
    struct default_pattern_traits<std::nullptr_t>
    {
      using type = default_nullptr_distribution;
    };


  //////////////////////////////////////////////////////////////////////////////
  // Default Pattern
  //
  // Returns the default input description for T.
  //
  // Template parameters:
  //    T -- The type for which 
  template <typename T>
    inline auto
    default_pattern()
      -> decltype(typename default_pattern_traits<T>::type {}())
    {
      return typename default_pattern_traits<T>::type {}();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Default Pattern Type
  //
  // An alias for the default generator type associated with T.
  template <typename T>
    using Default_pattern_type = decltype(default_pattern<T>());





} // namespace testing
