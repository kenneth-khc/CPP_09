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

/**
 Parses a CSV file of Bitcoin exchange rates on a given date into a database. */
Database	parseCsvFile(const char* = "data.csv");

/**
 Evaluates a text file with records of Bitcoin amounts on a given date, looking
 up the exchange rate on the given date and calculating the total value */
void		evaluate(const Database&, const char* = "input.txt");

#endif
