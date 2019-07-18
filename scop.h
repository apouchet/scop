/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:14:17 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/16 08:56:34 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCOP_H
# define SCOP_H

# ifndef GL_SILENCE_DEPRECATION
# define GL_SILENCE_DEPRECATION
# endif

# define STB_IMAGE_IMPLEMENTATION
# define WINDOWX 800
# define WINDOWY 800


# include <OpenGL/gl3.h>
# include <SDL2/SDL.h>

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>

// # include "stb_image.h"
# include "libft/libft.h"

# define PI 3.14159265359
# define X 0
# define Y 1
# define Z 2

// # include "obj/parsing.c"

typedef struct		s_obj
{
	// int				nbVertex;
	// int				nbNormal;
	// int				nbTexture;
	size_t			faceTri;
	size_t			faceQuad;
	char			*fileName;
	char			*texture;
	float			*v;
	float			*vt;
	float			*vn;
	float			*tabVertexQuad;
	size_t			tabVQuad;
	float			*tabNormalQuad;
	size_t			tabNQuad;
	float			*tabTextureQuad;
	size_t			tabTQuad;
	float			*tabVertexTri;
	size_t			tabVTri;
	float			*tabNormalTri;
	size_t			tabNTri;
	float			*tabTextureTri;
	size_t			tabTTri;
	char			*path;
	float			zoom;
}					t_obj;

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

typedef struct		s_gl
{	
	GLuint			vertexID;
	GLuint			fragmentID;
	GLuint			programID;
	GLint			attributeId;
	GLint			logSize;
	GLchar*			log;
	unsigned int	VAO_tri;
	unsigned int	VBO_tri[3];
	unsigned int	VAO_quad;
	unsigned int	VBO_quad[3];
}					t_gl;

typedef struct	s_shd
{
	GLchar*		vertexSource;
	GLchar*		fragmentSource;
	GLint		programState;
	GLint		vertexSize;
	GLint		fragmentSize;
}				t_shd;

typedef struct	s_matrix
{
	float		base[4][4];
	float		rotate[4][4];
	float		move[4][4];
	float		pers[4][4];


	float		top;
	float		bottom;
	float		left;
	float		right;
	float		fov;
	float		near;
	float		far;
	float		ratio;
}				t_matrix;

typedef struct	s_control
{
	int			key;
	int			end;
	float		moveX;
	float		moveY;
	float		moveZ;
	float		rotX;
	float		rotY;
	float		rotZ;
	int			step;
}				t_control;

char					*ft_get_file(char *name, char *file);

int						ft_size_file(char *name);

unsigned char			*ft_read_tga_headers(char *name, t_tga *tga);

int						ft_shaders(char *nameVS, char *nameFS, t_gl *gl);

int						ft_delete_shader(t_gl *gl);

void 					ft_parsing(t_obj *obj, char *name);

void					gl_perspective(t_matrix *mx);

int						ft_gl_error(char *msg, char *where, GLuint ID, t_gl *gl);

int						ft_start_sdl_opengl(t_sdl *sdl);

void					ft_trans(t_matrix *mx, float x, float y, float z);

void					ft_rotate(t_matrix *mx, double angleX, double angleY, double angleZ);

void					ft_perspective(t_matrix *mx);

void					ft_matrix(GLuint programID, t_control *ctrl, t_matrix *mx);

size_t					ft_atost(const char *s);

void					ft_glBuffer(t_obj *obj, t_gl *gl);

int						ft_size_file_pars(t_obj *obj, size_t *nbVertex, size_t *nbTexture, size_t *nbNormal);

char 					*ft_get_path(char *s);

int						ft_check_extention(char *s, char *ex);

void					ft_center(t_obj *obj, size_t *posVertex);

int						ft_get_nb_value(char *s);

void					ft_get_value(float **tab, int nb, char *line, size_t *pos);

void					ft_get_point(t_obj *obj, char *line, int type, int *i);

void					ft_get_face(t_obj *obj, char *line);

void					ft_get_data(t_obj *obj, size_t *posVertex, size_t posTexture, size_t posNormal);

void					ft_exit_pars(int type, char *msg, int l, char *line);

void					ft_other_command(t_obj *obj, char *line, size_t l);

int						ft_check_value(char *s, int max);

void					ft_fill(t_obj *obj, size_t a, size_t size, int type);

void					ft_exit_gl_sdl(t_gl *gl, t_sdl *sdl);

#endif
