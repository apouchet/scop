/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/27 13:29:46 by apouchet          #+#    #+#             */
/*   Updated: 2019/03/05 17:23:21 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

#ifndef GL_SILENCE_DEPRECATION
# define GL_SILENCE_DEPRECATION
#endif


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct	s_matrix
{
	float		base[4][4];
	float		rotate[4][4];
	float		move[4][4];
	float		pers[4][4];
}				t_matrix;

void	ft_start_matrix(t_matrix *mx)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		while (j < 4)
		{
			if (i == j)
			{
				mx->pers[i][j] = 1;
				mx->move[i][j] = 1;
				mx->rotate[i][j] = 1;
				mx->base[i][j] = 1;
			}
			else
			{
				mx->pers[i][j] = 0;
				mx->move[i][j] = 0;
				mx->rotate[i][j] = 0;
				mx->base[i][j] = 0;
			}
			j++;
		}
		i++;
	}
}

int		ft_gl_error(char *msg, char *where, GLuint ID, t_gl *gl)
{
	GLint logLength = 0;
	GLchar* log = NULL;
	
	glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLength);
	if (!(log = (GLchar*)malloc(logLength)))
	{
		printf("%s for %s\n", msg, where);
		if (gl)
			return (ft_delete_shader(gl));
		// fprintf(stderr,"Error memori allocation for log of shader compilation\n");
		return (0);
	}
	glGetShaderInfoLog(ID, logLength, &logLength, log);
	printf("%s error :\n%s",where, log);
	if (gl)
		ft_delete_shader(gl);
	free(log);
	return (0);
}

int		ft_start_sdl_opengl(t_sdl *sdl)
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Error initialisation of SDL :  %s\n", SDL_GetError());
		SDL_Quit();
		return (-1);
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	if((sdl->fenetre = SDL_CreateWindow("Test SDL 2.0 main.c",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL)) == 0)
	{
		printf("Error creation of the window : %s\n", SDL_GetError());
		SDL_Quit();
		return (-1);
	}
	if((sdl->contexteOpenGL = SDL_GL_CreateContext(sdl->fenetre)) == 0)
	{
		printf("%s\n", SDL_GetError());
		SDL_DestroyWindow(sdl->fenetre);
		SDL_Quit();
		return (-1);
	}
	return (1);
}

void	ft_trans(t_matrix *mx, float x, float y, float z)
{
	mx->base[3][0] = x;
	mx->base[3][1] = y;
	mx->base[3][2] = z;
}

void	ft_rotate(t_matrix *mx, double angleX, double angleY, double angleZ)
{

	float A;
	float B;
	float C;
	float D;
	float E;
	float F;
	// angleX = -angleX;

	A = cosf(angleX);
	B = sinf(angleX);
	C = cosf(angleY);
	D = sinf(angleY);
	E = cosf(angleZ);
	F = sinf(angleZ);
	mx->rotate[0][0] = C * E;
	mx->rotate[0][1] = B * D * E + A * F;
	mx->rotate[0][2] = -A * D * E + B * F;
	mx->rotate[0][3] = 0;

	mx->rotate[1][0] = - C * F;
	mx->rotate[1][1] = - B * D * F + A * E;
	mx->rotate[1][2] = A * D * F + B * E;
	mx->rotate[1][3] = 0;

	mx->rotate[2][0] = D;
	mx->rotate[2][1] = - B * C;
	mx->rotate[2][2] = A * C;
	mx->rotate[2][3] = 0;

	mx->rotate[3][0] = 0;
	mx->rotate[3][1] = 0;
	mx->rotate[3][2] = 0;
	mx->rotate[3][3] = 1.0f;
}

void	ft_perspective(t_matrix *mx, double fov, double ar, double near, double far)
{
    const float range = near - far;
    const float tanHalfFOV = tanf(((fov / 2.0) / 180) * PI);
    float	tanFov = tanf(((fov / 180) * PI) / 2);
    // const float tanHalfFOV = tanf(ToRadian(m_persProj.FOV / 2.0));

	mx->pers[0][0] = 1.0f / tanFov; 
    mx->pers[0][1] = 0.0f; 
    mx->pers[0][2] = 0.0f; 
    mx->pers[0][3] = 0.0f; 

    mx->pers[1][0] = 0.0f; 
    mx->pers[1][1] = 1.0f / tanFov; 
    mx->pers[1][2] = 0.0f; 
    mx->pers[1][3] = 0.0f; 

    mx->pers[2][0] = 0.0f; 
    mx->pers[2][1] = 0.0f; 
    mx->pers[2][2] = 1.0f / tanFov;//-(near + far) / range; 
 	mx->pers[2][3] = 0;//(2.0f * far * near) / range; // "perspective"

    mx->pers[3][0] = 0.0f;
    mx->pers[3][1] = 0.0f;
    mx->pers[3][2] = (2.0f * far * near) / range;
    mx->pers[3][3] = 5;
}


int		main(int argc, char **argv)
{	
	int		terminer;
	int		key;
	t_matrix mx;
	float	moveX = 0, moveY = 0, moveZ = 0;
	t_tga	tga;
	t_sdl	sdl;
	t_gl	gl;

	bzero(&tga, sizeof(t_tga));
	bzero(&sdl, sizeof(sdl));
	bzero(&gl, sizeof(gl));
	// ft_start_matrix(&mx);
	for (int z = 0; z < 4; z++)
	{
		for (int zz = 0; zz < 4; zz++)
		{
			if (z == zz)
			{
				mx.rotate[z][zz] = 1;
				mx.base[z][zz] = 1;
			}
			else
			{
				mx.rotate[z][zz] = 0;
				mx.base[z][zz] = 0;
			}
		}
	}
	printf("\n");
	terminer = 0;
	if (ft_start_sdl_opengl(&sdl) < 0)
		return (-1);
	// Boucle principale
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ft_shaders("Shaders/shader.vs", "Shaders/shader.fs", &gl);
	printf("OpenGL version : %s\n", glGetString(GL_VERSION));
	printf("Shader version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	float vertices[] = {-0.5, -0.5, 0.0, 0.5, 0.5, -0.5};
	float couleurs[] = {1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0};
	// float *text = (float*)malloc(sizeof(float) * 32);
	float text[] = {

	// positions          // colors           // texture coords
	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right		0
	0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right	1
	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // bottom left	2
	-0.5f, +0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,   // top left 		3

	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right		4
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right	5
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // bottom left	6
	-0.5f, +0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,    // top left		7

	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,   // top right		8 / 0
	0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,   // bottom left	9 / 1
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right		10 / 4
	0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right	11 / 5

	-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,   // top right		12 / 0
	-0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,   // bottom left	13 / 1
	-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right		14 / 4
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right	15 / 5

	0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,   // bottom right	16 / 0
	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f,   // bottom left	17 / 3
	0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right		18 / 4
	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,   // top left		19 / 7

	0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 0.0f,   // bottom right	20 / 1
	-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 0.0f,	0.0f, 0.0f,   // bottom left	21 / 2
	0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right		22 / 5
	-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f   // top left		23 / 6


	// 0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right
	// 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right
	// -0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // bottom left
	// -0.5f, +0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,   // top left 

	// 0.0f, 0.75f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right
	// 0.75f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right
	// 0.0f, -0.75f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // bottom left
	// -0.75f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f    // top left 

	};

	

	unsigned int indices[] = {
		//face
		// 0, 1, 3, // first triangle
		// 1, 2, 3, // second triangle
		// //fond
		// 4, 5, 7, // first triangle
		// 5, 6, 7,  // second triangle
		// //droit
		// 0, 1, 4,
		// 1, 5, 4,

		// 2, 3, 6,
		// 3, 7, 6,

		// avant
		0, 1, 2,
		0, 3, 2,
		// arrire
		4, 7, 6,
		4, 5, 6,
		//dessus
		19, 17, 16,
		19, 18, 16,
		//dessous
		23, 21, 20,
		23, 22, 20,
		//gauche
		14, 12, 13,
		14, 15, 13,
		//droite
		10, 8, 9,
		10, 11, 9		
	};
	
	unsigned int	VAO_text;
	unsigned int	VBO_indide;
	unsigned int	VBO_texture;
	int				width;
	int				height;
	int				nrChannels;
	unsigned int	texture1 = glGetUniformLocation(gl.programID, "Texture1");
	unsigned int	texture2 = glGetUniformLocation(gl.programID, "Texture2");
	unsigned char	*data;
	unsigned char	*data2;

	glGenVertexArrays(1, &VAO_text);
	glGenBuffers(1, &VBO_texture);
	glGenBuffers(1, &VBO_indide);
	glBindVertexArray(VAO_text);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(text), text, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indide);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// data2 = stbi_load("img/wall1.tga", &width, &height, &nrChannels, 0);
	// for (int i = 78740; i < 78756; i++)
	// {
	// 	// if (data[i] != data2[i])
	// 	if (i % 4 == 0)
	// 	printf("\n");
	// 	if (data[i] != data2[i])
	// 		printf("%d : moi = -%d-, stb = -%d-\n", i, data[i], data2[i]);
	// 	else
	// 		printf("ok pour %d\n", i);
	// 	// else if (i % 100 == 0)
	// 		// printf("ok : %d\n", i);
	// }
	// data2 = stbi_load("img/wall1.tga", &width, &height, &nrChannels, 0);
	
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	
	data2 = ft_read_tga_headers("img/wall1.tga", &tga);
	if (data2)
	{
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.width, tga.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
	    glGenerateMipmap(GL_TEXTURE_2D);
		free(data2);
	}
	else
	{
		printf("Failed to load texture\n");
		return (-1);
	}

	// printf("moi -> height = %d, width %d, nbr channel = %d\n", tga.height, tga.width, tga.bpp);
	// printf("stb -> height = %d, width %d, nbr channel = %d\n", height, width, nrChannels);
	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = ft_read_tga_headers("img/container.tga", &tga);
	if (data)
	{
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.width, tga.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	    glGenerateMipmap(GL_TEXTURE_2D);
		free(data);
	}
	else
	{
		printf("Failed to load texture\n");
		return (-1);
	}
	glUseProgram(gl.programID);
	glUniform1i(texture1, 0); // Texture unit 0 is for base images.
	glUniform1i(texture2, 1);

	// glGenTextures(1, &texture2);
	// glBindTexture(GL_TEXTURE_2D, texture2);
	// // set the texture wrapping parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// // set texture filtering parameters
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// // load image, create texture and generate mipmaps
	// data = ft_read_tga_headers("img/face.tga", &tga);
	// if (data)
	// {
	//     // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
	//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tga.width, tga.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//     glGenerateMipmap(GL_TEXTURE_2D);
	// }
	// else
	// {
	//     printf("Failed to load texture\n");
	// }
	// free(data);


	// unsigned int VBO_color, VBO_vertex, VAO;

	// glGenVertexArrays(1, &VAO);
 // 	glGenBuffers(1, &VBO_vertex);
 // 	glGenBuffers(1, &VBO_color);
	// glBindVertexArray(VAO);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// glEnableVertexAttribArray(0);
	// glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(couleurs), couleurs, GL_STATIC_DRAW);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// glEnableVertexAttribArray(1);
	// glBindVertexArray(0);
 
	// unsigned int transformLoc = glGetUniformLocation(gl.programID, "transform");

	// printf("trans = %d\n", transformLoc);
	// glUniform1i(glGetUniformLocation(gl.programID, "texture1"), 0); // set it manually
	// ourShader.setInt("texture2", 1); // or with shader class
	// printf("programme ID = %d\n", gl.programID);
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
		// glClear(GL_COLOR_BUFFER_BIT); // Nettoyage de l'écran
		// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		// glEnableVertexAttribArray(0);

		for (int z = 0; z < 4; z++)
		{
			for (int zz = 0; zz < 4; zz++)
			{
				if (z == zz)
				{
					mx.pers[z][zz] = 1;
				}
				else
				{
					mx.pers[z][zz] = 0;
				}
			}
		}
		ft_rotate(&mx, hor, ver, tan);
		ft_perspective(&mx, fov , 800 / 800, 0.01f, 100.0f);
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

		

		// glActiveTexture(GL_TEXTURE1);
		// glBindTexture(GL_TEXTURE_2D, texture1);
		// glActiveTexture(GL_TEXTURE2);
		// glBindTexture(GL_TEXTURE_2D, texture2);


		// render container
		glBindVertexArray(VAO_text);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

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




	// double angleX = 0;
	// double angleY = 0;
	// double angleZ = 0;

	// float tmp;
	// float tmpX;
	// float tmpY;
	// float tmpZ;

	// // rotation axe X
	// for (int i = 0; i < 4; i++)
	// {
	// 	tmp = text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX);
	// 	text[2 + 8 * i] = text[1 + 8 * i] * sinf(angleX) + text[2 + 8 * i] * cosf(angleX);
	// 	text[1 + 8 * i] = tmp;
	// }

	// // rotation axe Y
	// for (int i = 0; i < 4; i++)
	// {
	// 	tmp = text[0 + 8 * i] * cosf(angleY) + text[2 + 8 * i] * sinf(angleY);
	// 	text[2 + 8 * i] = text[0 + 8 * i] * -sinf(angleY) + text[2 + 8 * i] * cosf(angleY);
	// 	text[0 + 8 * i] = tmp;
	// }

	// // rotation axe Z
	// for (int i = 0; i < 4; i++)
	// {
	// 	tmp = text[0 + 8 * i] * cosf(angleZ) - text[1 + 8 * i] * sinf(angleZ);
	// 	text[1 + 8 * i] = text[0 + 8 * i] * sinf(angleZ) + text[1 + 8 * i] * cosf(angleZ);
	// 	text[0 + 8 * i] = tmp;
	// }

	// rotation axe XYZ
	// matrix = ft_rotate(matrix, 0, PI / 4, 0);
	// for (int i = 0; i < 8; i++)
	// {

	// 	// tmpZ = text[X + 8 * i] * -sinf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * cosf(angleY);
	// 	// tmpX = (text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY)) * cosf(angleZ) - (text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX)) * sinf(angleZ);
	// 	// text[Y + 8 * i] = (text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY)) * sinf(angleZ) + (text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX)) * cosf(angleZ);
	// 	// text[X + 8 * i] = tmpX;
	// 	// text[Z + 8 * i] = tmpZ;
		

	// 	// tmp = text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX);
	// 	// text[2 + 8 * i] = text[1 + 8 * i] * sinf(angleX) + text[2 + 8 * i] * cosf(angleX);
	// 	// text[1 + 8 * i] = tmp;

	// 	// tmp = text[0 + 8 * i] * cosf(angleY) + text[2 + 8 * i] * sinf(angleY);
	// 	// text[2 + 8 * i] = text[0 + 8 * i] * -sinf(angleY) + text[2 + 8 * i] * cosf(angleY);
	// 	// text[0 + 8 * i] = tmp;

	// 	// tmp = text[0 + 8 * i] * cosf(angleZ) - text[1 + 8 * i] * sinf(angleZ);
	// 	// text[1 + 8 * i] = text[0 + 8 * i] * sinf(angleZ) + text[1 + 8 * i] * cosf(angleZ);
	// 	// text[0 + 8 * i] = tmp;
	// }
	// for (int i = 0; i < 4; i++)
	// {
	// 				tmpX = text[X + 8 * i] * (cosf(angleZ)) - text[Y + 8 * i] * sinf(angleY) + text[Z + 8 * i] * sinf(angleZ);
	// 				tmpY = text[X + 8 * i] * sinf(angleZ) + text[Y + 8 * i] * (cosf(angleZ)) - text[Z + 8 * i] * sinf(angleX);
	// 	text[Z + 8 * i] = text[X + 8 * i] * -sinf(angleY) + text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * (cosf(angleX) + cosf(angleY));
	// 	text[X + 8 * i] = tmpX;
	// 	text[Y + 8 * i] = tmpY;
	// }