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

#include <algorithm>
#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

typedef std::vector<int>::size_type	size_type;

int	validatePositiveInteger(const char* str);

class PmergeMe
{
public:
	PmergeMe();
	PmergeMe(const PmergeMe&);
	~PmergeMe();

	PmergeMe	operator=(const PmergeMe&);

	void	parse(char** args, size_type count);
	void	fordJohnsonMergeInsertionSort();

	static void	logAndResetComparisons();

	struct Int
	{
		static int	comparisons;
		static int	amount;

		int	value;
		Int(int value);
		Int(const Int&);

		// TODO(kecheong): assignment operator
		bool	operator<(const Int&) const;
		bool	operator>(const Int&) const;
		bool	operator==(const Int&) const;

	};

	typedef std::vector<Int>						Element;
	typedef std::pair<Element, Element>				Pair;
	typedef std::vector<Element>::const_iterator	Bound;
	typedef std::pair<Element, Bound>				PendElement;
	typedef std::pair<Element, int>					BoundedPendEle;

private:

	std::vector<Int>	vec;
};

std::ostream&	operator<<(std::ostream&, const PmergeMe::Int&);
