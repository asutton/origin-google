// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_CONCEPTS_HPP
#define ORIGIN_DATA_CONCEPTS_HPP

#include <origin/sequence/concepts.hpp>
#include <origin/memory/concepts.hpp>

namespace origin
{
  //////////////////////////////////////////////////////////////////////////////
  // Container                                           data.concepts.container
  //
  // A container is a data structure that owns its elemets.
  //
  // FIXME: A container is not just a range. It has member functions (in
  // particular size, and empty, etc.)
  template <typename C>
    constexpr bool Container()
    {
      return Range<C>();
    }

} // namespace origin

#endif
