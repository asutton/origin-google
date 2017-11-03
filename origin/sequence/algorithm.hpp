// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_SEQUENCE_ALGORITHM_HPP
#define ORIGIN_SEQUENCE_ALGORITHM_HPP

#include <algorithm>

#include "concepts.hpp"

namespace origin
{
  // ------------------------------------------------------------------------ //
  //                                                                [algo.quant]
  //                              Quantifiers
  //
  // The quantifier algorithms evaluate a range of elements to determine if
  // all elements in that range posess that property, some (or any) do, or
  // if none do. The property is defined by a predicate function.


  // ------------------------------------------------------------------------ //
  //                                                            [algo.quant.all]
  //                                  All
  //
  // The all algorithms return true when some condition holds for all elements
  // in a range. There are serveral variations:
  //
  //    all_of(first, last, pred)
  //    all_of(range, pred)
  //
  //    all_match(first, last, value)
  //    all_match(first, last, value, comp)
  //    all_match(range, value)
  //    all_match(range, value, comp)

  // Returns true iff pred(x) is true for all x in range.
  template <typename R, typename P>
    inline bool
    all_of(const R& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::all_of(begin(range), end(range), pred);
    }

  // Returns true iff x == value for all x in [first, last).
  //
  // FIXME: This should call all_match(first, last, value, eq()), but I don't
  // have advanced functional support yet.
  template<typename I, typename T>
    inline Requires<Input_iterator<I>(), bool>
    all_match(I first, I last, const T& value)
    {
      while (first != last && *first == value)
        ++first;
      return first == last;
    }

  // Returns true iff comp(x, value) for all x in [first, last).
  template<typename I, typename T, typename C>
    inline bool
    all_match(I first, I last, const T& value, C comp)
    {
      while (first != last && comp(*first, value))
        ++first;
      return first == last;
    }

  // Returns true iff x == value for all x in range.
  template<typename R, typename T>
    inline bool
    all_match(const R& range, const T& value)
    {
      using std::begin;
      using std::end;
      return all_match(begin(range), end(range), value);
    }

  template<typename R, typename T, typename C>
    inline Requires<Input_range<R>(), bool>
    all_match(const R& range, const T& value, C comp)
    {
      using std::begin;
      using std::end;
      return all_match(begin(range), end(range), value, comp);
    }

  // ------------------------------------------------------------------------ //
  //                                                            [algo.quant.any]
  //                                 Any
  //
  // The any algorithms return true when some condition holds for any element
  // in a range. There are serveral variations:
  //
  //    any_of(first, last, pred)
  //    any_of(range, pred)
  //
  //    any_match(first, last, value)
  //    any_match(first, last, value, comp)
  //    any_match(range, value)
  //    any_match(range, value, comp)

  // Returns true if pred(x) is true for any element in range.
  template <typename R, typename P>
    inline bool
    any_of(const R& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::any_of(begin(range), end(range), pred);
    }

  // Returns true if x == value for some x in [first, last).
  template<typename I, typename T>
    inline Requires<Input_iterator<I>(), bool>
    any_match(I first, I last, const T& value)
    {
      while (first != last && *first != value)
        ++first;
      return first != last;
    }

  // Returns true if comp(x, value) is true for some x in [first, last).
  template<typename I, typename T, typename C>
    inline bool
    any_match(I first, I last, const T& value, C comp)
    {
      while (first != last && !comp(*first, value))
        ++first;
      return first != last;
    }

  // Returns true if x == value for some x in range.
  template<typename R, typename T>
    inline bool
    any_match(const R& range, const T& value)
    {
      using std::begin;
      using std::end;
      return any_match(begin(range), end(range), value);
    }

  template<typename R, typename T, typename C>
    inline Requires<Input_range<R>(), bool>
    any_match(const R& range, const T& value, C comp)
    {
      using std::begin;
      using std::end;
      return any_match(begin(range), end(range), value, comp);
    }


