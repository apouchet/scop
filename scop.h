/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:14:17 by apouchet          #+#    #+#             */
/*   Updated: 2019/02/13 17:14:19 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# include <OpenGL/gl3.h>
# include <SDL2/SDL.h>

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>

typedef struct	s_tga
{
	char 		idLength;
	char		colourMap;
	char		dataType;
	short int	colourOrigin;
	short int	colourLength;
	char		colourDepth;
	short int	xOrigin;
	short int	yOrigin;
	short int	width;
	short int	height;
	char		bpp;
	char		imagedescriptor;
}				t_tga;

typedef struct	s_sdl
{
	SDL_Window*		fenetre;
	SDL_GLContext	contexteOpenGL;
	SDL_Event		evenements;


}				t_sdl;

typedef struct	s_gl
{
	GLuint		vertexID;
	GLuint		fragmentID;
	GLuint		programID;
	GLint		attributeId;
}				t_gl;

typedef struct	s_shd
{
	GLchar*		vertexSource;
	GLchar*		fragmentSource;
	GLint		programState;
	GLint		vertexSize;
	GLint		fragmentSize;
}				t_shd;

char					*ft_get_file(char *name, char *file);

int						ft_size_file(char *name);

unsigned char			*ft_read_tga_headers(char *name, t_tga *tga);

int						ft_shaders(char *nameVS, char *nameFS, t_gl *gl);

int						ft_delete_shader(t_gl *gl);


#endif
