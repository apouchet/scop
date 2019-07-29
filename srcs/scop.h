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
#  define GL_SILENCE_DEPRECATION
# endif

# include <OpenGL/gl3.h>
# include <SDL2/SDL.h>
# include <stdlib.h>
# include <fcntl.h>
# include <unistd.h>
# include "../libft/libft.h"

# define WINDOWY 800
# define WINDOWX 800
# define PI 3.14159265359
# define X 0
# define Y 1
# define Z 2

typedef struct		s_obj
{
	size_t			f_tri;
	size_t			f_quad;
	char			*file_name;
	char			*texture;
	float			*v;
	float			*vt;
	float			*vn;
	float			*tab_v_quad;
	size_t			nb_v_quad;
	float			*tab_n_quad;
	size_t			nb_n_quad;
	float			*tab_t_quad;
	size_t			nb_t_quad;
	float			*tab_v_tri;
	size_t			nb_v_tri;
	float			*tab_n_tri;
	size_t			nb_n_tri;
	float			*tab_t_tri;
	size_t			nb_t_tri;
	char			*path;
	float			zoom;
}					t_obj;

typedef struct		s_tga
{
	char			id_length;
	char			colour_map;
	char			data_type;
	short int		colour_origin;
	short int		colour_length;
	char			colour_depth;
	short int		x_origin;
	short int		y_origin;
	short int		width;
	short int		height;
	char			bpp;
	char			imagedescriptor;
}					t_tga;

typedef struct		s_sdl
{
	SDL_Window		*fenetre;
	SDL_GLContext	contexte_opengl;
	SDL_Event		evenements;
}					t_sdl;

typedef struct		s_gl
{
	GLuint			vertex_id;
	GLuint			fragment_id;
	GLuint			program_id;
	GLint			attribute_id;
	GLint			log_size;
	GLchar			*log;
	unsigned int	vao_tri;
	unsigned int	vbo_tri[3];
	unsigned int	vao_quad;
	unsigned int	vbo_quad[3];
}					t_gl;

typedef struct		s_shd
{
	GLchar			*vertex_s;
	GLchar			*fragment_s;
	GLint			program_state;
	GLint			vertex_size;
	GLint			fragment_size;
}					t_shd;

typedef struct		s_matrix
{
	float			base[4][4];
	float			rotate[4][4];
	float			r[4][4][4];
	float			move[4][4];
	float			pers[4][4];
	float			top;
	float			bottom;
	float			left;
	float			right;
	float			fov;
	float			near;
	float			far;
	float			ratio;
}					t_matrix;

typedef struct		s_control
{
	int				key;
	int				end;
	int				step;
	float			move_x;
	float			move_y;
	float			move_z;
	float			rot_x;
	float			rot_y;
	float			rot_z;
	float			move;
}					t_control;

void				ft_control(t_control *ctrl, SDL_Event e, float zoom);

char				*ft_get_file(char *name, char *file);

int					ft_size_file(char *name);

unsigned char		*ft_read_tga_headers(char *name, t_tga *tga);

int					ft_shaders(char *name_vs, char *name_fs, t_gl *gl);

int					ft_delete_shader(t_gl *gl);

void				ft_parsing(t_obj *obj, char *name);

void				gl_perspective(t_matrix *mx);

int					ft_gl_error(char *msg, char *where, GLuint id, t_gl *gl);

int					ft_start_sdl_opengl(t_sdl *sdl);

void				ft_trans(t_matrix *mx, float x, float y, float z);

void				ft_rotate(t_matrix *mx, double angle_x, double angle_y,
	double angle_z);

void				ft_perspective(t_matrix *mx);

void				ft_matrix(GLuint program_id, t_control *ctrl, t_matrix *mx);

size_t				ft_atost(const char *s);

void				ft_glbuffer(t_obj *obj, t_gl *gl);

int					ft_size_file_pars(t_obj *obj, size_t *nb_vertex,
	size_t *nb_texture, size_t *nb_normal);

char				*ft_get_path(char *s);

int					ft_check_extention(char *s, char *ex);

void				ft_center(t_obj *obj, size_t *pos_vertex);

int					ft_get_nb_value(char *s);

void				ft_get_value(float **tab, int nb, char *line, size_t *pos);

void				ft_get_face(t_obj *obj, char *line);

void				ft_get_data(t_obj *obj, size_t *pos_vertex,
	size_t pos_texture, size_t pos_normal);

void				ft_exit_pars(int type, char *msg, int l, char *line);

void				ft_other_command(t_obj *obj, char *line, size_t l);

void				ft_check_value(char *s, int max);

void				ft_fill(t_obj *obj, size_t a, size_t size, int type);

void				ft_exit_gl_sdl(t_gl *gl, t_sdl *sdl);

void				ft_read_texture(GLuint program_id, char *texture);

#endif
