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
#include <iostream>
#include "PmergeMe.hpp"

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

int	main(int argc, char** argv)
{
	if (argc == 1)
	{
		std::cerr << "Error: expected list of positive numbers\n";
		return 1;
	}

	PmergeMe	sorter;

	sorter.parse(argv + 1, argc - 1);
	sorter.logAndResetComparisons();
	sorter.fordJohnsonMergeInsertionSort();
	sorter.logAndResetComparisons();

	// fordJohnsonMergeInsertion(nums);
	// doSort(nums, fordJohnsonMergeInsertion);
	// doSort(nums, Int::bubbleSort);
	// doSort(nums, Int::insertionSort);
	// doSort(nums, Int::mergeSort);

	return 1;
}
