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
#include <utility>
#include <ostream>
#include <vector>

const char* GREEN = "\033[0;32m";
const char* RED = "\033[0;31m";
const char* C_RESET = "\033[0m";

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe&) {}
PmergeMe	PmergeMe::operator=(const PmergeMe&) { return PmergeMe();}

PmergeMe::Int::Int(int value): value(value) { ++amount; }

PmergeMe::Int::Int(const Int& other): value(other.value) {}

int	PmergeMe::Int::amount = 0;
int	PmergeMe::Int::comparisons= 0;

bool	PmergeMe::Int::operator<(const Int& rhs) const
{
	++comparisons;
	return this->value < rhs.value;
}

bool	PmergeMe::Int::operator>(const Int& rhs) const
{
	++comparisons;
	return this->value > rhs.value;
}

bool	PmergeMe::Int::operator==(const Int& rhs) const
{
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

void	printPair(const PmergeMe::Pair& pair)
{
	std::cout << "{";
	const size_type	firstSize = pair.first.size();
	const size_type	secondSize = pair.second.size();
	for (size_type i = 0; i < firstSize; ++i)
	{
		std::cout << pair.first[i];
		if (i != firstSize - 1)
		{
			std::cout << ",";
		}
	}
	std::cout << "|";
	for (size_type i = 0; i < secondSize; ++i)
	{
		std::cout << pair.second[i];
		if (i != secondSize - 1)
		{
			std::cout << ",";
		}
	}
	std::cout << "}";
}

void	printPairs(const std::vector<PmergeMe::Pair>& pairs)
{
	std::cout << "-\n";
	std::for_each(pairs.begin(), pairs.end(), printPair);
	std::cout << "\n-\n";
}

template <typename T>
void	printNumbers(const T& container, const char* label, const char* colour = RED)
{
	std::cout << colour << label << ':' << C_RESET << ' ';
	std::for_each(container.begin(), container.end(), print);
	std::cout << '\n';
}

void	printMainChain(const std::vector<PmergeMe::Element>& main, const char* message = "main")
{
	for (size_type i = 0; i < main.size(); ++i)
	{
		printNumbers(main[i], message);
	}
}

void	printPend(const std::vector<PmergeMe::BoundedPendEle>& pend)
{
	for (size_type i = 0; i < pend.size(); ++i)
	{
		const std::vector<PmergeMe::Int>&	pe = pend[i].first;
		printNumbers(pe, "pend");
		std::cout << "Bound to idx " << pend[i].second << '\n';
	}
}

std::ostream&	operator<<(std::ostream& os, const PmergeMe::Int& num)
{
	return os << num.value;
}

std::vector<PmergeMe::Int>	flattenPairs(const std::vector<PmergeMe::Pair>& pairs)
{
	std::vector<PmergeMe::Int>	vec;

	for (size_type i = 0; i < pairs.size(); ++i)
	{
		const PmergeMe::Pair&				pair = pairs[i];
		const std::vector<PmergeMe::Int>&	first = pair.first;
		const std::vector<PmergeMe::Int>&	second = pair.second;
		for (size_type i = 0; i < first.size(); ++i)
		{
			vec.push_back(first[i]);
		}
		for (size_type i = 0; i < second.size(); ++i)
		{
			vec.push_back(second[i]);
		}
	}
	return vec;
}

std::vector<PmergeMe::Int>
flattenMainChain(const std::vector<PmergeMe::Element>& mainChain)
{
	std::vector<PmergeMe::Int>	vec;

	for (size_type i = 0; i < mainChain.size(); ++i)
	{
		const PmergeMe::Element&	element = mainChain[i];
		for (size_type j = 0; j < element.size(); ++j)
		{
			vec.push_back(element[j]);
		}
	}
	return vec;
}

void	PmergeMe::parse(char **args, size_type count)
{
	for (size_type i = 0; i < count; ++i)
	{
		int	num = validatePositiveInteger(args[i]);
		vec.push_back(num);
	}
	printNumbers(vec, "Before   ", RED);
}



// PmergeMe::Bound	findBound(const std::vector<PmergeMe::Element>& mainChain, Int number)
// {
// 	PmergeMe::Bound it = std::find(mainChain.begin(), mainChain.end(), number);
// 	return it;
// }

std::vector<PmergeMe::Pair>
createPairs(const std::vector<PmergeMe::Int>& sequence,
			size_type elementSize,
			size_type elementCount)
{
	std::vector<PmergeMe::Pair>	pairs;

	// TODO: double check the condition to terminate the loop
	size_type	eleCreated = 0;
	// for (size_type i = 0; i <= sequence.size() / 2;)
	for (size_type i = 0; eleCreated < elementCount;)
	{
		std::vector<PmergeMe::Int>	first;
		std::vector<PmergeMe::Int>	second;
		for (size_type count = 0; count < elementSize; ++count, ++i)
		{
			first.push_back(sequence[i]);
		}
		++eleCreated;
		for (size_type count = 0; count < elementSize; ++count, ++i)
		{
			second.push_back(sequence[i]);
		}
		++eleCreated;
		pairs.push_back(std::make_pair(first, second));
	}
	return pairs;
}

void	sortPairs(std::vector<PmergeMe::Pair>& pairs)
{
	for (size_type i = 0; i < pairs.size(); ++i)
	{
		PmergeMe::Pair&	pair = pairs[i];
		PmergeMe::Int&	lhs = pair.first.back();
		PmergeMe::Int&	rhs = pair.second.back();
		if (lhs > rhs)
		{
			std::swap(pair.first, pair.second);
		}
	}
}

PmergeMe::Element
createElement(const std::vector<PmergeMe::Int>& sequence,
			  int& index,
			  size_type elementSize)
{
	PmergeMe::Element	element;
	for (int i = 0; i < (int)elementSize; ++i, ++index)
	{
		element.push_back(sequence[index]);
	}
	return element;
}

std::vector<PmergeMe::Element>
doInsertions(std::vector<PmergeMe::Element>& mainChain,
			 std::vector<PmergeMe::BoundedPendEle>& pend)
{
	// insert from pend
	// TODO: jacobsthal numbers
	while (!pend.empty())
	{
		const PmergeMe::BoundedPendEle&	pendEle = pend.back();
		const PmergeMe::Element&		ele = pendEle.first;
		int								idx = pendEle.second;
		std::vector<PmergeMe::Element>::iterator	pos =
			std::lower_bound(mainChain.begin(),
							 mainChain.begin()+idx,
							 ele);
		std::cout << "inserting from pend\n";
		mainChain.insert(pos, ele);
		for (size_type i = 0; i < mainChain.size(); ++i)
		{
			printNumbers(mainChain[i], "main after inserting");
		}
		pend.pop_back();
	}
	return mainChain;
}

void	PmergeMe::fordJohnsonMergeInsertionSort()
{
	std::vector<Int>	copy = vec;
	size_type			elementSize = 1;
	std::vector<Int>	sequenceAfterPairings = copy;
	std::vector<Int>	oldSequence = copy;
	while (elementSize <= oldSequence.size() / 2)
	{
		const size_type	elementCount = oldSequence.size() / elementSize;
		// bool			hasLeftover = elementCount % 2;
		bool	hasLeftover = oldSequence.size() % 2;
		// create pairs
		std::vector<Pair>	pairs = createPairs(
									oldSequence,
									elementSize,
									hasLeftover ? elementCount-1 : elementCount);
		printPairs(pairs);
		sortPairs(pairs);
		printPairs(pairs);
		sequenceAfterPairings = flattenPairs(pairs);
		printNumbers(sequenceAfterPairings, "After flattening");
		if (hasLeftover)
		{
			std::vector<Int>	leftovers;
			// size_type			remainingCount = oldSequence.size() - ((elementCount-1) * elementSize);
			size_type			remainingCount = oldSequence.size() - sequenceAfterPairings.size();
			std::vector<Int>::iterator startIt = oldSequence.end() - remainingCount;
			while (startIt != oldSequence.end())
			{
				leftovers.push_back(*startIt);
				++startIt;
			}
			sequenceAfterPairings.insert(sequenceAfterPairings.end(), leftovers.begin(), leftovers.end());

			printNumbers(leftovers, "leftovers");
		}
		printNumbers(sequenceAfterPairings, "After adding straggler");
		oldSequence = sequenceAfterPairings;
		elementSize *= 2;
	}

	// we ran out of elements to form pairs so we are done
	// now we initialize main chain and pend to start inserting

	// elementSize is doubled to break out of the previous loop
	// so we need to half it back here to a viable size
	int	eSize = elementSize / 2; 
	std::vector<Int>	sequence = sequenceAfterPairings;
	std::vector<Int>	prevSequence = sequence;
	while (eSize >= 1)
	{
		std::vector<Element>		mainChain;
		std::vector<BoundedPendEle>	pend;
		std::vector<Int>			remaining;
		int							sequencePos = 0;
		size_type	elementCount = sequence.size() / eSize;
		// bool			hasLeftover = elementCount % 2;
		bool	hasLeftover = sequence.size() % 2;
		if (hasLeftover)
			--elementCount;

		// create main chain and pend
		std::vector<Element>	elementBs;
		size_type				elesCreated = 0;
		// while ((int)sequence.size() - sequencePos >= eSize)
		while (elesCreated < elementCount)
		{
			Element	b = createElement(sequence, sequencePos, eSize);
			++elesCreated;
			elementBs.push_back(b);
			if (elesCreated >= elementCount)
			{
				break ;
			}
			Element	a = createElement(sequence, sequencePos, eSize);
			++elesCreated;
			mainChain.push_back(a);
		}
		while ((size_type)sequencePos < sequence.size())
		{
			remaining.push_back(sequence[sequencePos]);
			++sequencePos;
		}
		mainChain.insert(mainChain.begin(), elementBs.front());
		for (size_type i = 1; i < elementBs.size(); ++i)
		{
			const Element&	b = elementBs[i];
			pend.push_back(std::make_pair(b, i+1));
		}
		printMainChain(mainChain);
		printPend(pend);

		if (!pend.empty())
		{
			mainChain = doInsertions(mainChain, pend);
		}
		sequence = flattenMainChain(mainChain);
		if (hasLeftover)
		{
			std::vector<Int>			leftovers;
			// size_type			remainingCount = oldSequence.size() - ((elementCount-1) * elementSize);
			size_type					remainingCount = prevSequence.size() - sequence.size();
			std::vector<Int>::iterator	startIt = prevSequence.end() - remainingCount;
			while (startIt != prevSequence.end())
			{
				leftovers.push_back(*startIt);
				++startIt;
			}
			sequence.insert(sequence.end(), leftovers.begin(), leftovers.end());
			printNumbers(leftovers, "leftovers");
		}
		// for (size_type i = 0; i < remaining.size(); ++i)
		// {
		// 	sequence.push_back(remaining[i]);
		// }

		eSize /= 2;
		std::cout << "loop done\n";
	}
}

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
