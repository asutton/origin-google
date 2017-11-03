// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;

struct s
{
  s(s&&) = delete;
  s(const s&) = delete;
};

struct t
{
  bool operator==(const t& a) const { return true; }
  int operator==(t& a) const { return true; }
};


int main()
{
  // Basic tests
  static_assert(Has_equal<int>(), "");
  static_assert(Convertible<Equal_result<int>, bool>(), "");

  static_assert(Has_not_equal<int>(), "");
  static_assert(Convertible<Not_equal_result<int>, bool>(), "");

  static_assert(Has_less<int>(), "");
  static_assert(Convertible<Less_result<int>, bool>(), "");

  static_assert(Has_greater<int>(), "");
  static_assert(Convertible<Greater_result<int>, bool>(), "");

  static_assert(Has_less_equal<int>(), "");
  static_assert(Convertible<Less_equal_result<int>, bool>(), "");

  static_assert(Has_greater_equal<int>(), "");
  static_assert(Convertible<Greater_equal_result<int>, bool>(), "");

  // Make sure that we compile for types without appropriate constructor,
  static_assert(!Has_equal<s>(), "");

  // The Has_* operators must yield the same result for qualified or reference
  // types as the value type.
  static_assert(Has_equal<t, t>() == Has_equal<t, const t>(), "");

  // The actual results of the expressions vary with the types of arguments.
  static_assert(Same<Equal_result<t, t>, int>(), "");
  static_assert(Same<Equal_result<t, const t>, bool>(), "");

  // TODO: Am I missing any other obvious tests?
}
