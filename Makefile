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
# LIB = -L /usr/local/Cellar/
# LIB = $(shell sdl2-config --libs)

FRAM = -framework OpenGL -framework Cocoa

SRC = ./main.c ./ft_read_tga.c ./ft_read_shader.c ./ft_file.c

OBJ = $(SRC:.c=.o)

all: $(NAME) 

$(NAME) : $(OBJ)
	gcc $(LIB) $(INCL) $(FRAM) $(OBJ) -o $(NAME)

%.o: %.c
	@$(CC) -o $@ -c $< $(INCL)

clean :
	rm -f $(OBJ)

re : fclean all

fclean : clean
	rm -f $(NAME)


# -I ~/.brew/include -L ~/.brew/lib -lSDL2 -framework OpenGL -framework Cocoa