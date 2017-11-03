// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_OPTIONAL_OPTIONAL_HPP
#define ORIGIN_DATA_OPTIONAL_OPTIONAL_HPP

#include <cassert>
#include <iosfwd>

#include <origin/data/concepts.hpp>

namespace origin
{
  //////////////////////////////////////////////////////////////////////////////
  // Optional
  //
  // The optional class template implements an optional "qualifier". The 
  // optional class extends the set of values of an underlying value type
  // with an "invalid" value, much like nullptr for pointer types.
  //
  // Optional types can also be compared for equality and ordered. When both
  // objects are initialized, these comparisons are the same as the underlying
  // type. Otherwise, the unitialized state acts like nullptr.
  //
  // An optional type has two states, initialized or uninitialized. A default
  // initialized optional type is uninitialized. Note that the the underlying
  // type is _not_ initialized until needed. Accessing the value of an optional
  // object in an uninitialized state results in undefined behavior.
  //
  // TODO: Check this class for exception safety.
  template <typename T>
    class optional
    {
    public:
      using value_type = T;

      // Default constructor
      optional();
      
      // Move semantics
      optional(optional&& x);
      optional& operator=(optional&& x);
      
      // Copy semantics
      optional(optional const& x);
      optional& operator=(optional const& x);
      
      
      // Value initialization
      template <typename... Args, 
                typename = Requires<Constructible<T, Args...>()>>
        optional(Args&&... args);

      template <typename Arg, 
                typename = Requires<Assignable<T, Arg>()>>
        optional& operator=(Arg&& args);

      template <typename... Args,
                typename = Requires<Constructible<T, Args...>()>>
        void assign(Args&&... args);

      
      // Nullptr initialization
      optional(std::nullptr_t);
      optional& operator=(std::nullptr_t);

      // Destruction
      ~optional();


      // Returns true if the value is initialized.
      bool initialized() const { return init; }
      
      // Dereference
      // Return a reference to the underlying object.
      T&       operator*()       { return get(); }
      const T& operator*() const { return get(); }

      // Arrow
      // Return a pointer to the underlying object.
      T*       operator->()       { return &get(); }
      const T* operator->() const { return &get(); }

      // Boolean
      explicit operator bool() const { return init; }
      
      // Mutators
      void swap(optional& x);
      void clear();

    private:
      // Return a pointer to the underlying memory.
      T*       ptr()       { return reinterpret_cast<T*>(&mem); }
      const T* ptr() const { return reinterpret_cast<T const*>(&mem); }
      
      // Checked pointer access
      T&       get()       { assert(init); return *ptr(); }
      const T& get() const { assert(init); return *ptr(); }

      template <typename... Args>
        void construct(Args&&... args);

    private:
      bool init;
      Aligned_storage<sizeof(T), alignof(T)> mem;
    };


  // Construct the value of the uninitialized optional value with the
  // given arguments. After construction, the value is initialized.
  template <typename T>
    template <typename... Args>
    inline void
    optional<T>::construct(Args&&... args)
    {
      assert(!init);
      new (ptr()) T(std::forward<Args>(args)...);
      init = true;
    }


  template <typename T>
    inline
    optional<T>::optional() 
      : init(false)
    { }

  // Move semantics
  template <typename T>      
    inline
    optional<T>::optional(optional&& x)
      : init(x.init)
    {
      if (x.init)
        new (ptr()) T {std::move(x.get())};
      x.init = false;
    }
    
  template <typename T>
    inline auto
    optional<T>::operator=(optional&& x) -> optional&
    {
      if (init) {
        if (x.init)
          *ptr() = std::move(*x.ptr());
        else
          clear();
      } else {
        if (x.init)
          construct(std::move(*x.ptr()));
      }     
      return *this;
    }
    
  // Copy semantics
  template <typename T>
    inline
    optional<T>::optional(optional const& x)
      : init(x.init)
    {
      if (x.init)
        new (ptr()) T(x.get());
    }
    
  template <typename T>
    inline auto
    optional<T>::operator=(optional const& x) -> optional& 
    {
      if (init) {
        if (x.init)
          *ptr() = *x.ptr();
        else
          clear();
      } else {
        if (x.init)
          construct(*x.ptr());
      }
      return *this;
    }

  // Value initialization
  template <typename T>
    template <typename... Args, typename Req>
      inline optional<T>::optional(Args&&... args)
        : init(true)
      {
        new (ptr()) T(std::forward<Args>(args)...);
      }

  template <typename T>
    template <typename Arg, typename Req>
      inline auto 
      optional<T>::operator=(Arg&& arg) -> optional&
      {
        assign(std::forward<Arg>(arg));
        return *this; 
      }

  template <typename T>
    template <typename... Args, typename Req>
      inline void
      optional<T>::assign(Args&&... args)
      {
        if (init)
          ptr()->~T();
        new (ptr()) T(std::forward<Args>(args)...);
      }


    // Nullptr initialization
  template <typename T>
    inline 
    optional<T>::optional(std::nullptr_t)
      : init {false}
    { }

  template <typename T>
    inline auto
    optional<T>::operator=(std::nullptr_t) -> optional&
    { 
      clear(); 
      return *this; 
    }

  // Destruction    
  template <typename T>
    inline
    optional<T>::~optional()
    {
      if (init)
        ptr()->~T(); 
    }

  template <typename T>
    inline void 
    optional<T>::swap(optional& x)
    {
      std::swap(init, x.init);
      std::swap(mem, x.mem);
    }

