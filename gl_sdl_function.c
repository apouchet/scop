
#include "scop.h"

void		gl_perspective(t_matrix *mx) 
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

int			ft_gl_error(char *msg, char *where, GLuint ID, t_gl *gl)
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

int			ft_start_sdl_opengl(t_sdl *sdl)
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

static void	ft_creat_glBuffer(t_obj *obj, unsigned int VBO[3], int type)
{
	int		size;
	float	*tab[3];

	size = (type == 1 ? obj->faceQuad * 4 : obj->faceTri * 3);
	tab[0] = obj->tabVertexTri;
	tab[1] = obj->tabNormalTri;
	tab[2] = obj->tabTextureTri;
	if (type == 1)
	{
		tab[0] = obj->tabVertexQuad;
		tab[1] = obj->tabNormalQuad;
		tab[2] = obj->tabTextureQuad;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), tab[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, size * 3 * sizeof(float), tab[1], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, size * 2 * sizeof(float), tab[2], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
}

void		ft_glBuffer(t_obj *obj, t_gl *gl)
{
	glGenVertexArrays(1, &gl->VAO_tri);
	glGenBuffers(1, &gl->VBO_tri[0]);
	glGenBuffers(1, &gl->VBO_tri[1]);
	glGenBuffers(1, &gl->VBO_tri[2]);
	glBindVertexArray(gl->VAO_tri);
	ft_creat_glBuffer(obj, gl->VBO_tri, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
	glGenVertexArrays(1, &gl->VAO_quad);
	glGenBuffers(1, &gl->VBO_quad[0]);
	glGenBuffers(1, &gl->VBO_quad[1]);
	glGenBuffers(1, &gl->VBO_quad[2]);
	glBindVertexArray(gl->VAO_quad);
	ft_creat_glBuffer(obj, gl->VBO_quad, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
	glBindVertexArray(0);
	free(obj->tabVertexQuad);
	free(obj->tabVertexTri);
}