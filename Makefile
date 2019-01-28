# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apouchet <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/07 05:07:37 by apouchet          #+#    #+#              #
#    Updated: 2016/11/22 18:30:23 by apouchet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# CFLAGS = -Wall -Werror -Wextra

NAME = scop

INCL = -I ~/.brew/include

LIB = -L ~/.brew/lib -lSDL2 

FRAM = -framework OpenGL -framework Cocoa

SRC = ./main.c

# OBJ = $(SRC:.c=.o)

all: $(NAME) 

$(NAME) : 
	gcc $(LIB) $(INCL) $(FRAM) $(SRC) -o $(NAME)

clean :
	rm -f $(OBJ)

re : fclean all

fclean : clean
	rm -f $(NAME)
