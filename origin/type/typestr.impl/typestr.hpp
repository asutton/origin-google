// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPE_TYPESTR_HPP
#  error This file cannot be included directly. Include type/typestr.hpp.
#endif

namespace type_impl
{
  // The typestr dispatcher is used to format type sequences. If multiple types
  // are given, the dispatcher will generate the string "{T1, T2, ...}". If a 
  // single type is given, the result is just the name of the type.
  template <typename... Args>
    struct typestr_dispatch
    {
      std::string operator()() const
      {
        return typelist_to_string<Args...>{}();
      }
    };
    
  template <typename T>
    struct typestr_dispatch<T>
    {
      std::string operator()() const
      {
        return type_to_string<T>{}();
      }
    };

    
  // Type to str.
  // The type_to_string class is a function object used to generate the name 
  // of a type. It is specialized for cv qualifiers and references so that 
  // they are preserved in the generated string.
  template <typename T>
    struct type_to_string
    {
      std::string operator()() const
      {
        return to_string(typeid(T));
      }
    };
    
  // Generate type names for const-qualified types.
  template <typename T>
    struct type_to_string<const T>
    {
      std::string operator()() const
      {
        return "const " + typestr<T>();
      }
    };

  // Generate type names for volatile-qualified types.
  template <typename T>
    struct type_to_string<volatile T>
    {
      std::string operator()() const
      {
        return "volatile " + typestr<T>();
      }
    };

  // Generate type names for cv-qualified types.
  template <typename T>
    struct type_to_string<const volatile T>
    {
      std::string operator()() const
      {
        return "const volatile " + typestr<T>();
      }
    };

  // Generate type names for pointers
  template <typename T>
    struct type_to_string<T*>
    {
      std::string operator()() const
      {
        return typestr<T>() + "*";
      }
    };

  // NOTE: We have to generate type strings differently for patterns like 
  // "T * cv" since they are written right-to-left. The only reason for this
  // is that we are not generating a canonical right-to-left spelling. In
  // other words, the choice to write "int const" as "const int" means that
  // that we have to be aware of some differences.

  // Generate type names for const pointers.
  template <typename T>
    struct type_to_string<T* const>
    {
      std::string operator()() const
      {
        return typestr<T>() + "* const";
      }
    };

  // Generate type names for vola
  template <typename T>
    struct type_to_string<T* volatile>
    {
      std::string operator()() const
      {
        return typestr<T>() + "* volatile";
      }
    };

  template <typename T>
    struct type_to_string<T* const volatile>
    {
      std::string operator()() const
      {
        return typestr<T>() + "* const volatile";
      }
    };


  // Generate reference strings. Note that we don't have to re-order references
  // since writing "T& const" is not a valid type expression.

  // Generate type names for lvalue-reference types.
  template <typename T>
    struct type_to_string<T&>
    {
      std::string operator()() const
      {
        return typestr<T>() + "&";
      }
    };

  // Generate type names for rvalue-reference types.
  template <typename T>
    struct type_to_string<T&&>
    {
      std::string operator()() const
      {
        return typestr<T>() + "&&";
      }
    };
    

  // A helper function for generating function parameter lists.
  template <typename... Args>
    inline std::string 
    function_args_to_string()
    {
      return '(' + typelist_to_string<Args...>{}() + ')';
    }


  // Generate type strings for functions.
  template <typename R, typename... Args>
    struct type_to_string<R(Args...)>
    {
      std::string operator()() const
      {
        return typestr<R>() + function_args_to_string<Args...>();
      }
    };

  // Generate function pointers
  template <typename R, typename... Args>
    struct type_to_string<R(*)(Args...)>
    {
      std::string operator()() const
      {
        return typestr<R>() + "(*)" + function_args_to_string<Args...>();
      }
    };


  // Generate array types differently that typeid does.
  template <typename T, std::size_t N>
    struct type_to_string<T[N]>
    {
      std::string operator()() const
      {
        return typestr<T>() + '[' + std::to_string(N) + ']';
      }
    };

  // Generate array references differently.
  template <typename T, std::size_t N>
    struct type_to_string<T(&)[N]>
    {
      std::string operator()() const
      {
        return typestr<T>() + "(&)" + '[' + std::to_string(N) + ']';
      }
    };



  // Type list to string
  // Compute an intercalated list of type names (i.e., T1, T2, ..., Tn). This 
  // function (object) is intended to be used by typestr.
  template <typename T, typename... Args>
    struct typelist_to_string<T, Args...>
    {
      std::string operator()() const
      {
        return typestr<T>() + ", " + typelist_to_string<Args...>{}();
      }
    };

  // Specialization for a single type.
  template <typename T>
    struct typelist_to_string<T>
    {
      std::string operator()() const { return typestr<T>(); }
    };

  // Specialization for an empty type list
  template<>
    struct typelist_to_string<>
    {
      std::string operator()() const { return std::string{}; }
    };

} // namespace type_impl