  // ------------------------------------------------------------------------ //
  //                                                           [algo.quant.none]
  //                                  None
  //
  // The any algorithms return true when some condition holds for any element
  // in a range. There are serveral variations:
  //
  //    none_of(first, last, pred)
  //    none_of(range, pred)
  //
  //    none_match(first, last, value)
  //    none_match(first, last, value, comp)
  //    none_match(range, value)
  //    none_match(range, value, comp)
    
  // Returns true if pred(x) is false for all x in range.
  template<typename R, typename P>
    inline bool
    none_of(const R& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::none_of(begin(range), end(range), pred);
    }

  // Returns true x == value is false for all x in [first, last).
  template<typename I, typename T>
    inline Requires<Input_iterator<I>(), bool>
    none_match(I first, I last, const T& value)
    {
      while (first != last && *first != value)
        ++first;
      return first == last;
    }

  template<typename I, typename T, typename C>
    inline bool
    none_match(I first, I last, const T& value, C comp)
    {
      while (first != last && !comp(*first, value))
        ++first;
      return first == last;
    }

  template<typename R, typename T>
    inline bool
    none_match(const R& range, const T& value)
    {
      using std::begin;
      using std::end;
      return none_match(begin(range), end(range), value);
    }

  template<typename R, typename T, typename C>
    inline Requires<Input_range<R>(), bool>
    none_match(const R& range, const T& value, C comp)
    {
      using std::begin;
      using std::end;
      return none_match(begin(range), end(range), value, comp);
    }


  // ------------------------------------------------------------------------ //
  //                                                            [algo.for_eaach]
  //                                  For Each
  //
  // The for_each algorithm applies the function, f, to each iterator i in
  // range.
  //
  // Parameters:
  //    range -- An Input_range
  //    f -- A unry Function on the value type of range
  //
  // Returns:
  //    The function f.
  template <typename R, typename F>
    inline F
    for_each(R&& range, F f)
    {
      using std::begin;
      using std::end;
      std::for_each(begin(range), end(range), f);
    }



  // ------------------------------------------------------------------------ //
  //                                                                 [algo.find]
  //                                    Find
  //
  // The find algorithms search///

  template <typename R, typename T>
    inline Iterator_of<R> 
    find(R&& range, const T& value)
    {
      using std::begin;
      using std::end;
      return std::find(begin(range), end(range), value);
    }


  template <typename R, typename P>
    inline Iterator_of<R>
    find_if(R&& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::find_if(begin(range), end(range), pred);
    }

