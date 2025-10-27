/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 21:00:11 by kecheong          #+#    #+#             */
/*   Updated: 2025/10/18 17:11:22 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <stdexcept>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const char* filename)
{
	parseCsvFile(filename);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other):
exchangeRates(other.exchangeRates) {}

BitcoinExchange::~BitcoinExchange() {}

typedef std::pair<std::string, float>	StrFloatPair;
typedef std::string::size_type			size_type;

/**
 Takes in the name of a CSV file with expected records of "date,exchange_rate"
 and parses each entry into a std::map for quick lookup, to be used as a
 Database of exchange rates */
void	BitcoinExchange::parseCsvFile(const char* filename)
{
	std::ifstream	csvFile(filename);
	if (!csvFile.is_open())
	{
		logAndExit("Error: could not open CSV file `" + std::string(filename) + "`");
	}

	std::string	line;
	std::getline(csvFile, line);
	if (line != "date,exchange_rate")
	{
		logAndExit("Error: invalid CSV format. expected header \"date,exchange_rate\"");
	}

	while (std::getline(csvFile, line))
	{
		size_type	commaPos = line.find(',');
		if (commaPos == std::string::npos)
		{
			logAndExit("Error: could not find delimiter `,` in record");
		}
		std::string	date = line.substr(0, commaPos);
		std::string	exchangeRateStr = line.substr(commaPos+1);
		float		exchangeRate = 0.0f;
		try
		{
			parseDate(date);
			exchangeRate = parseExchangeRate(exchangeRateStr);
		}
		catch (const std::invalid_argument& e)
		{
			const std::string	parseErr = e.what();
			logAndExit("Error while parsing the CSV file: " + parseErr);
		}

		StrFloatPair	pair(date, exchangeRate);
		exchangeRates.insert(pair);
	}
	if (exchangeRates.size() == 0)
	{
		logAndExit("Error: CSV file has no record");
	}
}

/**
 Takes in a filename of a file to evaluate, looking up each record's date
 from within the database of exchange rates, and printing out the total value.
 Each record's date and value is parsed with whitespaces trimmed to ensure it
 fits within our constraints.
 If a record is invalid or is badly formatted, it is simply skipped with a
 message printed stating the error. */
void	BitcoinExchange::evaluate(const char* inputFile) const
{
	if (exchangeRates.empty())
	{
		logAndExit("Error: no database loaded, cannot evaluate rates");
	}

	std::ifstream	infile(inputFile);
	if (!infile.is_open())
	{
		logAndExit("Error: could not open file.");
	}
	validateHeader(infile);

	std::string	line;
	while (std::getline(infile, line))
	{
		if (line.empty())
		{
			continue ;
		}
		const size_type separatorPos = line.find('|');
		if (separatorPos == std::string::npos)
		{
			std::cout << "Error: bad input `" << line << "`\n";
			continue ;
		}

		float				value = 0.0f;
		const std::string	date = trim(line.substr(0, separatorPos));
		const std::string	amount = trim(line.substr(separatorPos+1));
		try
		{
			parseDate(date);
			value = parseValue(amount);
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << "Error: " << e.what() << '\n';
			continue;
		}

		float	totalValue = 0.0f;
		try
		{
			totalValue = calculateTotalValue(date, value);
			std::cout << date << " => " << value << " = " << totalValue << '\n';
		}
		catch (const std::invalid_argument& e)
		{
			std::cout << "Error: no exchange rate found for " << date
					  << ", earliest recorded date is "
					  << exchangeRates.begin()->first << '\n';
		}
	}	
}

/**
 Takes in a date string, and parses its components to ensure it is a valid date.
 Dates are expected to be in the format YYYY-MM-DD, where
 - YYYY = 2001, 2002, 2010, 2020, etc...
 -   MM = 01, 02, 03, ... 10, 11, 12
 -   DD = 01, 02, 03, ... 29, 30, 31

 After checking that each component is of the right format, it uses the stdlib's
 std::tm which represents a valid calendar time to check the date, rejecting
 bad dates (day 31 on months without it, day 29 on a non-leap year etc) */
void	BitcoinExchange::parseDate(const std::string& dateString) const
{
	size_type	dash1 = dateString.find('-');
	size_type	dash2 = dateString.find('-', dash1+1);
	if (dash1 == std::string::npos || dash2 == std::string::npos)
	{
		throw std::invalid_argument("bad date input `" + dateString + "`, expected YYYY-MM-DD");
	}
	std::string	year = dateString.substr(0, dash1);
	if (year.size() != 4)
	{
		throw std::invalid_argument("bad year format `" + year + "`, expected YYYY");
	}
	std::string	month = dateString.substr(dash1+1, dash2-dash1-1);
	if (month.size() != 2)
	{
		throw std::invalid_argument("bad month format `" + month + "`, expected MM");
	}
	std::string	day = dateString.substr(dash2+1);
	if (day.size() != 2)
	{
		throw std::invalid_argument("bad day format `" + day + "`, expected DD");
	}

	char				dash;
	std::tm				date = {}; // calendar time
	std::stringstream	ss(dateString);

	ss >> date.tm_year  >> dash >> date.tm_mon >> dash >> date.tm_mday;
	date.tm_year -= 1900;
	date.tm_mon -= 1;

	std::tm		newDate = date;
	std::time_t	time = std::mktime(&newDate); // calendar time to unix epoch
	if (time == -1 ||
		newDate.tm_year != date.tm_year ||
		newDate.tm_mon != date.tm_mon ||
		newDate.tm_mday != date.tm_mday)
	{
		throw std::invalid_argument("bad date input `" + dateString + "`");
	}
}

