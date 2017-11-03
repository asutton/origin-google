// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/type/traits.hpp>
#include <origin/type/typestr.hpp>

using namespace std;
using namespace origin;

// A type guaranteed to fail tests.
struct fail_t { };

void f0()          { }
int  f1(int)       { }
char f2(int, char) { }

// Used for testing Codomain.
int global__ = 0;
const int& func(int, int) { return global__; }

struct Functor0 { void operator()() const          { } };
struct Functor1 { int  operator()(int) const       { } };
struct Functor2 { char operator()(int, char) const { } };

// A helper for differentiating result types.
template <int N>
  struct type { };

struct s 
{
  type<0> f0()       { }
  type<1> f0() const { }
  
  type<2> f1(int)       { }
  type<3> f1(int) const { }
  
  type<4> f2(int, char)       { }
  type<5> f2(int, char) const { }

  int g() { return 0; }
};

template <typename T>
  struct call_traits;

template <typename F, typename... Args>
  struct call_traits<F(Args...)>
  {
    using fn = F;
    using args = type_list<Args...>;
  };

template <typename Call>
  using Std_result_of = typename std::result_of<Call>::type;


// Check the following properties:
//    a) Call has the expected result type
//    b) The result type is the same in Origin as the Standard
//    c) Failure is handled gracefully.
template <typename Call, typename Result>
  void check_result_of()
  {
    using Fn = typename call_traits<Call>::fn;

    static_assert(Same<Result_of<Call>, Result>(), "");
    static_assert(Same<Result_of<Call>, Std_result_of<Call>>(), "");
    static_assert(Subst_failed<Result_of<Fn(fail_t)>>(), "");
  }

// Checks a and b from above but not c. Safe SFINAE appears to be broken
// with member function pointers.
template <typename Call, typename Result>
  void check_unsafe_result_of()
  {
    using Fn = typename call_traits<Call>::fn;

    static_assert(Same<Result_of<Call>, Result>(), "");
    static_assert(Same<Result_of<Call>, Std_result_of<Call>>(), "");
  }


// FIXME: Find a way to make this fail. I doubt that I can do it without
// support from the compiler.
//
// Submit this as a bug report against GCC.
false_type check_memfun(...);

template <typename F, typename C, typename... Args>
  auto check_memfun(F fptr, C&& c, Args&&... args)
    -> decltype((forward<C>(c).*fptr)(forward<Args>(args)...));


