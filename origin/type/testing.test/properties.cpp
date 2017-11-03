// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>

#include <origin/type/testing.hpp>

using namespace std;
using namespace origin;
using namespace origin::testing;

int main()
{
  context cxt;

  // Check relation properties
  std::equal_to<int> eq;
  check_reflexive(eq);
  check_symmetric(eq);
  check_transitive(eq);
  check_equivalence(eq);

  std::less<int> lt;
  check_irreflexive(lt);
  check_asymmetric(lt);
  check_antisymmetric(lt);
  check_strict_ordering(lt);
  check_strict_weak_ordering(lt);
  check_strict_total_ordering(lt);

  return cxt.failures();
}
