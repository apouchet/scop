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

// void	ft_creat_glBuffer(size_t size, size_t tabVer, size_t tabTex, size_t tabNor)
// {
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex_tri);
// 	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), tabVer, GL_STATIC_DRAW);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);

// 	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_tri);
// 	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), tabNor, GL_STATIC_DRAW);
// 	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(1);

// 	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_tri);
// 	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), tabTex, GL_STATIC_DRAW);
// 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(2);
// }

int		main(int argc, char **argv)
{	
	// char		*a = "		12345678910111213";
	// printf("a = -|%s|-, size_t = %zu\n", a, ft_atost(a));
	// return 0;
	int			terminer;
	int			key;
	float		moveX = 0;
	float		moveY = 0;
	float		moveZ;
	t_matrix	mx;
	t_tga		tga;
	t_sdl		sdl;
	t_gl		gl;
	t_obj		obj;


	bzero(&obj, sizeof(t_obj));
	bzero(&tga, sizeof(t_tga));
	bzero(&sdl, sizeof(sdl));
	bzero(&gl, sizeof(gl));
	bzero(&mx, sizeof(mx));


	terminer = 0;
	if (ft_start_sdl_opengl(&sdl) < 0)
		return (-1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ft_shaders("Shaders/shader.vs", "Shaders/shader.fs", &gl);
	printf("OpenGL version : %s\n", glGetString(GL_VERSION));
	printf("Shader version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (argc == 2)
		ft_parsing(&obj, argv[1]);
	else
		ft_parsing(&obj, NULL);
	printf("zoom = %f\n", obj.zoom);
	moveZ = -(obj.zoom + 1);
	printf("face tri %d\n", obj.faceTri);
	printf("face quad %d\n", obj.faceQuad);
	printf("face total %d\n", obj.faceQuad + obj.faceTri);
	
	float ww = 0.5;
	for (int w = 0; w < obj.faceTri * 3 * 3;)
	{
		// if (w + 1 % 3 == 0)
		// 	ww += 0.5f;
		if (ww > 1)
			ww = 0;
		// 	ww = 0;
		for (int k = 0; k < 9; k++)
			obj.tabNormalTri[w++] = ww;

		// obj.tabNormalTri[w] = ww;
		// obj.tabNormalTri[w + 1] = ww;
		// obj.tabNormalTri[w + 2] = ww;
		// w += 3;
		ww += 0.5f;
	}
	ww = 0;
	for (int w = 0; w < obj.faceQuad * 4 * 3;)
	{
		if (ww > 1)
			ww = 0;
		for (int k = 0; k < 12; k++)
			obj.tabNormalTri[w++] = ww;
		ww += 0.5f;
	}

	unsigned int VAO_tri;
	unsigned int VBO_vertex_tri, VBO_normal_tri, VBO_texture_tri;
	glGenVertexArrays(1, &VAO_tri);
	glGenBuffers(1, &VBO_vertex_tri);
	glGenBuffers(1, &VBO_normal_tri);
	glGenBuffers(1, &VBO_texture_tri);
	glBindVertexArray(VAO_tri);

	// ft_creat_glBuffer(size_t obj.faceTri * 3 * 3, obj.tabVertexTri, obj.tabTextureTri, obj.tabNormalTri)

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex_tri);
	glBufferData(GL_ARRAY_BUFFER, obj.faceTri * 3 * 3 * sizeof(float), obj.tabVertexTri, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_tri);
	glBufferData(GL_ARRAY_BUFFER, obj.faceTri * 3 * 3 * sizeof(float), obj.tabNormalTri, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_tri);
	glBufferData(GL_ARRAY_BUFFER, obj.faceTri * 3 * 2 * sizeof(float), obj.tabTextureTri, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.

	unsigned int VAO_quad;
	unsigned int VBO_vertex_quad, VBO_normal_quad, VBO_texture_quad;
	glGenVertexArrays(1, &VAO_quad);
	glGenBuffers(1, &VBO_vertex_quad);
	glGenBuffers(1, &VBO_normal_quad);
	glGenBuffers(1, &VBO_texture_quad);
	glBindVertexArray(VAO_quad);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex_quad);
	glBufferData(GL_ARRAY_BUFFER, obj.faceQuad * 4 * 3 * sizeof(float), obj.tabVertexQuad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_quad);
	glBufferData(GL_ARRAY_BUFFER, obj.faceQuad * 4 * 3 * sizeof(float), obj.tabNormalQuad, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_quad);
	glBufferData(GL_ARRAY_BUFFER, obj.faceQuad * 4 * 2 * sizeof(float), obj.tabTextureQuad, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	
	free(obj.tabVertexQuad);
	free(obj.tabVertexTri);

	unsigned int	texture = glGetUniformLocation(gl.programID, "Texture");
	unsigned char	*data;

	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	printf("texture = -|%s|-\n", obj.texture);
	if (obj.texture)
		data = ft_read_tga_headers(obj.texture, &tga);
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
		return (-1);
	}
	glUseProgram(gl.programID);
	// glUniform1i(texture1, 0); // Texture unit 0 is for base images.
	// glUniform1i(texture2, 1); // Texture unit 0 is for base images.


	float	tour = 20;
	float	hor = 0;
	float	ver = 0;
	float	tan = 0;
	int		fov = 45;
	float	acolor = 1;
	int		deg = 0;



	glUseProgram(gl.programID);
	while(!terminer)
	{
		SDL_WaitEvent(&sdl.evenements);
		key = sdl.evenements.window.event;
		// printf("key = %d - %C\n", key, key);
		if(key == SDL_WINDOWEVENT_CLOSE || key == ' ')
			terminer = 1;
		// ver = tan = hor = 0;
		else if (key == 'f')
			ver -= PI / tour;
		else if (key == 'h')
			ver += PI / tour;

		else if (key == 'g')
			hor -= PI / tour;
		else if (key == 't')
			hor += PI / tour;

		else if (key == 'y')
			tan -= PI / tour;
		else if (key == 'r')
			tan += PI / tour;
		// if (tour < 0)
		// 	tour += PI;
		// else if (tour > 3 * PI)
		// 	tour -= PI;
		// else if (key == 'w')
		// 	tour = 0;
		// else if (key == 'x')
		// 	tour = PI / 2;
		else if (key == 'v')
			fov += 5;
		else if (key == 'b')
			fov -= 5;

		else if (key == 'w')
			moveY += 0.1f;
		else if (key == 's')
			moveY -= 0.1f;
		else if (key == 'd')
			moveX += 0.1f;
		else if (key == 'a')
			moveX -= 0.1f;
		else if (key == 'z')
			moveZ += 0.1f;
		else if (key == 'x')
			moveZ -= 0.1f;

		else if (key == 'p')
			moveZ = moveX = moveY = 0;
		else if (key == 'o')
			ver = hor = tan = 0;
		else if (key == '.')
		{
			acolor = (acolor >= 1? (acolor == 1? 2 : 0) : 1);
			printf("acolor = %f\n", acolor);
		}
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		printf("fov = %d\n", fov);
		// glClear(GL_COLOR_BUFFER_BIT); // Nettoyage de l'écran
		// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		// glEnableVertexAttribArray(0);

		gl_perspective(&mx);

		ft_rotate(&mx, hor, ver, tan);
		ft_perspective(&mx, 0.01f, 100.0f);
		ft_trans(&mx, moveX, moveY, moveZ);

		unsigned int matLoc;

		matLoc = glGetUniformLocation(gl.programID, "color");
		glUniform1f(matLoc, acolor);

		matLoc = glGetUniformLocation(gl.programID, "rotate");
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mx.rotate[0][0]);

		matLoc = glGetUniformLocation(gl.programID, "pers");
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mx.pers[0][0]);

		matLoc = glGetUniformLocation(gl.programID, "move");
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mx.pers[0][0]);

		matLoc = glGetUniformLocation(gl.programID, "base");
		glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mx.base[0][0]);

		glActiveTexture(GL_TEXTURE2);
		glBindVertexArray(VAO_tri); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, obj.faceTri * 3);

		glBindVertexArray(VAO_quad);
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
