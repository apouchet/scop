/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/27 13:29:46 by apouchet          #+#    #+#             */
/*   Updated: 2019/02/08 00:16:30 by apouchet         ###   ########.fr       */
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

// // Déclaration des points de notre cube
// GLfloat cubeVertices[] =
// {
// 	// Face de devant
// 	1,1,1,
// 	-1,1,1,
// 	-1,-1,1,
// 	1,-1,1,
	
// 	1, 1, -1,
// 	-1, 1, -1,
	
// 	-1, -1, -1,
// 	1, -1, -1,
// };

// GLfloat cubeColours[] =
// {
// 	1, 0, 0,
// 	0, 1, 0,
// 	0, 1, 0,
// 	1, 0, 0,
	
// 	0, 0, 1,
// 	1, 1, 0,
	
// 	1, 1, 0,
// 	0, 0, 1,
// };

// GLubyte cubeIndices[] =
// {
// 	0, 1, 2, 3,
// 	0, 4, 7, 3,
// 	4, 5, 6, 7,
// 	1, 2, 6, 5,
// 	2, 3, 7, 6,
// 	0, 1, 5, 4
// };

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
	int		rd;
	unsigned char	*file;

	if (!(file = (unsigned char*)malloc(size + 1)) ||
		(rd = read(fd, file, size)) < size)
		return (NULL);
	file[size] = '\0';
	close(fd);
	printf("size file = %d / size = %zu\n", rd, size);
	return (file);

}

unsigned char	*ft_read_tga_headers(char *name, t_tga *tga)
{
	int				fd;
	int				rd;
	unsigned char	headers[19];
	// t_tga			tga;

	// printf("read : %s\n", name);
	if ((fd = open(name, O_RDONLY)) < 0 ||
		(rd = read(fd, headers, 18)) <= 0)
		return (NULL);
	// printf("fd = %d, rd = %d\n", fd, rd);
	headers[18] = '\0';
	// for (int j = 0; j < 18; j++)
	// 	printf("%.2d : %.2x | %.2d\n", j + 1, headers[j], headers[j]);
	tga->idLength = headers[0];
	tga->colourMap = headers[1];
	tga->dataType = headers[2];

	tga->colourOrigin = ft_short_little_debian(headers[4], headers[3]);
	// tga->colourOrigin = headers[4];
	// tga->colourOrigin <<= 8;
	// tga->colourOrigin += headers[3];

	tga->colourLength = ft_short_little_debian(headers[6], headers[5]);
	// tga->colourLength = headers[6];
	// tga->colourLength <<= 8;
	// tga->colourLength += headers[5];

	tga->colourDepth = headers[7];

	tga->xOrigin = ft_short_little_debian(headers[9], headers[8]);
	// tga->xOrigin = headers[9];
	// tga->xOrigin <<= 8;
	// tga->xOrigin += headers[8];

	tga->yOrigin = ft_short_little_debian(headers[11], headers[10]);
	// tga->yOrigin = headers[11];
	// tga->yOrigin <<= 8;
	// tga->yOrigin += headers[10];
	
	tga->width = ft_short_little_debian(headers[13], headers[12]);
	// tga->width = headers[13];
	// tga->width <<= 8;
	// tga->width = headers[12];
	
	tga->height = ft_short_little_debian(headers[15], headers[14]);
	// tga->height = headers[15];
	// tga->height <<= 8;
	// tga->height += headers[14];
	
	tga->bpp = headers[16];
	tga->imagedescriptor = headers[17];
	printf("height = : %hd\n", tga->height);
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
	long	fileLength = 0;
	char	buff[1001];

	if ((fd = open(name, O_RDONLY)) < 0)
		return (-1);
	printf("fd = %d\n", fd);
	
	while ((rd = read(fd, buff, 1000)) > 0)
	{
		// rd = read(fd, buff, 1000);
		// buff[rd] = '\0';
		// printf("buff = %s\n", buff);
		// if (rd <= 0)
			// return (fileLength);
		fileLength += rd;
		printf("rd = %d\n", rd);
	}
	printf("av close\n");
	close(fd);
	printf("ap close\n");

	if (rd < 0)
		return (rd);
	return (fileLength);
}

