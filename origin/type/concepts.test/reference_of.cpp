// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <string>
#include <vector>
#include <memory>

#include <origin/type/concepts.hpp>

using namespace std;
using namespace origin;

int main()
{
  // Test pointers (iterators in general)
  static_assert(Same<Reference_of<int*>, int&>(), "");
  static_assert(Same<Reference_of<const int*>, const int&>(), "");
  
  // Test smart pointers.
  using P = unique_ptr<int>;
  using Cp = unique_ptr<const int>;

  static_assert(Same<  Reference_of<P>,        int&  >(), "");
  static_assert(Same<  Reference_of<P&>,       int&  >(), "");
  static_assert(Same<  Reference_of<const P&>, int&  >(), "");

  static_assert(Same<  Reference_of<Cp>,        const int&  >(), "");
  static_assert(Same<  Reference_of<Cp&>,       const int&  >(), "");
  static_assert(Same<  Reference_of<const Cp&>, const int&  >(), "");

  // Test containers
  using V = vector<int>;
  static_assert(Same<  Reference_of<V>,        int&        >(), "");
  static_assert(Same<  Reference_of<V&>,       int&        >(), "");
  static_assert(Same<  Reference_of<const V>,  const int&  >(), "");
  static_assert(Same<  Reference_of<const V&>, const int&  >(), "");

  // Vector bool has a non-reference reference. Make sure our traits agree with
  // the class.
  using Vb = vector<bool>;
  static_assert(Same< Reference_of<Vb>,       Vb::reference        >(), "");
  static_assert(Same< Reference_of<const Vb>, Vb::const_reference  >(), "");
}

