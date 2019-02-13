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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION
#endif

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

GLuint vertexID = 0;
GLuint fragmentID = 0;
GLuint programID = 0;
GLint attributeId = 0;

typedef struct	s_tga
{
	char 		idLength;
	char		colourMap;
	char		dataType;
	short int	colourOrigin;
	short int	colourLength;
	char		colourDepth;
	short int	xOrigin;
	short int	yOrigin;
	short int	width;
	short int	height;
	char		bpp;
	char		imagedescriptor;
}				t_tga;


short	ft_short_little_debian(char a, char b)
{
	short nb;

	nb = a;
	nb <<= 8;
	nb += b;
	return (nb);
}

void ft_affich_tga_header(t_tga *tga)
{
	printf("id length :      %hhd\n", tga->idLength);
	printf("colour map :     %hhd\n", tga->colourMap);
	printf("data type :      %hhd\n", tga->dataType);
	printf("colour origin :  %hd\n", tga->colourOrigin);
	printf("colour length :  %hd\n", tga->colourLength);
	printf("colour depth :   %hhd\n", tga->colourDepth);
	printf("x origin :       %hd\n", tga->xOrigin);
	printf("y origin :       %hd\n", tga->yOrigin);
	printf("width :          %hd\n", tga->width);
	printf("height :         %hd\n", tga->height);
	printf("bpp :            %hhd\n", tga->bpp);
	printf("img descriptor : %hhd\n", tga->imagedescriptor);
}

unsigned char	*ft_read_tga(int fd, size_t size)
{
	int				rd;
	int				i;
	unsigned char	swap;
	unsigned char	*file;

	i = 0;
	if (!(file = (unsigned char*)malloc(size + 1)) ||
		(rd = read(fd, file, size)) < size)
		return (NULL);
	file[size] = '\0';
	while (i < size)
	{
		swap = file[i];
		file[i] = file[i + 2];
		file[i + 2] = swap;
		i += 3;
	}
	close(fd);
	printf("size file = %d / size = %zu\n", rd, size);
	return (file);

}

// int				ft_error_log(char *error)
// {
// 	GLint	logLength;
// 	GLchar*	log;

// 	logLength = 0;
// 	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
// 	if (!(log = (GLchar*)malloc(logLength)))
// 	{
// 		fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du shader\n");
// 		return (0);
// 	}
// 	glGetShaderInfoLog(shaderID, logLength, &logLength, log);
// 	fprintf(stderr,"%s%s",error, log);
// 	free(log);
// 	return (0);
// }

unsigned char	*ft_read_tga_headers(char *name, t_tga *tga)
{
	int				fd;
	int				rd;
	unsigned char	headers[100];

	if ((fd = open(name, O_RDONLY)) < 0 ||
		(rd = read(fd, headers, 18)) <= 0)
		return (NULL);
	headers[18] = '\0';
	tga->idLength = headers[0];
	tga->colourMap = headers[1];
	tga->dataType = headers[2];

	tga->colourOrigin = ft_short_little_debian(headers[4], headers[3]);
	tga->colourLength = ft_short_little_debian(headers[6], headers[5]);
	tga->colourDepth = headers[7];
	tga->xOrigin = ft_short_little_debian(headers[9], headers[8]);
	tga->yOrigin = ft_short_little_debian(headers[11], headers[10]);
	tga->width = ft_short_little_debian(headers[13], headers[12]);
	
	tga->height = ft_short_little_debian(headers[15], headers[14]);
	tga->bpp = headers[16];
	tga->imagedescriptor = headers[17];
	ft_affich_tga_header(tga);
	return (ft_read_tga(fd, tga->height * tga->width * (tga->bpp == 24 ? 3 : 4)));
}

void deleteShader()
{
	// On arrête d'utiliser le programme shader
	glUseProgram(0);
	// Deliage des shaders au programme
	glDetachShader(programID, fragmentID);
	glDetachShader(programID, vertexID);
	
	// Destruction du programme
	glDeleteProgram(programID);
	
	// Destruction des IDs des shaders
	glDeleteShader(fragmentID);
	glDeleteShader(vertexID);
}

int		size_file(char *name)
{
	int		fd;
	int		rd;
	long	fileLength;
	char	buff[1001];

	fileLength = 0;
	if ((fd = open(name, O_RDONLY)) < 0)
		return (-1);
	while ((rd = read(fd, buff, 1000)) > 0)
		fileLength += rd;
	close(fd);
	return (rd < 0 ? rd : fileLength);
}

char	*get_file(char *name, char *file)
{
	int		fd;
	int		rd;
	int		j;
	char	buff[1001];

	if (!(file = (char*)malloc(size_file(name) + 1)) ||
		(fd = open(name, O_RDONLY)) < 0)
		return (NULL);
	j = 0;
	while ((rd = read(fd, &file[j], 1000)) > 0)
		j += rd;
	file[j] = '\0';
	close(fd);
	if (rd < 0)
		return (NULL);
	return (file);
}

int		checkShaderCompilation(GLuint shaderID)
{
	GLint compilationStatus = 0;
		
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus != GL_TRUE)
	{
		// return (ft_error_log("Erreur de compilation:\n"));
		GLint logLength = 0;
		GLchar* log = NULL;
		
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		if (!(log = (GLchar*)malloc(logLength)))
		{
			fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du shader\n");
			return 0;
		}
		glGetShaderInfoLog(shaderID, logLength, &logLength, log);
		fprintf(stderr,"Erreur de compilation:\n%s",log);
		free(log);
		return (0);
	}
	return (1);
}

