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

/*
** #define STB_IMAGE_IMPLEMENTATION
** #include "stb_image.h"
*/

int		ft_delete_shader(t_gl *gl)
{
	/*
	** On arrête d'utiliser le programme shader
	*/
	glUseProgram(0);
	/*
	** Deliage des shaders au programme
	*/
	glDetachShader(gl->programID, gl->fragmentID);
	glDetachShader(gl->programID, gl->vertexID);
	/*
	** Destruction du programme
	*/
	glDeleteProgram(gl->programID);
	/*
	** Destruction des IDs des shaders
	*/
	glDeleteShader(gl->fragmentID);
	glDeleteShader(gl->vertexID);
	return (-1);
}

int		ft_size_file(char *name)
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

char	*ft_get_file(char *name, char *file)
{
	int		fd;
	int		rd;
	int		j;
	char	buff[1001];

	if (!(file = (char*)malloc(ft_size_file(name) + 1)) ||
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

int		ft_checkShader_compilation(GLuint shaderID, t_gl *gl)
{
	GLint compilationStatus = 0;
		
	glGetShaderiv(gl->vertexID, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus != GL_TRUE)
	{
		// return (ft_gl_error("Error memori allocation", "liage du shader", shaderID, NULL));
		GLint logLength = 0;
		GLchar* log = NULL;
		
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		if (!(log = (GLchar*)malloc(logLength)))
		{
			fprintf(stderr,"Error memori allocation for log of shader compilation\n");
			return 0;
		}
		glGetShaderInfoLog(shaderID, logLength, &logLength, log);
		fprintf(stderr,"Compilation error :\n%s",log);
		free(log);
		return (0);
	}
	return (1);
}

int		ft_shaders(char *nameVS, char *nameFS, t_gl *gl)
{
	char	*file;
	int		size;
	t_shd	shd;

	bzero(&shd, sizeof(t_shd));
	gl->vertexID = glCreateShader(GL_VERTEX_SHADER);
	gl->fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	// shd.vertexSource = (GLchar*)ft_get_file(nameVS, shd.vertexSource);
	// shd.fragmentSource = (GLchar*)ft_get_file(nameFS, shd.fragmentSource);
	if (!(shd.vertexSource = (GLchar*)ft_get_file(nameVS, shd.vertexSource)) ||
		!(shd.fragmentSource = (GLchar*)ft_get_file(nameFS, shd.fragmentSource)))
		return (ft_delete_shader(gl));
	shd.vertexSize = strlen(shd.vertexSource);
	shd.fragmentSize = strlen(shd.fragmentSource);
	glShaderSource(gl->vertexID, 1, (const GLchar**)(&shd.vertexSource), &shd.vertexSize);
	glShaderSource(gl->fragmentID, 1, (const GLchar**)(&shd.fragmentSource), &shd.fragmentSize);
	glCompileShader(gl->vertexID);
	glCompileShader(gl->fragmentID);
	if (!ft_checkShader_compilation(gl->vertexID, gl) ||
		!ft_checkShader_compilation(gl->fragmentID, gl))
		return (ft_delete_shader(gl));
	gl->programID = glCreateProgram();	
	glAttachShader(gl->programID, gl->vertexID);
	glAttachShader(gl->programID, gl->fragmentID);
	glLinkProgram(gl->programID);

	// Et encore une fois on vérifie si tout se passe bien
	glGetProgramiv(gl->programID , GL_LINK_STATUS , &shd.programState);
	if (shd.programState != GL_TRUE)
	{
		return (ft_gl_error("Error memori allocation", "liage du shader", gl->programID, gl));
		// // return (ft_error_log("Erreur lors du liage du shader:\n"));
		// // On récupère la taille du log
		// GLint logSize = 0;
		// GLchar* log = NULL;
		// glGetProgramiv(gl->programID, GL_INFO_LOG_LENGTH, &logSize);
		// if (!(log = (GLchar*)malloc(logSize)))
		// {
		// 	fprintf(stderr,"Error memori allocation for log of program compilation\n");
		// 	return (ft_delete_shader(gl));
		// }
		// glGetProgramInfoLog(gl->programID, logSize, &logSize, log);
		// fprintf(stderr,"Erreur lors du liage du shader:\n%s",log);
		// free(log);
		// return (ft_delete_shader(gl));
	}
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

int		main(int argc, char **argv)
{	
	int		terminer;
	int		a;
	t_tga	tga;
	t_sdl	sdl;
	t_gl	gl;

	bzero(&tga, sizeof(t_tga));
	bzero(&sdl, sizeof(sdl));
	bzero(&gl, sizeof(gl));
	terminer = 0;
	if (ft_start_sdl_opengl(&sdl) < 0)
		return (-1);
	// Boucle principale

	ft_shaders("Shaders/shader.vs", "Shaders/shader.fs", &gl);
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
	printf("programme ID = %d\n", gl.programID);
	
	while(!terminer)
	{
		SDL_WaitEvent(&sdl.evenements);
		a = sdl.evenements.window.event;
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
