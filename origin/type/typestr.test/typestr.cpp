// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

// Compre the result of typestr<T> to the text with which it is written,
// returning true when the two are the same. For example:
//
//    CHECK_TYPESTR(const int) // returns true
//
// Note that the result of the operation is sensitive to the spelling of T. In
// the example above, passing the argument as "int const" would cause the
// evaluation to base.
#define CHECK_TYPESTR(T) assert(typestr<T>() == #T)


// Check that the array reference matches the string given by x.
template <typename T>
  void check_array_ref(T&& x, const string& str)
  {
    assert(typestr<T>() == str);
  }

int main()
{
  CHECK_TYPESTR(int);

  // Cv-qualifiers.
  CHECK_TYPESTR(const int);
  CHECK_TYPESTR(volatile int);
  CHECK_TYPESTR(const volatile int);

  // Reference and pointer types.
  CHECK_TYPESTR(int&);
  CHECK_TYPESTR(int*);
  CHECK_TYPESTR(int**);
  CHECK_TYPESTR(int***);

  // The C Preprocessor can't expand this correctly.
  assert(typestr<int&&>() == "int&&");

  // Complex cv-qualified pointer types.
  CHECK_TYPESTR(const int*);
  CHECK_TYPESTR(const int* const);
  CHECK_TYPESTR(const int* const* const);
  CHECK_TYPESTR(const int*);
  CHECK_TYPESTR(const int* volatile);
  CHECK_TYPESTR(const int* const volatile);

  // Function and function pointer types.
  CHECK_TYPESTR(int(int, int));
  CHECK_TYPESTR(int(int&, int));
  CHECK_TYPESTR(int(*)(int, bool));

  // Check array types
  CHECK_TYPESTR(int[3]);

  int a[3];
  check_array_ref(a, "int(&)[3]");

  // TODO: Check member pointer types, too. I'm pretty sure I'm going to have
  // to write specializations on those.


  // Variadic arguments won't expand correctly for the CHECK_TYPESTR macro.
  assert((typestr<int, char, short>() == "int, char, short"));
  assert((typestr<const int&, char*>() == "const int&, char*"));


  // TODO: Test the use of typest with object arguments.
}