int		shaders(char *nameVS, char *nameFS)
{
	char	*file;
	int		size;
	GLchar* vertexSource = NULL;
	GLchar* fragmentSource = NULL;
	GLint programState = 0;
	GLint vertexSize = 0;
	GLint fragmentSize = 0;
	GLenum errorState = GL_NO_ERROR;

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	// printf("vertexID OK\n");
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	vertexSource = (GLchar*)get_file(nameVS, vertexSource);
	printf("vertex source :\n--------------------\n%s\n--------------------\n\n", vertexSource);
	fragmentSource = (GLchar*)get_file(nameFS, fragmentSource);
	printf("vertex source :\n--------------------\n%s\n--------------------\n\n", fragmentSource);
	// printf("get_file ok\n\n");
	if (!vertexSource || !fragmentSource)
	{
		// Ici, il faudrait faire en sorte que le programme s'arrête
		deleteShader();
		return (-1);
	}
	// Chargement des sources dans OpenGL
	vertexSize = strlen(vertexSource);
	fragmentSize = strlen(fragmentSource);
	glShaderSource(vertexID, 1, (const GLchar**)(&vertexSource), &vertexSize);
	glShaderSource(fragmentID, 1, (const GLchar**)(&fragmentSource), &fragmentSize);
	
	// Compilation du vertex shader
	glCompileShader(vertexID);
	glCompileShader(fragmentID);
	
	// Vérification des erreurs
	if (!checkShaderCompilation(vertexID) || !checkShaderCompilation(fragmentID))
	{
		deleteShader();
		return (-1);
	}

	// Creation de l'ID pour le programme
	programID = glCreateProgram();	
	// On attache les shader ensemble
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	printf("vertexID = %d\n", vertexID);
	printf("fragmentID = %d\n", fragmentID);
	// On peut enfin passer aux liage.
	glLinkProgram(programID);

	// Et encore une fois on vérifie si tout se passe bien
	glGetProgramiv(programID , GL_LINK_STATUS  , &programState);
	if (programState != GL_TRUE)
	{
		// return (ft_error_log("Erreur lors du liage du shader:\n"));
		// On récupère la taille du log
		GLint logSize = 0;
		GLchar* log = NULL;
		
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
		
		// On peut allouer la mémoire, une fois que l'on a la taille du log
		if (!(log = (GLchar*)malloc(logSize)))
		{
			fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du programme\n");
			deleteShader();
			return (-1);
		}
	
		// Et on récupère le log
		glGetProgramInfoLog(programID, logSize, &logSize, log);
		
		// On affiche
		fprintf(stderr,"Erreur lors du liage du shader:\n%s",log);
		
		free(log);
		deleteShader();
		return (-1);
	}
	printf("End Shaders\n--------------------\n\n");
	return (0);
}

int main(int argc, char **argv)
{	
	SDL_Window*		fenetre;
	SDL_GLContext	contexteOpenGL;
	SDL_Event		evenements;
	int				terminer;
	int				a;
	t_tga			tga;

	// SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	contexteOpenGL = 0;
	terminer = 0;
	fenetre = 0;
	if(SDL_Init(SDL_INIT_VIDEO) < 0) // Initialisation de la SDL
	{
		printf("Erreur lors de l'initialisation de la SDL :  %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	
	// Création de la fenêtre
	fenetre = SDL_CreateWindow("Test SDL 2.0 main.c", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if(fenetre == 0)
	{
		printf("Erreur lors de la creation de la fenetre : %s\n", SDL_GetError());
		SDL_Quit();
		return (-1);
	}
	// Création du contexte OpenGL
	contexteOpenGL = SDL_GL_CreateContext(fenetre);
	if(contexteOpenGL == 0)
	{
		printf("%s\n", SDL_GetError());
		SDL_DestroyWindow(fenetre);
		SDL_Quit();
		return (-1);
	}
	// Boucle principale

	shaders("shader.vs", "shader.fs");
	printf("OpenGL version : %s\n", glGetString(GL_VERSION));
	printf("Shader version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	float vertices[] = {-0.5, -0.5, 0.0, 0.5, 0.5, -0.5};
	float couleurs[] = {1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0};
	float text[] = {
	// positions          // colors           // texture coords
	0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // bottom left
	-0.5f, +0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	
	unsigned int VAO_text, VBO_indide, VBO_texture;
	glGenVertexArrays(1, &VAO_text);
	glGenBuffers(1, &VBO_texture);
	glGenBuffers(1, &VBO_indide);

	glBindVertexArray(VAO_text);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(text), text, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indide);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	// unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	
	// unsigned char *data = ft_read_tga_headers("img/wall1.tga", &tga);//stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	unsigned char *data = ft_read_tga_headers("img/container.tga", &tga);//stbi_load("container.jpg", &width, &height, &nrChannels, 0);
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

	glUseProgram(programID);
	printf("programme ID = %d\n", programID);
	
	while(!terminer)
	{
		SDL_WaitEvent(&evenements);
		a = evenements.window.event;
		if(a == SDL_WINDOWEVENT_CLOSE || a == 'q')
			terminer = 1;
		
		// glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Nettoyage de l'écran
		// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		// glEnableVertexAttribArray(0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		glBindVertexArray(VAO_text);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		SDL_GL_SwapWindow(fenetre); // Actualisation de la fenêtre
	}
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	// On quitte la SDL
	SDL_GL_DeleteContext(contexteOpenGL);
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
	return (0);
}


/*

https://learnopengl.com/Getting-started/Hello-Triangle-

gcc -L ~/.brew/lib -lSDL2 -I ~/.brew/include -framework OpenGL -framework Cocoa main.c

fichier modifier :
/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/OpenGL.framework/Headers/gl3.h

*/
