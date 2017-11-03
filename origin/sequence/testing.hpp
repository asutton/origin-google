// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_SEQUENCE_TESTING_HPP
#define ORIGIN_SEQUENCE_TESTING_HPP

#include <cassert>

#include <origin/type/testing.hpp>

#include "range.hpp"


namespace origin
{
// Random range generator
#include "testing.impl/random.hpp"

  namespace testing
  {
    ////////////////////////////////////////////////////////////////////////////
    // Sequence Testing                                                 seq.test
    //
    // The sequence testing library provides support testing iterators and
    // ranges. Iterators are not generally testable as isolated values, they
    // always part of a larger range. The test support in this library
    // emphasizes the testing of ranges, where iterator properties are tested as
    // part of the range. For example, testing the properties of Forward_range
    // entails testing of its iterator type as a Forward_iterator.
    ////////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////////
    // Iterator Properties                                   seq.test.properties
    //
    // Note that the iterator properties have two function call overloads: one
    // that takes a pair of arguments representing a bounded range, and one that
    // takes two iterators first and last. Additional arguments may also be
    // specified.
    ////////////////////////////////////////////////////////////////////////////


    // Regularity of Readable Iterators
    //
    // A readable iterator can be dereferenced multiple times, and without any
    // interleaving traversal operators. 
    //
    // Note that it is not universally required that the resulting of
    // dereferencing the same iterator always returns the same value, although
    // that is a general expectation. It could be the case, for example, that
    // a readable iterator refers to a sequence of volatile objects.
    struct regularity_of_reading
    {
      template <typename R>
        bool operator()(R&& range)
        {
          using std::begin;
          using std::end;
          return operator()(begin(range), end(range));
        }

      template <typename I>
        bool operator()(I first, I last) const
        {
          if (first != last) {
            // This could be a one-liner, but I want to sequence the dereference
            // operations (i.e., make the order deterministic).
            const Value_type<I>& a = *first;
            const Value_type<I>& b = *first;
            return ((void)a, (void)b, true);  // Supporess warnings...
          } else {
            return true;
          }
        }
    };


    // Iterator Move Assignment
    //
    // Move-assigning an object, a, through an iterator moves the state of a
    // into the object referenced by the iterator.
    struct iterator_move_assignment
    {
      template <typename R, typename T>
        bool operator()(R&& range, const T& a, const T& b)
        {
          using std::begin;
          using std::end;
          return operator()(begin(range), end(range), a, b);
        }

      template <typename I, typename T>
        bool operator()(I first, I last, const T& a, const T& b) const
        {
          if (first != last) {
            // Note that we can't just write (*first = x) == x because the result
            // type of assignment through an iterator is unspecified. Also, we
            // can only verify the assignment if I us known to be readable.
            // Finally, we can only verify the result when the two inputs are
            // known to be equal.
            *first = std::move(a);
            if (a == b)
              return Readable<I>() ? check_equal(*first, b) : true;
            else
              return true;
          } else {
            return true;
          }
        }
    };


    // Iterator copy Assignment
    //
    // Copy assigning an object, x, through an iterator copies the value of x
    // into the object referenced by the iterator.
    struct iterator_copy_assignment
    {
      template <typename R, typename T>
        bool operator()(R&& range, const T& x)
        {
          using std::begin;
          using std::end;
          return operator()(begin(range), end(range), x);
        }

      template <typename I, typename T>
        bool operator()(I first, I last, const T& x) const
        {
          if (first != last) {
            // Note that we can't just write (*first = x) == x because the result
            // type of assignment through an iterator is unspecified. Also, we
            // can only verify the assignment if I us known to be readable.
            *first = x;
            return Readable<I>() ? check_equal(*first, x) : true;
          } else {
            return true;
          }
        }
    };


    // Pre Increment
    //
    // Checks that the object returned by preincrement is the same as the
    // the object incremented.
    //
    // NOTE: We cannot test the correspondence between pre- and post-increment
    // in this concept. This is why requiring post-increment for weakly
    // incrementable types is fundamentally broken.
    struct pre_increment
    {
      template <typename R>
        bool operator()(R&& range) const
        {
          using std::begin;
          using std::end;
          return operator()(begin(range), end(range));
        }

