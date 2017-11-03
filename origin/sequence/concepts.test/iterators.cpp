// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <forward_list>
#include <iterator>
#include <iostream>
#include <list>
#include <vector>
#include <memory>

#include <origin/type/typestr.hpp>
#include <origin/sequence/concepts.hpp>
#include <origin/sequence/testing.hpp>

using namespace std;
using namespace origin;
using namespace origin::testing;

int main()
{
  context cxt;

  // An input iterator is input, but not output and not forward.
  using In = istream_iterator<int>;
  static_assert(Input_iterator<In>(), "");
  static_assert(!Output_iterator<In, int>(), "");
  static_assert(!Forward_iterator<In>(), "");

  // An output iterator is output but not readable.
  using Out = ostream_iterator<int>;
  static_assert(Output_iterator<Out, int>(), "");
  static_assert(!Readable<Out>(), "");


  // A forward iterator is input and forward but not bidirectional.
  using F = forward_list<int>;
  using Fi = F::iterator;
  static_assert(Input_iterator<Fi>(), "");
  static_assert(Forward_iterator<Fi>(), "");
  static_assert(!Bidirectional_iterator<Fi>(), "");

  F f {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  check_readable_range(f);
  check_writable_range<int>(f);
  check_writable_range<const int&>(f);
  check_writable_range<int&&>(f);
  check_permutable_range(f);
  check_mutable_range(f);
  check_weakly_incrementable_range(f);
  check_incrementable_range(f);
  check_input_range(f);
  check_output_range<int>(f);
  check_forward_range(f);

  // A bidirectional iterator is input, forward, and bidi but not random.
  using L = list<int>;
  using Li = L::iterator;
  static_assert(Input_iterator<Li>(), "");
  static_assert(Forward_iterator<Li>(), "");
  static_assert(Bidirectional_iterator<Li>(), "");
  static_assert(!Random_access_iterator<Li>(), "");

  L l {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  check_readable_range(l);

  // A random access iterator is input, forward, bidi, and random.
  using V = vector<int>;
  using Vi = V::iterator;
  static_assert(Input_iterator<Vi>(), "");
  static_assert(Forward_iterator<Vi>(), "");
  static_assert(Bidirectional_iterator<Vi>(), "");
  static_assert(Random_access_iterator<Vi>(), "");

  V v {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  check_readable_range(v);

  return cxt.failures();
}
