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

// gcc paring.c main.c ft_read_shader.c ft_read_tga.c ft_file.c -I ~/.brew/include -L ~/.brew/lib -lSDL2 -framework OpenGL -framework Cocoa libft/libft.a
#include "scop.h"

#ifndef GL_SILENCE_DEPRECATION
# define GL_SILENCE_DEPRECATION
#endif


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define WINDOWX 800
#define WINDOWY 800

typedef struct	s_matrix
{
	float		base[4][4];
	float		rotate[4][4];
	float		move[4][4];
	float		pers[4][4];

	float		top;
	float		bottom;
	float		left;
	float		right;
	float		fov;
	float		near;
	float		far;
	float		ratio;
}				t_matrix;

void	gl_perspective(t_matrix *mx) 
{ 
	float scale;

	mx->fov = 90;
	mx->near = 0.1f;
	mx->far = 100.0f;
	mx->ratio = WINDOWX / WINDOWY;
	scale = tan(mx->fov * 0.5 * PI / 180) * mx->near;

	mx->right = mx->ratio * scale;
	mx->left = -mx->right;

	mx->top = scale;
	mx->bottom = -mx->top;
}

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
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOWX, WINDOWY,
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
	printf("z = %f\n", z);
	mx->base[3][0] = x;
	mx->base[3][1] = y;
	mx->base[3][2] = z;
	mx->base[3][3] = 1;
}

void	ft_rotate(t_matrix *mx, double angleX, double angleY, double angleZ)
{
	float A;
	float B;
	float C;
	float D;
	float E;
	float F;

	A = cosf(angleX);
	B = sinf(angleX);
	C = cosf(angleY);
	D = sinf(angleY);
	E = cosf(angleZ);
	F = sinf(angleZ);
	mx->rotate[0][0] = C * E;
	mx->rotate[1][0] = B * D * E + A * F;
	mx->rotate[2][0] = -A * D * E + B * F;
	mx->rotate[3][0] = 0;

	mx->rotate[0][1] = - C * F;
	mx->rotate[1][1] = - B * D * F + A * E;
	mx->rotate[2][1] = A * D * F + B * E;
	mx->rotate[3][1] = 0;

	mx->rotate[0][2] = D;
	mx->rotate[1][2] = - B * C;
	mx->rotate[2][2] = A * C;
	mx->rotate[3][2] = 0;

	mx->rotate[0][3] = 0;
	mx->rotate[1][3] = 0;
	mx->rotate[2][3] = 0;
	mx->rotate[3][3] = 1.0f;
}

