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

void	ft_control(t_control *ctrl, SDL_Event evenements, float zoom)
{
	ctrl->key = 0;
	if (SDL_WaitEventTimeout(&evenements, 1))
	{
		ctrl->key = evenements.window.event;
		if (ctrl->key == SDL_WINDOWEVENT_CLOSE || ctrl->key == ' ')
			ctrl->end = 1;
		else if (ctrl->key == 'f')
			ctrl->rot_y -= PI / ctrl->step;
		else if (ctrl->key == 'h')
			ctrl->rot_y += PI / ctrl->step;
		else if (ctrl->key == 'g')
			ctrl->rot_x -= PI / ctrl->step;
		else if (ctrl->key == 't')
			ctrl->rot_x += PI / ctrl->step;
		else if (ctrl->key == 'y')
			ctrl->rot_z -= PI / ctrl->step;
		else if (ctrl->key == 'r')
			ctrl->rot_z += PI / ctrl->step;
		else if (ctrl->key == 'w')
			ctrl->move_y += 0.1f;
		else if (ctrl->key == 's')
			ctrl->move_y -= 0.1f;
		else if (ctrl->key == 'd')
			ctrl->move_x += 0.1f;
		else if (ctrl->key == 'a')
			ctrl->move_x -= 0.1f;
		else if (ctrl->key == 'z')
			ctrl->move_z += 0.1f;
		else if (ctrl->key == 'x')
			ctrl->move_z -= 0.1f;

		else if (ctrl->key == 'p')
		{
			ctrl->move_z = zoom;
		}
		else if (ctrl->key == 'o')
		{
			ctrl->rot_x = 0;
			ctrl->rot_y = 0;
			ctrl->rot_z = 0;
		}
	}
}



void	ft_read_texture(GLuint program_id, char *texture)
{
	t_tga			tga;
	unsigned int	gl_texture;
	unsigned char	*data;

	bzero(&tga, sizeof(t_tga));
	gl_texture = glGetUniformLocation(program_id, "Texture");
	glGenTextures(1, &gl_texture);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if ((data = ft_read_tga_headers((texture ? texture : "img/face.tga"), &tga)))
	{
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.width, tga.height, 0, ((tga.bpp == 4) ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
	    glGenerateMipmap(GL_TEXTURE_2D);
		free(data);
	}
	else
	{
		ft_printf("Failed to load texture\n");
		exit(0);
	}
}

void	ft_main_loop(t_sdl *sdl, t_gl *gl, t_obj *obj)
{
	t_matrix	mx;
	t_control	ctrl;
	size_t		i;

	bzero(&mx, sizeof(mx));
	bzero(&ctrl, sizeof(t_control));
	ctrl.step = 20;
	ctrl.move_z = obj->zoom;
	while(!ctrl.end)
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
			exit (-1);
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
	ft_printf("OpenGL version : %s\n", glGetString(GL_VERSION));
	ft_printf("Shader version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	ft_parsing(&obj, ft_find_arg(argc, argv, &i));
	// while (1);
	if (i == 0)
		ft_face_color(&obj);
	ft_glbuffer(&obj, &gl);
	ft_read_texture(gl.program_id, obj.texture);
	glUseProgram(gl.program_id);
	ft_main_loop(&sdl, &gl, &obj);
	return (0);
}
