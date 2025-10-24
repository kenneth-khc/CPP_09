/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:39:23 by kecheong          #+#    #+#             */
/*   Updated: 2025/10/19 21:39:24 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <algorithm>
#include <limits>
#include <utility>
#include <ostream>
#include <vector>
#include <set>

const char* GREEN = "\033[0;32m";
const char* RED = "\033[0;31m";
const char* C_RESET = "\033[0m";

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe&) {}
PmergeMe	PmergeMe::operator=(const PmergeMe&) { return PmergeMe();}

PmergeMe::Int::Int(int value): value(value) {}

PmergeMe::Int::Int(const Int& other): value(other.value) {}

int	PmergeMe::Int::amount = 0;
int	PmergeMe::Int::comparisons= 0;

bool	PmergeMe::Int::operator<(const Int& rhs) const
{
	++comparisons;
	// std::cout << value << " < " << rhs << '\n';
	return this->value < rhs.value;
}

bool	PmergeMe::Int::operator>(const Int& rhs) const
{
	++comparisons;
	// std::cout << value << " > " << rhs << '\n';
	return this->value > rhs.value;
}

bool	PmergeMe::Int::operator==(const Int& rhs) const
{
	++comparisons;
	// std::cout << value << " == " << rhs << '\n';
	return this->value == rhs.value;
}

void	PmergeMe::logAndResetComparisons()
{
    int sum = 0;
    for (int k = 1; k <= Int::amount; ++k)
    {
        double value = (3.0 / 4.0) * k;
        sum += static_cast<int>(ceil(log2(value)));
    }
    std::cout << "For " << Int::amount << " numbers, expected at least " << sum << " comparisons, "
	<< "total comparisons: " << Int::comparisons << '\n';

	Int::comparisons = 0;
}

void	print(const PmergeMe::Int& num)
{
	std::cout << num << " ";
}

template <typename T>
void	printNumbers(const T& container, const char* label, const char* colour = RED)
{
	std::cout << colour << label << ':' << C_RESET << ' ';
	std::for_each(container.begin(), container.end(), print);
	std::cout << '\n';
}


void
PmergeMe::printMainChain(const std::vector<Element>& main, const char* message) const
{
	std::cout << GREEN << message << ':' << C_RESET << ' ';
	for (MainChainConstIter it = main.begin(); it != main.end(); ++it)
	{
		std::cout << '|';
		const Vec& v = *it;
		for (VecConstIter vit = v.begin(); vit != v.end(); ++vit)
		{
			std::cout << *vit;
			if (vit != v.end() - 1)
			{
				std::cout << ',';
			}
		}
		std::cout << "| ";
	}
	std::cout << '\n';
}

void	PmergeMe::printPend(const std::vector<PendElement>& pend) const
{
	std::cout << "Pend:\n";
	for (PendConstIter it = pend.begin();
		 it != pend.end();
		 ++it)
	{
		std::cout << "|";
		const PendElement&	pair = *it;
		const Vec&			v = pair.first;
		for (VecConstIter itt = v.begin(); itt != v.end(); ++itt)
		{
			std::cout << *itt;
			if (itt != pair.first.end() - 1)
			{
				std::cout <<",";
			}
		}
		std::cout << "| -> " << pair.second << '\n';
	}
}

std::ostream&	operator<<(std::ostream& os, const PmergeMe::Int& num)
{
	return os << num.value;
}

std::vector<PmergeMe::Int>
PmergeMe::flatten(const std::vector<Pair>& pairs) const
{
	std::vector<Int>	vec;
	std::vector<Pair>::const_iterator	iter = pairs.begin();
	std::vector<Pair>::const_iterator	end = pairs.end();
	for (; iter != end; ++iter)
	{
		vec.insert(vec.end(), iter->first.begin(), iter->first.end());
		vec.insert(vec.end(), iter->second.begin(), iter->second.end());
	}
	return vec;
}

template <>
std::vector<PmergeMe::Int>	PmergeMe::flatten(const std::vector< std::vector<Int> >& mainChain) const
{
	std::vector<Int>	vec;
	std::vector<Element>::const_iterator	iter = mainChain.begin();
	std::vector<Element>::const_iterator	end = mainChain.end();

	for (; iter != end; ++iter)
	{
		vec.insert(vec.end(), iter->begin(), iter->end());
	}
	return vec;
}

template <>
std::list<PmergeMe::Int>	PmergeMe::flatten(const std::list< std::list<Int> >& mainChain) const
{
	std::list<Int>	list;
	std::list<ListElement>::const_iterator	iter = mainChain.begin();
	std::list<ListElement>::const_iterator	end = mainChain.end();

	for (; iter != end; ++iter)
	{
		list.insert(list.end(), iter->begin(), iter->end());
	}
	return list;
}

std::vector<PmergeMe::Int>
PmergeMe::flatten(const std::vector<Element>& mainChain) const
{
	std::vector<Int>	vec;
	std::vector<Element>::const_iterator	iter = mainChain.begin();
	std::vector<Element>::const_iterator	end = mainChain.end();

	for (; iter != end; ++iter)
	{
		vec.insert(vec.end(), iter->begin(), iter->end());
	}
	return vec;
}

void	PmergeMe::parse(char** args, size_type count)
{
	std::set<int>				parsedNumbers;
	std::pair<SetIter, bool>	result;

	for (size_type i = 0; i < count; ++i)
	{
		int	num = validatePositiveInteger(args[i]);
		result = parsedNumbers.insert(num);
		if (result.second == false)
		{
			std::cerr << "Error: duplicate number `" << num
					  << "` found, expected unique numbers" << std::endl;
			std::exit(1);
		}
		vec.push_back(num);
		list.push_back(num);
		++Int::amount;
	}
}

// std::vector<PmergeMe::Pair>
// PmergeMe::createPairs(std::vector<Int>& sequence,
// 					  size_type elementSize,
// 					  size_type elementCount) const
// {
// 	std::vector<Pair>	pairs;
// 	VecIter				iter = sequence.begin();
// 	for (size_type created = 0; created < elementCount; created += 2)
// 	{
// 		std::vector<Int>	first(iter, iter + elementSize);
// 		iter += elementSize;
// 		std::vector<Int>	second(iter, iter + elementSize);
// 		iter += elementSize;
// 		pairs.push_back(std::make_pair(first, second));
// 	}
// 	return pairs;
// }

// void	PmergeMe::sortPairs(std::vector<Pair>& pairs) const
// {
// 	std::vector<Pair>::iterator	iter = pairs.begin();
// 	std::vector<Pair>::iterator	end = pairs.end();
// 	for (; iter != end; ++iter)
// 	{
// 		Int&	lhs = iter->first.back();
// 		Int&	rhs = iter->second.back();
// 		if (rhs < lhs)
// 		{
// 			std::swap(iter->first, iter->second);
// 		}
// 	}
// }

bool	PmergeMe::compareLessThan(const Element& lhs, const Element& rhs)
{
	return lhs.back() < rhs.back();
}

bool	PmergeMe::compareLessThan(const ListElement& lhs, const ListElement& rhs)
{
	return lhs.back() < rhs.back();
}

size_type	PmergeMe::jacobsthal(size_type n) const
{
	if (n == 0)
	{
		return 0;
	}
	else if (n == 1)
	{
		return 1;
	}
	return jacobsthal(n - 1) + jacobsthal(n - 2) * 2;
}

size_type	PmergeMe::maxPairingSize(size_type num) const
{
	if (num % 2 != 0)
	{
		return num - 1;
	}
	return num;
}

size_type	PmergeMe::mergePairs(std::vector<Int>& unsorted) const
{
	std::vector<Int>	sequence = unsorted;
	size_type			eleSize = 1;

	// we merge and sort pairs as long as we can form a pair of 2 elements.
	// once the size of our element gets over half of the sequence, we break
	// and return that size to be used as the initial size for the insertion
	while (eleSize <= sequence.size() / 2)
	{
		// determine the number of elements that can be created to form pairs,
		// then sort the elements within each pair.
		// the number of elements that can be created must be even, otherwise
		// there would be a leftover who has no element to compare with, in that
		// case, store the leftovers temporarily to be appended back to the
		// flattened sequence.
		size_type			eleCount = maxPairingSize(sequence.size() / eleSize);
		std::vector<Int>	leftovers = storeLeftovers(sequence, eleCount, eleSize);
		std::vector<Pair>	pairs =
			createPairs< std::vector<Pair> >(sequence, eleSize, eleCount);

		sortPairs(pairs);
		sequence = flatten(pairs);
		sequence.insert(sequence.end(), leftovers.begin(), leftovers.end());
		eleSize *= 2;
	}
	unsorted = sequence;
	return eleSize / 2;
}

size_type	PmergeMe::mergePairs(std::list<Int>& unsorted) const
{
	std::list<Int>	sequence = unsorted;
	size_type		eleSize = 1;

	// we merge and sort pairs as long as we can form a pair of 2 elements.
	// once the size of our element gets over half of the sequence, we break
	// and return that size to be used as the initial size for the insertion
	while (eleSize <= sequence.size() / 2)
	{
		// determine the number of elements that can be created to form pairs,
		// then sort the elements within each pair.
		// the number of elements that can be created must be even, otherwise
		// there would be a leftover who has no element to compare with, in that
		// case, store the leftovers temporarily to be appended back to the
		// flattened sequence.
		size_type		eleCount = maxPairingSize(sequence.size() / eleSize);
		std::list<Int>	leftovers = storeLeftovers(sequence, eleCount, eleSize);
		std::list<ListPair>	pairs =
			createPairs< std::list<ListPair> >(sequence,
															   eleSize,
															   eleCount);

		sortPairs(pairs);
		sequence = flatten< std::list<Int>, std::list<ListPair>  >(pairs);
		sequence.insert(sequence.end(), leftovers.begin(), leftovers.end());
		eleSize *= 2;
	}
	unsorted = sequence;
	return eleSize / 2;
}

// std::vector<PmergeMe::Int>
// PmergeMe::storeLeftovers(const std::vector<Int>& sequence,
// 						 size_type eleCount,
// 						 size_type eleSize) const
// {
// 	std::vector<PmergeMe::Int>	leftovers;
// 	size_type	leftoverCount = sequence.size() %
// 								(eleCount * eleSize);
// 	if (leftoverCount)
// 	{
// 		leftovers.insert(leftovers.end(),
// 						 sequence.end() - leftoverCount,
// 						 sequence.end());
// 	}
// 	return leftovers;
// }

// std::vector<PmergeMe::Int>
// PmergeMe::insert(const std::vector<Int>& copy, size_type eleSize) const
// {
// 	std::vector<Int>	sequence = copy;
// 	while (eleSize >= 1)
// 	{
// 		size_type			eleCount = sequence.size() / eleSize;
// 		std::vector<Int>	leftovers = storeLeftovers(sequence,
// 													   eleCount,
// 													   eleSize);
// 		std::vector<Element>		mainChain;
// 		std::vector<PendElement>	pend;
// 		size_type					created = 0;
// 		VecConstIter				iter = sequence.begin();
// 		for (bool a = false;; a = !a)
// 		{
// 			Element	ele(iter, iter + eleSize);
// 			if (a || created == 0)
// 			{
// 				mainChain.push_back(ele);
// 			}
// 			else
// 			{
// 				pend.push_back(std::make_pair(ele, mainChain.size()));
// 			}
// 			iter += eleSize;
// 			++created;
// 			if (created >= eleCount)
// 			{
// 				break;
// 			}
// 		}
// 		if (!pend.empty())
// 		{
// 			insertPendElementsIntoMainChain(mainChain, pend);
// 		}
// 		sequence = flatten(mainChain);
// 		sequence.insert(sequence.end(), leftovers.begin(), leftovers.end());
// 		eleSize /= 2;
// 	}
// 	return sequence;
// }

bool	PmergeMe::validateSorted(const std::vector<Int>& sequence) const
{
	std::vector<Int>::const_iterator	it = std::adjacent_find(sequence.begin(),
																sequence.end(),
																std::greater<Int>());
	bool	isSorted = sequence.size() == vec.size() &&
					   it == sequence.end();
	if (isSorted)
	{
		std::cout << GREEN << "SORTED" << C_RESET << '\n';
	}
	else
	{
		std::cout << RED << "UNSORTED" << C_RESET << '\n';
	}
	printNumbers(sequence, "After", GREEN);
	if (isSorted)
	{
		std::cout << GREEN << "SORTED" << C_RESET << '\n';
		// std::exit(0);
		return true;
	}
	else
	{
		std::cout << RED << "UNSORTED" << C_RESET << '\n';
		// std::exit(1);
		return false;
	}
}

std::vector<PmergeMe::Int>	PmergeMe::fordJohnsonMergeInsertionSort() const
{
	std::vector<Int>	unsorted = vec;
	int 				elementSize = mergePairs(unsorted);
	std::vector<Int>	sequence =
		insert< std::vector<Element>, std::vector<PendElement> >
		(unsorted, elementSize);

	logAndResetComparisons();

	return sequence;
}

std::list<PmergeMe::Int>	PmergeMe::listMergeInsertion() const
{
	std::list<Int>	unsorted = list;
	int				elementSize = mergePairs(unsorted);
	std::list<Int>	sequence;
	 // =
	// 	insert< std::list<ListElement>, std::list<PendListElement> >
	// 	(unsorted, elementSize);

	logAndResetComparisons();

	return sequence;
}

// void
// PmergeMe::insertPendElementsIntoMainChain(std::vector<Element>& mainChain,
// 					   std::vector<PendElement>& pend) const
// {
// 	int	term = 3;
// 	int	insertedCount = 0;

// 	while (!pend.empty())
// 	{
// 		size_type	jacobsthalNumber = jacobsthal(term);
// 		PendRevIter	pendEle = pend.rbegin();
// 		PendRevIter	end = pend.rend();
// 		for (PendRevIter iter = pend.rbegin();
// 			 iter != end && pendEle->second > jacobsthalNumber;
// 			 ++iter)
// 		{
// 			if (iter->second <= jacobsthalNumber)
// 			{
// 				pendEle = iter;
// 			}
// 		}
// 		while (pendEle != pend.rend())
// 		{
// 			const std::vector<Int>&	group = pendEle->first;
// 			size_type				boundIndex = pendEle->second;
// 			MainChainIter			searchEnd = mainChain.begin() +
// 												boundIndex + insertedCount;
// 			const std::vector<Int>	element(group.begin(), group.end());
// 			const MainChainIter		insertPos =
// 				std::lower_bound(mainChain.begin(),
// 				searchEnd,
// 				element,
// 				compareLessThan);

// 			mainChain.insert(insertPos, element);
// 			++insertedCount;
// 			pend.erase(pendEle.base()-1);
// 			++pendEle;
// 		}
// 		++term;
// 	}
// }

int	validatePositiveInteger(const char* str)
{
	char*	parseEnd = NULL;
	long	num = std::strtol(str, &parseEnd, 0);
	if (num < 0)
	{
		std::cerr << "Error: unexpected negative number `" << num << "`\n";
		std::exit(1);
	}
	if (num > std::numeric_limits<int>::max())
	{
		std::cerr << "Error: unexpected number `" << num
				  << "`, expected integer between 0-"
				  << std::numeric_limits<int>::max() << '\n';
		std::exit(1);
	}
	return num;
}