  template <typename T>
    inline void 
    optional<T>::clear()
    {
      if(init) {
        ptr()->~T();
        init = false;
      }
    }




  // Equality_comparable<optional<T>>
  // Two optional objects compare equal when they have the same initialization
  // and the initializedd values compare true.
  template <typename T>
    inline bool 
    operator==(const optional<T>& a, const optional<T>& b)
    {
      if (a.initialized() == b.initialized())
        return a ? *a == *b : true;
      else
        return false;
    }
    
  template <typename T>
    inline bool 
    operator!=(const optional<T>& a, const optional<T>& b)
    {
      return !(a == b);
    }


  // Equality_comparable<optional<T>, T>
  // An optional object, a, compares equal to an object b of type T when a
  // is initialized and has the same value as b.
  template <typename T>
    inline bool 
    operator==(const optional<T>& a, const T& b) 
    { 
      return a && *a == b; 
    }
  
  template <typename T>
    inline bool 
    operator==(const T& a, const optional<T>& b) 
    { 
      return b && *b == a; 
    }

  template <typename T>
    inline bool 
    operator!=(const T& a, const optional<T>& b) 
    { 
      return !(a == b); 
    }

  template <typename T>
    inline bool 
    operator!=(const optional<T>& a, const T& b) 
    { 
      return !(a == b); 
    }


  // Equality_comparable<Optional<T>, nullptr_t>
  // An optional object compares equal to nullptr when it is uninitialized.
  template <typename T>
    inline bool 
    operator==(const optional<T>& a, std::nullptr_t) 
    { 
      return !a; 
    }

  template <typename T>
    inline bool 
    operator==(std::nullptr_t, const optional<T>& a) 
    { 
      return !a; 
    }

  template <typename T>
    inline bool 
    operator!=(const optional<T>& a, std::nullptr_t) 
    { 
      return (bool)a; 
    }
  
  template <typename T>
    inline bool 
    operator!=(std::nullptr_t, const optional<T>& a) 
    { 
      return (bool)a; 
    }


  // Totally ordered
  // When considering total ordereings, the "uninitialized state" is considered
  // to be less than all other values.
  template <typename T>
    inline bool 
    operator<(const optional<T>& a, const optional<T>& b)
    {
      if(!b)
        return false;
      else if(!a)
        return true;
      else
        return *a < *b;
    }
    
  template <typename T>
    inline bool 
    operator>(const optional<T>& a, const optional<T>& b) 
    { 
      return b < a;
    }
    
  template <typename T>
    inline bool 
    operator<=(const optional<T>& a, const optional<T>& b) 
    { 
      return !(b < a); 
    }

  template <typename T>
    inline bool 
    operator>=(const optional<T>& a, const optional<T>& b) 
    { 
      return !(a < b);
    }

    // Totally_ordered<optional<T>, T>
  template <typename T>
    inline bool 
    operator<(const optional<T>& a, const T& b) 
    {
      return a ? *a < b : true; 
    }
    
  template <typename T>
    inline bool 
    operator<(const T& a, const optional<T>& b) 
    { 
      return b ? a < *b : false; 
    }
    
  template <typename T>
    inline bool 
    operator>(const optional<T>& a, const T& b)  
    { 
      return a ? *a > b : false;
    }
    
  template <typename T>
    inline bool 
    operator>(const T& b, const optional<T>& a)  
    { 
      return a ? b > *a : true;
    }
    
  template <typename T>
    inline bool 
    operator<=(const optional<T>& a, const T& b) 
    { 
      return !(b < a); 
    }
    
  template <typename T>
    inline bool 
    operator<=(const T& a, const optional<T>& b) 
    { 
      return !(b < a); 
    }
    
  template <typename T>
    inline bool 
    operator>=(const optional<T>& a, const T& b) 
    { 
      return !(a < b); 
    }
    
  template <typename T>
    inline bool 
    operator>=(const T& a, const optional<T>& b) 
    { 
      return !(a < b); 
    }

  
  // Totally_ordered<optional<T>, nullptr_t>
  // In these comparisons, nullptr is used to represent the uninitialized state.
  template <typename T>
    inline bool 
    operator<(const optional<T>& a, std::nullptr_t)  
    {
      return false; 
    }
    
  template <typename T>
    inline bool 
    operator<(std::nullptr_t, const optional<T>& b)  
    { 
      return (bool)b; 
    }
    
  template <typename T>
    inline bool 
    operator>(const optional<T>& a, std::nullptr_t)  
    { 
      return (bool)a; 
    }
    
  template <typename T>
    inline bool 
    operator>(std::nullptr_t, const optional<T>& a)  
    { 
      return false; 
    }
    
  template <typename T>
    inline bool 
    operator<=(const optional<T>& a, std::nullptr_t) 
    { 
      return !a; 
    }
    
  template <typename T>
    inline bool 
    operator<=(std::nullptr_t, const optional<T>& b) 
    { 
      return true; 
    }
    
  template <typename T>
    inline bool 
    operator>=(const optional<T>& a, std::nullptr_t) 
    { 
      return true; 
    }
    
  template <typename T>
    inline bool 
    operator>=(std::nullptr_t, const optional<T>& b) 
    { 
      return !b; 
    }


  // Output streamable
  template <typename T, typename Char, typename Traits>
    inline std::basic_ostream<Char, Traits>&
    operator<<(std::basic_ostream<Char, Traits>& os, const optional<T>& opt)
    {
      if (opt)
        os << *opt;
      else
        os << "null";
      return os;
    }


} // namespace origin


#endif

