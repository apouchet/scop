# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: apouchet <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2016/11/07 05:07:37 by apouchet          #+#    #+#              #
#    Updated: 2019/03/03 15:19:38 by apouchet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: all, $(NAME), clean, re, fclean

CFLAGS =  -Wall -Werror -Wextra

CC = gcc

NAME = scop

INCL = -I ~/.brew/include

LIB = $(shell sdl2-config --libs)

LIBFT = ./libft/libft.a

LIB_SRC = ./libft/

FRAM = -framework OpenGL -framework Cocoa

SRC = ./srcs/ft_parsing.c ./srcs/main.c ./srcs/ft_read_tga.c \
	./srcs/ft_read_shader.c ./srcs/ft_file.c ./srcs/gl_sdl_function.c \
	./srcs/ft_matrix.c ./srcs/ft_zoom.c ./srcs/ft_get_data.c \
	./srcs/ft_parsing_other.c ./srcs/ft_control.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJ)
	make -C $(LIB_SRC)
	gcc $(CFLAGS) $(LIB) $(LIBFT) $(INCL) $(FRAM) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCL) -Wno-deprecated-declarations

clean :
	@echo "Clean obj scop"
	@rm -f $(OBJ)
	@make clean -C $(LIB_SRC)

re : fclean all

fclean : clean
	rm -f $(NAME)
	rm -f $(LIBFT)
