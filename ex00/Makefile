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

CXX := g++
CXXFLAGS := -Wall -Werror -Wextra -std=c++98 -pedantic -g3

srcs := main.cpp
objs := $(addprefix obj/, $(srcs:.cpp=.o))

.PHONY: all
all: $(NAME)

$(NAME): $(objs)
	$(CXX) $(CXXFLAGS) $(objs) -o $(NAME)

obj/%.o: %.cpp | obj
	$(CXX) $(CXXFLAGS) $< -c -o $@

obj:
	@mkdir -p obj/

.PHONY: clean
clean:
	$(RM) -r obj/

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

.PHONY: debug
debug: CXXFLAGS += -g3
debug: $(NAME)

.PHONY: fsan
fsan: CXXFLAGS += -fsanitize=address -g3
fsan: $(NAME)
