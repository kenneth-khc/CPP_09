/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 20:57:21 by kecheong          #+#    #+#             */
/*   Updated: 2025/10/18 17:11:28 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>

typedef std::map<std::string, float>	Database;

class	BitcoinExchange
{
public:
	BitcoinExchange();
	BitcoinExchange(const char* filename);
	BitcoinExchange(const BitcoinExchange&);
	~BitcoinExchange();

	BitcoinExchange	operator=(const BitcoinExchange&);

	/**
	 Parses a CSV file of Bitcoin exchange rates on a given date into a database. */
	void	parseCsvFile(const char* = "data.csv");

	/** Evaluates an input file of dates and btcs and calculates the value
	    with our loaded database */
	void	evaluate(const char* inputFile = "input.txt") const;

private:
	Database	exchangeRates;

	void		parseDate(const std::string&) const;
	float		parseExchangeRate(const std::string&) const;
	void		validateHeader(std::ifstream&) const;
	float		parseValue(const std::string&) const;
	float		calculateTotalValue(const std::string&, float) const;

	std::string	trim(const std::string&) const;
	std::string	leftTrim(const std::string&) const;
	std::string	rightTrim(const std::string&) const;
	void		logAndExit(const std::string&) const;
};

#endif
