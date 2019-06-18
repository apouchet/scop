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

# CFLAGS = -Wall -Werror -Wextra

NAME = scop

INCL = -I ~/.brew/include

# LIB = -L ~/.brew/lib -lSDL2 
# LIB = -L /usr/local/Cellar/
LIB = $(shell sdl2-config --libs)

LIBFT = ./libft/libft.a

FRAM = -framework OpenGL -framework Cocoa

SRC = ./parasing.c ./main.c ./ft_read_tga.c ./ft_read_shader.c ./ft_file.c 

OBJ = $(SRC:.c=.o)

all: $(NAME) 

$(NAME) : $(OBJ)
	gcc $(LIB) $(LIBFT) $(INCL) $(FRAM) $(OBJ) -o $(NAME)

%.o: %.c
	@$(CC) -o $@ -c $< $(INCL) -Wno-deprecated-declarations

clean :
	rm -f $(OBJ)

re : fclean all

fclean : clean
	rm -f $(NAME)


# -I ~/.brew/include -L ~/.brew/lib -lSDL2 -framework OpenGL -framework Cocoa