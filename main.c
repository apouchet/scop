/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/27 13:29:46 by apouchet          #+#    #+#             */
/*   Updated: 2019/02/11 15:12:25 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

#ifndef GL_SILENCE_DEPRECATION
# define GL_SILENCE_DEPRECATION
#endif


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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

float	**ft_trans(float **matrix, float x, float y, float z)
{
	matrix[3][0] += x;
	matrix[3][1] += y;
	matrix[3][2] += z;
	return (matrix);
}

float	**ft_rotate(float **matrix, double angleX, double angleY, double angleZ)
{
	matrix[0][0] = cosf(angleY) + cosf(angleZ) - 1;
	matrix[0][1] = sinf(angleZ);
	matrix[0][2] = -sinf(angleY);

	matrix[1][0] = -sinf(angleZ);
	matrix[1][1] = cosf(angleX) + cosf(angleZ) - 1;
	matrix[1][2] = sinf(angleX);

	matrix[2][0] = sinf(angleY);
	matrix[2][1] = -sinf(angleX);
	matrix[2][2] = cosf(angleX) + cosf(angleY) - 1;



	// matrix[0][0] += cosf(angleY) + cosf(angleZ);
	// matrix[0][1] += sinf(angleZ);
	// matrix[0][2] += -sinf(angleY);

	// matrix[1][0] += -sinf(angleZ);
	// matrix[1][1] += cosf(angleX) + cosf(angleZ);
	// matrix[1][2] += sinf(angleX);

	// matrix[2][0] += sinf(angleY);
	// matrix[2][1] += -sinf(angleX);
	// matrix[2][2] += cosf(angleX) + cosf(angleY);

	return (matrix);
}

// void	ft_rotate(float text[32], double angleX, double angleY, double angleZ)
// {
// 	float tmpX;
// 	float tmpY;
// 	float tmpZ;

// 	for (int i = 0; i < 8; i++)
// 	{
// 		tmpZ = text[X + 8 * i] * -sinf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * cosf(angleY);
// 		tmpX = (text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY)) * cosf(angleZ) - (text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX)) * sinf(angleZ);
// 		text[Y + 8 * i] = (text[X + 8 * i] *  cosf(angleY) + (text[Y + 8 * i] * sinf(angleX) + text[Z + 8 * i] * cosf(angleX)) * sinf(angleY)) * sinf(angleZ) + (text[Y + 8 * i] * cosf(angleX) - text[Z + 8 * i] * sinf(angleX)) * cosf(angleZ);
// 		text[X + 8 * i] = tmpX;
// 		text[Z + 8 * i] = tmpZ;
// 	}
// 	// return (text);
// }

int		main(int argc, char **argv)
{	
	int		terminer;
	int		key;
	float	*matrix;
	float	**mat;
	t_tga	tga;
	t_sdl	sdl;
	t_gl	gl;

	bzero(&tga, sizeof(t_tga));
	bzero(&sdl, sizeof(sdl));
	bzero(&gl, sizeof(gl));
	mat = (float**)malloc(sizeof(float*) * 4);
	for (int z = 0; z < 4; z++)
	{
		mat[z] = (float*)malloc(sizeof(float) * 4);
		for (int zz = 0; zz < 4; zz++)
		{
			if (z == zz)
				mat[z][zz] = 1;
			else
				mat[z][zz] = 0;
		}
		printf("\nmat[%d] = ", z);
		for (int zz = 0; zz < 4; zz++)
			printf("%f,", mat[z][zz]);
	}
	printf("\n");
	// mat[0][0] = 1;
	// mat[1][1] = 1;
	// mat[2][2] = 1;
	// mat[3][3] = 1;
	// for (int z = 0; z < 16; z++)
	// {
	// 	printf("%f ,", mat[z]);
	// 	if ((z + 1) % 4 == 0)
	// 		printf("\n");
	// }
	// for (int z = 0; z < 16; z++)
	// 	matrix[z] = 1;
	terminer = 0;
	if (ft_start_sdl_opengl(&sdl) < 0)
		return (-1);
	// Boucle principale

	ft_shaders("Shaders/shader.vs", "Shaders/shader.fs", &gl);
	printf("OpenGL version : %s\n", glGetString(GL_VERSION));
	printf("Shader version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	float vertices[] = {-0.5, -0.5, 0.0, 0.5, 0.5, -0.5};
	float couleurs[] = {1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0};
	// float *text = (float*)malloc(sizeof(float) * 32);
	float text[] = {

	// positions          // colors           // texture coords
	0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // bottom left
	-0.5f, +0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f,   // top left 

	// 0.5f, 0.5f, 1.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right
	// 0.5f, -0.5f, 1.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right
	// -0.5f, -0.5f, 1.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // bottom left
	// -0.5f, +0.5f, 1.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f    // top left 

	// 0.0f, 0.75f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right
	// 0.75f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right
	// 0.0f, -0.75f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // bottom left
	// -0.75f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f    // top left 
	};

	

	unsigned int indices[] = {

		// 1, 7, 5,
		// 1, 3, 7, 
		// 1, 4, 3, 
		// 1, 2, 4, 
		// 3, 8, 7, 
		// 3, 4, 8, 
		// 5, 7, 8, 
		// 5, 8, 6, 
		// 1, 5, 6, 
		// 1, 6, 2, 
		// 2, 6, 8, 
		// 2, 8, 4 

		0, 1, 3, // first triangle
		1, 2, 3, // second triangle

		// 4, 5, 7, // first triangle
		// 5, 6, 7  // second triangle
	};
	// mat = ft_trans(mat, 0.5, -0.5, 0);
	
	unsigned int	VAO_text;
	unsigned int	VBO_indide;
	unsigned int	VBO_texture;
	int				width;
	int				height;
	int				nrChannels;
	unsigned int	texture, texture2;
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
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = ft_read_tga_headers("img/container.tga", &tga);
	// data2 = stbi_load("img/face.tga", &width, &height, &nrChannels, 0);
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
	// printf("moi -> height = %d, width %d, nbr channel = %d\n", tga.height, tga.width, tga.bpp);
	// printf("stb -> height = %d, width %d, nbr channel = %d\n", height, width, nrChannels);
	if (data)
	{
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.width, tga.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	    glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("Failed to load texture\n");
		return (-1);
	}
	free(data);


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
	unsigned int VBO_color, VBO_vertex, VAO;

	glGenVertexArrays(1, &VAO);
 	glGenBuffers(1, &VBO_vertex);
 	glGenBuffers(1, &VBO_color);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(couleurs), couleurs, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
 
	glUseProgram(gl.programID);
	// unsigned int transformLoc = glGetUniformLocation(gl.programID, "transform");

	// printf("trans = %d\n", transformLoc);
	// glUniform1i(glGetUniformLocation(gl.programID, "texture1"), 0); // set it manually
	// ourShader.setInt("texture2", 1); // or with shader class
	// printf("programme ID = %d\n", gl.programID);
	int tour;
	while(!terminer)
	{
		tour++;
		SDL_WaitEvent(&sdl.evenements);
		key = sdl.evenements.window.event;
		printf("key = %d - %C\n", key, key);
		if(key == SDL_WINDOWEVENT_CLOSE || key == 'q')
			terminer = 1;
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Nettoyage de l'écran
		// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		// glEnableVertexAttribArray(0);

		// for (int mm = 0; mm < 4; mm++)
			// printf("matrix[%d] = %f\n", mm, matrix[mm]);

		// unsigned int matrixLoc = glGetUniformLocation(gl.programID, "matrix");
		// glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, matrix);
		mat = ft_rotate(mat, 0, 0, (tour / (PI * 2)) / 2);	
		
		unsigned int matLoc = glGetUniformLocation(gl.programID, "mat_x");
		glUniform4fv(matLoc, 1, mat[0]);
		matLoc = glGetUniformLocation(gl.programID, "mat_y");
		glUniform4fv(matLoc, 1, mat[1]);
		matLoc = glGetUniformLocation(gl.programID, "mat_z");
		glUniform4fv(matLoc, 1, mat[2]);
		matLoc = glGetUniformLocation(gl.programID, "mat_w");
		glUniform4fv(matLoc, 1, mat[3]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);


		// render container
		glBindVertexArray(VAO_text);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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