      template <typename I>
        bool operator()(I first, I last) const
        {
          if (first != last) {
            auto* p = &first;
            auto* q = &++first;
            return p == q;
          } else {
            return true;
          }
        }
    };


    // Post Increment
    //
    // Checks that the object returned by post-increment compares equal to
    // the original value.
    struct post_increment
    {
      template <typename R>
        bool operator()(R&& range) const
        {
          using std::begin;
          using std::end;
          return operator()(begin(range), end(range));
        }

      template <typename I>
        bool operator()(I first, I last) const
        {
          if (first != last) {
            auto orig = first;
            return first++ == orig;
          } else {
            return true;
          }
        }
    };


    // Increment
    //
    // Chck that pre-and post-inrement both increment the given value.
    struct increment
    {
      template <typename R>
        bool operator()(R&& range) const
        {
          using std::begin;
          using std::end;
          return operator()(begin(range), end(range));
        }

      template <typename I>
        bool operator()(I first, I last) const
        {
          if (first != last) {
            auto copy = first;
            return (++first, first) == ++copy;
          } else {
            return true;
          }
        }
    };


    ////////////////////////////////////////////////////////////////////////////
    // Range Concepts                                          seq.test.concepts
    //
    // The following classes implement testing suppot for range and iterator
    // concepts.
    ////////////////////////////////////////////////////////////////////////////



    ////////////////////////////////////////////////////////////////////////////
    // Readable Range
    //
    template <typename R>
      struct readable_range
      {
        template <typename Rv>
          void operator()(Rv&& rvar)
          { 
            quick_check(read, rvar);
          }

        regularity_of_reading read;
      };



    ////////////////////////////////////////////////////////////////////////////
    // Writable Range
    //
    // The objects in a writable range can be assigned values of type T. 
    //
    // Note that this test is specialized so that tests where T has type U&&
    // actually uses the move assignment test. When T has some other type, both
    // copy and move are evaluated.
    template <typename R, typename T>
      struct writable_range
      {
        template <typename Rv, typename Tv>
          void operator()(Rv&& rvar, Tv&& tvar) const
          {
            quick_check(move, rvar, tvar, tvar);
            quick_check(copy, rvar, tvar);
          }

        iterator_move_assignment move;
        iterator_copy_assignment copy;
      };

    // Specialization for move semantics.
    template <typename R, typename T>
      struct writable_range<R, T&&>
      {
        template <typename Rv, typename Tv>
          void operator()(Rv&& rvar, Tv&& tvar) const
          {
            quick_check(move, rvar, tvar, tvar);
          }

        iterator_move_assignment move;
      };


    ////////////////////////////////////////////////////////////////////////////
    // Permutable range
    //
    // A permutable rang allows the states of its referenced objects to be
    // exchanged using move assignment.
    template <typename R>
      struct permutable_range
      {
        template <typename Rv>
          void operator()(Rv&& rvar)
          { 
            auto tvar = quantify_over<Value_type<R>>();
            quick_check(move, rvar, tvar, tvar);
          }

        iterator_move_assignment move;
      };


    ////////////////////////////////////////////////////////////////////////////
    // Mutable Range
    //
    // A mutable range allows the values of its referenced objects to be
    // exchanged or modified using copy assignment.
    template <typename R>
      struct mutable_range
      {
        template <typename Rv>
          void operator()(Rv&& rvar)
          { 
            auto tvar = quantify_over<Value_type<R>>();
            quick_check(move, rvar, tvar, tvar);
            quick_check(copy, rvar, tvar);
          }

        iterator_move_assignment move;
        iterator_copy_assignment copy;
      };


    ////////////////////////////////////////////////////////////////////////////
    // Weakly Incrementable Range
    //
    // NOTE: This can't really be tested with weakly incrementable types. 
    template <typename R>
      struct weakly_incrementable_range
      {
        template <typename Rv>
          void operator()(Rv&& rvar) const
          {
            quick_check(incr, rvar);
          }

        pre_increment incr;
      };


