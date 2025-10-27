/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:17:25 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/13 09:17:25 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>
#include <stack>
#include <list>

/****************** privated *******************/

RPN::RPN() {}

RPN::~RPN() {}

RPN::RPN(const RPN&) {}

RPN	RPN::operator=(const RPN&) { return *this; }

/***********************************************/

int	RPN::evaluateExpression(const std::string& expression)
{
	std::stack< int, std::list<int> >	operands;
	std::stringstream					errmsg;
	std::stringstream					input(expression);
	std::string							token;

	while (input >> token)
	{
		if (token.size() != 1)
		{
			errmsg << "unexpected `" << token << '`';
			logAndExit(errmsg.str());
		}
		const char&	c = token[0];
		if (std::isdigit(c))
		{
			operands.push(c - '0');
		}
		else if (isValidOperator(c))
		{
			try
			{
				int	rhs = getOperand(operands);
				int	lhs = getOperand(operands);
				int	result = calculate(lhs, rhs, c);
				operands.push(result);
			}
			catch (const std::out_of_range&)
			{
				errmsg << "no operands left for operation `" << token << '`';
				logAndExit(errmsg.str());
			}
		}
		else
		{
			errmsg << "unexpected `" << token << '`';
			logAndExit(errmsg.str());
		}
	}
	if (operands.size() != 1)
	{
		logAndExit("invalid expression, not all operands are processed");
	}
	return operands.top();
}

bool	RPN::isValidOperator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/';
}

int	RPN::getOperand(std::stack< int, std::list<int> >& operands)
{
	if (operands.empty())
	{
		throw std::out_of_range("no operands");
	}
	else
	{
		int	operand = operands.top();
		operands.pop();
		return operand;
	}
}

int	RPN::calculate(int lhs, int rhs, char operation)
{
	switch (operation)
	{
		case '+':
		{
			return lhs + rhs;
		}
		case '-':
		{
			return lhs - rhs;
		}
		case '*':
		{
			return lhs * rhs;
		}
		case '/':
		{
			if (rhs == 0) logAndExit("dividing by 0");
			return lhs / rhs;
		}
		default: throw std::invalid_argument("invalid operator");
	}
}

void	RPN::logAndExit(const std::string& message)
{
	std::cerr << "Error: " << message << std::endl;
	std::exit(1);
}
