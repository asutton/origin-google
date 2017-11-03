// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_SEQUENCE_CONCEPTS_HPP
#define ORIGIN_SEQUENCE_CONCEPTS_HPP

#include <iterator>

#include <origin/type/concepts.hpp>

namespace origin
{
#include "concepts.impl/traits.hpp"

  //////////////////////////////////////////////////////////////////////////////
  // Reading and Writing                                                 iter.rw
  //
  // There are three concepts describing reading from and writing through
  // iterators using the unary * operator.
  //////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////
  // Readable
  //
  // A type I is readable if it defines an associated value type and can be
  // dereferenced. The associated value type (accessed through Value_type<I>)
  // is the type of the object being dereference (excluding references or
  // cv-qualifiers). The result of dereferencing a readable type can be bound
  // to a constant reference to the value type. That is:
  //
  //    const Value_type<I>& x = *i;
  //
  // Must be a valid declaration for all Readable types.
  //
  // A Readable object can be dereferenced multiple times, having the same
  // result each time, assuming there are no intervening changes to the
  // iterator.
  //
  // Template Parameters:
  //    I -- The type being tested
  //
  // Returns:
  //    True if and only if I satisfies the syntactic requirements of the 
  //    Readable concept.
  template <typename I>
    constexpr bool Readable()
    {
      // Some iterators have void as their value type so we can't just write
      // "const Value_type<I>&" when describing that requirement. We have to
      // do this...
      using Ref = Require_lvalue_reference<Add_const<Value_type<I>>>;
      
      return Has_value_type<I>()
          && Has_dereference<I>() 
          && Convertible<Dereference_result<I>, Ref>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Writable
  //
  // An iterator type, I, is Writable if it permits the assignment of values of
  // type T to its referenced objects. That is, the following expression must be
  // valid:
  //
  //    *i = t;
  // 
  // where i has type I and t is an expression having  type T. If I is also
  // Readable and the assigned value type is Equality comparable, then *i == t
  // after the assignment.
  //
  // Note that the type T can be a reference type (e.g., const T& or T&&). This
  // is useful for writing test for copy and move assignment through the
  // iterator.
  //
  // Template Parameters:
  //    I -- An iterator type
  //    T -- The type of values being written through iterators of type I
  //
  // Returns:
  //    True if and only if I satisfies the syntactic requirements of the 
  //    Writable concept with respect to the value type T.
  template <typename I, typename T>
    constexpr bool Writable()
    {
      return Assignable<Dereference_result<I>, T>();
    };


    
  //////////////////////////////////////////////////////////////////////////////
  // Permutable
  //
  // An iterator type, I, is Permutable if its referenced values can be moved
  // between iterators (of the same type). For example.
  //
  //    *i = move(*j);
  //
  // Permutable iterators are required by many of the STL algorithms, including
  // the sort and heap algorithms.
  //
  // Template Parameters:
  //    I -- The type being tested.
  //
  // Retruns:
  //    True if and only if I satisfies the requirements of the Permutable 
  //    concept.
  template <typename I>
    constexpr bool Permutable()
    {
      return Readable<I>() 
          && Movable<Value_type<I>>()
          && Assignable<Dereference_result<I>, Value_type<I>&&>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Mutable
  //
  // An iterator type is mutable if its referenced values can be copied between
  // iterators (of the same type). For example:
  //
  //    *i = *j;
  //
  // Note that all mutable iterator types are also permutable. Mutable iterators
  // are not commoly required by STL algorithms. Most of algorithms that
  // exchange values do so through move operations (i.e,. permutation).
  //
  // Template Parameters:
  //    I -- The type being tested.
  //
  // Retruns:
  //    True if and only if I satisfies the requirements of the Mutable concept.
  template <typename I>
    constexpr bool Mutable()
    {
      return Permutable<I>() 
          && Copyable<Value_type<I>>() 
          && Writable<I, const Value_type<I>&>();
    }
    
    

  //////////////////////////////////////////////////////////////////////////////
  // Incrementable Types                                               iter.incr
  //
  // An incrementable type is one whose value can be incremented or advanced.
  // There are three concepts describing incrementable types: weakly
  // incrementable, incrementable, and decrementable. These concepts provide
  // a basis for writing algorithms that only operate on the traversal aspects
  // of an iterator (e.g., advance and distance).
  //
  // Note that all incrementable types should be equality comparable since the
  // values of these types generally represent an offset, index, or state.
  // However, the C++ standard does not require equality comparison for
  // output iterators, so we cannot enforce this requirement at this level.
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  // Pseudo Incrementable
  //
  // This concept exists solely to describe the synatx and semantics of
  // incrementing strict output iterators like ostream_iterator and
  // ostreambuf_iterator. These types can be incremented, but the operation
  // has virtually no meaning. 
  //
  // This is a terrible concept.
  template <typename I>
    constexpr bool Pseudo_incrementable()
    {
      return Copyable<I>()

          // ++i return I&
          && Has_pre_increment<I>()
          && Same<Pre_increment_result<I>, I&>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Weakly Incrementable
  //
  // A weakly incrementable type, I, can pre-incremented and has an associated
  // difference type. The difference type encodes the maximum difference between
  // any two iterators of type I.
  //
  // A weakly incrementable iterator is not required to be post-incremented
  // because we cannot give strong semantics for the operation. This is a
  // difference from the usual requirements of the C++ standard. Despite the
  // difference, we fully expect that Origin-conforming weakly-incrementable
  // iterators will continue to work with standard iterators.
  //
  // Template Parameters:
  //    I -- The type being tested
  //
  // Returns:
  //    True if and only if I satisfies the requirements of the 
  //    Weakly_incrementable concept.
  //
  // NOTE: Weakly incrementable types are not required to be equality 
  // comparable, even though they should be. This allows us to reuse this
  // concept to describe standard-conforming output iterators.
  template <typename I>
    constexpr bool Weakly_incrementable()
    {
      return Copyable<I>()
          
          // Difference_type<I> must be Signed
          && Has_difference_type<I>()
          && Signed<Difference_type<I>>()
          
          // ++i return I&
          && Has_pre_increment<I>()
          && Same<Pre_increment_result<I>, I&>();
    }
    

  //////////////////////////////////////////////////////////////////////////////
  // Incrementable
  //
  // Like a weakly incremetable types, an incrementable type is also pre- and 
  // post-incremetable, but the requirements on those operations are stronger.
  // In particular, both operations are required to be equality preserving, and
  // the result of post-incrementing an incrementable object returns a copy
  // of the previous value or state.
  //
  // Template Parameters:
  //    I -- The type being tested
  //
  // Returns:
  //    True if I is incrementable.
  template <typename I>
    constexpr bool Incrementable()
    {
      return Weakly_incrementable<I>()

          // i++ returns I
          && Has_post_increment<I>()
          && Same<Post_increment_result<I>, I>()

          && Equality_comparable<I>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Decrementable
  //
  // A decrementable type is an inrementable type that can also be pre- and
  // post-decremented. Like the incrementable concept, the pre- and post-
  // decrement operators are required to be equality preserving, and the
  // post-decrement operator returns a copy of the previous state.
  //
  // Note that there is no weakly decrementable. A type cannot support 
  // decrementing without also supportng incrementing, and the ability to
  // decrement requires that the incremnt operation to be regular.
  template <typename I>
    constexpr bool Decrementable()
    {
      return Incrementable<I>()

          // --i returns I&
          && Has_pre_decrement<I>()
          && Same<Pre_decrement_result<I>, I&>()

          // i-- returns I
          && Has_post_decrement<I>()
          && Same<Post_decrement_result<I>, I>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Strict Weakly Incrementable
  //
  // It is sometimes necessary to overload an algorithm for differently for
  // weakly incrementable types than for incrementable types. This type
  // predicate provides that capability.
  //
  // Template Parameters:
  //    I -- The type being tested
  //
  // Returns:
  //    True if I is weakly incrementable but not incrementable.
  template <typename I>
    constexpr bool Strict_weakly_incrementable()
    {
      return Weakly_incrementable<I>() && !Incrementable<I>();
    }


  // TODO: Create another concept that mirrors random access iterators, but
  // does not include readability or writability. Not sure what the name should
  // be...



  //////////////////////////////////////////////////////////////////////////////
  // Iterator Types                                                   iter.types
  //
  // An Iterator is a type that is incrementable and (inclusively) readable or
  // writable.
  //////////////////////////////////////////////////////////////////////////////



  //////////////////////////////////////////////////////////////////////////////
  // Iterator Category
  //
  // An iterator, I, has an associated tag class called its "category". This
  // describes the concept modeled by the type I.
  //
  // Template Parameters:
  //    I -- An iterator type
  //
  // Aliases:
  //    The iterator category of I.
  //
  // FIXME: This will not fail gracefully. Wrap this in a type trait to prevent
  // unindented substitution or compilation failures.
  template <typename I>
    using Iterator_category = 
      typename sequence_impl::iterator_category_traits<I>::type;


  // Returns true if and only if Iterator_category<I> is a valid type name.
  template <typename I>
    constexpr bool Has_iterator_category()
    {
      return Subst_succeeded<Iterator_category<I>>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Iterator
  //
  // An Iterator, in the most general sense is a type that can be incremented
  // and decremented, although no claims are made on the behavior or result of
  // dereferencing. In that sense, the concept is incomplete but nonetheless
  // useful to describe very general requirements on iterator types.
  //
  // An Iterator is minimally a weakly incrementable and dereferenceable type.
  // Note that no constraints are placed on the result of the dereference
  // operator.
  //
  // Template Parameters:
  //    I -- The type being tested
  //
  // Returns:
  //    True if and only if I satisfies the syntactic requirements of the
  //    Iterator concept.
  template <typename I>
    constexpr bool Iterator()
    {
      return Weakly_incrementable<I>() 
          && Has_dereference<I>()
          && Has_iterator_category<I>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Input Iterator
  //
  // An input iterator is a kind of iterator, I, that allows reading elements of
  // type T from an input sequence or stream in a single pass, much reading data
  // from a tape.
  //
  // Note that there is only ever one "active" iterator in an input sequence:
  // the iterator representing the current position in that sequence. An
  // iterator past the end of the sequence is not considered to be "active".
  //
  // An input iterator is readable, weakly incrementable and equality
  // comparable.
  //
  // Template Parameters:
  //    I -- The type being tested
  //
  // Returns:
  //    True if and only if I satisfies the syntactic requirements of the
  //    Input_iterator concept.
  template <typename I>
    constexpr bool Input_iterator()
    {
      return Readable<I>()
          && Weakly_incrementable<I>() 
          && Equality_comparable<I>()
          && Derived<Iterator_category<I>, std::input_iterator_tag>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Output Iterator
  //
  // An Output_iterator, I, is a kind of iterator that allows writing elements
  // of type T to an output sequence or stream in in a single pass, much like
  // writing data to a tape. The syntax for writing through an iterator is:
  //
  //    *i = t;
  //
  // Note that there is only ever one "active" iterator in an output sequence:
  // the iterator representing the current position in that sequence. An
  // iterator past the end of the sequence is not considered to be "active".
  //
  // There are two kinds of output iterators: (standard) output iterators and
  // strong output iterators. Both are required to be weakly incrementable
  // and copy writable, but a regular output iterator is also required to be
  // equality comparable.
  //
  // Note that there is no corresponding output iterator for that requires or
  // permits move assignment. Moving an objects state into an object of a 
  // different type is not a sound operation.
  //
  // NOTE: Ideally, all iterators should be equality comparable. However, the
  // C++ standard has an established convention of not requiring equality for
  // output iterators and subsequently over-constraining some algorithms that
  // both write through and compare iterators by requiring them to be forward
  // iterators.


  // An output iterator, I, is a weakly incrementable type through which
  // values of type T may be written.
  //
  // Template Parameters:
  //    I -- The type being tested
  //    T -- A type whose values are written through T.
  //
  // Returns:
  //    True if and only if I satisfies the syntactic requirements of the 
  //    Output_iterator concepts with respect to a written type T.
  //
  // NOTE: The iterator category is not checked in this concept, because it
  // isn't obvious that it is ever actually checked. Also, a forward iterator
  // that is also an output iterator does not derive from the output iterator
  // tag class.
  //
  // NOTE: This concept is degenrate. Basically, it has to match the
  // expectations of output iterators in the C++ standard, which require that
  // the value, reference, and pointer types are void. At least the difference
  // type of output iterators should be defined.
  template <typename I, typename T>
    constexpr bool Output_iterator()
    {
      return Pseudo_incrementable<I>()
          && Writable<I, T>();
    }


  // A strong output iterator, I, is an output iterator that is also equality
  // comparable. Strong output iterators occur infrequently in the STL 
  // algorithms. Only fill, generate, and iota require this concept.
  //
  // Template Parameters:
  //    I -- The type being tested
  //    T -- A type whose values are written through T.
  //
  // Returns:
  //    True if and only if I satisfies the syntactic requirements of the 
  //    Strong_output_iterator concepts with respect to a written type T.
  //
  // FIXME: Rename to Regular_output_iterator?
  template <typename I, typename T>
    constexpr bool Strong_output_iterator()
    {
      return Output_iterator<I, T>() && Equality_comparable<I>();
    }
 
 
  //////////////////////////////////////////////////////////////////////////////
  // Forward Iterator
  //
  // A forward iterator, I, is an iterator over a sequence of objects, allowing
  // both reading and possibly writing (if allowed by the underlying object
  // type) and that can be traversed only in a forward direction. Forward 
  // iterators generalize the traversal of singly linked lists.
  //
  // The sequences of objects is required to have a lifetime is greater than
  // that of any iterator over it. This allows multiple passes over the
  // sequence.
  //
  // A forward iterator is an input iterator that is incrementable and readable,
  // and allows multiple passes over its underlying sequence. 
  //
  // Note that an underlying sequence whose objects can be moved is a permutable
  // forward iterator. If the objects can be copied, then the sequence is a
  // mutable forward iterator. The Permutable and Mutable concepts must be
  // required separately.
  //
  // Template Paramters:
  //    I -- The type being tested
  //
  // Returns:
  //    True if and I satisfies the requirements of the Forward_iterator 
  //    concept.
  template <typename I>
    constexpr bool Forward_iterator()
    {
      return Readable<I>() 
          && Incrementable<I>()
          && Derived<Iterator_category<I>, std::forward_iterator_tag>();
    }
    

  //////////////////////////////////////////////////////////////////////////////
  // Bidirectional Iterator
  //
  // A bidirectional iterator, I, is an iterator over a sequence of objects,
  // allowing both reading and possibly writing (if allowed by the underlying
  // object type) and that can be traversed both forwards and backwards.
  // Bidirectional iterators generalize the traversal of doubly linked lists.
  //
  // As with forward iterators, the sequences of objects is required to have a
  // lifetime is greater than that of any iterator over it. This allows multiple
  // passes over the sequence.
  //
  // A bidirectional iterator is a forward iterator that is decrementable.
  //
  // Note that an underlying sequence whose objects can be moved is a permutable
  // bidirectional iterator. If the objects can be copied, then the sequence is
  // a mutable bidirectional iterator. The Permutable and Mutable concepts must
  // be required separately.
  //
  // Template Paramters:
  //    I -- The type being tested
  //
  // Returns:
  //    True if and I satisfies the requirements of the Bidirectional_iterator 
  //    concept.
  template <typename I>
    constexpr bool Bidirectional_iterator()
    {
      return Readable<I>()
          && Decrementable<I>()
          && Derived<Iterator_category<I>, std::bidirectional_iterator_tag>();
    };

    
  //////////////////////////////////////////////////////////////////////////////
  // Random Access Iterator
  //
  // A random access iterator is a bidirectional iterator that can advance 
  // any number of steps in constant time.
  template <typename I>
    constexpr bool Random_access_iterator()
    {
      return Readable<I>()
          && Decrementable<I>()

          // i += n returns I&  
          && Has_plus_assign<I, Difference_type<I>>()
          && Same<Plus_assign_result<I, Difference_type<I>>, I&>()
          
          // i -= n returns I&
          && Has_minus_assign<I, Difference_type<I>>()
          && Same<Minus_assign_result<I, Difference_type<I>>, I&>()
          
          // i + n returns I
          && Has_plus<I, Difference_type<I>>()
          && Same<Plus_result<I, Difference_type<I>>, I>()
          
          // n + i returns I
          && Has_plus<Difference_type<I>, I>()
          && Same<Plus_result<Difference_type<I>, I>, I>()
          
          // i - n returns I
          && Has_minus<I, Difference_type<I>>()
          && Same<Minus_result<I, Difference_type<I>>, I>()
          
          // i - j returns Difference_type<I>
          && Has_minus<I>()
          && Same<Minus_result<I>, Difference_type<I>>()
          
          // i[n] returns Reference_of<I>
          && Has_subscript<I, Difference_type<I>>()
          && Same<Subscript_result<I, Difference_type<I>>, Dereference_result<I>>()

          && Derived<Iterator_category<I>, std::random_access_iterator_tag>();
    };
    
    

  //////////////////////////////////////////////////////////////////////////////
  // Iterator Bounds                                                 iter.bounds
  //
  // The following concepts differentiate different kinds of iterators up to
  // but not including other kinds (e.g., input but not forward, output but not
  // forward, non-random access). 
  //
  // These are not part of the core iterator concepts, and should not be 
  // necessary if concepts can be partially ordered.
  //////////////////////////////////////////////////////////////////////////////


  // Returns true if I is strictly an input iterator, an input iterator but not 
  // a forward iterator.
  template <typename I>
    constexpr bool Strict_input_iterator()
    {
      return Input_iterator<I>() && !Forward_iterator<I>();
    }
    
    
  // Returns true if I is strictly an output iterator, an output iterator but
  // not readable.
  template <typename I>
    constexpr bool Strict_output_iterator()
    {
      return Output_iterator<I> && !Readable<I>();
    }
  

  // Returns true if I is any kind of iterator except random access.
  template <typename I>
    constexpr bool Non_random_access_iterator()
    {
      return Weakly_incrementable<I>() && !Random_access_iterator<I>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Iterator Range Properties                                   iter.properties
  //
  // The following predicates describe properties of iterator ranges (pairs of
  // iterators).
  //
  // Note that none of these functions actually check the stated property. For
  // the most part, these properties are uncheckable. These are primarily
  // provided for the purpose of annotation.
  //////////////////////////////////////////////////////////////////////////////


  // Returns true if [first, n) is a weak range.
  template <typename I>
    inline bool 
    is_weak_range(I first, Difference_type<I> n) 
    { 
      static_assert(Weakly_incrementable<I>(), "");
      return n >= 0;
    }
  
  // Returns true if [first, n) is a counted range.
  template <typename I>
    inline bool 
    is_counted_range(I first, Difference_type<I> n) 
    { 
      static_assert(Weakly_incrementable<I>(), "");
      return n >= 0;
    }
    
  // Returns true if [first, last) is a bounded range.
  template <typename I>
    inline auto 
    is_bounded_range(I first, I last) 
      -> Requires<Non_random_access_iterator<I>(), bool>
    {
      static_assert(Equality_comparable<I>(), "");
      return true; 
    }

  // Overload for random access iterators. Minimally, we can guarantee that
  // first <= last, even though some or all elements of [first, last) may not
  // be valid iterators.
  template <typename I>
    inline auto 
    is_bounded_range(I first, I last) 
      -> Requires<Random_access_iterator<I>(), bool>
    {
      return first <= last; 
    }
    
  // Returns true if the weak range [first, n) is readable everywhere except
  // its limit.
  template <typename I>
    inline bool 
    is_readable_range(I first, Difference_type<I> n) 
    {
      static_assert(Readable<I>(), "");
      return is_weak_range(first, n); 
    }
  
  // Returns true if the bounded range [first, last) is readable everywhere
  // except its limit.
  template <typename I>
    inline bool 
    is_readable_range(I first, I last) 
    { 
      static_assert(Readable<I>(), "");
      return is_bounded_range(first, last); 
    }
    
  // Returns true if the weak range [first, n) is writable everywhere except
  // its limit.
  template <typename I, typename T>
    inline bool 
    is_writable_range(I first, Difference_type<I> n, T const& value)
    {
      static_assert(Writable<I, T>(), "");
      return is_weak_range(first, n);
    }
   
  // Returns true if the bounded range [first, last) is writable everywhere 
  // except its limit.
  template <typename I, typename T>
    inline bool 
    is_writable_range(I first, I last, T const& value) 
    { 
      static_assert(Writable<I, T>(), "");
      return is_bounded_range(first, last);
    }
    
  // Returns true if the weak range [first, n) is mutable everywhere except its
  // limit.
  template <typename I>
    inline bool 
    is_mutable_range(I first, Difference_type<I> n)
    {
      return n > 0 ? is_writable_range(first, n, *first) : true;
    }
    
  // Returns true if the bounded range [first, last) is mutable everywhere
  // except its limit.
  template <typename I>
    inline bool 
    is_mutable_range(I first, I last)
    {
      return first != last ? is_writable_range(first, last, *first) : true;
    }

  // Return true if the weak range [first, n) is permutable everywhere except
  // its limit.
  template <typename I>
    inline bool 
    is_permutable_range(I first, Difference_type<I> n)
    {
      return n > 0 ? is_movable_range(first, n, *first) : true;
    }
    
  // Return true if the bounded range [first, n) is permutable everywhere
  // except its limit.
  template <typename I>
    inline bool 
    is_permutable_range(I first, I last)
    {
      return first != last ? is_movable_range(first, last, *first) : true;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Ranges                                                            seq.range
  //
  // A range is a type that exposes a pair of iterators that are accessible
  // through begin() and end() operations. For example:
  //
  //    R r = ...; // a range
  //    auto first = begin(r);
  //    auto last = end(r);
  //    auto i = find(first, last, value);
  //
  // Ranges also enable the use of the range-based for loop:
  //
  //    for (auto& x : r)
  //      cout << x << ' ';
  //    cout << '\n';
  //
  // Note that the iterator associated with r must be Readable. More 
  // specifically, the requirements of a range-based for loop require the
  // type of r to be an Input_range.
  //
  // The range concepts parallel the iterator concepts. There is a corresponding
  // range concept for each of the iterator concepts.


  //////////////////////////////////////////////////////////////////////////////
  // Range Traits                                               seq.range.traits
  //
  // The begin and end result aliases deduces the type of the iterators i and j
  // in the following program:
  //
  //    using std::begin;
  //    using std::end;
  //    auto i = begin(r);
  //    auto j = end(r);
  //
  // Note that the type referred to by these alias may depends on type of R.
  // If R is const qualified, then the resulting iterator may be a const
  // iterator.


  //////////////////////////////////////////////////////////////////////////////
  // Begin Result
  //
  // Refers to the result of the expression std::begin(r) or end(r).
  //
  // Template Parameters:
  //    R -- A range type
  //
  // Aliases:
  //    The result type of the begin() operation on Range.
  template <typename R>
    using Begin_result = typename sequence_impl::get_begin_result<R>::type;

  // Returns true if either std::begin(r) or begin(r) is a valid expression.
  template <typename R>
    constexpr bool Has_begin()
    {
      return Subst_succeeded<R>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // End Result
  //
  // Refers to the result of the expression std::end(r) or end(r).
  //
  // Template Parameters:
  //    R -- A range type
  //
  // Aliases:
  //
  //  The result type of the end() operation on a Range.
  template <typename R>
    using End_result = typename sequence_impl::get_end_result<R>::type;

  // Returns true if either std::end(r) or end(r) is a valid expression.
  template <typename R>
    constexpr bool Has_end()
    {
      return Subst_succeeded<R>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Iterator Of
  //
  // The iterator type of a range is the type returned by both begin(r) and 
  // end(r) for some range(r). Note that the type referred to this alias
  // may be different for R and const R. However, the alias is always defined
  // for both.
  //
  // Template Parameters:
  //    R -- A range type
  //
  // Aliases:
  //    The type returned by both begin and end operations of a range.
  template <typename R>
    using Iterator_of = Begin_result<R>;



  //////////////////////////////////////////////////////////////////////////////
  // Range
  //
  // A range is type that exposes an underlying iterator that is accessible
  // through the begin and end operations. Note that the Range concept only
  // describes the most basic traversal category of iterators (weakly
  // incrementable), and requires neither reading nor writing.
  template <typename R>
    constexpr bool Range()
    {
      return Has_begin<R>()
          && Has_end<R>()
          && Same<Begin_result<R>, End_result<R>>()
          && Iterator<Iterator_of<R>>();
    }


  //////////////////////////////////////////////////////////////////////////////
  // Input Range
  //
  // Template Parameters:
  //    R -- A range type
  //
  // Returns:
  //    True if and only if R satisfies the syntactic requirements of a Range
  // whose associated iterator type is an Input_iterator.
  template <typename R>
    constexpr bool Input_range()
    {
      return Range<R>() && Input_iterator<Iterator_of<R>>();
    }

  //////////////////////////////////////////////////////////////////////////////
  // Output Range
  //
  // Template Parameters:
  //    R -- A range type
  //
  // Returns:
  //    True if and only if R satisfies the syntactic requirements of a Range
  // whose associated iterator type is an Output_iterator.
  template <typename R>
    constexpr bool Output_range()
    {
      return Range<R>() && Output_iterator<Iterator_of<R>>();
    }

  //////////////////////////////////////////////////////////////////////////////
  // Forward Range
  //
  // Template Parameters:
  //    R -- A range type
  //
  // Returns:
  //    True if and only if R satisfies the syntactic requirements of a Range
  // whose associated iterator type is an Forward_iterator.
  template <typename R>
    constexpr bool Forward_range()
    {
      return Range<R>() && Forward_iterator<Iterator_of<R>>();
    }

  //////////////////////////////////////////////////////////////////////////////
  // Bidirectional Range
  //
  // Template Parameters:
  //    R -- A range type
  //
  // Returns:
  //    True if and only if R satisfies the syntactic requirements of a Range
  // whose associated iterator type is an Bidirectional_iterator.
  template <typename R>
    constexpr bool Bidirectional_range()
    {
      return Range<R>() && Bidirectional_iterator<Iterator_of<R>>();
    }

  //////////////////////////////////////////////////////////////////////////////
  // Random Access Range
  //
  // Template Parameters:
  //    R -- A range type
  //
  // Returns:
  //    True if and only if R satisfies the syntactic requirements of a Range
  // whose associated iterator type is an Random_access_iterator.
  template <typename R>
    constexpr bool Random_access_range()
    {
      return Range<R>() && Random_access_iterator<Iterator_of<R>>();
    }

} // namespace origin

#endif