void	ft_perspective(t_matrix *mx, double fov, double ar, double near, double far)
{
	const float range = near - far;
	const float tanHalfFOV = tanf(((fov / 2.0) / 180) * PI);
	float	tanFov = tanf(((fov / 180) * PI) / 2);

	mx->pers[0][0] = 2 * mx->near / (mx->right - mx->left);
	mx->pers[1][0] = 0;
	mx->pers[2][0] = 0;
	mx->pers[3][0] = 0;

	mx->pers[0][1] = 0;
	mx->pers[1][1] = 2 * mx->near / (mx->top - mx->bottom);
	mx->pers[2][1] = 0;
	mx->pers[3][1] = 0;

	mx->pers[0][2] = 0;
	mx->pers[1][2] = 0;
	mx->pers[2][2] = -2 * mx->far * mx->near / (mx->far - mx->near);
	mx->pers[3][2] = 0;

	mx->pers[0][3] = -((mx->right + mx->left) / (mx->right - mx->left));
	mx->pers[1][3] = -((mx->top + mx->bottom) / (mx->top - mx->bottom));
	mx->pers[2][3] = -((mx->far + mx->near) / (mx->far - mx->near));
	mx->pers[3][3] = 2;
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
	t_obj obj;


	bzero(&obj, sizeof(t_obj));
	bzero(&tga, sizeof(t_tga));
	bzero(&sdl, sizeof(sdl));
	bzero(&gl, sizeof(gl));
	// ft_start_matrix(&mx);
	// float *lol = ft_parsing(&obj);	
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

	// float vertices[] = {-0.5, -0.5, 0.0, 0.5, 0.5, -0.5};
	// float couleurs[] = {1.0, 0.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0};
	// float *text = (float*)malloc(sizeof(float) * 32);
	// float *vertices = ft_parsing(&obj);
	float *vertices;
	if (argc == 2)
		ft_parsing(&obj, argv[1]);
	else
		ft_parsing(&obj, NULL);
	printf("\n");
	// float *vertices = (float*)malloc(sizeof(float) * 9);
	// // float vertices[9];
	// vertices[0] = -0.5;
	// vertices[1] = -0.5;
	// vertices[2] = 0.0;
	// vertices[3] = 0.5;
	// vertices[4] = -0.5;
	// vertices[5] = 0.0;
	// vertices[6] = 0.0;
	// vertices[7] = 0.5;
	// vertices[8] = 0.0;

	printf("sizeof vertice %lu\n", sizeof(vertices));
	// printf("nb vertex = %d, nb face = %d\n\n", obj.nbVertex, obj.face);
	
	// float vertices[] = {
 //        -0.5f, -0.5f, 0.0f, // left  
 //         0.5f, -0.5f, 0.0f, // right 
 //         0.0f,  0.5f, 0.0f  // top   
 //    };

	// for (int ww = 0; ww < 9; ww++)
	// 	printf("%f%s", vertices[ww], ((ww == 8)? "\n\n" : ", "));

	printf("face tri %d\n", obj.faceTri);
	printf("face quad %d\n", obj.faceQuad);
	for (int ii = 0; ii < obj.faceQuad * 4 ; ii += 4)
	{
		printf("quad[%d] = %f, %f, %f, %f\n", ii / 4, obj.tabVertexQuad[ii], obj.tabVertexQuad[ii + 1], obj.tabVertexQuad[ii + 2], obj.tabVertexQuad[ii + 3]);
	}

	unsigned int VBO_tri, VAO_tri;
	unsigned int VBO_vertex_tri, VBO_normal_tri, VBO_texture_tri;
	glGenVertexArrays(1, &VAO_tri);
	glGenBuffers(1, &VBO_vertex_tri);
	printf("obj.posT = %d\n", obj.posT);
	if (obj.posN)
		glGenBuffers(1, &VBO_normal_tri);
	if (obj.posT)
		glGenBuffers(1, &VBO_texture_tri);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO_tri);

	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, obj.face * 3 * 2 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex_tri);
	glBufferData(GL_ARRAY_BUFFER, obj.faceTri * 3 * 3 * sizeof(float), obj.tabVertexTri, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	if (obj.posN)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_tri);
		glBufferData(GL_ARRAY_BUFFER, obj.faceTri * 3 * 3 * sizeof(float), obj.tabNormalTri, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
	}
	if (obj.posT)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_tri);
		glBufferData(GL_ARRAY_BUFFER, obj.faceTri * 3 * 2 * sizeof(float), obj.tabTextureTri, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
	}
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.

	unsigned int VBO_quad, VAO_quad;
	unsigned int VBO_vertex_quad, VBO_normal_quad, VBO_texture_quad;
	glGenVertexArrays(1, &VAO_quad);
	glGenBuffers(1, &VBO_vertex_quad);
	if (obj.posN)
		glGenBuffers(1, &VBO_normal_quad);
	if (obj.posT)
		glGenBuffers(1, &VBO_texture_quad);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO_quad);

	// glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData(GL_ARRAY_BUFFER, obj.face * 3 * 2 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex_quad);
	glBufferData(GL_ARRAY_BUFFER, obj.faceQuad * 4 * 3 * sizeof(float), obj.tabVertexQuad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	if (obj.posN)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_quad);
		glBufferData(GL_ARRAY_BUFFER, obj.faceQuad * 4 * 3 * sizeof(float), obj.tabNormalQuad, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
	}
	if (obj.posT)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO_texture_quad);
		glBufferData(GL_ARRAY_BUFFER, obj.faceQuad * 4 * 2 * sizeof(float), obj.tabTextureQuad, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);
	}
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);

	// // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);
	
	free(obj.tabVertexQuad);
	free(obj.tabVertexTri);
	// unsigned int 	VBO_vertex;
	// unsigned int	VAO_text;
	// unsigned int	VAO_lol;
	// unsigned int	VBO_indide;
	// unsigned int	VBO_texture;
	int				width;
	int				height;
	int				nrChannels;
	unsigned int	texture1 = glGetUniformLocation(gl.programID, "Texture1");
	unsigned int	texture2 = glGetUniformLocation(gl.programID, "Texture2");
	unsigned char	*data;
	unsigned char	*data2;

	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// data = ft_read_tga_headers("obj_file/sword.tg", &tga);
	printf("texture = -|%s|-\n", obj.texture);
	if (obj.texture)
		data = ft_read_tga_headers(obj.texture, &tga);
	else
		data = ft_read_tga_headers("img/face.tga", &tga);
	// data = stbi_load("obj_file/WoodCabinDif.tga", &width, &height, &nrChannels, 0);
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
	glUniform1i(texture1, 0); // Texture unit 0 is for base images.
	glUniform1i(texture2, 1);


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
		gl_perspective(&mx);

		ft_rotate(&mx, hor, ver, tan);
		ft_perspective(&mx, fov , WINDOWY / WINDOWX, 0.01f, 100.0f);
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
		glActiveTexture(GL_TEXTURE2);
		// glBindTexture(GL_TEXTURE_2D, texture2);

		// glUseProgram(shaderProgram);
		glBindVertexArray(VAO_tri); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, obj.faceTri * 3);

		glBindVertexArray(VAO_quad);
		for (int k = 0; k < obj.faceQuad * 4; k += 4)
			glDrawArrays(GL_TRIANGLE_FAN, k, 4);

		// glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
		// glDrawArrays(GL_TRIANGLE_STRIP, 0, obj.faceQuad * 4);

		// render container
		// glBindVertexArray(VAO_lol);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 36, GL_FLOAT, 0);

		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0);

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