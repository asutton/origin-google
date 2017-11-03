// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_FUNCTIONAL_HPP
#define ORIGIN_TYPE_FUNCTIONAL_HPP

#include <functional>

#include "concepts.hpp"

namespace origin
{
  // ------------------------------------------------------------------------ //
  //                                                                [functional]
  //                          Functional Library
  //
  // The functional library provides function objects and related utilitiese to
  // support high-order programming.


  // The negated_predicate function object computes the negation of a given
  // predicate.
  template<typename P>
    struct negated_predicate
    {
      negated_predicate(P p) : pred(p) { }

      template<typename... Args>
        inline bool
        operator()(Args&&... args) const
        {
          return !pred(std::forward<Args>(args)...);
        }

      P pred;
    };

  // Returns a negated predicate.
  template<typename P>
    inline negated_predicate<P> 
    negate(P pred) { return negated_predicate<P>(pred); }

} // namespace origin


#endif
