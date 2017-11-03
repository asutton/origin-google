// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <algorithm>
#include <forward_list>
#include <list>
#include <vector>

#include <origin/sequence/testing.hpp>

using namespace std;
using namespace origin;
using namespace origin::testing;


// Random number generator.
context cxt;

template <typename T>
  void check()
  {
    using std::begin;
    using std::end;

    // Initialize the container with a base sequence.
    T seq { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // Construct a random generator over the range.
    auto gen = quantify_over_range(seq);

    // Check that element in the generate range is in the originating vector.
    for(int i = 0; i < 10; ++i) {
      auto r = gen();
      auto first = begin(r);
      auto last = end(r);

      // From the first iterator i where *i == *first, the sequences
      // [first, last) and [i, i + last - first) compare equal.
      if (first != last) {
        auto i = find(begin(seq), end(seq), *first);
        assert(equal(first, last, i));
      }
    }
  }


int main()
{
  check<forward_list<int>>();
  check<list<int>>();
  check<vector<int>>();
}