    ////////////////////////////////////////////////////////////////////////////
    // Incrementable Range
    //
    template <typename R>
      struct incrementable_range
      {
        template <typename Rv>
          void operator()(Rv&& rvar) const
          {
            quick_check(pre, rvar);
            quick_check(post, rvar);
            quick_check(inc, rvar);
          }

        pre_increment  pre;
        post_increment post;
        increment      inc;
      };


    ////////////////////////////////////////////////////////////////////////////
    // Input Range
    //
    template <typename R>
      struct input_range
      {
        template <typename Rv>
          void operator()(Rv&& rvar) const
          {
            quick_check(inc, rvar);
            quick_check(read, rvar);
          }

        weakly_incrementable_range<R> inc;
        readable_range<R>             read;
      };



    ////////////////////////////////////////////////////////////////////////////
    // Output Range
    //
    template <typename R, typename T>
      struct output_range
      {
        template <typename Rv, typename Tv>
          void operator()(Rv&& rvar, Tv&& tvar) const
          {
            quick_check(inc, rvar);
            quick_check(write, rvar, tvar);
          }

        weakly_incrementable_range<R> inc;
        writable_range<R, T>          write;
      };



    ////////////////////////////////////////////////////////////////////////////
    // Forward Range
    //
    template <typename R>
      struct forward_range
      {
        template <typename Rv>
          void operator()(Rv&& rvar) const
          {
            quick_check(inc, rvar);
            quick_check(read, rvar);
          }

        incrementable_range<R> inc;
        readable_range<R>      read;
      };



    ////////////////////////////////////////////////////////////////////////////
    // Check Functions                                            seq.test.check
    ////////////////////////////////////////////////////////////////////////////

    // Check that R satisfies the requirements of Readable_range.
    template <typename R>
      void 
      check_readable_range(R&& range)
      {
        auto rvar = quantify_over_range(range);
        quick_check(readable_range<Remove_reference<R>> {}, rvar);
      }

    // Check that R sastisfies the requirements of Writable_range with respect
    // to the type T. Note that T may be (and is generally expected to be)
    // a reference type.
    template <typename T, typename R>
      void 
      check_writable_range(R&& range)
      {
        auto rvar = quantify_over_range(range);
        auto tvar = quantify_over<T>();
        quick_check(writable_range<R, T> {}, rvar, tvar);
      }

    // Check that R sastisifes the requirements of Permutable_range.
    template <typename R>
      void 
      check_permutable_range(R&& range)
      {
        auto rvar = quantify_over_range(range);
        quick_check(permutable_range<Remove_reference<R>> {}, rvar);
      }

    // Check that R satisfies the requirements of Mutable_range.
    template <typename R>
      void 
      check_mutable_range(R&& range)
      {
        auto rvar = quantify_over_range(range);
        quick_check(mutable_range<Remove_reference<R>> {}, rvar);
      }

    template <typename R>
      void 
      check_weakly_incrementable_range(R&& range)
      {
        auto rvar = quantify_over_range(range);
        quick_check(weakly_incrementable_range<Remove_reference<R>> {}, rvar);
      }

    template <typename R>
      void check_incrementable_range(R&& range)
      {
        auto rvar = quantify_over_range(range);
        quick_check(incrementable_range<Remove_reference<R>> {}, rvar);
      }


    // Check that I satisfies the requirements of the Input_iterator concept.
    template <typename R>
      void
      check_input_range(R&& range)
      {
        auto rvar = quantify_over_range(range);
        quick_check(input_range<Remove_reference<R>> {}, rvar);
      }

    template <typename T, typename R>
      void
      check_output_range(R&& range)
      {
        auto tvar = quantify_over<T>();
        auto rvar = quantify_over_range(range);
        quick_check(output_range<Remove_reference<R>, T> {}, rvar, tvar);
      }

    template <typename R>
      void
      check_forward_range(R&& range)
      {
        auto rvar = quantify_over_range<R>(range);
        quick_check(forward_range<Remove_reference<R>> {}, rvar);
      }

    template <typename I>
      void
      check_bidirectional_iterator(context& cxt)
      {

      }

    template <typename I>
      void
      check_random_access_iterator(context& cxt)
      {

      }

  } // namespace testing

} // namespace origin

#endif
