// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <sstream>
#include <string>

#include <origin/type/empty.hpp>

using namespace std;
using namespace origin;


void 
check_write()
{
  // Writing an empty object to a stream does not modify the stream. 
  string s;
  std::ostringstream ss(s);
  ss << empty_t{};
  assert(s.empty());
}

void 
check_read()
{
  // Reading an empty object from a stream does not modify the stream. In
  // fact, reading an empty object is valid as long as the stream is in a
  // valid state!
  string s = "1 2 3";
  std::istringstream ss(s);
  int a, b, c;
  empty_t e;
  ss >> a >> e >> b >> e >> c >> e;
  assert(a == 1);
  assert(b == 2);
  assert(c == 3);
}

int main()
{
  check_write();
  check_read();
}
