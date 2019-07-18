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
	glDetachShader(gl->programID, gl->fragmentID);
	glDetachShader(gl->programID, gl->vertexID);
	glDeleteProgram(gl->programID);
	glDeleteShader(gl->fragmentID);
	glDeleteShader(gl->vertexID);
	return (-1);
}

static int		ft_checkShader_compilation(GLuint shaderID, t_gl *gl)
{
	GLint compilationStatus;

	compilationStatus = 0;
	glGetShaderiv(gl->vertexID, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus != GL_TRUE)
	{
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &gl->logSize);
		if (!(gl->log = (GLchar*)malloc(gl->logSize)))
		{
			fprintf(stderr,"Error memori allocation for log of shader compilation\n");
			return (0);
		}
		glGetShaderInfoLog(shaderID, gl->logSize, &gl->logSize, gl->log);
		fprintf(stderr,"Compilation error :\n%s",gl->log);
		free(gl->log);
		return (0);
	}
	return (1);
}

int				ft_shader_start(t_gl *gl, t_shd *shd)
{
	shd->vertexSize = strlen(shd->vertexSource);
	shd->fragmentSize = strlen(shd->fragmentSource);
	glShaderSource(gl->vertexID, 1, (const GLchar**)(&shd->vertexSource), &shd->vertexSize);
	glShaderSource(gl->fragmentID, 1, (const GLchar**)(&shd->fragmentSource), &shd->fragmentSize);
	glCompileShader(gl->vertexID);
	glCompileShader(gl->fragmentID);
	if (!ft_checkShader_compilation(gl->vertexID, gl) ||
		!ft_checkShader_compilation(gl->fragmentID, gl))
		return (ft_delete_shader(gl));
	gl->programID = glCreateProgram();	
	glAttachShader(gl->programID, gl->vertexID);
	glAttachShader(gl->programID, gl->fragmentID);
	glLinkProgram(gl->programID);
	glGetProgramiv(gl->programID , GL_LINK_STATUS , &shd->programState);
	return (1);
}

int				ft_shaders(char *nameVS, char *nameFS, t_gl *gl)
{
	t_shd	shd;

	bzero(&shd, sizeof(t_shd));
	gl->vertexID = glCreateShader(GL_VERTEX_SHADER);
	gl->fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	if (!(shd.vertexSource = (GLchar*)ft_get_file(nameVS, shd.vertexSource)) ||
		!(shd.fragmentSource = (GLchar*)ft_get_file(nameFS, shd.fragmentSource)))
		return (ft_delete_shader(gl));
	if (ft_shader_start(gl, &shd) < 0)
		return (-1);
	if (shd.programState != GL_TRUE)
	{
		glGetProgramiv(gl->programID, GL_INFO_LOG_LENGTH, &gl->logSize);
		if (!(gl->log = (GLchar*)malloc(gl->logSize)))
		{
			fprintf(stderr,"Error memori allocation for log of program compilation\n");
			return (ft_delete_shader(gl));
		}
		glGetProgramInfoLog(gl->programID, gl->logSize, &gl->logSize, gl->log);
		fprintf(stderr,"Erreur lors du liage du shader:\n%s",gl->log);
		free(gl->log);
		return (ft_delete_shader(gl));
	}
	return (0);
}
