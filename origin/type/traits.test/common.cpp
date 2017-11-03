// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>

#include <origin/type/traits.hpp>
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

struct s {
  s(s&&) = delete;
  s(const s&) = delete;
};

struct base { };
struct derived : base { };

int main()
{
  // Common type is reflexive.
  static_assert(Common<int, int>(), "");
  static_assert(Same<Common_type<int, int>, int>(), "");


  // Common type compares value types, it does not honor const-qualifiers or
  // references. These tests are kind of strange since they don't appear to
  // confom to the rules for ?:. However, it is important that we know that
  // common types are not distorted by reference types or object qualifiers.

  static_assert(Common<const int, const int>(), "");
  static_assert(Same<Common_type<const int, const int>, int>(), "");

  // Common type of T and const T is T because it only addresses value types.
  static_assert(Common<int, const int>(), "");
  static_assert(Same<Common_type<int, const int>, int>(), "");

  // Common type does not respect reference types or const-qualified reference
  // types.
  static_assert(Common<int&, int&>(), "");
  static_assert(Same<Common_type<int&, int&>, int>(), "");
  static_assert(Same<Common_type<const int&, const int&>, int>(), "");

  // Make sure that we don't fail deduction when various operators are
  // deleted.
  static_assert(Same<Common_type<s, s>, s>(), "");
  static_assert(Same<Common_type<s, const s&>, s>(), "");
  static_assert(!Common<s, int>(), "");


  // Common type follows C++ promotion rules for built-in arithmetic types.
  static_assert(Common<int, long>(), "");
  static_assert(Same<Common_type<int, long>, long>(), "");

  // Int to float is also a promotion.
  static_assert(Common<int, float>(), "");
  static_assert(Same<Common_type<int, float>, float>(), "");

  // The common type of a base class and derived class is the base.
  static_assert(Common<derived, base>(), "");
  static_assert(Same<Common_type<derived, base>, base>(), "");

  // TODO: More common type tests for inheritance.
  static_assert(Common<string, char*>(), "");
  static_assert(Same<Common_type<string, char*>, string>(), "");
  
  static_assert(Common<string, const char*>(), "");
  static_assert(Same<Common_type<string, const char*>, string>(), "");

  // Signed and unsigned types have the common type unsigned (I think that this
  // is a problem in the language). Note that for integer types with size less
  // than int, the compiler will inherently promote to int.
  static_assert(Common<signed int, unsigned int>(), "");
  static_assert(Same<Common_type<signed char, unsigned char>, int>(), "");
  static_assert(Same<Common_type<signed short, unsigned short>, int>(), "");
  static_assert(Same<Common_type<signed int, unsigned int>, unsigned int>(), "");
  static_assert(Same<Common_type<signed long, unsigned long>, unsigned long>(), "");


  // Tests on variadic common type
  // The common type of a single type is that type.
  static_assert(Same<Common_type<int>, int>(), "");

  // The common type of 3 type is the type of the "most common". This is also
  // independent of order.
  static_assert(Same<Common_type<char, short, int>, int>(), "");
  static_assert(Same<Common_type<short, char, int>, int>(), "");
  static_assert(Same<Common_type<char, int, short>, int>(), "");
  static_assert(Same<Common_type<short, int, char>, int>(), "");
  static_assert(Same<Common_type<int, char, short>, int>(), "");
  static_assert(Same<Common_type<int, short, char>, int>(), "");


  // Test with the expander
  // Test the expander.
  using List = type_list<char, short, int>;
  static_assert(Same<Common_type<Expand<List>>, int>(), "");

  using Tuple = std::tuple<int, int, int>;
  static_assert(Same<Common_type<Expand<Tuple>>, int>(), "");
}
