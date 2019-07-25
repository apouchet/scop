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

CFLAGS =  -Wall -Werror -Wextra

CC = gcc

NAME = scop

INCL = -I ~/.brew/include

# LIB = -L ~/.brew/lib -lSDL2 
# LIB = -L /usr/local/Cellar/
LIB = $(shell sdl2-config --libs)

LIBFT = ./libft/libft.a

FRAM = -framework OpenGL -framework Cocoa

SRC = ./srcs/ft_parsing.c ./srcs/main.c ./srcs/ft_read_tga.c ./srcs/ft_read_shader.c ./srcs/ft_file.c ./srcs/gl_sdl_function.c ./srcs/ft_matrix.c ./srcs/ft_zoom.c ./srcs/ft_get_data.c ./srcs/ft_parsing_other.c

OBJ = $(SRC:.c=.o)

all: $(NAME) 

$(NAME) : $(OBJ)
	gcc $(CFLAGS) $(LIB) $(LIBFT) $(INCL) $(FRAM) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(INCL) -Wno-deprecated-declarations

clean :
	rm -f $(OBJ)

re : fclean all

fclean : clean
	rm -f $(NAME)


# -I ~/.brew/include -L ~/.brew/lib -lSDL2 -framework OpenGL -framework Cocoa