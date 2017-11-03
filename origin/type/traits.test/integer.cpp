// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;

int main()
{
  static_assert(Integer<char>(), "");
  static_assert(Integer<signed char>(), "");
  static_assert(Integer<unsigned char>(), "");

  static_assert(Same<Make_signed<char>, signed char>(), "");
  static_assert(Same<Make_signed<signed char>, signed char>(), "");
  static_assert(Same<Make_signed<unsigned char>, signed char>(), "");

  static_assert(Same<Make_unsigned<char>, unsigned char>(), "");
  static_assert(Same<Make_unsigned<signed char>, unsigned char>(), "");
  static_assert(Same<Make_unsigned<unsigned char>, unsigned char>(), "");


  static_assert(Integer<signed short>(), "");
  static_assert(Integer<unsigned short>(), "");
  
  static_assert(Same<Make_signed<signed short>, signed short>(), "");
  static_assert(Same<Make_signed<unsigned short>, signed short>(), "");
  
  static_assert(Same<Make_unsigned<signed short>, unsigned short>(), "");
  static_assert(Same<Make_unsigned<unsigned short>, unsigned short>(), "");


  static_assert(Integer<signed int>(), "");
  static_assert(Integer<unsigned int>(), "");

  static_assert(Same<Make_signed<signed int>, signed int>(), "");
  static_assert(Same<Make_signed<unsigned int>, signed int>(), "");
  
  static_assert(Same<Make_unsigned<signed int>, unsigned int>(), "");
  static_assert(Same<Make_unsigned<unsigned int>, unsigned int>(), "");


  static_assert(Integer<signed long>(), "");
  static_assert(Integer<unsigned long>(), "");

  static_assert(Same<Make_signed<signed long>, signed long>(), "");
  static_assert(Same<Make_signed<unsigned long>, signed long>(), "");
  
  static_assert(Same<Make_unsigned<signed long>, unsigned long>(), "");
  static_assert(Same<Make_unsigned<unsigned long>, unsigned long>(), "");


  static_assert(Integer<signed long long>(), "");
  static_assert(Integer<unsigned long long>(), "");

  static_assert(Same<Make_signed<signed long long>, signed long long>(), "");
  static_assert(Same<Make_signed<unsigned long long>, signed long long>(), "");
  
  static_assert(Same<Make_unsigned<signed long long>, unsigned long long>(), "");
  static_assert(Same<Make_unsigned<unsigned long long>, unsigned long long>(), "");
}
