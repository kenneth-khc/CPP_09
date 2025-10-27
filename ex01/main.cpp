/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:16:48 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/13 09:52:12 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <iostream>

int	main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: unexpected arguments\n"
				  << "Usage: ./RPM <expression>" << std::endl;
		return 1;
	}

	int result = RPN::evaluateExpression(argv[1]);
	std::cout << result << std::endl;
	return 0;
}
