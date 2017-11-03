// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_EMPTY_HPP
#define ORIGIN_TYPE_EMPTY_HPP

#include <iosfwd>

namespace origin
{

  //////////////////////////////////////////////////////////////////////////////
  // Empty type
  //
  // The empty type is an empty, trivial type that is meant to be used as a
  // placeholder for unspecified types in containers or other data structures.
  struct empty_t { };


  // Streamable<empty_t>
  // Reading or writing a streamable type is a no-op. Origin designates no
  // textual representation for objects of this type.
  template <typename Char, typename Traits>
    inline std::basic_ostream<Char, Traits>& 
    operator<<(std::basic_ostream<Char, Traits>& os, empty_t)
    { 
      return os; 
    }

  template <typename Char, typename Traits>
    inline std::basic_istream<Char, Traits>& 
    operator>>(std::basic_istream<Char, Traits>& is, empty_t&)
    { 
      return is; 
    }

} // namespace origin

#endif