/** Parses a string into an exchange rate, where it must be a valid float */
float	BitcoinExchange::parseExchangeRate(const std::string& exchangeRate) const
{
	const char*	str = exchangeRate.c_str();
	const char*	end = str + exchangeRate.size();
	char*		parseEnd = NULL;

	errno = 0;
	float	value = std::strtof(str, &parseEnd);
	if (parseEnd != end || errno == ERANGE)
	{
		throw std::invalid_argument("bad exchange rate `" + exchangeRate + "`");
	}
	return value;
}

/**
 Parses the first line of a file to ensure it has a header of "date | value".
 Find the left and right substrings by extracting them based on the position
 of the `|`, then trim leading and trailing whitespaces on them to be more
 user friendly. */
void	BitcoinExchange::validateHeader(std::ifstream& infile) const
{
	std::string	line;

	std::getline(infile, line);
	size_type	separatorPos = line.find("|");

	if (separatorPos == std::string::npos)
	{
		logAndExit("Error: unexpected header format `"
		           + line + "`, expected \"date | value\"");
	}
	std::string	word1 = trim(line.substr(0, separatorPos));
	if (word1 != "date")
	{
		logAndExit("Error: bad header, unexpected `" + word1 + "`, expected `date`");
	}
	std::string	word2 = trim(line.substr(separatorPos+1));
	if (word2 != "value")
	{
		logAndExit("Error: bad header, unexpected `" + word2 + "`, expected `value`");
	}
}
/**
 Takes in a string and parses it into a valid amount of bitcoins.
 Values can only be positive and within the range of 0 to 1000. */
float BitcoinExchange::parseValue(const std::string& valueString) const
{
	const char*	str = valueString.data();
	const char*	strEnd = str + valueString.size();
	char*		parseEnd;
	const float value = std::strtof(str, &parseEnd);

	if (parseEnd != strEnd)
	{
		throw std::invalid_argument("invalid number `" + valueString + "`");
	}
	if (value < 0)
	{
		throw std::invalid_argument("not a positive number, expected 0-1000, got `" +
		                            valueString + "` instead");
	}
	if (value > 1000)
	{
		throw std::invalid_argument("too large a number, expected 0-1000, got `" +
		                            valueString + "` instead");
	}
	return value;
}

/**
 Finds the exchange rate on a given date by looking it up within the database,
 and calculates the total value by multiplying with the given amount.
 If the date does not exist, use the closest date in the past. */
float	BitcoinExchange::calculateTotalValue(const std::string& date,
											 float amount) const
{
	float						exchangeRate = 0.0f;
	Database::const_iterator	foundIt = exchangeRates.lower_bound(date);
	if (foundIt == exchangeRates.end())
	{
		--foundIt;
	}
	else if (foundIt->first != date)
	{
		if (foundIt == exchangeRates.begin())
		{
			throw std::invalid_argument("no recorded date");
		}
		else
		{
			--foundIt;
		}
	}
	exchangeRate = foundIt->second;
	return exchangeRate * amount;
}

std::string	BitcoinExchange::leftTrim(const std::string& str) const
{
	std::string	trimmed;
	size_type	i = 0;

	while (std::isspace(str[i]))
	{
		++i;
	}
	while (i < str.size())
	{
		trimmed += str[i];
		++i;
	}
	return trimmed;
}

std::string	BitcoinExchange::rightTrim(const std::string& str) const
{
	if (str.empty())
	{
		return str;
	}

	std::string	trimmed;
	size_type	i = str.size() - 1;

	while (std::isspace(str[i]))
	{
		--i;
	}
	for (size_type j = 0; j <= i; ++j)
	{
		trimmed += str[j];
	}
	return trimmed;
}

/** Trims leading and trailing whitespaces on a string */
std::string	BitcoinExchange::trim(const std::string& str) const
{
	std::string	leftTrimmed = leftTrim(str);
	std::string rightTrimmed = rightTrim(leftTrimmed);
	return rightTrimmed;
}

/** Prints an error message to stderr and exits with status code 1 */
void	BitcoinExchange::logAndExit(const std::string& message) const
{
	std::cerr << message << std::endl;
	std::exit(1);
}
