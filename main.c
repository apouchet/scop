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

#include "Shaders/Shader.h"


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


	shaderBasique("shader.vs", "shader.fs");
	shaderBasique.charger();



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
