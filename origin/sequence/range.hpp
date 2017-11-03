// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_SEQUENCE_RANGE_HPP
#define ORIGIN_SEQUENCE_RANGE_HPP

#include <origin/type/concepts.hpp>

#include "concepts.hpp"
#include "iterator.hpp"

namespace origin
{
  // Reference deduction for ranges. Note that the reference type for standard
  // Containers will not be deduced using this mechanism. They define nested
  // types.
  template <typename R>
    auto deduce_reference(default_t, R&& range)
      -> Requires<Range<R>(), Reference_of<Iterator_of<R>>>;


  //////////////////////////////////////////////////////////////////////////////
  // Bounded range
  //
  // The bounded range class encapsulates an iterator range bounded by a pair of
  // iterators. Iterating over a bounded range is equivalent to iterating over
  // the same pair of iterators. That is:
  //
  //    while (first != last) { 
  //      const auto& x = *first;
  //      // do something
  //      ++first; 
  //    }
  //
  // is euqivalent to:
  //
  //    for (const auto& x : bounded_range<I>{first, last})
  //      // do something.
  //
  // This is essentially the same as the boost::iterator_range, or 
  // std::pair<I, I> with appropriate overloads for begin and end.
  //
  // Template parameters:
  //    I -- An Iterator type.
  //
  // Invariants: 
  //    is_bounded_range(this->begin(), this->end());
  template <typename I>
    class bounded_range
    {
      static_assert(Weakly_incrementable<I>(), "");
      static_assert(Equality_comparable<I>(), "");
    public:
      using iterator = I;

      // Initialize the bounded range so that both values are the same. The
      // range is initially empty.
      bounded_range() 
        : first(), last(first)
      { }
    
      // Initialize the bounded range over [first, last).
      bounded_range(I f, I l)
        : first(f), last(l)
      { 
        assert(is_bounded_range(first, last));
      }
      
      // Iterators
      iterator begin() const { return first; }
      iterator end() const   { return last; }
      
    private:
      I first;
      I last;
    };



  //////////////////////////////////////////////////////////////////////////////
  // Range Size
  //
  // Returns the size of the range. The size of a range is equivalent to its
  // distance, but the result type is guaranteed to be unsigned.
  //
  // If the range has a member function named size(), then that operation is
  // selected over the default comptuation.

  // Specialization for ranges with x.size(). Note that this is declared as
  // constexpr because some container-like types have constexpr size member
  // functions.
  template <typename T>
    inline constexpr 
    auto size(const T& x) -> decltype(x.size()) 
    { 
      return x.size(); 
    }

  // For C-arrays.
  template <typename T, std::size_t N>
    inline constexpr 
    std::size_t size(T(&)[N])
    {
      return N;
    }
    
  // For ranges without r.size().
  template <typename R>
    inline auto 
    size(const R& range) 
      -> Requires<Range<R>() && !Has_member_size<R>(),
                  Make_unsigned<Difference_type<R>>
      >
    {
      using std::begin;
      using std::end;
      return distance(begin(range), end(range));
    }
  


} // namespace origin

#endif
