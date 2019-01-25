/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 11:30:21 by apouchet          #+#    #+#             */
/*   Updated: 2019/01/23 12:09:31 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include <OpenGL/gl3.h>
#include <SDL2/SDL.h>

// #include "Shaders/Shader.h"

long getFileSize(FILE* pFile)
{
    long length = 0;
 
    fseek(pFile,0,SEEK_END);
 
    length = ftell(pFile);
 
    // Ne pas oublier de mettre le fichier à son début, sinon on ne peut pas le lire
    fseek(pFile,0,SEEK_SET);
 
    return length;
}




// Pour plus de simplicité, j'ajoute une fonction qui vérifie la compilation des shaders
char checkShaderCompilation(GLuint shaderID)
{
    GLint compilationStatus = 0;
 
    // Vérification de la compilation pour le vertex shader
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
 
        return 0;
    }
 
    return 1; // Pas d'erreur
}

void loadShader()
{
    // Lecture des fichiers
    // Certaines personnes aiment avoir le fichier du vertex shader avec l'extension .vert
    // et le fichier du fragment shader avec l'extension .frag
    GLchar* vertexSource = (GLchar*)readFile("data/simple.vert");
    GLchar* fragmentSource = (GLchar*)readFile("data/simple.frag");
    GLint programState = 0;
    GLint vertexSize = 0;
    GLint fragmentSize = 0;
 
    // Création des IDs
    vertexID = glCreateShader(GL_VERTEX_SHADER);
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
 
    // Vérification des fichiers
    if ( !vertexSource || !fragmentSource )
    {
        // Ici, il faudrait faire en sorte que le programme s'arrête
        deleteShader();    // Nettoyage
        return;
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
        return;
    }
 
 
    // Creation de l'ID pour le programme
    programID = glCreateProgram();
 
    // On attache les shaders ensemble
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
            return;
        }
 
        // Et on récupère le log
        glGetProgramInfoLog(programID, logSize, &logSize, log);
 
        // On affiche
        fprintf(stderr,"Erreur lors du liage du shader:\n%s",log);
 
        free(log);
        deleteShader();
        return;
    }
 
    // Voilà, nous sommes prêt
    glUseProgram(programID);
}

void	shaders(char *name)
{
	int		fd = open(name, O_RDONLY);
	long fileLength = getFileSize(fd);

	char* fileContent = (char*)malloc(fileLength+1); // +1 pour terminer la chaine proprement
	if ( fileContent == NULL )
	{
		return -3;
	}
	 
	fread(fileContent,fileLength,1,pFile);
	// Termine le tableau qui contient le shader
	fileContent[fileSize] = '\0';
	// Utilisation avec glShaderSource()
	 
	free(fileContent);
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
		return -1;
	}
	// Création du contexte OpenGL
	contexteOpenGL = SDL_GL_CreateContext(fenetre);
	if(contexteOpenGL == 0)
	{
		printf("%s\n", SDL_GetError());
		SDL_DestroyWindow(fenetre);
		SDL_Quit();
		return -1;
	}
	// Boucle principale
	printf("OpenGL %s\n", glGetString(GL_VERSION));


	// shaderBasique("shader.vs", "shader.fs");
	// shaderBasique.charger();



	float vertices[] = {-0.5, -0.5,   0.0, 0.5,   0.5, -0.5};
	while(!terminer)
	{
		SDL_WaitEvent(&evenements);
		a = evenements.window.event;
		printf("a = -%d- / -%c- \n", a, a);
		if(a == SDL_WINDOWEVENT_CLOSE || a == 'q')
			terminer = 1;	
		glClear(GL_COLOR_BUFFER_BIT); // Nettoyage de l'écran
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glEnableVertexAttribArray(0);
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
