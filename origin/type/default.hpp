// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_DEFAULT_HPP
#define ORIGIN_TYPE_DEFAULT_HPP

namespace origin
{
  //////////////////////////////////////////////////////////////////////////////
  // Default Type
  //
  // The default type is a tag class used to indicate the selection of a default
  // value. This is only used to support class template specialization.
  struct default_t { };
  
} // namespace origin

#endif
