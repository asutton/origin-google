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

  // Cross-type Equality
  //
  // The equality of two values of different types sharing a common type, C, is
  // equivalent to the same operation after converting both arguments to the
  // common type. The requirement applies symmetrically. Both must hold:
  //
  //    t == u <=> C(t) == C(u)
  //    u == t <=> C(u) == C(t)
  struct crosstype_equality
  {
    template <typename T, typename U>
      bool operator()(const T& t, const U& u) const
      {
        using C = Common_type<T, U>;
        return (t == u) == (C(t) == C(u))
            && (u == t) == (C(u) == C(t));
      }
  };


  // Cross-type Distintion 
  //
  // The distinction of two values of different types sharing a common type, C,
  // is equivalent to the same operation after converting both arguments to the
  // common type. The requirement applies symmetrically. Both must hold:
  //
  //    t != u <=> C(t) != C(u)
  //    u != t <=> C(u) != C(t)
  //
  struct crosstype_distinction
  {
    template <typename T, typename U>
      bool operator()(const T& t, const U& u) const
      {
        using C = Common_type<T, U>;
        return (t != u) == (C(t) != C(u))
            && (u != t) == (C(u) != C(t));
      }
  };


  // Cross-type Less Than 
  //
  // The ordering of two values of different types sharing a common type, C, is
  // equivalent to the same operation after converting both arguments to the
  // common type. The requirement applies symmetrically. Both must hold:
  //
  //    t < u <=> C(t) < C(u)
  //    u < t <=> C(u) < C(t)
  struct crosstype_less
  {
    template <typename T, typename U>
      bool operator()(const T& t, const U& u) const
      {
        using C = Common_type<T, U>;
        return (t < u) == (C(t) < C(u))
            && (u < t) == (C(u) < C(t));
      }
  };


  // Cross-type Greater Than 
  //
  // The ordering of two values of different types sharing a common type, C, is
  // equivalent to the same operation after converting both arguments to the
  // common type. The requirement applies symmetrically. Both must hold:
  //
  //    t > u <=> C(t) > C(u)
  //    u > t <=> C(u) > C(t)
  struct crosstype_greater
  {
    template <typename T, typename U>
      bool operator()(const T& t, const U& u) const
      {
        using C = Common_type<T, U>;
        return (t > u) == (C(t) > C(u))
            && (u > t) == (C(u) > C(t));
      }
  };


  // Cross-type Less Than or Equal To
  //
  // The ordering of two values of different types sharing a common type, C, is
  // equivalent to the same operation after converting both arguments to the
  // common type. The requirement applies symmetrically. Both must hold:
  //
  //    t <= u <=> C(t) <= C(u)
  //    u <= t <=> C(u) <= C(t)
  struct crosstype_less_equal
  {
    template <typename T, typename U>
      bool operator()(const T& t, const U& u) const
      {
        using C = Common_type<T, U>;
        return (t <= u) == (C(t) <= C(u))
            && (u <= t) == (C(u) <= C(t));
      }
  };


  // Cross-type Greater Than or Equal To
  //
  // The ordering of two values of different types sharing a common type, C, is
  // equivalent to the same operation after converting both arguments to the
  // common type. The requirement applies symmetrically. Both must hold:
  //
  //    t >= u <=> C(t) >= C(u)
  //    u >= t <=> C(u) >= C(t)
  struct crosstype_greater_equal
  {
    template <typename T, typename U>
      bool operator()(const T& t, const U& u) const
      {
        using C = Common_type<T, U>;
        return (t >= u) == (C(t) >= C(u))
            && (u >= t) == (C(u) >= C(t));
      }
  };


  // Complement of Equality
  //
  // Distinction is the complement of equality. That is:
  //
  //    a != b <=> !(a == b).
  //
  struct complement_of_equality
  {
    template <typename T>
      bool operator()(const T& a, const T& b) const
      {
        return (a != b) == !(a == b);
      }
  };


  // Complement of Less
  //
  // The complement of less is greater or equal. That is:
  //
  //    a > b <=> !(a < b).
  //
  struct complement_of_less
  {
    template <typename T>
      bool operator()(const T& a, const T& b) const
      {
        return (a >= b) == !(a < b);
      }
  };



  // Converse of Less
  //
  // The complement of less is greater. That is:
  //
  //    a > b <=> b < a
  //
  struct converse_of_less
  {
    template <typename T>
      bool operator()(const T& a, const T& b) const
      {
        return (a > b) == (b < a);
      }
  };


  // Complement of Converse of Less
  //
  // The complement of the converse of less is less or equal. That is:
  //
  //    a <= b <=> !(b < a)
  //
  struct complement_of_converse_of_less
  {
    template <typename T>
      bool operator()(const T& a, const T& b) const
      {
        return (a <= b) == !(b < a);
      }
  };



  struct move_construction
  {
    template <typename T>
      bool operator()(const T& a, const T& b) const
      {
        return a == b ? check_equal(T {std::move(a)}, b) : true;
      }
  };

  struct move_assignment
  {
    template <typename T>
      bool operator()(T a, const T& b, const T& c) const
      {
        // Sequence the assignment and comparison so that we don't induce
        // requirements on the result of assignment.
        return b == c ? (a = std::move(b), check_equal(a, c)) : true;
      }
  };

  struct copy_construction
  {
    template <typename T>
      bool operator()(const T& x) const
      {
        return check_equal(T {x}, x);
      }
  };


  struct copy_assignment
  {
    template <typename T>
      bool operator()(T a, const T& b) const
      {
        // Sequence the assignment and comparison so that we don't induce
        // requirements on the result of assignment.
        return (a = b, check_equal(a, b));
      }
  };



  //////////////////////////////////////////////////////////////////////////////
  // Equality Comparable
  //
  // The equality comparable test can be used to evaluate the semantics of the
  // equality comparison relation. That is, the == operator must satisfy
  // the requirements of an equivalence relation, and the != operator must
  // be the negation of the equality operator.
  //
  // In actuality, the == operator must compare for value equality, but we have
  // no way to actually test that.
  template <typename T, typename U = T>
    struct equality_comparable
    {
      static_assert(Equality_comparable<T, U>(), "");

      using C = Common_type<T, U>;

      template <typename Tv, typename Uv>
        void operator()(Tv&& tvar, Uv&& uvar)
        {
          auto cvar = quantify_over<C>();
          operator()(tvar, uvar, cvar);
        }

      template <typename Tv, typename Uv, typename Cv>
        void operator()(Tv&& tvar, Uv&& uvar, Cv&& cvar) const
        {
          quick_check(eq_t, tvar);
          quick_check(eq_u, uvar);
          quick_check(eq_c, cvar);

          // Create "quantified" variables for the given expressions.
          quick_check(cross_eq, tvar, uvar);
          quick_check(cross_df, tvar, uvar);
        }

      equality_comparable<T> eq_t;
      equality_comparable<U> eq_u;
      equality_comparable<C> eq_c;

      crosstype_equality     cross_eq;
      crosstype_distinction  cross_df;
  };


  // Specialization when testing for a single type.
  template <typename T>
    struct equality_comparable<T, T>
    {
      static_assert(Equality_comparable<T>(), "");

      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          auto eqvar = single(std::equal_to<T> {});
          quick_check(eq, eqvar, tvar);
          quick_check(df, tvar, tvar);
        }

      equivalence            eq;
      complement_of_equality df;
    };


  
  //////////////////////////////////////////////////////////////////////////////
  // Weakly Ordered
  //
  // The weakly ordered comparable test can be used to test the semantics of
  // standard relational operators <, >, <=, and >=.
  //
  //
  template <typename T, typename U = T>
    struct weakly_ordered
    {
      static_assert(Weakly_ordered<T, U>(), "");
      
      using C = Common_type<T, U>;

      template <typename Tv, typename Uv>
        void operator()(Tv&& tvar, Uv&& uvar) const
        {
          auto cvar = quantify_over<C>();
          operator()(tvar, uvar, cvar);
        }
      
      template <typename Tv, typename Uv, typename Cv>
        void operator()(Tv&& tvar, Uv&& uvar, Cv&& cvar) const
        {
          quick_check(ord_t, tvar);
          quick_check(ord_u, uvar);
          quick_check(ord_c, cvar);

          quick_check(cross_lt, tvar, uvar);
          quick_check(cross_gt, tvar, uvar);
          quick_check(cross_le, tvar, uvar);
          quick_check(cross_ge, tvar, uvar);
        }

      weakly_ordered<T> ord_t;
      weakly_ordered<U> ord_u;
      weakly_ordered<C> ord_c;

      crosstype_less          cross_lt;
      crosstype_greater       cross_gt;
      crosstype_less_equal    cross_le;
      crosstype_greater_equal cross_ge;
    };


  // Specialization for the unary type.
  template <typename T>
    struct weakly_ordered<T, T>
    {
      static_assert(Weakly_ordered<T>(), "");

      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          auto ltvar = single(std::less<T> {});
          quick_check(lt, ltvar, tvar);
          quick_check(gt, tvar, tvar);
          quick_check(le, tvar, tvar);
          quick_check(ge, tvar, tvar);
        }

      strict_weak_ordering           lt;
      converse_of_less               gt;
      complement_of_converse_of_less le;
      complement_of_less             ge;
    };



  //////////////////////////////////////////////////////////////////////////////
  // Totally Ordered
  //
  // The totally ordered comparable test can be used to test the semantics of
  // standard relational operators <, >, <=, and >=, and their relationship
  // to ==.
  //
  template <typename T, typename U = T>
    struct totally_ordered
    {
      static_assert(Totally_ordered<T, U>(), "");

      using C = Common_type<T, U>;

      template <typename Tv, typename Uv>
        void
        operator()(Tv&& tvar, Uv&& uvar) const 
        {
          auto cvar = quantify_over<C>();
          operator()(tvar, uvar, cvar);
        }

      template <typename Tv, typename Uv, typename Cv>
        void
        operator()(Tv&& tvar, Uv&& uvar, Cv&& cvar) const
        {
          quick_check(ord_t, tvar);
          quick_check(ord_u, uvar);
          quick_check(ord_c, cvar);

          quick_check(cross_lt, tvar, uvar);
          quick_check(cross_gt, tvar, uvar);
          quick_check(cross_le, tvar, uvar);
          quick_check(cross_ge, tvar, uvar);
        }

      totally_ordered<T> ord_t;
      totally_ordered<U> ord_u;
      totally_ordered<C> ord_c;

      crosstype_less          cross_lt;
      crosstype_greater       cross_gt;
      crosstype_less_equal    cross_le;
      crosstype_greater_equal cross_ge;      
    };
    
  template <typename T>
    struct totally_ordered<T, T>
    {
      static_assert(Totally_ordered<T>(), "");

      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          auto ltvar = single(std::less<T> {});
          quick_check(lt, ltvar, tvar);
          quick_check(gt, tvar, tvar);
          quick_check(le, tvar, tvar);
          quick_check(ge, tvar, tvar);
        }

      strict_total_ordering          lt;
      converse_of_less               gt;
      complement_of_converse_of_less le;
      complement_of_less             ge;
    };



  //////////////////////////////////////////////////////////////////////////////
  // Move Constructuble
  //
  template <typename T>
    struct move_constructible
    {
      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          quick_check(ctor, tvar, tvar);
        }

      move_construction ctor;
    };


  //////////////////////////////////////////////////////////////////////////////
  // Movable
  //
  template <typename T>
    struct movable
    {
      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          quick_check(ctor, tvar, tvar);
          quick_check(assign, tvar, tvar, tvar);
        }

      move_construction ctor;
      move_assignment   assign;
    };


  //////////////////////////////////////////////////////////////////////////////
  // Copy Constructuble
  //
  template <typename T>
    struct copy_constructible
    {
      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          quick_check(move, tvar);
          quick_check(ctor, tvar);
        }

      move_constructible<T> move;
      copy_construction ctor;
    };


  //////////////////////////////////////////////////////////////////////////////
  // Copyable
  //
  template <typename T>
    struct copyable
    {
      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          quick_check(move, tvar);
          quick_check(ctor, tvar);
          quick_check(assign, tvar, tvar);
        }

      movable<T>        move;
      copy_construction ctor;
      copy_assignment   assign;
    };


  //////////////////////////////////////////////////////////////////////////////
  // Default Constructible
  //
  // The default value of a default constructible type is unique.
  //
  // Note that an object is only set tothe default value when an object is
  // default initialized, not simply default constructed. For example:
  //
  //    int x;    // x does not have the default value
  //    int x {}; // x is default-initialized to 0.
  //
  template <typename T>
    struct default_constructible
    {
      static_assert(Default_constructible<T>(), "");

      bool operator()() const
      {
        return check_equal(T{}, T{});
      }
    };


  //////////////////////////////////////////////////////////////////////////////
  // Semiregular
  //
  template <typename T>
    struct semiregular
    {
      static_assert(Semiregular<T>(), "");

      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          quick_check(def);
          quick_check(copy, tvar);
        }

      default_constructible<T> def;
      copyable<T>              copy;
    };


  //////////////////////////////////////////////////////////////////////////////
  // Regular
  //
  template <typename T>
    struct regular
    {
      static_assert(Regular<T>(), "");

      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          quick_check(semi, tvar);
          quick_check(eq, tvar);
        }

      semiregular<T>         semi;
      equality_comparable<T> eq;
    };


  //////////////////////////////////////////////////////////////////////////////
  // Ordered
  //
  template <typename T>
    struct ordered
    {
      static_assert(Ordered<T>(), "");

      template <typename Tv>
        void operator()(Tv&& tvar) const
        {
          quick_check(reg, tvar);
          quick_check(ord, tvar);
        }

      regular<T>         reg;
      totally_ordered<T> ord;
    };



  // TODO: Implement testing for function objects. 

  // Check that T is equality comparable.
  template <typename T>
    void 
    check_equality_comparable()
    {
      auto tvar = quantify_over<T>();
      quick_check(equality_comparable<T> {}, tvar);
    }

  // Check that T and U satisfy the requirements of cross-type equality
  // comparison.
  template <typename T, typename U>
    void 
    check_equality_comparable()
    {
      auto tvar = quantify_over<T>();
      auto uvar = quantify_over<U>();
      quick_check(equality_comparable<T, U> {}, tvar, uvar);
    }

  // Check that T is weakly ordered.
  template <typename T>
    void 
    check_weakly_ordered()
    {
      auto tvar = quantify_over<T>();
      quick_check(weakly_ordered<T> {}, tvar);
    }

  // Check that T and U satisfy the requirements of cross-type weak ordering.
  template <typename T, typename U>
    void 
    check_weakly_ordered()
    {
      auto tvar = quantify_over<T>();
      auto uvar = quantify_over<U>();
      quick_check(weakly_ordered<T, U> {}, tvar, uvar);
    }

  // Check that T is totally ordered.
  template <typename T>
    void 
    check_totally_ordered()
    {
      auto tvar = quantify_over<T>();
      quick_check(weakly_ordered<T> {}, tvar);
    }

  // Check that T and U satisfy the requirements of cross-type total ordering.
  template <typename T, typename U>
    void 
    check_totally_ordered()
    {
      auto tvar = quantify_over<T>();
      auto uvar = quantify_over<U>();
      quick_check(weakly_ordered<T, U> {}, tvar, uvar);
    }

  // Check that T is move constructible.
  template <typename T>
    void
    check_move_constructible()
    {
      auto tvar = quantify_over<T>();
      quick_check(move_constructible<T> {}, tvar);
    }

  // Check that T is movable.
  template <typename T>
    void
    check_movable()
    {
      auto tvar = quantify_over<T>();
      quick_check(movable<T> {}, tvar);
    }

  // Check that T is copy constructible.
  template <typename T>
    void
    check_copy_constructible()
    {
      auto tvar = quantify_over<T>();
      quick_check(copy_constructible<T> {}, tvar);
    }

  // Check that T is copyable.
  template <typename T>
    void
    check_copyable()
    {
      auto tvar = quantify_over<T>();
      quick_check(copyable<T> {}, tvar);
    }

  // Check that T is default constructible.
  template <typename T>
    void
    check_default_constructible()
    {
      quick_check(default_constructible<T> {});
    }

  // Check that T is semiregular.
  template <typename T>
    void 
    check_semiregular()
    {
      auto tvar = quantify_over<T>();
      quick_check(semiregular<T> {}, tvar);
    }

  // Check that T is regular.
  template <typename T>
    void 
    check_regular()
    {
      auto tvar = quantify_over<T>();
      quick_check(regular<T> {}, tvar);
    }

  // Check that T is ordered
  template <typename T>
    void
    check_ordered()
    {
      auto tvar = quantify_over<T>();
      quick_check(ordered<T> {}, tvar);
    }

} // namespace origin

