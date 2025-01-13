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

#include <stack>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

int	toInt(const char& c)
{
	return c - '0';
}

bool	isOperator(const char& c)
{
	return c == '+' || c == '-' || c == '*' || c == '/';
}

void	terminate(const std::string& errorMessage)
{
	std::cerr << errorMessage << '\n';
	std::exit(1);
}

int	main(int argc, char** argv)
{
	if (argc != 2)
	{
		terminate("Error. Usage: ./RPN \" \"");
	}
	std::string	argumentList(argv[1]);
	std::stack<int>	operands;
	for (std::string::const_iterator it = argumentList.begin();
		 it != argumentList.end();
		 ++it)
		{
			const char&	curr = *it;
			const char& next = *(it+1);
			if (std::isdigit(curr) && next == ' ')
			{
				operands.push(toInt(curr));
			}
			else if (isOperator(curr))
			{
				int	rhs = operands.top();
				operands.pop();
				int	lhs = operands.top();
				operands.pop();
				if (curr == '+')
					operands.push(lhs + rhs);
				else if (curr == '-')
					operands.push(lhs - rhs);
				else if (curr == '*')
					operands.push(lhs * rhs);
				else if (curr == '/')
				{
					if (rhs == 0)
					{
						terminate("Error: dividing by 0");
					}
					operands.push(lhs / rhs);
				}
			}
			else if (curr != ' ')
			{
				terminate("Error");
			}
		}
	if (operands.size() != 1)
	{
		terminate("More than 1 operand left in stack after evaluating expression");
	}
	std::cout << "Result is " << operands.top() << '\n';
}
