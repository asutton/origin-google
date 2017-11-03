// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_TESTING_HPP
#  error This file cannot be included directly. Include type/testing.hpp.
#endif

namespace testing
{

  //////////////////////////////////////////////////////////////////////////////
  // Function Properties
  //
  // The following classes describe testable properties of relations and
  // operation. There are two kinds of testable properties:
  //
  //    - a logical formula quantified over the domain of a function, and
  //    - a set of properties.
  //
  // Every logical formula (axiom, property, proposition, etc.) is encapsulated
  // within a class. The class implements a test oracle that decides whether the
  // property holds for specific inputs.
  //
  // Every set of properties is also encapsulated within a class. Unlike the
  // properties above, these must be tested against sets of values. This is
  // done by testing the property over a quantified set of values (usually
  // randomly generated).




  //////////////////////////////////////////////////////////////////////////////
  // Check Equal
  //
  // Returns true if two values, a and b, compare equal.
  //
  // Unless specifically overloaded, this function compares for equuality using
  // the == operator, which implicitly requires that T is equality comparable.
  // Overloads of this function must implement an equivalence relation.
  //
  // Parameters:
  //    a, b -- The arguments being compared for equality.
  //
  // Returns:
  //    True when a and b compare equal.
  template <typename T>
    inline bool
    check_equal(const T& a, const T& b)
    {
      return a == b;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Check Less
  //
  // Returns true if a value, a, is strictly ordered before the value, b.
  //
  // Unless specifically overloaded, this function compares for order using the
  // < operator, which implicitly requires that T is weakly ordered. Overloads
  // of this function must implement a strict weak order.
  //
  // Parameters:
  //    a, b -- The arguments being compared for order.
  //
  // Returns:
  //    True when a is strictly ordered before b.
  template <typename T>
    inline bool
    check_less(const T& a, const T& b)
    {
      return a == b;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Reflexive Property
  //
  // A relation is reflexive if r(a, a) is true for all a of the domain of r.
  struct reflexive
  {
    template <typename R, typename T>
      bool operator()(R r, const T& a) const 
      {
        return r(a, a); 
      }
  };



  //////////////////////////////////////////////////////////////////////////////
  // Irreflexive Property
  //
  // A relation is irreflexive if r(a, a) is false for all a of the domain of r. 
  // Equivalently, !r(a, a) is true for all a in the domain of r.
  struct irreflexive
  {
    template <typename R, typename T>
      bool operator()(R r, const T& a) const 
      { 
        return !r(a, a); 
      }
  };
 


  //////////////////////////////////////////////////////////////////////////////
  // Symmetric Property
  //
  // A relation is symmetric if r(a, b) => r(b, a) for all a and b in the
  // domain of r.
  struct symmetric
  {
    template <typename R, typename T>
      bool operator()(R r, const T& a, const T& b) const
      { 
        return r(a, b) ? r(b, a) : true; 
      }
  };


  //////////////////////////////////////////////////////////////////////////////
  // Asymmetric Property
  //
  // A relation is asymmetric if r(a, b) => !r(b, a) for all a and b in the
  // domain of r.
  struct asymmetric
  {
    template <typename R, typename T>
      bool operator()(R r, const T& a, const T& b) const
      {
        return r(a, b) ? !r(b, a) : true;
      }
  };


  
  //////////////////////////////////////////////////////////////////////////////
  // Antisymmetric Property
  //
  // A relation is antisymmetric if r(a, b) && r(b, a) => a == b for all a and b
  // in the domain of r.
  struct antisymmetric
  {
    template <typename R, typename T>
      bool operator()(R r, const T& a, const T& b) const
      {
        return r(a, b) && r(b, a) ? a == b : true;
      }
  };



  //////////////////////////////////////////////////////////////////////////////
  // Transitive Property
  //
  // A relation is transitive if r(a, b) && r(b, c) => r(a, c) for all a, b,
  // and c in the domain of r.
  struct transitive
  {
    template <typename R, typename T>
      bool operator()(R r, const T& a, const T& b, const T& c) const
      {
        return r(a, b) && r(b, c) ? r(a, c) : true;
      }
  };



  //////////////////////////////////////////////////////////////////////////////
  // Trichotomy Law
  //
  // A relation, r, obeys the trichotomy law if exactly one of the following
  // holds:
  //
  //    r(a, b)
  //    !r(a, b)
  //    a == b
  //
  // for all a and b in the domain of r.
  struct trichotomous
  {
    template <typename R, typename T>
      bool operator()(R r, const T& a, const T& b) const
      {
        if (a == b)         // Equality is the same as equivalence
          return !r(a, b) && !r(b, a);
        else if (r(a, b))   // Asymmetric property
          return !r(b, a);
        else                // Exactly one must hold.
          return r(b, a);
      }
  };

    

  //////////////////////////////////////////////////////////////////////////////
  // Equivalence Relation
  //
  // An equivalence relation is a relation that is reflexive, symmetric, and 
  // transitive.
  struct equivalence
  {
    template <typename R, typename T>
      void operator()(R&& rvar, T&& tvar) const
      {
        quick_check(refl, rvar, tvar);
        quick_check(sym, rvar, tvar, tvar);
        quick_check(trans, rvar, tvar, tvar, tvar);
      }

    reflexive refl;
    symmetric sym;
    transitive trans;
  };


  //////////////////////////////////////////////////////////////////////////////
  // Strict Ordering Relation
  //
  // An ordering relation is irreflexive, asymmetric, and transitive. Without
  // further qualification a strict ordering is assumed to be a partial
  // ordering.
  struct strict_ordering
  {
    template <typename R, typename T>
      void operator()(R&& rvar, T&& tvar) const
      {
        quick_check(irrefl, rvar, tvar);
        quick_check(asym, rvar, tvar, tvar);
        quick_check(trans, rvar, tvar, tvar, tvar);
      }

    irreflexive irrefl;
    asymmetric asym;
    transitive trans;
  };


  //////////////////////////////////////////////////////////////////////////////
  // Strict Weak Ordering Relation
  //
  // A strict weak ordering is a strict ordering relation, r, where the
  // symmetric complement, !r(a, b) && !r(b, a), is an equivalence.
  struct strict_weak_ordering
  {
    template <typename R, typename T>
      void operator()(R&& rvar, T&& tvar) const
      {
        // Derive the symmetric complement of r and quantify over it. I
        // wouldn't need to do this if I had a symmetric_complement function.
        using U = Result_of<T()>;
        auto r = rvar();
        auto sc = [r](const U& a, const U& b) { return !r(a, b) && !r(b, a); }; 
        auto svar = single(sc);
        
        quick_check(ord, rvar, tvar);
        quick_check(eq, svar, tvar);
      }

    strict_ordering ord;
    equivalence eq;
  };


  //////////////////////////////////////////////////////////////////////////////
  // Strict Total Ordering Relation
  //
  // A strict total ordering is a special case of a strict weak ordering where
  // the symmetric complement is the same as equality. Another way of saying
  // this is that the relation is transitive and trichotomous.
  struct strict_total_ordering
  {
    template <typename R, typename T>
      void operator()(R&& rvar, T&& tvar) const
      {
        quick_check(trans, rvar, tvar, tvar, tvar);
        quick_check(tri, rvar, tvar, tvar);
      }

    transitive trans;
    trichotomous tri;
  };



  //////////////////////////////////////////////////////////////////////////////
  // Testing functions


  // Check that r, having domain T, is reflexive.
  template <typename R>
    void 
    check_reflexive(R r)
    {
      using D = Domain<R>;
      auto rvar = single(r);
      auto tvar = quantify_over<D>();
      quick_check(reflexive {}, rvar, tvar);
    }

  // Check that r, having domain T, is irreflexive.
  template <typename R>
    void 
    check_irreflexive(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(irreflexive {}, rvar, tvar);
    }

  // Check that r, having domain T, is symmetric.
  template <typename R>
    void 
    check_symmetric(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(symmetric {}, rvar, tvar, tvar);
    }

  // Check that r, having domain T, is asymmetric.
  template <typename R>
    void 
    check_asymmetric(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(asymmetric {}, rvar, tvar, tvar);
    }

  // Check that r, having domain T, is antisymmetric.
  template <typename R>
    void 
    check_antisymmetric(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(antisymmetric {}, rvar, tvar, tvar);
    }

  // Check that r, having domain T, is transitive.
  template <typename R>
    void 
    check_transitive(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(transitive {}, rvar, tvar, tvar, tvar);
    }

  // Check that r, having domain T, is trichotomous.
  template <typename R>
    void 
    check_trichotomous(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(trichotomous {}, rvar, tvar);
    }

  // Check that r, having domain T, is an equivalence relation.
  template <typename R>
    void 
    check_equivalence(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(equivalence {}, rvar, tvar);
    }

  // Check that r, having domain T, is a strict ordering relation.
  template <typename R>
    void 
    check_strict_ordering(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(strict_ordering {}, rvar, tvar);
    }

  // Check that r, having domain T, is a strict weak ordering relation.
  template <typename R>
    void 
    check_strict_weak_ordering(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(strict_weak_ordering {}, rvar, tvar);
    }

  // Check that r, having domain T, is a strict total ordering relation.
  template <typename R>
    void 
    check_strict_total_ordering(R r)
    {
      auto rvar = single(r);
      auto tvar = quantify_over<Domain<R>>();
      quick_check(strict_total_ordering {}, rvar, tvar);
    }


} // namespace testing
