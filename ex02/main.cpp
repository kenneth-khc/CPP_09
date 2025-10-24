/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 21:39:27 by kecheong          #+#    #+#             */
/*   Updated: 2025/10/19 21:39:28 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <cstdlib>
#include <ctime>
#include <iostream>
#include "PmergeMe.hpp"
#include <time.h>

#include <cmath>

void	print(PmergeMe::Int num)
{
	std::cout << num.value << "\n";
}

// template <typename T>
// void	doSort(const T& numbers, T (*sort)(const T&))
// {
// 	const char* GREEN = "\033[0;32m";
// 	const char* RED = "\033[0;31m";
// 	const char* C_RESET = "\033[0m";

// 	Int::comparisons = 0;
// 	const T	sorted = sort(numbers);
// 	typename T::const_iterator	it = std::adjacent_find(sorted.begin(), sorted.end(), std::greater<Int>());
// 	bool	isSorted = sorted.size() > 1 && it == sorted.end();
// 	if (isSorted)
// 	{
// 		std::cout << GREEN << "SORTED            " << C_RESET;
// 	}
// 	else
// 	{
// 		std::cout << RED << "UNSORTED          " << C_RESET;
// 	}
// 	Int::print(sorted);
// 	Int::printComparisons();
// 	std::cout << '\n';
// }

template <typename T, typename Ret>
Ret	benchmark(Ret (T::*sortingFunc)() const, T& obj, const char* containerType)
{
	struct timespec	execStart, execEnd;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &execStart);

	Ret	sorted = (obj.*sortingFunc)();

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &execEnd);

	const double	THOUSAND = 1000.;
	const double	MILLION = 1000000.;
	const double	BILLION = 1000000000.;

	unsigned long	startNanosec = execStart.tv_sec * BILLION + execStart.tv_nsec;
	unsigned long	endNanosec = execEnd.tv_sec * MILLION + execEnd.tv_nsec;
	unsigned long	elapsedNanosec = endNanosec - startNanosec;

	double	microseconds = elapsedNanosec / THOUSAND;
	double	milliseconds= elapsedNanosec / MILLION;
	double	seconds = elapsedNanosec / BILLION;

	size_type	size = sorted.size();
	const char*	message = size == 1 ? "element" : "elements";
	std::cout << "Time to process a range of " << size << ' ' << message
			  << " with " << containerType << " : "
			  << microseconds << " μs" << " / "
			  << milliseconds << " ms" << " / "
			  << seconds << " s" << '\n';

	return sorted;
}


int	main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "Error: expected list of positive numbers\n";
		return 1;
	}

	PmergeMe	sorter;

	sorter.parse(argv + 1, argc - 1);
	std::cout << "Before:   ";
	for (int i = 1; i < argc; ++i)
	{
		std::cout << argv[i];
		if (i != argc - 1)
		{
			std::cout << " ";
		}
	}
	std::cout << '\n';

	PmergeMe::Int::comparisons = 0;
	std::vector<PmergeMe::Int>	sorted =
		benchmark(&PmergeMe::fordJohnsonMergeInsertionSort, sorter, "std::vector");

	std::list<PmergeMe::Int>	sortedList =
		benchmark(&PmergeMe::listMergeInsertion, sorter, "std::list");

	std::vector<PmergeMe::Int>::iterator	iter = sorted.begin();
	std::vector<PmergeMe::Int>::iterator	end = sorted.end();
	std::cout << "After:    ";
	for (; iter != end; ++iter)
	{
		std::cout << *iter;
		if (iter != end - 1)
		{
			std::cout << " ";
		}
	}
	std::cout << '\n';
	return 0;
}
