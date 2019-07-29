/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/27 13:29:46 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/16 12:10:10 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	ft_main_loop(t_sdl *sdl, t_gl *gl, t_obj *obj)
{
	t_matrix	mx;
	t_control	ctrl;
	size_t		i;

	bzero(&mx, sizeof(mx));
	bzero(&ctrl, sizeof(t_control));
	ctrl.step = 20;
	ctrl.move = 0.2f;
	ctrl.move_z = obj->zoom;
	while (!ctrl.end)
	{
		i = 0;
		ft_control(&ctrl, sdl->evenements, obj->zoom);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ft_matrix(gl->program_id, &ctrl, &mx);
		glActiveTexture(GL_TEXTURE2);
		glBindVertexArray(gl->vao_tri);
		glDrawArrays(GL_TRIANGLES, 0, obj->f_tri * 3);
		glBindVertexArray(gl->vao_quad);
		while (i < obj->f_quad * 4)
			glDrawArrays(GL_TRIANGLE_FAN, i += 4, 4);
		SDL_GL_SwapWindow(sdl->fenetre);
	}
	ft_exit_gl_sdl(gl, sdl);
}

void	ft_face_color(t_obj *obj)
{
	float	color;
	size_t	i;
	size_t	j;

	color = 0;
	i = 0;
	while (i < obj->f_tri * 3 * 3)
	{
		j = i;
		if (color > 1)
			color = 0;
		while (i - j < 9)
			obj->tab_n_tri[i++] = color;
		color += 0.5f;
	}
	i = 0;
	while (i < obj->f_quad * 4 * 3)
	{
		j = i;
		if (color > 1)
			color = 0;
		while (i - j < 12)
			obj->tab_n_quad[i++] = color;
		color += 0.5f;
	}
}

char	*ft_find_arg(int argc, char **argv, int *i)
{
	int j;
	int	name;

	j = 1;
	name = 0;
	while (j < argc)
	{
		if (ft_strcmp(argv[j], "-n") == 0 && *i == 0)
			*i = j;
		else if (name == 0)
			name = j;
		else
		{
			ft_printf("Invalid argument\n./scop [file.obj]\n");
			exit(-1);
		}
		j++;
	}
	if (name != 0)
		return (argv[name]);
	else
		return (NULL);
}

int		main(int argc, char **argv)
{
	t_sdl		sdl;
	t_gl		gl;
	t_obj		obj;
	int			i;

	i = 0;
	bzero(&obj, sizeof(t_obj));
	bzero(&sdl, sizeof(sdl));
	bzero(&gl, sizeof(gl));
	if (ft_start_sdl_opengl(&sdl) < 0)
		return (-1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	if (ft_shaders("Shaders/shader.vs", "Shaders/shader.fs", &gl) < 0)
		return (-1);
	ft_printf("OpenGL version: %s\n", glGetString(GL_VERSION));
	ft_printf("Shader version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	ft_parsing(&obj, ft_find_arg(argc, argv, &i));
	if (i == 0)
		ft_face_color(&obj);
	ft_glbuffer(&obj, &gl);
	ft_read_texture(gl.program_id, obj.texture);
	glUseProgram(gl.program_id);
	ft_main_loop(&sdl, &gl, &obj);
	return (0);
}
