// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <string>

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;

struct s 
{
  explicit operator bool() const { return true; }
};

struct t
{
  operator bool() const { return true; }
};

struct base { };
struct derived : base { };

int main()
{
  // Convertibility is reflexive.
  static_assert(Convertible<int, int>(), "");

  // Promotion is conversion.
  static_assert(Convertible<int, long>(), "");
  static_assert(Convertible<int, float>(), "");

  // A reference can be "converted" to a non-reference via copy initialization.
  static_assert(Convertible<int&, int>(), "");

  // But an rvalue cannot be converted to an lvalue reference.
  static_assert(!Convertible<int, int&>(), "");

  // const T can be converted to T and vice versa because the of copy
  // initialization.
  static_assert(Convertible<const int, int>(), "");
  static_assert(Convertible<int, const int>(), "");

  // A T* converts to const T*, but not the other way around.
  static_assert(Convertible<int*, const int*>(), "");
  static_assert(!Convertible<const int*, int*>(), "");

  // C-strings convert to strings.
  static_assert(Convertible<const char*, string>(), "");
  static_assert(!Convertible<string, const char*>(), "");

  // Pointers convert to bool
  static_assert(Convertible<int*, bool>(), "");

  // Explicitly cast to bool does not imply convertibility, but implicit cast
  // to bool does.
  static_assert(!Convertible<s, bool>(), "");
  static_assert(Convertible<t, bool>(), "");

  // A derived type converts to a base type. So do pointers and references.
  static_assert(Convertible<derived, base>(), "");
  static_assert(Convertible<derived*, base*>(), "");
  static_assert(Convertible<derived&, base&>(), "");

  // Down-casting is not a convertibility!
  static_assert(!Convertible<base, derived>(), "");

  // TODO: Am I missing anyhting?
}
