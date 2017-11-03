// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MEMORY_CONCEPTS_HPP
#define ORIGIN_MEMORY_CONCEPTS_HPP

#include <memory>

#include <origin/type/concepts.hpp>

namespace origin
{
  namespace memory_impl
  {
    // Deduce the result type of the expression a.allocate(n) for an object
    // of type A and an integer n.
    template <typename A>
      struct get_allocate_result
      {
      private:
        template <typename X>
          static auto check(X& x) -> decltype(x.allocate(1));

        static subst_failure check(...);
      public:
        using type = decltype(check(std::declval<A&>()));
      };

    // Deduce the result type of the expression a.deallocate(p) for an object
    // of type A and pointer p, of type A::pointer.
    //
    // FIXME: I think I'm presuming the existence of associated member types
    // I should check those before hand.
    template <typename A>
      struct get_deallocate_result
      {
      private:
        template <typename X>
          static auto check(X& x) -> decltype(x.deallocate(nullptr, 1));

        static subst_failure check(...);
      public:
        using type = decltype(check(std::declval<A&>()));
      };
  } // namespace memory_impl



  template <typename A>
    using Allocate_result = 
      typename memory_impl::get_allocate_result<A>::type;

  template <typename A>
    constexpr bool Has_allocate()
    {
      return Subst_succeeded<Allocate_result<A>>();
    }



  template <typename A>
    using Deallocate_result = 
      typename memory_impl::get_deallocate_result<A>::type;

  template <typename A>
    constexpr bool Has_deallocate()
    {
      return Subst_succeeded<Deallocate_result<A>>();
    }



  //////////////////////////////////////////////////////////////////////////////
  // Allocator                                            mem.concepts.allocator
  //
  // An allocator is an object that is responsible solely for the allocation
  // and deallocation of memory. Conceptually, an allocator sits astride a
  // block of memory and provides locations for objects. Minimally an allocator
  // must provide two operations:
  //
  //    a.allocate(n)
  //    a.deallocate(p, n)
  //
  // The allocate method creates storage for n objects, returning a pointer to
  // the first object. The deallocate method returns the storage of n objects
  // pointed to by p to the allocator.
  //
  // An "allocating type" is one that dynamically allocates memory using an
  // allocator. The basic requirement of allocating types is that every
  // constructor (except move constructors) can take an additional argument of
  // allocator type. 
  template <typename A>
    constexpr bool Allocator()
    {
      return Has_allocate<A>() && Has_deallocate<A>();
    }


  // Returns true iff T can be allocator-constructed over args...
  template <typename T, typename... Args>
    constexpr bool Allocator_constructible()
    {
      return Constructible<T, Args..., std::allocator<void>>();
    };

} // namespace origin

#endif