char	*get_file(char *name, char *file)
{
	int		fd;
	int		rd;
	int		i;
	int		j;
	char	buff[1001];

	if (!(file = (char*)malloc(size_file(name) + 1)))
		return (NULL);
	if ((fd = open(name, O_RDONLY)) < 0)
		return (NULL);
	printf("fd = %d\n", fd);
	j = 0;
	while ((rd = read(fd, buff, 1000)) > 0)
	{
		i = 0;
		buff[rd] = '\0';
		while (buff[i])
			file[j++] = buff[i++];
		printf("buff = %s\n", buff);
	}
	file[j] = '\0';
	close(fd);
	if (rd < 0)
		return (NULL);
	return (file);
}

// Pour plus de simplicité, j'ajoute une fonction qui vérifie la compilation des shaders
int     checkShaderCompilation(GLuint shaderID)
{
	GLint compilationStatus = 0;
		
	// Verification de la compialtion pour le vertex shader
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &compilationStatus);
	if ( compilationStatus != GL_TRUE )
	{
		// Nous savons que la compilation a échoué, donc nous devons savoir pourquoi
		// Nous devons récupéré la taille du log
		GLint logLength = 0;
		GLchar* log = NULL;
		
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		// Maintenant que nous avons la taille, nous pouvons alloué la mémoire suffisante pour ce log
		log = (GLchar*)malloc(logLength);
		if ( log == NULL )
		{
			fprintf(stderr,"Erreur d'allocation de mémoire pour le log de la compilation du shader\n");
			return 0;
		}
		glGetShaderInfoLog(shaderID, logLength, &logLength, log);
		// On peut afficher le message
		fprintf(stderr,"Erreur de compilation:\n%s",log);
		// Et on n'oublie pas de libéré la mémoire
		free(log);
		return (0);
	}
	return (1);
}

