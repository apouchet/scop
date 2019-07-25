/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gl_sdl_function.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 14:11:31 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/25 14:11:33 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		ft_exit_gl_sdl(t_gl *gl, t_sdl *sdl)
{
	glDeleteShader(gl->vertex_id);
	glDeleteShader(gl->fragment_id);
	SDL_GL_DeleteContext(sdl->contexte_opengl);
	SDL_DestroyWindow(sdl->fenetre);
	SDL_Quit();
}

int			ft_gl_error(char *msg, char *where, GLuint id, t_gl *gl)
{
	GLint	log_length;
	GLchar	*log;

	log = NULL;
	log_length = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
	if (!(log = (GLchar*)malloc(log_length)))
	{
		ft_printf("%s for %s\n", msg, where);
		if (gl)
			return (ft_delete_shader(gl));
		return (0);
	}
	glGetShaderInfoLog(id, log_length, &log_length, log);
	ft_printf("%s error :\n%s", where, log);
	if (gl)
		ft_delete_shader(gl);
	free(log);
	return (0);
}

int			ft_start_sdl_opengl(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		ft_printf("Error initialisation of SDL :  %s\n", SDL_GetError());
		SDL_Quit();
		return (-1);
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
		SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	if ((sdl->fenetre = SDL_CreateWindow("Scop", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, WINDOWX, WINDOWY,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)) == 0
		|| (sdl->contexte_opengl = SDL_GL_CreateContext(sdl->fenetre)) == 0)
	{
		if (sdl->fenetre == 0)
			ft_printf("Error creation of the window : %s\n", SDL_GetError());
		else
			ft_printf("%s\n", SDL_GetError());
		SDL_Quit();
		return (-1);
	}
	return (1);
}

static void	ft_creat_glbuffer(t_obj *obj, unsigned int vbo[3], int type)
{
	int		size;
	float	*tab[3];

	size = (type == 1 ? obj->f_quad * 4 : obj->f_tri * 3);
	tab[0] = obj->tab_v_tri;
	tab[1] = obj->tab_n_tri;
	tab[2] = obj->tab_t_tri;
	if (type == 1)
	{
		tab[0] = obj->tab_v_quad;
		tab[1] = obj->tab_n_quad;
		tab[2] = obj->tab_t_quad;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * 4, tab[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * 4, tab[1], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * 4, (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, size * 2 * 4, tab[2], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * 4, (void*)0);
	glEnableVertexAttribArray(2);
}

void		ft_glbuffer(t_obj *obj, t_gl *gl)
{
	glGenVertexArrays(1, &gl->vao_tri);
	glGenBuffers(1, &gl->vbo_tri[0]);
	glGenBuffers(1, &gl->vbo_tri[1]);
	glGenBuffers(1, &gl->vbo_tri[2]);
	glBindVertexArray(gl->vao_tri);
	ft_creat_glbuffer(obj, gl->vbo_tri, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glGenVertexArrays(1, &gl->vao_quad);
	glGenBuffers(1, &gl->vbo_quad[0]);
	glGenBuffers(1, &gl->vbo_quad[1]);
	glGenBuffers(1, &gl->vbo_quad[2]);
	glBindVertexArray(gl->vao_quad);
	ft_creat_glbuffer(obj, gl->vbo_quad, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	free(obj->tab_v_quad);
	free(obj->tab_v_tri);
}
