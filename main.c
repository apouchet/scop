/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/27 13:29:46 by apouchet          #+#    #+#             */
/*   Updated: 2019/01/27 19:38:12 by apouchet         ###   ########.fr       */
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

// Déclaration des points de notre cube
GLfloat cubeVertices[] =
{
	// Face de devant
	1,1,1,
	-1,1,1,
	-1,-1,1,
	1,-1,1,
	
	1, 1, -1,
	-1, 1, -1,
	
	-1, -1, -1,
	1, -1, -1,
};

GLfloat cubeColours[] =
{
	1, 0, 0,
	0, 1, 0,
	0, 1, 0,
	1, 0, 0,
	
	0, 0, 1,
	1, 1, 0,
	
	1, 1, 0,
	0, 0, 1,
};

GLubyte cubeIndices[] =
{
	0, 1, 2, 3,
	0, 4, 7, 3,
	4, 5, 6, 7,
	1, 2, 6, 5,
	2, 3, 7, 6,
	0, 1, 5, 4
};

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

int		shaders(char *name)
{
	char	*file;
	int		size;
	printf("start shaders\n");
	GLchar* vertexSource = NULL;
	GLchar* fragmentSource = NULL;
	printf("2\n");
	GLint programState = 0;
	GLint vertexSize = 0;
	printf("4\n");
	GLint fragmentSize = 0;
	GLenum errorState = GL_NO_ERROR; 
	printf("6\n");

	// Création des IDs
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	printf("vertexID OK\n");
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	printf("fragmentID OK\n");

	printf("size ok\n");
	
	printf("malloc ok\n");
	vertexSource = (GLchar*)get_file("shader.vs", vertexSource);
	fragmentSource = (GLchar*)get_file("shader.fs", fragmentSource);
	printf("get_file ok\n\n");
	printf("vertexSource = %s\n", vertexSource);
	printf("fragmentSource = %s\n", fragmentSource);


	
	
	
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
	return (0);
}

int main(int argc, char **argv)
{	
	SDL_Window*		fenetre = 0;
	SDL_GLContext	contexteOpenGL = 0;
	SDL_Event		evenements;
	int				terminer = 0;
	int				a;

	// Version d'OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	if(SDL_Init(SDL_INIT_VIDEO) < 0) // Initialisation de la SDL
	{
		printf("Erreur lors de l'initialisation de la SDL :  %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}
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
	printf("OpenGL %s\n", glGetString(GL_VERSION));



	float vertices[] = {-0.5, -0.5,   0.0, 0.5,   0.5, -0.5};
	shaders("shader.fs");
	float couleurs[] = {0.0, 204.0 / 255.0, 1.0,	0.0, 204.0 / 255.0, 1.0,	0.0, 204.0 / 255.0, 1.0};
	
	while(!terminer)
	{
		SDL_WaitEvent(&evenements);
		a = evenements.window.event;
		// printf("a = -%d- / -%c- \n", a, a);
		if(a == SDL_WINDOWEVENT_CLOSE || a == 'q')
			terminer = 1;	
		glClear(GL_COLOR_BUFFER_BIT); // Nettoyage de l'écran
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, couleurs);
		glEnableVertexAttribArray(1);
		glDrawArrays(GL_TRIANGLES, 0, 3); // On affiche le triangle
		glDisableVertexAttribArray(0); // On désactive le tableau Vertex Attrib puisque l'on en a plus besoin
		SDL_GL_SwapWindow(fenetre); // Actualisation de la fenêtre
	}
	// On quitte la SDL
	SDL_GL_DeleteContext(contexteOpenGL);
	SDL_DestroyWindow(fenetre);
	SDL_Quit();
	return (0);
}


/*

fichier modifier :
/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/System/Library/Frameworks/OpenGL.framework/Headers/gl3.h

*/