int		shaders(char *nameVS, char *nameFS)
{
	char	*file;
	int		size;
	// printf("start shaders\n");
	GLchar* vertexSource = NULL;
	GLchar* fragmentSource = NULL;
	// printf("2\n");
	GLint programState = 0;
	GLint vertexSize = 0;
	// printf("4\n");
	GLint fragmentSize = 0;
	GLenum errorState = GL_NO_ERROR; 
	// printf("6\n");

	// Création des IDs
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	// printf("vertexID OK\n");
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	// printf("fragmentID OK\n");

	// printf("size ok\n");
	
	// printf("malloc ok\n");
	vertexSource = (GLchar*)get_file(nameVS, vertexSource);
	fragmentSource = (GLchar*)get_file(nameFS, fragmentSource);
	// printf("get_file ok\n\n");
	// printf("vertexSource = %s\n", vertexSource);
	// printf("fragmentSource = %s\n", fragmentSource);


	
	
	
	// Lecture des fichiers
	// Certaines personnes aiment avoir le fichier du vertex shader avec l'extion .vert
	// et le fichier du fragement shader avec l'extension .frag
	// vertexSource = (GLchar*)readFile("data/simple.vert");
	// fragmentSource = (GLchar*)readFile("data/simple.frag");
	
	if ( !vertexSource || !fragmentSource )
	{
		// Ici, il faudrait faire en sorte que le programme s'arrête
		deleteShader();    // Nettoyage
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
	if ( !checkShaderCompilation(vertexID) || !checkShaderCompilation(fragmentID))
	{
		deleteShader();
		return (-1);
	}

	// Creation de l'ID pour le programme
	programID = glCreateProgram();	
	// On attache les shader ensemble
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	// On peut enfin passer aux liage.
	glLinkProgram(programID);

	// Et encore une fois on vérifie si tout se passe bien
	glGetProgramiv(programID , GL_LINK_STATUS  , &programState);
	if ( programState != GL_TRUE)
	{
		// On récupère la taille du log
		GLint logSize = 0;
		GLchar* log = NULL;
		
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logSize);
		
		// On peut allouer la mémoire, une fois que l'on a la taille du log
		log = (GLchar*)malloc(logSize);
		if ( log == NULL )
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
	/*
	attributeId = glGetAttribLocation(programID, "couleur");
	errorState = glGetError();
	if ( attributeId == -1 || errorState != GL_NO_ERROR )
	{
		fprintf(stderr,"Erreur (%d) lors de la récupération de l'id de la variable attribute 'couleur'\n",errorState);
	}

	// Voilà, nous sommes prêt
	glUseProgram(programID);
	
	glEnableVertexAttribArray(attributeId);
	errorState = glGetError();
	
	glVertexAttribPointer(attributeId, 3, GL_FLOAT, GL_TRUE, 0, cubeColours);
	if ( errorState != GL_NO_ERROR )
	{
		fprintf(stderr,"Erreur (%d) lors du passage du tableau de valeur à la variable attribute 'couleur'\n",errorState);
	}
	*/
	return (0);
}

int main(int argc, char **argv)
{	
	SDL_Window*		fenetre = 0;
	SDL_GLContext	contexteOpenGL = 0;
	SDL_Event		evenements;
	int				terminer = 0;
	int				a;
	t_tga			tga;


	// SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) // Initialisation de la SDL
	{
		printf("Erreur lors de l'initialisation de la SDL :  %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	// Version d'OpenGL
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	// SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	

	// Création de la fenêtre
	fenetre = SDL_CreateWindow("Test SDL 2.0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
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
	// float couleurs[] = {0.0, 204.0 / 255.0, 1.0,	0.0, 204.0 / 255.0, 1.0,	0.0, 204.0 / 255.0, 1.0};
	float couleurs[] = {1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0};

	// float test[] = {
 //    // positions          // colors           // texture coords
 //     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
 //     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
 //    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
 //    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	// };
	unsigned int VBO_color, VBO_vertex, VAO;


	// unsigned int texture;
	// glGenTextures(1, &texture);
	// glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	// int width, height, nrChannels;
	// unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	
	// unsigned char *data = ft_read_tga_headers("img/wall1.tga", &tga);//stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	// if (data)
	// {
	//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga.width, tga.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//     glGenerateMipmap(GL_TEXTURE_2D);
	// }
	// else
	//     printf("Failed to load texture\n");

	glGenVertexArrays(1, &VAO);
 	glGenBuffers(1, &VBO_color);
 	glGenBuffers(1, &VBO_vertex);
	glBindVertexArray(VAO); 
	
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// glEnableVertexAttribArray(2); 

	// glBindTexture(GL_TEXTURE_2D, texture);
	// glBindVertexArray(VAO);
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(test), test, GL_STATIC_DRAW);
	// // glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	// glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(couleurs), couleurs, GL_STATIC_DRAW);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColours), cubeColours, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	// glBindVertexArray(0); 


	glUseProgram(programID);
	
	float	frame = 0;
	while(!terminer)
	{
		SDL_WaitEvent(&evenements);
		a = evenements.window.event;
		// printf("a = -%d- / -%c- \n", a, a);
		if(a == SDL_WINDOWEVENT_CLOSE || a == 'q')
			terminer = 1;
		// printf("couleur = %f\n", couleurs[0]);

		// frame++;
		//printf("frame = %f\n", frame / 60);
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Nettoyage de l'écran
		// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		// glEnableVertexAttribArray(0);

		// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, couleurs);
		// glEnableVertexAttribArray(1);

		// glDrawArrays(GL_TRIANGLES, 0, 3); // On affiche le triangle

		// glDisableVertexAttribArray(1); // On désactive le tableau Vertex Attrib puisque l'on en a plus besoin
		// glDisableVertexAttribArray(0); // On désactive le tableau Vertex Attrib puisque l'on en a plus besoin
		// float timeValue = (float)(SDL_GetTicks()) / 1000;
		// // printf("time : %f\n", timeValue);
		// float redValue = sin(timeValue) / 2.0f + 0.5f;
		// int vertexColorLocation = glGetUniformLocation(programID, "ourColor");
		// glUniform4f(vertexColorLocation, redValue, 0.0f, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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
