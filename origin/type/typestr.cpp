// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cstring>

#include "typestr.hpp"

namespace origin
{
  namespace type_impl
  {
    std::string to_string(const std::type_info& info)
    {
#if defined(__GNUC__)
        static constexpr std::size_t max_buffer = 8192;
        char buf[max_buffer];
        std::size_t n = max_buffer;
        abi::__cxa_demangle(info.name(), buf, &n, 0); 
        return std::move(std::string(buf, ::strlen(buf)));
#endif
    }

  } // namespace type_impl
} // namespace origin