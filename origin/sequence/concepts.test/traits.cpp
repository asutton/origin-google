// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/sequence/concepts.hpp>

using namespace std;
using namespace origin;

// Test the aliases begin/end to ensure that they follow the basic patterns
// for looking those operations up.


// A clas that defines begin/end the way it should.
struct member
{
  int* begin() { return nullptr; }
  int* end()   { return nullptr; }

  const int* begin() const { return nullptr; }
  const int* end()   const { return nullptr; }
};


namespace N
{
  // A class that defines begin/end using the ADL style operation.
  struct adl { };

  char* begin(adl&) { return nullptr; }
  char* end(adl&)   { return nullptr; }

  const char* begin(const adl&) { return nullptr; }
  const char* end(const adl&)   { return nullptr; }
}


// Check that the Begin_result and End_result traits actually do what
// they are supposed to do. They must be the same as if using decltype.
template <typename R>
  void check_be(R& r)
  {
    using std::begin;
    using std::end;
    static_assert(Same<decltype(begin(r)), Begin_result<R>>(), "");
    static_assert(Same<decltype(end(r)), End_result<R>>(), "");
  }


int main()
{
  member m;
  const member& cm = m;
  
  check_be(m);
  check_be(cm);

  N::adl a;
  const N::adl& ca = a;
  check_be(a);
  check_be(ca);
}
