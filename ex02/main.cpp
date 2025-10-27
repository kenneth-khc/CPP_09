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

#include "PmergeMe.hpp"
#include <iostream>
#include <deque>
#include <vector>

void	printBefore(int count, char** args)
{
	std::cout << "Before: ";
	for (int i = 0; i < count; ++i)
	{
		std::cout << args[i];
		if (i != count - 1)
		{
			std::cout << " ";
		}
	}
	std::cout << '\n';
}

template <typename T>
void	printAfter(const T& container)
{
	std::cout << "After : ";
	typename T::const_iterator	iter = container.begin();
	typename T::const_iterator	end = container.end();
	for (; iter != end; ++iter)
	{
		std::cout << *iter;
		if (iter != end - 1)
		{
			std::cout << " ";
		}
	}
	std::cout << '\n';
}

int	main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "Error: expected list of positive numbers\n";
		return 1;
	}

	PmergeMe<std::vector>	vecSorter(argc - 1, argv + 1);
	PmergeMe<std::deque>	deqSorter(argc - 1, argv + 1);

	printBefore(argc - 1, argv + 1);

	std::vector<int> sortedVec = vecSorter.benchmark(&PmergeMe<std::vector>::mergeInsertionSort);
	std::deque<int> sortedDeq = deqSorter.benchmark(&PmergeMe<std::deque>::mergeInsertionSort);

	printAfter(sortedVec);

	vecSorter.displayTimeTaken("std::vector");
	deqSorter.displayTimeTaken("std::deque");

#ifdef TESTS
	const char* YELLOW = "\x1B[0;33m";
	const char* RESET = "\x1B[0;0m";

	std::cout << YELLOW << "Bubble sort:" << RESET << '\n';
	PmergeMe<std::vector>	bubbleSorter(argc - 1, argv + 1);
	std::vector<int> bubbleSorted = bubbleSorter.benchmark(&PmergeMe<std::vector>::bubbleSort);
	bubbleSorter.displayTimeTaken("std::vector");

	std::cout << YELLOW << "Insertion sort:" << RESET << '\n';
	PmergeMe<std::vector>	insertionSorter(argc - 1, argv + 1);
	std::vector<int> insertionSorted = insertionSorter.benchmark(&PmergeMe<std::vector>::insertionSort);
	insertionSorter.displayTimeTaken("std::vector");

#endif

	return 0;
}
