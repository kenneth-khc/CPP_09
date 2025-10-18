/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 20:51:05 by kecheong          #+#    #+#             */
/*   Updated: 2025/10/18 17:11:36 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>
#include <cstdlib>

int	main(int argc, char **argv)
{
	const Database	exchangeRates = parseCsvFile();
	if (argc < 2)
	{
		std::cerr << "Error: missing file to evaluate, usage: ./btc <file>\n";
		std::exit(1);
	}
	else
	{
		evaluate(exchangeRates, argv[1]);
	}
}
