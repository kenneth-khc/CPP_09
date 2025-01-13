/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 20:51:05 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/13 08:06:24 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <string>
#include <cassert>
#include <algorithm>
#include <limits>
#include <cmath>
#include <ctime>
#include "BitcoinExchange.hpp"

template<typename Type>
void	println(const Type& t)
{
	std::cout << t << '\n';
}

int	stringToInt(const std::string& numStr)
{
	std::string::difference_type	n = std::count(numStr.begin(), numStr.end(),
												   '.');

	std::cout << "> " << n << '\n';
	return 0;
}

/* C++11: type alias
 * see: https://en.cppreference.com/w/cpp/language/type_alias
 * using Database = std::map<std::string,double> */
typedef std::map<std::string,double>	Database;

Database	parseDatabase()
{
	std::ifstream	csvFile("data.csv");
	if (!csvFile.is_open())
	{
		println("Error: could not open CSV file");
		std::exit(1);
	}
	std::string	buffer;
	std::getline(csvFile, buffer);
	if (buffer != "date,exchange_rate")
	{
		println("Error: invalid CSV format. expected format \"date,exchange_rate\"");
		std::exit(1);
	}
	Database	exchangeRates;
	while (std::getline(csvFile, buffer))
	{
		std::string::size_type	n = buffer.find(',');
		std::string				dateString = buffer.substr(0, n);
		std::string				numString = buffer.substr(n+1, buffer.size());
		float					num = std::strtof(numString.data(), NULL);
		exchangeRates.insert(std::pair<std::string,double>(dateString, num));
	}
	return exchangeRates;
}

double	validateValue(const std::string& buffer, std::string::size_type n)
{
	const std::string	numString = buffer.substr(n+2, buffer.size());
	const char*			str = numString.data();
	const char*			strEnd = str + numString.size();
	char*				checkEnd;
	const double		value = std::strtod(str, &checkEnd);
	if (checkEnd != strEnd)
	{
		std::cout << "Error: invalid number " << numString << '\n';
		return std::numeric_limits<double>::quiet_NaN();
	}
	if (value < 0)
	{
		std::cout << "Error: not a positive number. expected 0 - 1000\n";
		return std::numeric_limits<double>::quiet_NaN();
	}
	if (value > 1000)
	{
		std::cout << "Error: too large a number. expected 0 - 1000\n";
		return std::numeric_limits<double>::quiet_NaN();
	}
	return value;
}

template<typename NumberType>
NumberType	toNum(const std::string& str)
{
	NumberType	num;
	std::istringstream(str) >> num;
	return num;
}

/* Converts a dateString into a tm struct that holds components of a date 
 * eg. "2011-01-02" -> tm_year=2011, tm_mon=00, tm_day=02
 * the timestruct could be "off", for eg. "2012-13-32" would be a valid tm
 * but it would get normalized by mktime() into year=2013, mon=01, day=01
 * see https://en.cppreference.com/w/cpp/chrono/c/tm
 *
 * a valid dateString for this function is in the format "YY-MM-DD"
 * throws exceptions if the seperators are missing */
std::tm	dateStringToCalendarDate(const std::string& dateString)
{
	std::string	year;
	std::string	month;
	std::string	day;

	std::string::size_type	yearMonthDelim = dateString.find('-', 0);
	if (yearMonthDelim == dateString.npos)
	{
		throw std::invalid_argument("Error: missing year-month seperator in "
								+ dateString + '\n');
	}
	year = dateString.substr(0, yearMonthDelim);
	std::string::size_type	monthDayDelim = dateString.find('-', yearMonthDelim+1);
	if (monthDayDelim == dateString.npos)
	{
		throw std::invalid_argument("Error: missing month-day seperator in "
								+ dateString + '\n');
	}
	month = dateString.substr(yearMonthDelim+1, dateString.size()-1 - monthDayDelim);
	day = dateString.substr(monthDayDelim+1);
	std::tm	calendarTime = std::tm();
	calendarTime.tm_year = toNum<int>(year);
	calendarTime.tm_mon = toNum<int>(month)-1;
	calendarTime.tm_mday = toNum<int>(day);
	return calendarTime;
}

/* Converts a dateString into a time_t (time in seconds since Epoch)
 * conversion is done through dateString -> struct tm -> time_t
 * gonna assume the dateString is valid because I only call it on the
 * database where every date is already validated, and I'm lazy */
std::time_t	dateStringToTime(const std::string& dateString)
{
	std::tm	calendarTime = dateStringToCalendarDate(dateString);
	return std::mktime(&calendarTime);
}

/* Validate a dateString by first converting it into calendar time (struct tm)
 * make another copy of the calendar time, then call mktime() on one of them
 * which would normalize struct tm if the values were off
 * by comparing the copy and original, if any of the fields are not equal
 * it means the dateString was invalid
 * we need the date converted to time_t so pass that back through a ref */
bool	validateDate(const std::string& dateString, std::time_t& time)
{
	std::tm	calendarTime = std::tm();
	try
	{
		calendarTime = dateStringToCalendarDate(dateString);
	}
	catch (std::invalid_argument& e)
	{
		std::cout << e.what();
		return false;
	}
	std::tm	copy(calendarTime);
	time = std::mktime(&copy);
	if (time < 0 ||
		calendarTime.tm_year != copy.tm_year ||
		calendarTime.tm_mon != copy.tm_mon ||
		calendarTime.tm_mday != copy.tm_mday)
	{
		std::cout << "Error: invalid date " << dateString << '\n';
		return false;
	}
	return true;
}

int	main(int argc, char **argv)
{
	std::ifstream	infile;
	if (argc == 2)
	{
		infile.open(argv[1]);
	}
	if (!infile.is_open())
	{
		println("Error: could not open file.");
		std::exit(1);
	}
	std::string	buffer;
	std::getline(infile, buffer);
	if (buffer != "date | value")
	{
		println("Error: invalid format. expected format \"date | value\"");
		std::exit(1);
	}

	const Database	database = parseDatabase();

	while (std::getline(infile, buffer))
	{
		std::string	dateString;
		std::string::size_type	n = buffer.find('|');
		dateString = buffer.substr(0, n-1);
		if (n == buffer.npos)
		{
			std::cout << "Error: bad input => " << dateString
					  << ". expected format \"date | value\"" << '\n';
			continue ;
		}
		std::time_t	inputTime;
		if (validateDate(dateString, inputTime) == false)
			continue;
		double	value = validateValue(buffer, n);
		if (std::isnan(value))
		{
			continue ;
		}
		Database::const_iterator	it = database.find(dateString);
		float	exchangeRate;
		if (it != database.end())
		{
			exchangeRate = it->second;
		}
		else
		{
			for (it = database.begin(); it != database.end(); ++it)
			{
				std::time_t	databaseDateTime = dateStringToTime(it->first);
				if (databaseDateTime < inputTime)
				{
					exchangeRate = it->second;
				}
				// found the closest time, stop searching
				else break;
			}
		}
		float	result = value * exchangeRate;
		std::cout << dateString << " => " << value << " = " << result << '\n';
	}
}

