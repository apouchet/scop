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

// gcc paring.c main.c ft_read_shader.c ft_read_tga.c ft_file.c -I ~/.brew/include -L ~/.brew/lib -lSDL2 -framework OpenGL -framework Cocoa libft/libft.a
#include "scop.h"

void	ft_control(t_control *ctrl, SDL_Event evenements)
{
	// SDL_PumpEvents();
	ctrl->key = 0;
	if (SDL_WaitEventTimeout(&evenements, 1))
	{
		// SDL_WaitEvent(&evenements);
		ctrl->key = evenements.window.event;
		if(ctrl->key == SDL_WINDOWEVENT_CLOSE || ctrl->key == ' ')
			ctrl->end = 1;
		else if (ctrl->key == 'f')
			ctrl->rotY -= PI / ctrl->step;
		else if (ctrl->key == 'h')
			ctrl->rotY += PI / ctrl->step;
		else if (ctrl->key == 'g')
			ctrl->rotX -= PI / ctrl->step;
		else if (ctrl->key == 't')
			ctrl->rotX += PI / ctrl->step;
		else if (ctrl->key == 'y')
			ctrl->rotZ -= PI / ctrl->step;
		else if (ctrl->key == 'r')
			ctrl->rotZ += PI / ctrl->step;
		else if (ctrl->key == 'w')
			ctrl->moveY += 0.1f;
		else if (ctrl->key == 's')
			ctrl->moveY -= 0.1f;
		else if (ctrl->key == 'd')
			ctrl->moveX += 0.1f;
		else if (ctrl->key == 'a')
			ctrl->moveX -= 0.1f; 
		else if (ctrl->key == 'z')
			ctrl->moveZ += 0.1f;
		else if (ctrl->key == 'x')
			ctrl->moveZ -= 0.1f;

		// else if (ctrl->key == 'p')
		// 	ctrl->moveZ = moveX = moveY = 0;
		// else if (ctrl->key == 'o')
		// 	ctrl->ver = hor = tan = 0;
		// else 
	}
}



void	ft_read_texture(GLuint programID, char *texture)
{
	t_tga			tga;
	unsigned int	gl_texture;
	unsigned char	*data;

	bzero(&tga, sizeof(t_tga));
	gl_texture = glGetUniformLocation(programID, "Texture");
	glGenTextures(1, &gl_texture);
	glBindTexture(GL_TEXTURE_2D, gl_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (texture)
		data = ft_read_tga_headers(texture, &tga);
	else
		data = ft_read_tga_headers("img/face.tga", &tga);
	if (data)
	{
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.width, tga.height, 0, ((tga.bpp == 4) ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
	    glGenerateMipmap(GL_TEXTURE_2D);
		free(data);
	}
	else
	{
		printf("Failed to load texture\n");
		exit(0);
		// return (-1);
	}
}

void	ft_main_loop(t_sdl *sdl, t_gl *gl, t_obj *obj)
{
	t_matrix	mx;
	t_control	ctrl;
	int			i;

	bzero(&mx, sizeof(mx));
	bzero(&ctrl, sizeof(t_control));
	ctrl.step = 20;
	ctrl.moveZ = -(obj->zoom + 1);
	while(!ctrl.end)
	{
		i = 0;
		ft_control(&ctrl, sdl->evenements);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ft_matrix(gl->programID, &ctrl, &mx);
		glActiveTexture(GL_TEXTURE2);
		glBindVertexArray(gl->VAO_tri);
		glDrawArrays(GL_TRIANGLES, 0, obj->faceTri * 3);
		glBindVertexArray(gl->VAO_quad);
			while (i < obj->faceQuad * 4)
				glDrawArrays(GL_TRIANGLE_FAN, i += 4, 4);
		SDL_GL_SwapWindow(sdl->fenetre);
	}
	ft_exit_gl_sdl(gl, sdl);
}

void	ft_face_color(t_obj *obj)
{
	float	color;
	int		i;
	int		j;

	color = 0;
	i = 0;
	while (i < obj->faceTri * 3 * 3)
	{
		j = i;
		if (color > 1)
			color = 0;
		while (i - j < 9)
			obj->tabNormalTri[i++] = color;
		color += 0.5f;
	}
	i = 0;
	while (i < obj->faceQuad * 4 * 3)
	{
		j = i;
		if (color > 1)
			color = 0;
		while (i - j < 12)
			obj->tabNormalQuad[i++] = color;
		color += 0.5f;
	}
}

int		main(int argc, char **argv)
{	
	t_sdl		sdl;
	t_gl		gl;
	t_obj		obj;

	bzero(&obj, sizeof(t_obj));
	bzero(&sdl, sizeof(sdl));
	bzero(&gl, sizeof(gl));
	if (ft_start_sdl_opengl(&sdl) < 0)
		return (-1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	if (ft_shaders("Shaders/shader.vs", "Shaders/shader.fs", &gl) < 0)
		return (-1);
	// printf("OpenGL version : %s\n", glGetString(GL_VERSION));
	// printf("Shader version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	if (argc == 2)
		ft_parsing(&obj, argv[1]);
	else
		ft_parsing(&obj, NULL);
	// printf("face tri %d\n", obj.faceTri);
	// printf("face quad %d\n", obj.faceQuad);
	// printf("face total %d\n", obj.faceQuad + obj.faceTri);
	ft_face_color(&obj);
	ft_glBuffer(&obj, &gl);
	ft_read_texture(gl.programID, obj.texture);
	glUseProgram(gl.programID);
	ft_main_loop(&sdl, &gl, &obj);
	return (0);
}


/*

https://learnopengl.com/Getting-started/Hello-Triangle-

gcc -L ~/.brew/lib -lSDL2 -I ~/.brew/include -framework OpenGL -framework Cocoa main.c

fichier modifier :
/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/OpenGL.framework/Headers/gl3.h

*/
