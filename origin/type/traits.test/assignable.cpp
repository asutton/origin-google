// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/type/traits.hpp>

using namespace std;
using namespace origin;

struct foo { };

struct move_only
{
  move_only(move_only&& x) { }
  move_only& operator=(move_only&& x) { return *this; }

  move_only(const move_only& x) = delete;
  move_only& operator=(const move_only& x) = delete;
};

struct copyable
{
  // Copy semantics
  copyable(const copyable& x) { }
  copyable& operator=(const copyable& x) { return *this; }

  // Conversion for foo.
  copyable(const foo& x) { }
  copyable& operator=(const foo& x) { return *this; }
};

int main()
{
  static_assert(Assignable<move_only, move_only&&>(), "");
  static_assert(!Assignable<move_only, const move_only&>(), "");

  static_assert(Assignable<copyable, copyable&&>(), "");
  static_assert(Assignable<copyable, const copyable&>(), "");

  static_assert(Assignable<copyable, foo&&>(), "");
  static_assert(Assignable<copyable, const foo&>(), "");
}
