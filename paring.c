/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 15:00:05 by apouchet          #+#    #+#             */
/*   Updated: 2019/02/19 22:29:02 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <unistd.h>
// #include "libft/libft.h"
#include "scop.h"

int		ft_check_extention(char *s, char *ex)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	while (i > 0 && s[i] != '.')
		i--;
	if (i >= 0 && ft_strcmp(&s[i], ex) == 0)
		return (1);
	else
		return (0);
}

void	ft_exit_pars(int type, char *msg, int l, char *line)
{
	if (type == 1)
	{
		printf("ERROR LINE %d : %s\n", l, msg);
		printf("Line : \"%s\"\n", line);
	}
	else if (type == 2)
		printf("ERROR : %s\n", msg);
	if (type == 3)
	{
		printf("ERROR : %s\n", msg);
		printf("Line : \"%s\"\n", line);
	}
	if (type == 4)
		printf("ERROR : %s - %s\n", msg, line);
	exit(0);
}

int		ft_get_nb_value(char *s)
{
	int nb;
	int i;

	i = 0;
	nb = 0;
	// if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.' || s[i] == '-')
	// 	nb ++;
	while (s[i])
	{
		while (ft_isspace(s[i]))
			i++;
		if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.' || s[i] == '-')
			nb++;
		while ((s[i] >= '0' && s[i] <= '9') || s[i] == '.' || s[i] == '-' || s[i] == '/')
			i++;
	}
	// printf("--------------- face type = %d ---------------\n", nb);
	return (nb);
}

void			ft_other_command(t_obj *obj, char *line, size_t l)
{
	size_t	tmp;

	if (ft_strncmp(line, "f ", 2) == 0)
	{
		if ((tmp = ft_get_nb_value(&line[2])) < 3 || tmp > 4)
			ft_exit_pars(1, "Invalide Type, Can Only Read 3 or 4 Vertex Per Face", l, line);
		(ft_get_nb_value(&line[2]) == 3) ? obj->faceTri++ : obj->faceQuad++;
	}
	else if (ft_strncmp(line, "usemtl ", 7) == 0)
	{
		obj->texture = ft_strtrim(&line[7]); // strdup !!!
		if (ft_strncmp(obj->texture, "None\0 ", 5) != 0 && ft_strncmp(obj->texture, "Material\0 ", 9) != 0)
		{
			if (ft_check_extention(obj->texture, ".tga") == 1)
				obj->texture = ft_strjoin_free(&obj->path, &obj->texture, 2);
			else
				ft_exit_pars(1, "Invalide Texture, Only Read TGA File", l, line);
		}
		else
			ft_strdel(&obj->texture);
	}
	else if (ft_strncmp(line, "mtllib ", 7) && ft_strncmp(line, "s ", 2) && ft_strncmp(line, "o ", 2)
		&& ft_strncmp(line, "g ", 2) && line[0] != '#' && line[ft_while_space(line, 0)])
			ft_exit_pars(1, "Unknow Command", l, line);
}

// void			ft_count_type_v(t_obj, line, )

static int		ft_size_file_pars(t_obj *obj, size_t *nbVertex, size_t *nbTexture, size_t *nbNormal)
{
	int		fd;
	int		get;
	size_t	l;
	char	*line;

	l = 0;
	if ((fd = open(obj->fileName, O_RDONLY)) < 0)
		ft_exit_pars(2, "Fail To Open File", 0, NULL);
	while ((get = get_next_line(fd, &line)) > 0)
	{
		l++;
		if (ft_strncmp(line, "v ", 2) == 0)// && obj->face == 0)
			(*nbVertex)++;
		else if (ft_strncmp(line, "vt ", 3) == 0)// && obj->face == 0)
			(*nbTexture)++;
		else if (ft_strncmp(line, "vn ", 3) == 0)// && obj->face == 0)
			(*nbNormal)++;
		else
			ft_other_command(obj, line, l);
		free(line);
	}
	free(line);
	close(fd);
	return (get < 0 ? -1 : 1);
}

void		ft_alloc(t_obj *obj, size_t nbVertex, size_t nbTexture, size_t nbNormal)
{
	if (!(obj->v = (float*)ft_memalloc(sizeof(float) * (nbVertex * 3)))
		|| !(obj->vt = (float*)ft_memalloc(sizeof(float) * (nbTexture * 2)))
		|| !(obj->vn = (float*)ft_memalloc(sizeof(float) * (nbNormal * 3))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	if (obj->faceTri > 0
		&& (!(obj->tabVertexTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 3)))
		|| !(obj->tabNormalTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 3)))
		|| !(obj->tabTextureTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 2)))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	if (obj->faceQuad > 0
		&& (!(obj->tabVertexQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 3)))
		|| !(obj->tabNormalQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 3)))
		|| !(obj->tabTextureQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 2)))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
}

int		ft_check_value(char *s, int max)
{
	int i;
	int dot;
	int size;

	i = (s[0] == '-' ? 1 : 0);
	size = 0;
	dot = (max > 20 ? 0 : -1);
	while (s[i] == '0')
		i++;
	while (ft_isdigit(s[i]) || s[i] == '.')
	{
		if (s[i] == '.')
			dot = (max > 20 ? dot + 1: 10);
		else if (dot <= 0)
			size++;
		i++;
	}
	if (size < max && dot <= 1)
		return (1);
	s[i] = '\0';
	if (size > max)
		printf("Error Value To Big - More Than %d Character In : %s\n", max, s);
	else
		printf("Error - To Many Dot In : %s\n", s);
	return (-1);
}

void		ft_get_value(float **tab, int nb, char *line, size_t *pos)
{
	int	i;
	float *a;

	a = *tab;
	i = 0; 
	/// ????????
	// if (ft_count_word(line) != nb)
	// {
	// 	printf("line = %s\n", line);
	// 	printf("word = %d \n", ft_count_word(line));
	// 	printf("nb = %d \n", nb);
	// 	ft_exit_pars(3, "Invalid Number Of Value", 0, line);
	// }
	/// ????????

	while (ft_isspace(line[i]) || ft_isdigit(line[i]) || line[i] == '.' || line[i] == '-' || line[i] == '+')
		i++;
	if (line[i])
		ft_exit_pars(3, "Invalid Line", 0, line);
	i = 0;
	while (nb > 0)
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (ft_check_value(&line[i], 35) < 0) // size max of numbers
			exit(0);
		a[(*pos)++] = ft_atof(&line[i]);
		i = ft_find_space(line, i);
		nb--;
	}
}

void		ft_fill(t_obj *obj, size_t a, int size, int type)
{
	int i;

	i = 0;
	// if (a + size - 1 > obj->nbVertex * size) !!!!!!!!!!!!!!!!!!!!!!!
	// 	return ;
	if (type < 10 && a + size - 1> obj->faceTri * 3 * 3)
	{
		printf("fail tri\n");
		exit(0);
	}
	if (type >= 10 && a + size  - 1> obj->faceQuad * 4 * 3)
	{
		printf("fail quad\n");
		exit(0);
	}
	while (i < size)
	{
		if (type == 1)
			obj->tabVertexTri[obj->tabVTri++] = obj->v[a * size + i++];
		else if (type == 10)
			obj->tabVertexQuad[obj->tabVQuad++] = obj->v[a * size + i++];
		else if (type == 2)
			obj->tabNormalTri[obj->tabNTri++] = obj->vn[a * size + i++];
		else if (type == 20)
			obj->tabNormalQuad[obj->tabNQuad++] = obj->vn[a * size + i++];
		else if (type == 3)
			obj->tabTextureTri[obj->tabTTri++] = obj->vt[a * size + i++];
		else if (type == 30 )
			obj->tabTextureQuad[obj->tabTQuad++] = obj->vt[a * size + i++];
	}
}

void	ft_get_point(t_obj *obj, char *line, int type, int *i)
{
	size_t a[3];
	size_t step;

	step = 0;
	while (line[*i] && !ft_isspace(line[*i]))
	{
		if (line[*i] == '-')
			ft_exit_pars(4, "Negative Value Imposible", 0, line);
		if (ft_isdigit(line[*i]))
			a[step++] = ft_atol(&line[*i]) - 1;
		else if (line[*i - 1] == '/' && (line[*i] == '/' || !line[*i] || ft_isspace(line[*i])))
			a[step++] = 0;
		else
		// {
			ft_exit_pars(4, "Bad Syntaxe", 0, line);
		// 	printf("fail -%s- i = %d\n", &line[*i], *i);
		// 	exit(-1);
		// }
		// a[step++] = ft_atol(&line[i]) - 1; 				//				//					/// creat atoi - > size_t!!!
		while (ft_isdigit(line[*i]))
			(*i)++;
		if (line[*i] == '/')
			(*i)++;
	}
	if (step == 0 || step > 3)
		exit(-1);
	ft_fill(obj, a[0], 3, 1 * type);
	if (step >= 2)
		ft_fill(obj, a[1], 2, 3 * type);
	if (step == 3)
		ft_fill(obj, a[2], 3, 2 * type);
}

void		ft_get_face(t_obj *obj, char *line)
{
	int		i;
	int		type;

	i = 0;
	type = ft_get_nb_value(line) == 3 ? 1 : 10;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (line[i])
			ft_get_point(obj, line, type, &i);
	}
}


void		ft_get_data(t_obj *obj, size_t *posVertex)
{
	int		fd;
	int		get;
	char	*line;
	size_t	posTexture;
	size_t	posNormal;

	posTexture = 0;
	posNormal = 0;
	if ((fd = open(obj->fileName, O_RDONLY)) < 0)
		ft_exit_pars(4, "Can't Read File", 0, obj->fileName);
	while ((get = get_next_line(fd, &line)) > 0)
	{
		if (ft_strncmp(line, "v ", 2) == 0)
			ft_get_value(&obj->v, 3, &line[2], posVertex);
		else if (ft_strncmp(line, "vt ", 3) == 0)
			ft_get_value(&obj->vt, 2, &line[3], &posTexture);
		else if (ft_strncmp(line, "vn ", 3) == 0)
			ft_get_value(&obj->vn, 3, &line[3], &posNormal);
		else if (ft_strncmp(line, "f ", 2) == 0)
			ft_get_face(obj, &line[2]);
		free(line);
	}
	// for (int i = 0; i < *posVertex; i++)
		// printf("v[%d] = %f\n", i, obj->v[i]);
	free(line);
	close(fd);
	if (get < 0)
		ft_exit_pars(2, "Read File Error", 0, NULL);
}

char 	*ft_get_path(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	while (i > 0 && s[i] != '/')
		i--;
	if (i > 0)
		return (ft_strndup(s, i + 1));
	else
		return (ft_strdup("./"));
}

void	ft_min_max(t_obj *obj, float *tab, size_t *posVertex)
{
	int i;

	i = 0;
	while (i < *posVertex)
	{
		if(tab[0] > obj->v[i])
			tab[0] = obj->v[i];
		else if(tab[1] < obj->v[i])
			tab[1] = obj->v[i];
		i++;
		if(tab[2] > obj->v[i])
			tab[2] = obj->v[i];
		else if(tab[3] < obj->v[i])
			tab[3] = obj->v[i];
		i++;
		if(tab[4] > obj->v[i])
			tab[4] = obj->v[i];
		else if(tab[5] < obj->v[i])
			tab[5] = obj->v[i];
		i++;
	}
}

void	ft_center(t_obj *obj, size_t *posVertex)
{
	float midX;
	float midY;
	float midZ;
	float *tab;
	int i;

	if (!(tab = (float*)ft_memalloc(sizeof(float) * 6)))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	ft_min_max(obj, tab, posVertex);
	midX = (tab[1] + tab[0]) / 2;
	midY = (tab[3] + tab[2]) / 2;
	midZ = (tab[5] + tab[4]) / 2;
	i = 0;
	while (i < obj->tabVTri)
	{
		obj->tabVertexTri[i++] -= midX;
		obj->tabVertexTri[i++] -= midY;
		obj->tabVertexTri[i++] -= midZ;
	}
	i = 0;
	while (i < obj->tabVQuad)
	{
		obj->tabVertexQuad[i++] -= midX;
		obj->tabVertexQuad[i++] -= midY;
		obj->tabVertexQuad[i++] -= midZ;
	}
	free(tab);
}

void 	ft_parsing(t_obj *obj, char *name)
{
	size_t posVertex;
	size_t nbVertex;
	size_t nbTexture;
	size_t nbNormal;

	nbVertex = 0;
	nbTexture = 0;
	nbNormal = 0;
	posVertex = 0;
	if (name == NULL)
		obj->fileName = "lowtri.obj";
	else if (ft_check_extention(name, ".obj"))
	{
		obj->fileName = name;
		obj->path = ft_get_path(name);
		printf("path = %s\n", obj->path);
	}
	else
		ft_exit_pars(2, "Invalid File Name, Need To Be A \".obj\"", 0, NULL);
	printf("file name = %s\n", obj->fileName);
	if (ft_size_file_pars(obj, &nbVertex, &nbTexture, &nbNormal) < 0)
		exit(0);
	printf("vertex  = %zu\n", nbVertex);
	printf("texture = %zu\n", nbTexture);
	printf("normal  = %zu\n", nbNormal);
	printf("nb face Tri = %d\n", obj->faceTri);
	printf("nb face Quad = %d\n", obj->faceQuad);
	printf("nb face total = %d\n", obj->faceQuad + obj->faceTri);
	printf("texture name = -%s-\n", obj->texture);
	// obj->posI = 0;
	// obj->tabVTri = 0;
	// obj->tabNTri = 0;
	// obj->tabTTri = 0;
	// obj->tabVQuad = 0;
	// obj->tabNQuad = 0;
	// obj->tabTQuad = 0;
	// obj->size = (3 + 3 + 2) * 3 * (obj->faceTri + obj->faceQuad);
	
	ft_alloc(obj, nbVertex, nbTexture, nbNormal);
	ft_get_data(obj, &posVertex);
	ft_center(obj, &posVertex);
}















