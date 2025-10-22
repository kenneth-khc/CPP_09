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
#include <vector>
#include <set>

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
	};


	PmergeMe();
	PmergeMe(const PmergeMe&);
	~PmergeMe();

	PmergeMe	operator=(const PmergeMe&);

	void				parse(char** args, size_type count);
	std::vector<Int>	fordJohnsonMergeInsertionSort() const;
	bool				validateSorted(const std::vector<PmergeMe::Int>&) const;


	static void	logAndResetComparisons();


	typedef std::vector<Int>::size_type				size_type;
	typedef std::set<int>::iterator					SetIter;
	typedef std::vector<Int>						Element;
	typedef std::pair<Element, Element>				Pair;
	typedef std::vector<Element>::const_iterator	Bound;
	typedef std::pair<Element, size_type>			PendElement;
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

	size_type	mergePairs(std::vector<Int>&) const;

	std::vector<Int>	insert(const std::vector<Int>&, size_type) const;

	std::vector<Int>	storeLeftovers(const std::vector<PmergeMe::Int>&,
	                	               size_type, size_type) const;
	size_type			maxPairingSize(size_type) const;
	std::vector<Pair>	createPairs(std::vector<Int>& sequence,
									size_type elementSize,
									size_type elementCount) const;
	void	sortPairs(std::vector<Pair>& pairs) const;

	// std::vector<Element>
	void
	insertPendElementsIntoMainChain(std::vector<Element>&,
									std::vector<PendElement>&) const;
	static bool
	compareLessThan(const Element&, const Element&);
	size_type	jacobsthal(size_type n) const;

	std::vector<Int>
	flatten(const std::vector<PmergeMe::Pair>&) const;

	std::vector<PmergeMe::Int>
	flatten(const std::vector<PmergeMe::Element>&) const;
	// debug utils
	void
	printMainChain(const std::vector<Element>&, const char* = "main") const;
	void
	printPend(const std::vector<PendElement>&) const;
};

std::ostream&	operator<<(std::ostream&, const PmergeMe::Int&);
