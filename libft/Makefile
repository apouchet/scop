# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apouchet <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/08/21 15:42:31 by apouchet          #+#    #+#              #
#    Updated: 2019/02/14 18:44:05 by apouchet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all, clean, fclean, re

CC = gcc

CFLAGS = -Weverything -Wall -Werror -Wextra
# CFLAGS = -Wall -Werror -Wextra

NAME = libft.a

INC = src/include

SRC_ELSE = $(wildcard src/else/*.c)
SRC_IS = $(wildcard src/is/*.c)
SRC_MEM = $(wildcard src/mem/*.c)
SRC_PUT = $(wildcard src/put/*.c)
SRC_STR = $(wildcard src/str/*.c)
SRC_LST = $(wildcard src/lst/*.c)
SRC_PTF = $(wildcard src/ft_printf/*.c)
SRC_GNL = $(wildcard src/gnl/*.c)

OBJ_ELSE = $(SRC_ELSE:.c=.o)
OBJ_IS = $(SRC_IS:.c=.o)
OBJ_MEM = $(SRC_MEM:.c=.o)
OBJ_PUT = $(SRC_PUT:.c=.o)
OBJ_STR = $(SRC_STR:.c=.o)
OBJ_LST = $(SRC_LST:.c=.o)
OBJ_PTF = $(SRC_PTF:.c=.o)
OBJ_GNL = $(SRC_GNL:.c=.o)


all : $(NAME)

$(NAME) : $(OBJ_ELSE) $(OBJ_IS) $(OBJ_MEM) $(OBJ_PUT) $(OBJ_STR) $(OBJ_LST) $(OBJ_PTF) $(OBJ_GNL)
	@echo ""
	@ar rc $(NAME) $(OBJ_ELSE) $(OBJ_IS) $(OBJ_MEM) $(OBJ_PUT) $(OBJ_STR) $(OBJ_LST) $(OBJ_PTF) $(OBJ_GNL)
	@ranlib $(NAME)
	@echo "Creation libft.a"

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS) -I $(INC)
	@printf "\033[1;32m+\033[0m"

clean :
	@rm -rf $(OBJ_ELSE)
	@echo "Clean ft_else"
	@rm -rf $(OBJ_IS)
	@echo "Clean ft_is"
	@rm -rf $(OBJ_MEM)
	@echo "Clean ft_mem"
	@rm -rf $(OBJ_PUT)
	@echo "Clean ft_put"
	@rm -rf $(OBJ_STR)
	@echo "Clean ft_str"
	@rm -rf $(OBJ_LST)
	@echo "Clean ft_printf"
	@rm -rf $(OBJ_PTF)
	@echo "Clean get_next_line"
	@rm -rf $(OBJ_GNL)

fclean : clean
	@echo "Fclean libft"
	@rm -rf $(NAME)

re : fclean $(NAME)

