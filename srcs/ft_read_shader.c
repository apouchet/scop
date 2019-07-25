/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_shader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:16:33 by apouchet          #+#    #+#             */
/*   Updated: 2019/02/13 17:16:34 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int				ft_delete_shader(t_gl *gl)
{
	glUseProgram(0);
	glDetachShader(gl->program_id, gl->fragment_id);
	glDetachShader(gl->program_id, gl->vertex_id);
	glDeleteProgram(gl->program_id);
	glDeleteShader(gl->fragment_id);
	glDeleteShader(gl->vertex_id);
	return (-1);
}

static int		ft_check_shader_compilation(GLuint shader_id, t_gl *gl)
{
	GLint compilation_status;

	compilation_status = 0;
	glGetShaderiv(gl->vertex_id, GL_COMPILE_STATUS, &compilation_status);
	if (compilation_status != GL_TRUE)
	{
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &gl->log_size);
		if (!(gl->log = (GLchar*)malloc(gl->log_size)))
		{
			ft_printf("Error malloc for log of shader compilation\n");
			return (0);
		}
		glGetShaderInfoLog(shader_id, gl->log_size, &gl->log_size, gl->log);
		ft_printf("Compilation error :\n%s", gl->log);
		free(gl->log);
		return (0);
	}
	return (1);
}

int				ft_shader_start(t_gl *gl, t_shd *shd)
{
	shd->vertex_size = strlen(shd->vertex_s);
	shd->fragment_size = strlen(shd->fragment_s);
	glShaderSource(gl->vertex_id, 1, (const GLchar**)(&shd->vertex_s),
		&shd->vertex_size);
	glShaderSource(gl->fragment_id, 1, (const GLchar**)(&shd->fragment_s),
		&shd->fragment_size);
	glCompileShader(gl->vertex_id);
	glCompileShader(gl->fragment_id);
	if (!ft_check_shader_compilation(gl->vertex_id, gl)
		|| !ft_check_shader_compilation(gl->fragment_id, gl))
		return (ft_delete_shader(gl));
	gl->program_id = glCreateProgram();
	glAttachShader(gl->program_id, gl->vertex_id);
	glAttachShader(gl->program_id, gl->fragment_id);
	glLinkProgram(gl->program_id);
	glGetProgramiv(gl->program_id, GL_LINK_STATUS, &shd->program_state);
	free(shd->vertex_s);
	free(shd->fragment_s);
	return (1);
}

int				ft_shaders(char *name_vs, char *name_fs, t_gl *gl)
{
	t_shd	shd;

	bzero(&shd, sizeof(t_shd));
	gl->vertex_id = glCreateShader(GL_VERTEX_SHADER);
	gl->fragment_id = glCreateShader(GL_FRAGMENT_SHADER);
	if (!(shd.vertex_s = (GLchar*)ft_get_file(name_vs, shd.vertex_s)) ||
		!(shd.fragment_s = (GLchar*)ft_get_file(name_fs, shd.fragment_s)))
		return (ft_delete_shader(gl));
	if (ft_shader_start(gl, &shd) < 0)
		return (-1);
	if (shd.program_state != GL_TRUE)
	{
		glGetProgramiv(gl->program_id, GL_INFO_LOG_LENGTH, &gl->log_size);
		if (!(gl->log = (GLchar*)malloc(gl->log_size)))
		{
			ft_printf("Error malloc for log of program compilation\n");
			return (ft_delete_shader(gl));
		}
		glGetProgramInfoLog(gl->program_id, gl->log_size,
			&gl->log_size, gl->log);
		ft_printf("Erreur lors du liage du shader:\n%s", gl->log);
		free(gl->log);
		return (ft_delete_shader(gl));
	}
	return (0);
}
