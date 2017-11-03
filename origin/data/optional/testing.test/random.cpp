// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/data/optional/testing.hpp>
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;
using namespace origin::testing;

int main()
{
  context cxt;
  cxt.random_engine().seed(time(0));

  // FIXME: Is there any way to really test this?
  using T = optional<int>;
  auto gen = quantify_over<T>();
  for (int i = 0; i < 10; ++i) {
    cout << gen() << '\n';
  }

  return 0;
}
