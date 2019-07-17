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
	SDL_WaitEvent(&evenements);
	ctrl->key = evenements.window.event;
	// printf("key = %d - %C\n", key, key);
	if(ctrl->key == SDL_WINDOWEVENT_CLOSE || ctrl->key == ' ')
		ctrl->end = 1;
	// ver = tan = hor = 0;
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
	// if (ctrl.step < 0)
	// 	ctrl.step += PI;
	// else if (ctrl.step > 3 * PI)
	// 	ctrl.step -= PI;
	// else if (key == 'w')
	// 	ctrl.step = 0;
	// else if (key == 'x')
	// 	ctrl.step = PI / 2;
	// else if (key == 'v')
	// 	fov += 5;
	// else if (key == 'b')
	// 	fov -= 5;

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

void	ft_creat_glBuffer(t_obj *obj, unsigned int VBO[3], int type)
{
	int		size;
	float	*tab[3];

	size = (type == 1 ? obj->faceQuad * 4 : obj->faceTri * 3);
	tab[0] = obj->tabVertexTri;
	tab[1] = obj->tabNormalTri;
	tab[2] = obj->tabTextureTri;
	if (type == 1)
	{
		tab[0] = obj->tabVertexQuad;
		tab[1] = obj->tabNormalQuad;
		tab[2] = obj->tabTextureQuad;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), tab[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), tab[1], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, size * 2 * sizeof(float), tab[2], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
}

void	ft_glBuffer(t_obj *obj, t_gl *gl)
{
	glGenVertexArrays(1, &gl->VAO_tri);
	glGenBuffers(1, &gl->VBO_tri[0]);
	glGenBuffers(1, &gl->VBO_tri[1]);
	glGenBuffers(1, &gl->VBO_tri[2]);
	glBindVertexArray(gl->VAO_tri);
	ft_creat_glBuffer(obj, gl->VBO_tri, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
	glGenVertexArrays(1, &gl->VAO_quad);
	glGenBuffers(1, &gl->VBO_quad[0]);
	glGenBuffers(1, &gl->VBO_quad[1]);
	glGenBuffers(1, &gl->VBO_quad[2]);
	glBindVertexArray(gl->VAO_quad);
	ft_creat_glBuffer(obj, gl->VBO_quad, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
	free(obj->tabVertexQuad);
	free(obj->tabVertexTri);
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

int		main(int argc, char **argv)
{	
	// char		*a = "		12345678910111213";
	// printf("a = -|%s|-, size_t = %zu\n", a, ft_atost(a));
	// return 0;
	// int			end;
	// int			key;
	// float		moveX = 0;
	// float		moveY = 0;
	// float		moveZ;
	t_matrix	mx;
	t_sdl		sdl;
	t_gl		gl;
	t_obj		obj;
	t_control	ctrl;

	bzero(&ctrl, sizeof(t_control));
	bzero(&obj, sizeof(t_obj));
	bzero(&sdl, sizeof(sdl));
	bzero(&gl, sizeof(gl));
	bzero(&mx, sizeof(mx));
	ctrl.step = 20;

	// end = 0;
	if (ft_start_sdl_opengl(&sdl) < 0)
		return (-1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ft_shaders("Shaders/shader.vs", "Shaders/shader.fs", &gl);
	// printf("OpenGL version : %s\n", glGetString(GL_VERSION));
	// printf("Shader version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (argc == 2)
		ft_parsing(&obj, argv[1]);
	else
		ft_parsing(&obj, NULL);
	// printf("zoom = %f\n", obj.zoom);
	ctrl.moveZ = -(obj.zoom + 1);
	// printf("face tri %d\n", obj.faceTri);
	// printf("face quad %d\n", obj.faceQuad);
	// printf("face total %d\n", obj.faceQuad + obj.faceTri);
	
	float ww = 0.5;
	for (int w = 0; w < obj.faceTri * 3 * 3;)
	{
		if (ww > 1)
			ww = 0;
		for (int k = 0; k < 9; k++)
			obj.tabNormalTri[w++] = ww;
		ww += 0.5f;
	}
	ww = 0;
	for (int w = 0; w < obj.faceQuad * 4 * 3;)
	{
		if (ww > 1)
			ww = 0;
		for (int k = 0; k < 12; k++)
			obj.tabNormalQuad[w++] = ww;
		ww += 0.5f;
	}

	ft_glBuffer(&obj, &gl);

	ft_read_texture(gl.programID, obj.texture);

	float	acolor = 1;

	glUseProgram(gl.programID);
	while(!ctrl.end)
	{
		ft_control(&ctrl, sdl.evenements);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// printf("fov = %d\n", fov);
		// glClear(GL_COLOR_BUFFER_BIT); // Nettoyage de l'écran
		// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		// glEnableVertexAttribArray(0);
		
		ft_matrix(gl.programID, &ctrl, &mx);
		glActiveTexture(GL_TEXTURE2);
		glBindVertexArray(gl.VAO_tri); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, obj.faceTri * 3);

		glBindVertexArray(gl.VAO_quad);
		for (int k = 0; k < obj.faceQuad * 4; k += 4)
			glDrawArrays(GL_TRIANGLE_FAN, k, 4);
		SDL_GL_SwapWindow(sdl.fenetre); // Actualisation de la fenêtre
	}
	glDeleteShader(gl.vertexID);
	glDeleteShader(gl.fragmentID);
	// On quitte la SDL
	SDL_GL_DeleteContext(sdl.contexteOpenGL);
	SDL_DestroyWindow(sdl.fenetre);
	SDL_Quit();
	return (0);
}


/*

https://learnopengl.com/Getting-started/Hello-Triangle-

gcc -L ~/.brew/lib -lSDL2 -I ~/.brew/include -framework OpenGL -framework Cocoa main.c

fichier modifier :
/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/OpenGL.framework/Headers/gl3.h

*/
