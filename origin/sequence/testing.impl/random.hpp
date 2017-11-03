// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_SEQUENCE_TESTING_HPP
#  error This file cannot be included directly. Include sequence/testing.hpp.
#endif

namespace testing
{
  //////////////////////////////////////////////////////////////////////////////
  // Range Generator
  //
  // The iterator generator returns random bounded ranges into an underlying
  // sequence. The range is effectively generated from a 2 element random
  // sample over the sequence.
  //
  // Note that the iterator generator requires R to be a Forward_range. It
  // is not possible to randomly generate iterators into a range whose
  // referenced objects may "evaporate" during traversal. Effectively, we're
  // looking for R to be a container or array.
  template <typename R, 
            typename Dist = std::uniform_int_distribution<Size_type<R>>>
    class range_generator
    {
      static_assert(Forward_range<R>(), "");

      using I = Iterator_of<R>;
      using Param = typename Dist::param_type;
    public:
      using result_type = bounded_range<I>;

      range_generator(R& r)
        : range(r), dist()
      {
        assert(size(range) > 2);
      }

      template <typename Prng>
        result_type operator()(Prng& prng)
        {
          using std::begin;
          using std::end;
          using std::next;

          // Generate 2 random numbers: the first indicating the first 
          // iterator in the sequence and the second indicating a limiting
          // iterator.
          Size_type<R> n = size(range);
          Size_type<R> x = dist(prng, Param(0, n - 1));
          Size_type<R> y = dist(prng, Param(x, n));

          // Get the corresponding iterators (without restarting from begin)
          // and return the range.
          I first = next(begin(range), x);
          I last = next(first, y - x);
          return {first, last};
        }

    private:
      R& range;
      Dist dist;
    };


  // Generate random subranges of the given range.
  template <typename R, typename Dist = range_generator<Remove_reference<R>>>
    inline auto 
    quantify_over_range(R&& range)
      -> decltype(quantify_over(Dist {range}))
    {
      return quantify_over(Dist {range});
    }

} // namespace testing