int main()
{
  // FIXME: See comments for check_memfun.
  // auto p = &s::g;
  // using X = decltype(check_memfun(p, s(), 0));


  using Func0 = decltype(f0);
  using Func1 = decltype(f1);
  using Func2 = decltype(f2);

  using Fptr0 = decltype(&f0);
  using Fptr1 = decltype(&f1);
  using Fptr2 = decltype(&f2);

  // BUG: At the time of writing, declaring these in global scope will cause
  // GCC to generate incorrect typeid information. The demangler seems to
  // "no-op" on the buffer allocated for name generation. Not sure why.
  auto lambda0 = [](){};
  auto lambda1 = [](int n) { return n; };
  auto lambda2 = [](int n, char c) { return c; };

  using Lambda0 = decltype(lambda0);
  using Lambda1 = decltype(lambda1);
  using Lambda2 = decltype(lambda2);

  // Non-const member function types
  using Memfun0 = type<0> (s::*)();
  using Memfun1 = type<2> (s::*)(int);
  using Memfun2 = type<4> (s::*)(int, char);

  // Const meember funtion types
  using Const_memfun0 = type<1> (s::*)() const;
  using Const_memfun1 = type<3> (s::*)(int) const;
  using Const_memfun2 = type<5> (s::*)(int, char) const;

  // These are all function types.
  static_assert(Function_type<Func0>(), "");
  static_assert(Function_type<Func1>(), "");
  static_assert(Function_type<Func2>(), "");

  // A basic type identity: &f == F* when F = decltype(f). This is mostly a
  // compiler sanity test.
  static_assert(Same<Fptr0, Func0*>(), "");
  static_assert(Same<Fptr1, Func1*>(), "");
  static_assert(Same<Fptr2, Func2*>(), "");

  // A function pointer does not have function type; it is a pointer. Neither
  // is a member functin pointer.
  static_assert(!Function_type<Func0*>(), "");
  static_assert(!Function_type<Memfun0>(), "");
  static_assert(!Function_type<Const_memfun0>(), "");

  // A functor does not have function type (it has class type). Neither does
  // a lambda expression
  static_assert(!Function_type<Functor0>(), "");
  static_assert(!Function_type<Functor1>(), "");
  static_assert(!Function_type<Functor2>(), "");
  
  static_assert(!Function_type<Lambda0>(), "");
  static_assert(!Function_type<Lambda1>(), "");
  static_assert(!Function_type<Lambda2>(), "");


  // Result_of requires a call expression type. That is: a callable type F
  // the the arguments with which it is called.
  check_result_of<Fptr0(), void>();
  check_result_of<Fptr1(int), int>();
  check_result_of<Fptr2(int, char), char>();

  check_result_of<Functor0(), void>();
  check_result_of<Functor1(int), int>();
  check_result_of<Functor2(int, char), char>();

  check_result_of<Lambda0(), void>();
  check_result_of<Lambda1(int), int>();
  check_result_of<Lambda2(int, char), char>();

  // Check member functions called on class objects.
  check_unsafe_result_of<Memfun0(s), type<0>>();
  check_unsafe_result_of<Memfun1(s, int), type<2>>();
  check_unsafe_result_of<Memfun2(s, int, char), type<4>>();

  // Check member functions called on class pointers.
  check_unsafe_result_of<Memfun0(s*), type<0>>();
  check_unsafe_result_of<Memfun1(s*, int), type<2>>();
  check_unsafe_result_of<Memfun2(s*, int, char), type<4>>();

  // FIXME: This results in compilation failure (can't convert const s* to
  // s*) for the same reasons described above.
  // check_unsafe_result_of<Memfun0(const s*), type<0>>();

  // Check member functions called on class objects.
  check_unsafe_result_of<Const_memfun0(s), type<1>>();
  check_unsafe_result_of<Const_memfun1(s, int), type<3>>();
  check_unsafe_result_of<Const_memfun2(s, int, char), type<5>>();

  // Check member functions called on class pointers and const class pointers.
  check_unsafe_result_of<Const_memfun0(s*), type<1>>();
  check_unsafe_result_of<Const_memfun1(s*, int), type<3>>();
  check_unsafe_result_of<Const_memfun2(s*, int, char), type<5>>();

  check_unsafe_result_of<Const_memfun0(const s*), type<1>>();
  check_unsafe_result_of<Const_memfun1(const s*, int), type<3>>();
  check_unsafe_result_of<Const_memfun2(const s*, int, char), type<5>>();

  // FIXME: I am not currently testing the result of member object pointers. I
  // don't think it tests where the object is a pointer, e



  // Arity and parameters
  static_assert(Function_arity<Func0>() == 0, "");
  static_assert(Function_arity<Func1>() == 1, "");
  static_assert(Function_arity<Func2>() == 2, "");

  static_assert(Same<Parameters_of<Func0>, type_list<>>(), "");
  static_assert(Same<Parameters_of<Func1>, type_list<int>>(), "");
  static_assert(Same<Parameters_of<Func2>, type_list<int, char>>(), "");

  static_assert(Function_arity<Functor0>() == 0, "");
  static_assert(Function_arity<Functor1>() == 1, "");
  static_assert(Function_arity<Functor2>() == 2, "");

  static_assert(Same<Parameters_of<Functor0>, type_list<>>(), "");
  static_assert(Same<Parameters_of<Functor1>, type_list<int>>(), "");
  static_assert(Same<Parameters_of<Functor2>, type_list<int, char>>(), "");

  static_assert(Function_arity<Lambda0>() == 0, "");
  static_assert(Function_arity<Lambda1>() == 1, "");
  static_assert(Function_arity<Lambda2>() == 2, "");

  static_assert(Same<Parameters_of<Lambda0>, type_list<>>(), "");
  static_assert(Same<Parameters_of<Lambda1>, type_list<int>>(), "");
  static_assert(Same<Parameters_of<Lambda2>, type_list<int, char>>(), "");
 

  // Homogeneity
  using Eq = equal_to<int>;
  using Plus = plus<int>;

  static_assert(Homogeneous_function<Eq>(), "");

  // Nullary and unary functions are homogeneous by the definition of Same.
  //
  // FIXME: Should nullary functions be homogeneous? This is different than
  // what is presented in EoP.
  static_assert(Homogeneous_function<Func0>(), "");
  static_assert(Homogeneous_function<Func1>(), "");

  static_assert(Homogeneous_function<Functor0>(), "");
  static_assert(Homogeneous_function<Functor1>(), "");

  static_assert(Homogeneous_function<Lambda0>(), "");
  static_assert(Homogeneous_function<Lambda1>(), "");

  // These functions are known to be non-homogenous.
  static_assert(!Homogeneous_function<Func2>(), "");
  static_assert(!Homogeneous_function<Functor2>(), "");
  static_assert(!Homogeneous_function<Lambda2>(), "");

  // TODO: What if the function takes congruent types modulo references and/op
  // cv-qualifiers? Is it still homogeneous? Does it have a domain? How often
  // do we find such things in real life?

  // Function domain
  static_assert(Same<Domain<Eq>, int>(), "");

  // Domain of unary functions is simply the argument type.
  static_assert(Same<Domain<Func1>, int>(), "");
  static_assert(Same<Domain<Functor1>, int>(), "");
  static_assert(Same<Domain<Lambda1>, int>(), "");

  // Domain of non-homogeneous functions is subst_failure.
  static_assert(Subst_failed<Domain<Func2>>(), "");
  static_assert(Subst_failed<Domain<Functor2>>(), "");
  static_assert(Subst_failed<Domain<Lambda2>>(), "");


  // Codomain
  static_assert(Same<Codomain<Eq>, bool>(), "");
  static_assert(Same<Codomain<Plus>, int>(), "");

  // Codomain is a structural type.
  static_assert(Same<Codomain<decltype(&func)>, int>(), "");

  // The codomain is just the abstracted result type.
  static_assert(Same<Codomain<Func0>, void>(), "");
  static_assert(Same<Codomain<Functor0>, void>(), "");
  static_assert(Same<Codomain<Lambda0>, void>(), "");
}
