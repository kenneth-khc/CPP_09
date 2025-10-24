/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:39:14 by kecheong          #+#    #+#             */
/*   Updated: 2025/10/19 21:39:15 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <cmath>
#include <iterator>
#include <vector>
#include <set>
#include <list>

typedef std::vector<int>::size_type	size_type;

int	validatePositiveInteger(const char* str);

class PmergeMe
{
public:
	struct Int
	{
		static int	comparisons;
		static int	amount;

		int	value;
		Int(int value);
		Int(const Int&);

		bool	operator<(const Int&) const;
		bool	operator>(const Int&) const;
		bool	operator==(const Int&) const;
		Int		operator=(const Int& other)
		{
			this->value = other.value;
			return *this;
		}
	};


	PmergeMe();
	PmergeMe(const PmergeMe&);
	~PmergeMe();

	PmergeMe	operator=(const PmergeMe&);

	void				parse(char** args, size_type count);
	std::vector<Int>	fordJohnsonMergeInsertionSort() const;
	std::list<Int>		listMergeInsertion() const;
	bool				validateSorted(const std::vector<PmergeMe::Int>&) const;


	static void	logAndResetComparisons();


	typedef std::vector<Int>::size_type				size_type;
	typedef std::set<int>::iterator					SetIter;
	typedef std::vector<Int>						Element;
	typedef std::list<Int>							ListElement;
	typedef std::pair<Element, Element>				Pair;
	typedef std::pair<ListElement, ListElement>		ListPair;
	typedef std::vector<Element>::const_iterator	Bound;
	typedef std::pair<Element, size_type>			PendElement;
	typedef std::pair<ListElement, size_type>		PendListElement;
	typedef std::vector<PendElement>				Pend;
	typedef Pend::iterator							PendIter;
	typedef Pend::reverse_iterator					PendRevIter;
	typedef Pend::const_iterator					PendConstIter;
	typedef std::vector<Element>					MainChain;
	typedef MainChain::iterator						MainChainIter;
	typedef MainChain::const_iterator 				MainChainConstIter;
	typedef std::vector<Int>						Vec;
	typedef Vec::iterator							VecIter;
	typedef Vec::const_iterator						VecConstIter;

private:

	std::vector<Int>	vec;
	std::list<Int>		list;

	// template <typename T>
	// size_type			mergePairs(T&) const;
	size_type			mergePairs(std::vector<Int>&) const;
	size_type			mergePairs(std::list<Int>&) const;

	size_type			maxPairingSize(size_type) const;
	// std::vector<Pair>	createPairs(std::vector<Int>& sequence,
	// 								size_type elementSize,
	// 								size_type elementCount) const;

	template <typename PairType, typename SequenceType>
	PairType	createPairs(const SequenceType& sequence,
							size_type elementSize,
							size_type elementCount) const
	{
		PairType	pairs;
		// std::vector<Pair>	pairs;
		typename SequenceType::const_iterator	iter = sequence.begin();
		typename SequenceType::const_iterator	end = iter;
		for (size_type created = 0; created < elementCount; created += 2)
		{
			// std::vector<Int>	first(iter, iter + elementSize);
			std::advance(end, elementSize);
			SequenceType	first(iter, end);
			std::advance(iter, elementSize);
			// iter += elementSize;
			std::advance(end, elementSize);
			// std::vector<Int>	second(iter, iter + elementSize);
			SequenceType	second(iter, end);
			// iter += elementSize;
			std::advance(iter, elementSize);
			pairs.push_back(std::make_pair(first, second));
		}
		return pairs;
	}

	template <typename T>
	void	sortPairs(T& pairs) const
	{
		typename T::iterator	iter = pairs.begin();
		typename T::iterator	end = pairs.end();
		for (; iter != end; ++iter)
		{
			Int&	lhs = iter->first.back();
			Int&	rhs = iter->second.back();
			if (rhs < lhs)
			{
				std::swap(iter->first, iter->second);
			}
		}
	}

	template <typename Container>
	Container	storeLeftovers(const Container& sequence,
							   size_type eleCount,
							   size_type eleSize) const
	{
		Container	leftovers;
		size_type	leftoverCount = sequence.size() %
									(eleCount * eleSize);
		if (leftoverCount)
		{
			typename Container::const_iterator	end = sequence.end();
			std::advance(end, -leftoverCount);
			leftovers.insert(leftovers.end(),
			                 end,
							 // sequence.end() - leftoverCount,
							 sequence.end());
		}
		return leftovers;
	}
	// std::vector<Int>	storeLeftovers(const std::vector<Int>&,
	// 								   size_type, size_type) const;
	// std::vector<Int>	storeLeftovers(const std::list<Int>&,
	// 								   size_type, size_type) const;

	// std::vector<Int>	insert(const std::vector<Int>&, size_type) const;

	template <typename ElementType, typename PendElementType, typename SequenceType>
	SequenceType	insert(const SequenceType& copy, size_type eleSize) const
	{
		SequenceType	sequence = copy;
		while (eleSize >= 1)
		{
			size_type		eleCount = sequence.size() / eleSize;
			SequenceType	leftovers = storeLeftovers(sequence,
													   eleCount,
													   eleSize);
			ElementType						mainChain;
			PendElementType					pend;
			size_type						created = 0;
			typename SequenceType::iterator	iter = sequence.begin();
			typename SequenceType::iterator	end = sequence.begin();
			for (bool a = false;; a = !a)
			{
				std::advance(end, eleSize);
				typename ElementType::value_type ele(iter, end);
				if (a || created == 0)
				{
					mainChain.push_back(ele);
				}
				else
				{
					pend.push_back(std::make_pair(ele, mainChain.size()));
				}
				// iter += eleSize;
				std::advance(iter, eleSize);
				++created;
				if (created >= eleCount)
				{
					break;
				}
			}
			if (!pend.empty())
			{
				insertPendElementsIntoMainChain(mainChain, pend);
			}
			sequence = flatten<SequenceType>(mainChain);
			sequence.insert(sequence.end(), leftovers.begin(), leftovers.end());
			eleSize /= 2;
		}
		return sequence;
	}

	// void				insertPendElementsIntoMainChain(std::vector<Element>&,
	// 								std::vector<PendElement>&) const;

	// TODO: FIGURE HOW PASSING COMPARISON FUNCTION TO LOWER BOUND
	// WHEN TEMPLATED
	template <typename ElementType, typename PendElementType>
	void	insertPendElementsIntoMainChain(ElementType& mainChain, PendElementType& pend)const
	{
		int	term = 3;
		int	insertedCount = 0;

		while (!pend.empty())
		{
			size_type	jacobsthalNumber = jacobsthal(term);
			typename PendElementType::reverse_iterator	pendEle = pend.rbegin();
			typename PendElementType::reverse_iterator	iter = pend.rbegin();
			typename PendElementType::reverse_iterator	end = pend.rend();
			for (;
				 iter != end && pendEle->second > jacobsthalNumber;
				 ++iter)
			{
				if (iter->second <= jacobsthalNumber)
				{
					pendEle = iter;
				}
			}
			while (pendEle != pend.rend())
			{
				const typename PendElementType::value_type::first_type	group = pendEle->first;
				// const std::vector<Int>&	group = pendEle->first;
				size_type				boundIndex = pendEle->second;
				typename ElementType::iterator	searchEnd = mainChain.begin();
				std::advance(searchEnd, boundIndex + insertedCount);
				typename ElementType::value_type	element(group.begin(), group.end());
				typename ElementType::iterator	insertPos =
					std::lower_bound(mainChain.begin(),
					searchEnd,
					element,
					compareLessThan);

				mainChain.insert(insertPos, element);
				++insertedCount;

				typename PendElementType::iterator	it = pendEle.base();
				std::advance(it, -1);
				pend.erase(it);
				++pendEle;
			}
			++term;
		}
	}

	static bool
	compareLessThan(const Element&, const Element&);

	static bool
	compareLessThan(const ListElement&, const ListElement&);

	// template <typename Type>
	// static bool
	// compareLessThan(const Type& lhs, const Type& rhs)
	// {
	// 	return lhs.back() < rhs.back();
	// }

	size_type	jacobsthal(size_type n) const;

	std::vector<Int>
	flatten(const std::vector<PmergeMe::Pair>&) const;

	std::vector<PmergeMe::Int>
	flatten(const std::vector<PmergeMe::Element>&) const;

	template <typename OutType, typename InType>
	OutType	flatten(const InType& sequence) const
	{
		OutType	container;
		typename InType::const_iterator	iter = sequence.begin();
		typename InType::const_iterator	end = sequence.end();
		for (; iter != end; std::advance(iter, 1))
		{
			container.insert(container.end(), iter->first.begin(), iter->first.end());
			container.insert(container.end(), iter->second.begin(), iter->second.end());
		}
		return container;
	}

	void
	printMainChain(const std::vector<Element>&, const char* = "main") const;
	void
	printPend(const std::vector<PendElement>&) const;
};

std::ostream&	operator<<(std::ostream&, const PmergeMe::Int&);
