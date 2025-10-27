/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:17:21 by kecheong          #+#    #+#             */
/*   Updated: 2025/01/13 09:17:22 by kecheong         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <stack>
#include <list>

class	RPN
{
public:
	static int	evaluateExpression(const std::string&);

private:
	RPN();
	~RPN();
	RPN(const RPN&);
	RPN	operator=(const RPN&);

	static bool	isValidOperator(char);
	static int	getOperand(std::stack< int, std::list<int> >&);
	static int	calculate(int lhs, int rhs, char operation);
	static void	logAndExit(const std::string&);
};

