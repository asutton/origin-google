// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_OPTIONAL_TESTING_HPP
#define ORIGIN_DATA_OPTIONAL_TESTING_HPP

#include <origin/data/testing.hpp>

#include "optional.hpp"

namespace origin
{
  namespace testing 
  {
    template <typename T, typename Value = Default_pattern_type<T>>
      struct optional_value_pattern
      {
        using Init = std::bernoulli_distribution ;

        optional_value_pattern(double p = 0.9, const Value& val = {})
          : init(p), value(val)
        { }

        template <typename Eng>
          optional<T> operator()(Eng&& eng)
          {
            if (init(eng))
              return optional<T> {value(eng)};
            else
              return {};
          }

        Init init;   // The distribution of initialized/uninitialized values
        Value value; // The pattern describing randomly generated values
      };


    // Create the default pattern for generating optional values. Values have
    // a 1% chance of being created uninitialized.
    template <typename T>
      struct default_optional_pattern
      {
        optional_value_pattern<T> operator()() const
        {
          auto x = optional_value_pattern<T>(0.9, default_pattern<T>());
          return x;
        }
      };
    
    template <typename T>
      struct default_pattern_traits<optional<T>>
      {
        using type = default_optional_pattern<T>;
      };
  }
}

#endif

