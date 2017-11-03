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
  // Context 
  //
  // The context class...
  class context
  {
  public:
    context();
    ~context();

    // Singleton access
    static context& instance();

    // Properties

    // Returns a reference to the pseudo-random number generator provided by the
    // testing context.
    std::minstd_rand& random_engine() { return prng; }

    // Get and set the stream to which errors are written 
    std::ostream& error_stream()                { return *os; }
    void          error_stream(std::ostream& s) { this->os = &s; }

    // Returns the number of observed test case failures.
    std::size_t failures() const { return fail; }

    // Returns the number of values instantiated from quantified values for
    // each testable property.
    std::size_t repetitions() const { return repeat; }

    // Testing

    template <typename Prop, typename... Args>
      void check(Prop prop, Args&&... args);

    template <typename Prop, typename... Args>
      void quick_check(Prop prop, Args&&... args);


  // private:

    // Write an error message for a failed test.
    template <typename Prop, typename... Args>
      void error(Prop prop, Args&&... args);

  private:
    // Singleton
    static context* inst;

    // Resources
    std::minstd_rand prng;    // The pseudo-random number generator
    std::ostream* os;         // A logging output stream

    // Quick-check poliices.
    std::size_t repeat;   // Number of repetitions for quantified tests.
    std::size_t fail;     // Number of observed failures
  };


  // Evaluate the given property with respect to the specified arguments as if
  // it were a test case. If the property evaluates to false, then log the
  // failure.
  template <typename Prop, typename... Args>
    void context::check(Prop prop, Args&&... args)
    {
      if (!prop(std::forward<Args>(args)...)) {
        ++fail;
        error(prop, std::forward<Args>(args)...);
      }
    }


  namespace type_impl
  {
    // Support for checking properties. If the requested property is a
    // predicate function, then we instantiate the quantified arguments.
    // Otherwise, we delegate to the testing function, and it will eventually
    // request evaluation of a predicate -- hopefully.

    // FIXME: If Prop returns  bool, but takes generators, this specialization
    // can be instantiated incorrectly, resulting in a copiler error when
    // the expression args()... is used. It may be better to check that
    // all Args... are generating sets.
    template <typename Prop, typename... Args>
      inline Requires<Predicate<Prop, Result_of<Args()>...>(), void>
      check_prop(context& cxt, Prop& prop, Args&&... args)
      {
        for (std::size_t i = 0; i < cxt.repetitions(); ++i)
          cxt.check(prop, args()...);
      }

    // When Prop is not a predicate, we call it as a function.
    template <typename Prop, typename... Args>
      inline Requires<!Predicate<Prop, Result_of<Args()>...>(), void>
      check_prop(context& cxt, Prop& prop, Args&&... args)
      {
        prop(std::forward<Args>(args)...);
      }
  } // namespace type_impl


  // Evaluate the given property using the values in the specified arguments.
  // The property is evaluated a number (this->repeat) of times. Each evaluation
  // is treated as a random test case.
  template <typename Prop, typename... Args>
    void context::quick_check(Prop prop, Args&&... args)
    {
      type_impl::check_prop(*this, prop, std::forward<Args>(args)...);
    }


  namespace type_impl
  {
    // Write the specified argument to os along with its type. The output 
    // format is, "v:T" where v is the value and T is its type.
    template <typename Stream, typename T>
      Requires<Output_streamable<T>(), void>
      log_arg(Stream& os, T&& value)
      {
        os << value << ':' << typestr(std::forward<T>(value));
      }

    // If T is not streamable, indicate that in the output sequence. A 
    // non-streamable value is represented by an underscore: "_:T".
    template <typename Stream, typename T>
      Requires<!Output_streamable<T>(), void>
      log_arg(Stream& os, T&& value)
      {
        os << '_' << ':' << typestr(std::forward<T>(value));
      }


    // If there are no arguments, then we don't have to log them.
    template <typename Stream>
      void log_args(Stream& os)
      { }

    // Write the 1-element sequence of arguments to os.
    template <typename Stream, typename T>
      void log_args(Stream& os, T&& value)
      {
        log_arg(os, std::forward<T>(value));
      }

    // Write a comma-separate sequence of arguments to os.
    template <typename Stream, typename T, typename... Args>
      void log_args(Stream& os, T&& value, Args&&... args)
      {
        log_arg(os, std::forward<T>(value));
        os << ',' << ' ';
        log_args(os, std::forward<Args>(args)...);
      }
  }

  // Write an error message to this->os regarding the failure of a test case.
  // The message includes the name of the failed property and the values of
  // the arguments, if streamable.
  template <typename Prop, typename... Args>
    void
    context::error(Prop prop, Args&&... args)
    {
      *os << "error:" << ' ' << typestr<Prop>() << ':' << ' ';
      type_impl::log_args(*os, std::forward<Args>(args)...);
      *os << '\n';
    }



  //////////////////////////////////////////////////////////////////////////////
  // Quantify Over
  //
  // The quantify_over returns a random value generator (a quantifier) over a
  // set of values. there are two overloads of this function:
  //
  //    quantify_over(dist)
  //    quantifiy_over<T>()
  //
  // These functions are used to generate quantified variables for use with the
  // quick-check function. Note that the objects returned by this function are
  // not quantified over the entire set of values of, say, T. They are random
  // value generators that (should) produce a reasonably well distributed set
  // of values for that type.
  //


  // This overload of the quantify_over function returns a random value
  // generator over the input description (e.g., random number distribution),
  // dist.
  //
  // Parameters:
  //    dist -- A random input description.
  //
  // Returns:
  //    A random value generator.
  //
  // Example: Suppose we want to test a property with integer values that
  // are binomially distributed over 5 trials with even probability. We can
  // construct a distribution and then build a quantifier over it.
  //
  //    binomial_distribution dist {5, 0.5};
  //    auto n = quantify_over(dist);
  //    quick_check(equvalence_relation {}, equal_to<int> {}, n);
  //
  template <typename Dist>
    auto quantify_over(const Dist& dist)
      -> decltype(std::bind(dist, context::instance().random_engine()))
    {
      return std::bind(dist, context::instance().random_engine());
    }


  // This overload of the quantify_over function returns a random value
  // generator over the input description (e.g., random number distribution),
  // dist.
  //
  // Template Parameters:
  //    T -- The type of values to quantify over.
  //
  // Returns:
  //    A random value generator.
  //
  // Example: This is the typical usage scenario for randomized testing. The
  // default quantifier selects all possible values with equal probability.
  //
  //    auto n = quantify_over<int>(cxt);
  //    quick_check(equvalence_relation {}, equal_to<int> {}, n);
  //
  template <typename T>
    auto quantify_over()
      -> decltype(quantify_over(default_pattern<T>()))
    {
      return quantify_over(default_pattern<T>());
    }



  //////////////////////////////////////////////////////////////////////////////
  // Default Quantifier
  //
  // The quantifier type alias refers to the default quantifier for the type T.
  // This type is unspecified, but is gunaranteed to be nullary function whose
  // result type is T.
  template <typename T>
    using Default_quantifier = 
      decltype(quantify_over<T>(std::declval<context&>()));



  //////////////////////////////////////////////////////////////////////////////
  // Check
  //
  // The check function evaluates the property, prop, against the sequence of
  // arguments, args, using the given testing context. Here, prop must be a
  // predicate function that can be called with args... as arugments; it is
  // the test oracle for the input sequence given in args.
  //
  // Parameters:
  //    cxt -- The testing context
  //    prop -- The property under test
  //    args -- A sequence of inputs to the property
  //
  // Notes: Unless you are specifically checking individual values, you 
  // should prefer to use quick_check.
  template <typename Prop, typename... Args>
    inline void
    check(Prop prop, Args&&... args)
    {
      context::instance().check(prop, std::forward<Args>(args)...);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Quick Check
  //
  // The quick_check function evaluates the property, prop, over a sequence of
  // quantified variables, args. Here, each argument in args must be a generator
  // function that returns an input to prop, and prop must be a predicate
  // taking the arguments generated by args. The property is checked in the
  // context of cxt.
  //
  // Parameters:
  //    cxt -- The testing context
  //    prop -- The property under test
  //    args -- A sequence of inputs to the property
  template <typename Prop, typename... Args>
    inline void
    quick_check(Prop prop, Args&&... args)
    {
       context::instance().quick_check(prop, std::forward<Args>(args)...);
    }

} // namespace testing
