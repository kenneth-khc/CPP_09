# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kecheong <kecheong@student.42kl.edu.my>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/12 20:48:22 by kecheong          #+#    #+#              #
#    Updated: 2025/01/12 21:06:15 by kecheong         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := btc

CXX := c++
CXXFLAGS := -Wall -Werror -Wextra -std=c++98 -pedantic

srcs := main.cpp
objs := $(addprefix obj/, $(srcs:.cpp=.o))

all: $(NAME)

$(NAME): $(objs)
	$(CXX) $(CXXFLAGS) $(objs) -o $(NAME)

obj/%.o: %.cpp | obj
	$(CXX) $(CXXFLAGS) $< -c -o $@

obj:
	@mkdir -p obj/

clean:
	$(RM) -r obj/

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: CXXFLAGS += -g3
debug: $(NAME)

fsan: CXXFLAGS += -fsanitize=address -g3
fsan: $(NAME)

.PHONY: all clean fclean re debug fsan
