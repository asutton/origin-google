// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_SEQUENCE_ITERATOR_HPP
#define ORIGIN_SEQUENCE_ITERATOR_HPP

#include <cstring>
#include <iterator>

#include "algorithm.hpp"

namespace origin
{
	//////////////////////////////////////////////////////////////////////////////
	// Filter Iterator Adaptor
	//
	// A filter iterator is
	//
	// Tempate Parameters:
	//		I -- The underlying iterator
	// 		P -- A predicate function
	template <typename I, typename P>
		class filter_iterator
		{
			static_assert(Input_iterator<I>(), "");
			static_assert(Predicate<P, Value_type<I>>(), "");
		public:
			using value_type = Value_type<I>;
			using reference = Reference_of<I>;
			using pointer = Pointer_of<I>;

			// Constructors

			// Construct a filter iterator over the range [first, last).
			filter_iterator(I first, I last, P pred = {});

			// Construct a filter iterator over the empty range [last, last).
			filter_iterator(I last, P pred = {});


			// Properties

			// Returns the underlying iterator.
			I base() const { return first(); }

			// Returns the current iterator.
			const I& first() const { return std::get<0>(data); }
			
			// Returns an iterator past the end of the sequence.
			const I& last() const { return std::get<1>(data); }

			// Returns the predicate function of the filter iterator.
			const P& pred() const { return std::get<2>(data); }


			// Readable
			reference operator*() const;
			pointer operator->() const;


			// Increment
			filter_iterator& operator++();
			filter_iterator operator++(int);

		private:
			I&       first() { return std::get<0>(data); }
			I&       last()  { return std::get<1>(data); }

			void advance();

		private:
			std::tuple<I, I, P> data;
		};


	template <typename I, typename P>
		inline
		filter_iterator<I, P>::filter_iterator(I first, I last, P pred)
			: data(std::find_if(first, last, pred), last, pred)
		{ }

	template <typename I, typename P>
		inline
		filter_iterator<I, P>::filter_iterator(I last, P pred)
			: data(last, last, pred)
		{ }

	template <typename I, typename P>
		inline auto
		filter_iterator<I, P>::operator*() const -> reference
		{
			return *first();
		}

	template <typename I, typename P>
		inline auto
		filter_iterator<I, P>::operator->() const -> pointer
		{
			return &operator*();
		}

	template <typename I, typename P>
		inline filter_iterator<I, P>&
		filter_iterator<I, P>::operator++()
		{
			advance();
			return *this;
		}

	template <typename I, typename P>
		inline filter_iterator<I, P>
		filter_iterator<I, P>::operator++(int)
		{
			I tmp = first();
			advance();
			return {tmp, last(), pred()};
		}

	template <typename I, typename P>
		inline void
		filter_iterator<I, P>::advance()
		{
			first() = find_next_if(first(), last(), pred());
		}


	// Equality comparable
	//
	// The behavior is undefined if a.pred() is not the same as b.pred().
	template <typename I, typename P>
		inline bool
		operator==(const filter_iterator<I, P>& a, const filter_iterator<I, P>& b)
		{
			return a.first() == b.first()
					&& a.last() == b.last();
		}

	template <typename I, typename P>
		inline bool
		operator!=(const filter_iterator<I, P>& a, const filter_iterator<I, P>& b)
		{
			return !(a == b);
		}


	// Make Filter

	template <typename I, typename P>
		inline filter_iterator<I, P>
		make_filter(I first, I last, P pred)
		{
			return {first, last, pred};
		}

	template <typename I, typename P>
		inline filter_iterator<I, P>
		make_filter(I last, P pred)
		{
			return {last, pred};
		}

} // namespace origin

#endif
