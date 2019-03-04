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
	/*
	** On arrête d'utiliser le programme shader
	*/
	glUseProgram(0);
	/*
	** Deliage des shaders au programme
	*/
	glDetachShader(gl->programID, gl->fragmentID);
	glDetachShader(gl->programID, gl->vertexID);
	/*
	** Destruction du programme
	*/
	glDeleteProgram(gl->programID);
	/*
	** Destruction des IDs des shaders
	*/
	glDeleteShader(gl->fragmentID);
	glDeleteShader(gl->vertexID);
	return (-1);
}

static int		ft_checkShader_compilation(GLuint shaderID, t_gl *gl)
{
	GLint compilationStatus = 0;
		
	glGetShaderiv(gl->vertexID, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus != GL_TRUE)
	{
		// return (ft_gl_error("Error memori allocation", "liage du shader", shaderID, NULL));
		GLint logLength = 0;
		GLchar* log = NULL;
		
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		if (!(log = (GLchar*)malloc(logLength)))
		{
			fprintf(stderr,"Error memori allocation for log of shader compilation\n");
			return 0;
		}
		glGetShaderInfoLog(shaderID, logLength, &logLength, log);
		fprintf(stderr,"Compilation error :\n%s",log);
		free(log);
		return (0);
	}
	return (1);
}

int				ft_shaders(char *nameVS, char *nameFS, t_gl *gl)
{
	char	*file;
	int		size;
	t_shd	shd;

	bzero(&shd, sizeof(t_shd));
	gl->vertexID = glCreateShader(GL_VERTEX_SHADER);
	gl->fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	// shd.vertexSource = (GLchar*)ft_get_file(nameVS, shd.vertexSource);
	// shd.fragmentSource = (GLchar*)ft_get_file(nameFS, shd.fragmentSource);
	if (!(shd.vertexSource = (GLchar*)ft_get_file(nameVS, shd.vertexSource)) ||
		!(shd.fragmentSource = (GLchar*)ft_get_file(nameFS, shd.fragmentSource)))
		return (ft_delete_shader(gl));
	shd.vertexSize = strlen(shd.vertexSource);
	shd.fragmentSize = strlen(shd.fragmentSource);
	glShaderSource(gl->vertexID, 1, (const GLchar**)(&shd.vertexSource), &shd.vertexSize);
	glShaderSource(gl->fragmentID, 1, (const GLchar**)(&shd.fragmentSource), &shd.fragmentSize);
	glCompileShader(gl->vertexID);
	glCompileShader(gl->fragmentID);
	if (!ft_checkShader_compilation(gl->vertexID, gl) ||
		!ft_checkShader_compilation(gl->fragmentID, gl))
		return (ft_delete_shader(gl));
	gl->programID = glCreateProgram();	
	glAttachShader(gl->programID, gl->vertexID);
	glAttachShader(gl->programID, gl->fragmentID);
	glLinkProgram(gl->programID);

	// Et encore une fois on vérifie si tout se passe bien
	glGetProgramiv(gl->programID , GL_LINK_STATUS , &shd.programState);
	if (shd.programState != GL_TRUE)
	{
		// return (ft_gl_error("Error memori allocation", "liage du shader", gl->programID, gl));
		// return (ft_error_log("Erreur lors du liage du shader:\n"));
		// On récupère la taille du log
		GLint logSize = 0;
		GLchar* log = NULL;
		glGetProgramiv(gl->programID, GL_INFO_LOG_LENGTH, &logSize);
		if (!(log = (GLchar*)malloc(logSize)))
		{
			fprintf(stderr,"Error memori allocation for log of program compilation\n");
			return (ft_delete_shader(gl));
		}
		glGetProgramInfoLog(gl->programID, logSize, &logSize, log);
		fprintf(stderr,"Erreur lors du liage du shader:\n%s",log);
		free(log);
		return (ft_delete_shader(gl));
	}
	return (0);
}
