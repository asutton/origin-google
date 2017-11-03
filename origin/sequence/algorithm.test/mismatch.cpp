// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/sequence/algorithm.hpp>

using namespace std;
using namespace origin;


int main()
{
  using V = vector<int>;
  
  V v1 {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto f1 = v1.begin();
  auto l1 = v1.end();

  V v2 {0, 1, 2, 3, 5, 6, 7, 8, 9};
  auto f2 = v2.begin();
  // auto l2 = v2.end();
  
  const V& cv1 = v1;
  auto cf1 = cv1.begin();
  auto cl1 = cv1.end();
  
  const V& cv2 = v2;
  auto cf2 = cv2.begin();
  // auto cl2 = cv2.end();

  using I = V::iterator;
  using C = V::const_iterator;
  using Pii = pair<I, I>;
  using Pic = pair<I, C>;
  using Pci = pair<C, I>;
  using Pcc = pair<C, C>;


  // Mismatch using ==
  static_assert(Same<decltype(range_mismatch(v1, v2)), Pii>(), "");
  assert(range_mismatch(v1, v2) == mismatch(f1, l1, f2));

  static_assert(Same<decltype(range_mismatch(cv1, v2)), Pci>(), "");
  assert(range_mismatch(cv1, v2) == mismatch(cf1, cl1, f2));

  static_assert(Same<decltype(range_mismatch(v1, cv2)), Pic>(), "");
  assert(range_mismatch(v1, cv2) == mismatch(f1, l1, cf2));

  static_assert(Same<decltype(range_mismatch(cv1, cv2)), Pcc>(), "");
  assert(range_mismatch(cv1, cv2) == mismatch(cf1, cl1, cf2));


  // Mismatch using == 
  std::equal_to<int> eq;

  static_assert(Same<decltype(range_mismatch(v1, v2, eq)), Pii>(), "");
  assert(range_mismatch(v1, v2, eq) == mismatch(f1, l1, f2, eq));

  static_assert(Same<decltype(range_mismatch(cv1, v2, eq)), Pci>(), "");
  assert(range_mismatch(cv1, v2, eq) == mismatch(cf1, cl1, f2, eq));

  static_assert(Same<decltype(range_mismatch(v1, cv2, eq)), Pic>(), "");
  assert(range_mismatch(v1, cv2, eq) == mismatch(f1, l1, cf2, eq));

  static_assert(Same<decltype(range_mismatch(cv1, cv2)), Pcc>(), "");
  assert(range_mismatch(cv1, cv2, eq) == mismatch(cf1, cl1, cf2, eq));

}