  template <typename R, typename P>
    inline Iterator_of<R>
    find_if_not(R&& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::find_if_not(begin(range), end(range), pred);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Find Next
  //
  // Returns the first iterator i in [first + 1, last) where *i == value.
  //
  // Parameters:
  //    - first, last -- A bounded readable range
  //    - value       -- A value
  //
  // Returns:
  //   The first iterator in [first + 1, last) where *i == value.
  //
  // Remarks: 
  //   This algorithm is not a part of the C++ standard library.
  template <typename I, typename T>
    inline I
    find_next(I first, I last, const T& value)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Equality_comparable<T, Value_type<I>>(), "");
      if (first != last)
        return std::find(std::next(first), last, value);
      else
        return last;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Find Next If
  //
  // Returns the first iterator i in [first + 1, last) where pred(*i) is true.
  //
  // Parameters:
  //    - first, last -- A bounded readable range
  //    - pred        -- A predicate function
  //
  // Returns:
  //   The first iterator in [first + 1, last) where pred(*i) is true.
  //
  // Remarks: 
  //   This algorithm is not a part of the C++ standard library.
  template <typename I, typename P>
    inline I
    find_next_if(I first, I last, P pred)
    {
      static_assert(Input_iterator<I>(), "");
      static_assert(Predicate<P, Value_type<I>>(), "");
      if (first != last)
        return std::find_if(std::next(first), last, pred);
      else
        return last;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Find End
  //
  
  // NOTE: This should be called search_end or search_last. It's not really
  // a find algorithm.
  template <typename R1, typename R2>
    inline Iterator_of<R1>
    find_end(R1&& range1, const R2& range2)
    {
      using std::begin;
      using std::end;
      return std::find_end(begin(range1), end(range1), 
                           begin(range2), end(range2));
    }

  template <typename R1, typename R2, typename C>
    inline Iterator_of<R1>
    find_end(R1&& range1, const R2& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::find_end(begin(range1), end(range1),
                           begin(range2), end(range2), 
                           comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Adjacent Find
  //
  // NOTE: This shold be called find_adjacent -- a much better name for it.
  template <typename R>
    inline Iterator_of<R>
    adjacent_find(R&& range)
    {
      using std::begin;
      using std::end;
      return std::adjacent_find(begin(range), end(range));
    }

  template <typename R, typename C>
    inline Iterator_of<R>
    adjacent_find(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::adjacent_find(begin(range), end(range), comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Count
  //
  //////////////////////////////////////////////////////////////////////////////

  template <typename R, typename T>
    inline Difference_type<R>
    count(const R& range, const T& value)
    {
      using std::begin;
      using std::end;
      return std::count(begin(range), end(range), value);
    }

  template <typename R, typename P>
    inline Difference_type<R>
    count_if(const R& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::count_if(begin(range), end(range), pred);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Mismatch
  //
  // NOTE: This is named range_mismatch to avoid overload ambiguities with
  // std::mismatch.
  template <typename R1, typename R2>
    inline std::pair<Iterator_of<R1>, Iterator_of<R2>>
    range_mismatch(R1&& range1, R2&& range2)
    {
      using std::begin;
      using std::end;
      return std::mismatch(begin(range1), end(range1), begin(range2));
    }

  template <typename R1, typename R2, typename C>
    inline std::pair<Iterator_of<R1>, Iterator_of<R2>>
    range_mismatch(R1&& range1, R2&& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::mismatch(begin(range1), end(range1), begin(range2), comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Equal
  //
  // FIXME: This is a terrible name for equal since std::equal_range does
  // something decidedly different.

  template <typename R1, typename R2>
    inline bool
    range_equal(const R1& range1, const R2& range2)
    {
      using std::begin;
      using std::end;
      return std::equal(begin(range1), end(range1), begin(range2));
    }

  template <typename R1, typename R2, typename C>
    inline bool
    range_equal(const R1& range1, const R2& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::equal(begin(range1), end(range1), begin(range2), comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Is Permutation
  //

  template <typename R1, typename R2>
    inline bool 
    range_is_permutation(const R1& range1, const R2& range2)
    {
      using std::begin;
      using std::end;
      return std::is_permutation(begin(range1), end(range1), begin(range2));
    }

  template <typename R1, typename R2, typename C>
    inline bool 
    range_is_permutation(const R1& range1, const R2& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::is_permutation(begin(range1), end(range1), 
                                 begin(range2), 
                                 comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Search
  //
  template <typename R1, typename R2>
    inline Iterator_of<R1>
    search(R1&& range1, const R2& range2)
    {
      using std::begin;
      using std::end;
      return std::search(begin(range1), end(range1), 
                         begin(range2), end(range2)); 
    }

  template <typename R1, typename R2, typename C>
    inline Iterator_of<R1>
    search(R1&& range1, const R2& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::search(begin(range1), end(range1), 
                         begin(range2), end(range2),
                         comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Search N
  //
  template <typename R, typename T>
    inline Iterator_of<R>
    search_n(R&& range, Difference_type<R> n, const T& value)
    {
      using std::begin;
      using std::end;
      return std::search_n(begin(range), end(range), n, value);
    }

  template <typename R, typename T, typename C>
    inline Iterator_of<R>
    search_n(R&& range, Difference_type<R> n, const T& value, C comp)
    {
      using std::begin;
      using std::end;
      return std::search_n(begin(range), end(range), n, value, comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Copy
  //

  template <typename R1, typename R2>
    inline Iterator_of<R2>
    copy(const R1& range1, R2&& range2)
    {
      using std::begin;
      using std::end;
      return std::copy(begin(range1), end(range1), begin(range2));
    }

  template <typename R1, typename R2, typename P>
    inline Iterator_of<R2>
    copy_if(const R1& range1, R2&& range2, P pred)
    {
      using std::begin;
      using std::end;
      return std::copy_if(begin(range1), end(range1), begin(range2), pred);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Move
  //

  template <typename R1, typename R2>
    inline Iterator_of<R2>
    move(const R1& range1, R2&& range2)
    {
      using std::begin;
      using std::end;
      return std::move(begin(range1), end(range1), begin(range2));
    }



  //////////////////////////////////////////////////////////////////////////////
  // Fill
  //

  template <typename R, typename T>
    inline void
    fill(R&& range, const T& value)
    {
      using std::begin;
      using std::end;
      std::fill(begin(range), end(range), value);
    }



  //////////////////////////////////////////////////////////////////////////////
  // Generate
  //

  template <typename R, typename Gen>
    inline Gen
    generate(R&& range, Gen gen)
    {
      using std::begin;
      using std::end;
      std::generate(begin(range), end(range), gen);
      return gen;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Transform
  //

  template <typename R1, typename R2, typename Op>
    inline Iterator_of<R2>
    range_transform(const R1& range1, R2&& range2, Op op)
    {
      using std::begin;
      using std::end;
      return std::transform(begin(range1), end(range1), begin(range2), op);
    }

  template <typename R1, typename R2, typename R3, typename Op>
    inline Iterator_of<R3>
    range_transform(const R1& range1, const R2& range2, R3&& range3, Op op)
    {
      using std::begin;
      using std::end;
      return std::transform(begin(range1), end(range1), 
                            begin(range2), 
                            begin(range3), 
                            op);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Replace
  //

  template <typename R, typename T>
    inline void
    replace(R&& range, const T& orig, const T& value)
    {
      using std::begin;
      using std::end;
      std::replace(begin(range), end(range), orig, value);
    }

  template <typename R, typename P, typename T>
    inline void
    replace_if(R&& range, P pred, const T& value)
    {
      using std::begin;
      using std::end;
      std::replace_if(begin(range), end(range), pred, value);
    }

  template <typename R1, typename R2, typename T>
    inline Iterator_of<R2>
    replace_copy(const R1& range1, R2&& range2, const T& orig, const T& value)
    {
      using std::begin;
      using std::end;
      return std::replace_copy(begin(range1), end(range1), 
                               begin(range2),
                               orig,
                               value);
    }

  template <typename R1, typename R2, typename P, typename T>
    inline Iterator_of<R2>
    replace_copy_if(const R1& range1, R2&& range2, P pred, const T& value)
    {
      using std::begin;
      using std::end;
      return std::replace_copy_if(begin(range1), end(range1), 
                                 begin(range2), 
                                 pred, 
                                 value);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Remove
  //

  template <typename R, typename T>
    inline Iterator_of<R>
    remove(R&& range, const T& value)
    {
      using std::begin;
      using std::end;
      return std::remove(begin(range), end(range), value);
    }

  template <typename R, typename P>
    inline Iterator_of<R>
    remove_if(R&& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::remove_if(begin(range), end(range), pred);
    }

  template <typename R1, typename R2, typename T>
    inline Iterator_of<R2>
    remove_copy(const R1& range1, R2&& range2, const T& value)
    {
      using std::begin;
      using std::end;
      return std::remove_copy(begin(range1), end(range1), begin(range2), value);
    }

  template <typename R1, typename R2, typename P>
    inline Iterator_of<R2>
    remove_copy_if(const R1& range1, R2&& range2, P pred)
    {
      using std::begin;
      using std::end;
      return std::remove_copy_if(begin(range1), end(range1), 
                                 begin(range2), 
                                 pred);
    }



  //////////////////////////////////////////////////////////////////////////////
  // Unique
  //

  template <typename R>
    inline Iterator_of<R>
    unique(R&& range)
    {
      using std::begin;
      using std::end;
      return std::unique(begin(range), end(range));
    }

  template <typename R, typename C>
    inline Iterator_of<R>
    unique(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::unique(begin(range), end(range), comp);
    }

  // NOTE: The ternary range overload collides with the ternary iterator
  // opverload, so we differentiate the overloads by name.

  template <typename R1, typename R2>
    inline Iterator_of<R2>
    range_unique_copy(const R1& range1, R2&& range2)
    {
      using std::begin;
      using std::end;
      return std::unique_copy(begin(range1), end(range1), begin(range2));
    }

  template <typename R1, typename R2, typename C>
    inline Iterator_of<R2>
    range_unique_copy(const R1& range1, R2&& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::unique_copy(begin(range1), end(range1), begin(range2), comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Reverse
  //

  template <typename R>
    inline void
    reverse(R&& range)
    {
      using std::begin;
      using std::end;
      std::reverse(begin(range), end(range));
    }

  template <typename R1, typename R2>
    inline void
    reverse_copy(const R1& range1, R2&& range2)
    {
      using std::begin;
      using std::end;
      std::reverse_copy(begin(range1), end(range1), begin(range2));
    }


  //////////////////////////////////////////////////////////////////////////////
  // Shuffle
  //
  //
  // NOTE: It would be nice to get rid of the original random_shuffle algoriths
  // and just focus on the new random framework.

  template <typename R>
    inline void
    random_shuffle(R&& range)
    {
      using std::begin;
      using std::end;
      return std::random_shuffle(begin(range), end(range));
    }

  template <typename R, typename Gen>
    inline void
    random_shuffle(R&& range, Gen&& gen)
    {
      using std::begin;
      using std::end;
      return std::random_shuffle(begin(range), end(range), 
                                 std::forward<Gen>(gen));
    }

  template <typename R, typename Gen>
    inline void
    shuffle(R&& range, Gen&& gen)
    {
      using std::begin;
      using std::end;
      return std::shuffle(begin(range), end(range), std::forward<Gen>(gen));
    }


  //////////////////////////////////////////////////////////////////////////////
  // Partitions

  template <typename R, typename P>
    inline bool
    is_partitioned(const R& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::is_partitioned(begin(range), end(range), pred);
    }

  template <typename R, typename P>
    inline Iterator_of<R>
    partition(R&& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::partition(begin(range), end(range), pred);
    }

  template <typename R, typename P>
    inline Iterator_of<R>
    stable_partition(R&& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::stable_partition(begin(range), end(range), pred);
    }

  template <typename R1, typename R2, typename R3, typename P>
    inline std::pair<Iterator_of<R2>, Iterator_of<R3>>
    partition_copy(const R1& range1, R2&& true_range, R3&& false_range, P pred)
    {
      using std::begin;
      using std::end;
      return std::partition_copy(begin(range1), end(range1), 
                                 begin(true_range),
                                 begin(false_range),
                                 pred);
    }

  template <typename R, typename P>
    inline Iterator_of<R>
    partition_point(R&& range, P pred)
    {
      using std::begin;
      using std::end;
      return std::partition_point(begin(range), end(range), pred);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Sorting
  //
  //////////////////////////////////////////////////////////////////////////////


  template <typename R>
    inline void
    sort(R&& range)
    {
      using std::begin;
      using std::end;
      std::sort(begin(range), end(range));
    }

  // NOTE: This avoids overlaod collision because the argument types are
  // different (std::sort takes two iterators). If we relax the requirement
  // that iterators must the same, then this won't work without a lot of
  // enable-if'ing.
  template <typename R, typename C>
    inline void
    sort(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      std::sort(begin(range), end(range), comp);
    }

  template <typename R>
    inline void 
    stable_sort(R&& range)
    {
      using std::begin;
      using std::end;
      std::stable_sort(begin(range), end(range));
    }

  template <typename R, typename C>
    inline void
    stable_sort(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      std::stable_sort(begin(range), end(range), comp);
    }


  // NOTE: We don't provide a range-based partial_sort for the same reason that
  // we don't provide a range-based rotate: a middle iterator.

  template <typename R1, typename R2>
    inline Iterator_of<R2>
    partial_sort_copy(const R1& range1, R2&& range2)
    {
      using std::begin;
      using std::end;
      return std::partial_sort_copy(begin(range1), end(range1), 
                                    begin(range2), end(range2));
    }


  template <typename R1, typename R2, typename C>
    inline Iterator_of<R2>
    partial_sort_copy(const R1& range1, R2&& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::partial_sort_copy(begin(range1), end(range1), 
                                    begin(range2), end(range2),
                                    comp);
    }

  template <typename R>
    inline bool
    is_sorted(const R& range)
    {
      using std::begin;
      using std::end;
      return std::is_sorted(begin(range), end(range));
    }

  template <typename R, typename C>
    inline bool
    is_sorted(const R& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::is_sorted(begin(range), end(range), comp);
    }

  template <typename R>
    inline Iterator_of<R>
    is_sorted_until(R&& range)
    {
      using std::begin;
      using std::end;
      return std::is_sorted_until(begin(range), end(range));
    }

  template <typename R, typename C>
    inline Iterator_of<R>
    is_sorted_until(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::is_sorted_until(begin(range), end(range), comp);
    }



  //////////////////////////////////////////////////////////////////////////////
  // Binary Search

  template <typename R, typename T>
    inline Iterator_of<R>
    lower_bound(R&& range, const T& value)
    {
      using std::begin;
      using std::end;
      return std::lower_bound(begin(range), end(range), value);
    }

  template <typename R, typename T, typename C>
    inline Iterator_of<R>
    lower_bound(R&& range, const T& value, C comp)
    {
      using std::begin;
      using std::end;
      return std::lower_bound(begin(range), end(range), value, comp);
    }

  template <typename R, typename T>
    inline Iterator_of<R>
    upper_bound(R&& range, const T& value)
    {
      using std::begin;
      using std::end;
      return std::upper_bound(begin(range), end(range), value);
    }

  template <typename R, typename T, typename C>
    inline Iterator_of<R>
    upper_bound(R&& range, const T& value, C comp)
    {
      using std::begin;
      using std::end;
      return std::upper_bound(begin(range), end(range), value, comp);
    }

  template <typename R, typename T>
    inline std::pair<Iterator_of<R>, Iterator_of<R>>
    equal_range(R&& range, const T& value)
    {
      using std::begin;
      using std::end;
      return std::equal_range(begin(range), end(range), value);
    }

  template <typename R, typename T, typename C>
    inline std::pair<Iterator_of<R>, Iterator_of<R>>
    equal_range(R&& range, const T& value, C comp)
    {
      using std::begin;
      using std::end;
      return std::equal_range(begin(range), end(range), value, comp);
    }

  template <typename R, typename T>
    inline bool
    binary_search(const R& range, const T& value)
    {
      using std::begin;
      using std::end;
      return std::binary_search(begin(range), end(range), value);
    }

  template <typename R, typename T, typename C>
    inline bool
    binary_search(const R& range, const T& value, C comp)
    {
      using std::begin;
      using std::end;
      return std::binary_search(begin(range), end(range), value, comp);
    }



  //////////////////////////////////////////////////////////////////////////////
  // Merge

  template <typename R1, typename R2, typename R3>
    inline Iterator_of<R3>
    merge(const R1& range1, const R2& range2, R3&& range3)
    {
      using std::begin;
      using std::end;
      return std::merge(begin(range1), end(range1),
                        begin(range2), end(range2),
                        begin(range3));
    }

  template <typename R1, typename R2, typename R3, typename C>
    inline Iterator_of<R3>
    merge(const R1& range1, const R2& range2, R3&& range3, C comp)
    {
      using std::begin;
      using std::end;
      return std::merge(begin(range1), end(range1),
                        begin(range2), end(range2),
                        begin(range3),
                        comp);
    }


  //////////////////////////////////////////////////////////////////////////////
  // Set Operations

  template <typename R1, typename R2>
    inline bool
    includes(const R1& range1, const R2& range2)
    {
      using std::begin;
      using std::end;
      return std::includes(begin(range1), end(range1), 
                           begin(range2), end(range2));
    }

  template <typename R1, typename R2, typename C>
    inline bool
    includes(const R1& range1, const R2& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::includes(begin(range1), end(range1), 
                           begin(range2), end(range2),
                           comp);
    }


  template <typename R1, typename R2, typename R3>
    inline Iterator_of<R3>
    set_union(const R1& range1, const R2& range2, R3&& result)
    {
      using std::begin;
      using std::end;
      return std::set_union(begin(range1), end(range1), 
                            begin(range2), end(range2),
                            begin(result));
    }

  template <typename R1, typename R2, typename R3, typename C>
    inline Iterator_of<R3>
    set_union(const R1& range1, const R2& range2, R3&& result, C comp)
    {
      using std::begin;
      using std::end;
      return std::set_union(begin(range1), end(range1), 
                            begin(range2), end(range2),
                            begin(result),
                            comp);
    }


  template <typename R1, typename R2, typename R3>
    inline Iterator_of<R3>
    set_intersection(const R1& range1, const R2& range2, R3&& result)
    {
      using std::begin;
      using std::end;
      return std::set_intersection(begin(range1), end(range1), 
                                   begin(range2), end(range2),
                                   begin(result));
    }

  template <typename R1, typename R2, typename R3, typename C>
    inline Iterator_of<R3>
    set_intersection(const R1& range1, const R2& range2, R3&& result, C comp)
    {
      using std::begin;
      using std::end;
      return std::set_intersection(begin(range1), end(range1), 
                                   begin(range2), end(range2),
                                   begin(result),
                                   comp);
    }


  template <typename R1, typename R2, typename R3>
    inline Iterator_of<R3>
    set_difference(const R1& range1, const R2& range2, R3&& result)
    {
      using std::begin;
      using std::end;
      return std::set_difference(begin(range1), end(range1), 
                                 begin(range2), end(range2),
                                 begin(result));
    }

  template <typename R1, typename R2, typename R3, typename C>
    inline Iterator_of<R3>
    set_difference(const R1& range1, const R2& range2, R3&& result, C comp)
    {
      using std::begin;
      using std::end;
      return std::set_difference(begin(range1), end(range1), 
                                 begin(range2), end(range2),
                                 begin(result),
                                 comp);
    }

  template <typename R1, typename R2, typename R3>
    inline Iterator_of<R3>
    set_symmetric_difference(const R1& range1, const R2& range2, R3&& result)
    {
      using std::begin;
      using std::end;
      return std::set_difference(begin(range1), end(range1), 
                                 begin(range2), end(range2),
                                 begin(result));
    }

  template <typename R1, typename R2, typename R3, typename C>
    inline Iterator_of<R3>
    set_symmetric_difference(const R1& range1, const R2& range2, R3&& result, C comp)
    {
      using std::begin;
      using std::end;
      return std::set_symmetric_difference(begin(range1), end(range1), 
                                           begin(range2), end(range2),
                                           begin(result),
                                           comp);
    }

  //////////////////////////////////////////////////////////////////////////////
  // Heap Operations

  template <typename R>
    inline void 
    push_heap(R&& range)
    {
      using std::begin;
      using std::end;
      std::push_heap(begin(range), end(range));
    }

  template <typename R, typename C>
    inline void
    push_heap(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      std::push_heap(begin(range), end(range), comp);
    }

  template <typename R>
    inline void
    pop_heap(R&& range)
    {
      using std::begin;
      using std::end;
      std::pop_heap(begin(range), end(range));
    }

  template <typename R, typename C>
    inline void
    pop_heap(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      std::pop_heap(begin(range), end(range), comp);
    }

  template <typename R>
    inline void
    make_heap(R&& range)
    {
      using std::begin;
      using std::end;
      std::make_heap(begin(range), end(range));
    }

  template <typename R, typename C>
    inline void
    make_heap(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      std::make_heap(begin(range), end(range), comp);
    }

  template <typename R>
    inline void
    sort_heap(R&& range)
    {
      using std::begin;
      using std::end;
      std::sort_heap(begin(range), end(range));
    }

  template <typename R, typename C>
    inline void
    sort_heap(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      std::sort_heap(begin(range), end(range), comp);
    }

  template <typename R>
    inline bool
    is_heap(R&& range)
    {
      using std::begin;
      using std::end;
      return std::is_heap(begin(range), end(range));
    }

  template <typename R, typename C>
    inline bool
    is_heap(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::is_heap(begin(range), end(range), comp);
    }

  template <typename R>
    inline Iterator_of<R>
    is_heap_until(R&& range)
    {
      using std::begin;
      using std::end;
      return std::is_heap_until(begin(range), end(range));
    }

  template <typename R, typename C>
    inline Iterator_of<R>
    is_heap_until(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::is_heap_until(begin(range), end(range), comp);
    }



  //////////////////////////////////////////////////////////////////////////////
  // Min and Max

  template <typename R>
    inline Iterator_of<R>
    min_element(R&& range)
    {
      using std::begin;
      using std::end;
      return std::min_element(begin(range), end(range));
    }

  template <typename R, typename C>
    inline Iterator_of<R>
    min_element(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::min_element(begin(range), end(range), comp);
    }

  template <typename R>
    inline Iterator_of<R>
    max_element(R&& range)
    {
      using std::begin;
      using std::end;
      return std::max_element(begin(range), end(range));
    }

  template <typename R, typename C>
    inline Iterator_of<R>
    max_element(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::max_element(begin(range), end(range), comp);
    }

  template <typename R>
    inline std::pair<Iterator_of<R>, Iterator_of<R>>
    minmax_element(R&& range)
    {
      using std::begin;
      using std::end;
      return std::minmax_element(begin(range), end(range));
    }

  template <typename R, typename C>
    inline std::pair<Iterator_of<R>, Iterator_of<R>>
    minmax_element(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::minmax_element(begin(range), end(range), comp);
    }


  // NOTE: These versions of min and max are slightly different than those
  // in the standard library. The standard min/max algorithms require that the
  // result type is const T& (although T& should be fine, too). We relax the
  // strict reference requirement so that the result is simply that of
  // dereferencing the iterator. This makes these algorithms work with 
  // vector<bool>, for example.

  template <typename R>
    inline auto
    min(R&& range) -> decltype(*min_element(range))
    {
      return *min_element(range);
    }

  template <typename R, typename C>
    inline auto
    min(R&& range, C comp) -> decltype(*min_element(range, comp))
    {
      return *min_element(range, comp);
    }

  template <typename R>
    inline auto
    max(R&& range) -> decltype(*max_element(range))
    {
      return *max_element(range);
    }

  template <typename R, typename C>
    inline auto
    max(R&& range, C comp) -> decltype(*max_element(range, comp))
    {
      return *max_element(range);
    }

  template <typename R>
    inline std::pair<Reference_of<R>, Reference_of<R>>
    minmax(R&& range)
    {
      auto p = minmax_element(range);
      return {*p.first, *p.second};
    }

  template <typename R, typename C>
    inline std::pair<Reference_of<R>, Reference_of<R>>
    minmax(R&& range, C comp)
    {
      auto p = minmax_element(range);
      return {*p.first, *p.second};
    }



  //////////////////////////////////////////////////////////////////////////////
  // Lexicographical comparison

  template <typename R1, typename R2>
    inline bool
    lexicographical_compare(const R1& range1, const R2& range2)
    {
      using std::begin;
      using std::end;
      return std::lexicographical_compare(begin(range1), end(range1),
                                          begin(range2), end(range2));
    }

  template <typename R1, typename R2, typename C>
    inline bool
    lexicographical_compare(const R1& range1, const R2& range2, C comp)
    {
      using std::begin;
      using std::end;
      return std::lexicographical_compare(begin(range1), end(range1),
                                          begin(range2), end(range2),
                                          comp);
    }



  //////////////////////////////////////////////////////////////////////////////
  // Permutation Generators

  template <typename R>
    inline bool
    next_permutation(R&& range)
    {
      using std::begin;
      using std::end;
      return std::next_permutation(begin(range), end(range));
    }

  template <typename R, typename C>
    inline bool
    next_permutation(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::next_permutation(begin(range), end(range), comp);
    }

  template <typename R>
    inline bool
    prev_permutation(R&& range)
    {
      using std::begin;
      using std::end;
      return std::prev_permutation(begin(range), end(range));
    }

  template <typename R, typename C>
    inline bool
    prev_permutation(R&& range, C comp)
    {
      using std::begin;
      using std::end;
      return std::prev_permutation(begin(range), end(range), comp);
    }

} // namespace origin

#endif
