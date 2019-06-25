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

void	ft_exit_pars(int type, char *msg, int line)
{
	if (type == 1)
		printf("ERROR LINE %d : %s\n", line, msg);
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

static int		ft_size_file_pars(t_obj *obj)
{
	int		fd;
	int		get;
	int		l;
	char	*line;

	l = 0;
	printf("obj->fileName = %s\n", obj->fileName);
	if ((fd = open(obj->fileName, O_RDONLY)) < 0)
	{
		printf("Error fd = %d\n", fd);
		return (-1);
	}
	printf("fd = %d\n", fd);
	while ((get = get_next_line(fd, &line)) > 0)
	{
		l++;
		// printf("line : %d\n", l);
		
		if (ft_strncmp(line, "v ", 2) == 0)// && obj->face == 0)
			obj->nbVertex++;
		else if (ft_strncmp(line, "vt ", 3) == 0)// && obj->face == 0)
			obj->nbTexture++;
		else if (ft_strncmp(line, "vn ", 3) == 0)// && obj->face == 0)
			obj->nbNormal++;
		else if ((ft_strncmp(line, "v ", 2) == 0 || ft_strncmp(line, "vt ", 3) == 0 || ft_strncmp(line, "vn ", 3) == 0) && (obj->faceTri || obj->faceQuad))
		{
			printf("ERROR LINE %d: -|%s|-\n", l, line);
			printf("Please Mouve All \"face\" At The End Of The File\n");
			return (-1);
		}
		else if (ft_strncmp(line, "f ", 2) == 0)
		{
			if (!obj->faceTri && !obj->faceQuad)
			{
				obj->type_face = ft_get_nb_value(&line[2]);
				if (obj->type_face < 3 || obj->type_face > 4)
				{
					printf("ERROR LINE %d: -|%s|-\n", l, line);
					printf("ERROR : Invalide Type Face - %d\n", obj->type_face);
					return (-1);
				}
			}
			// else if (obj->type_face != ft_get_nb_value(&line[2]))
			// {
			// 	printf("ERROR LINE %d: -|%s|-\n", l, line);
			// 	printf("ERROR - Invalide Type Face\n");
			// 	return (-1);
			// }
			if (ft_get_nb_value(&line[2]) == 3)
			{
				printf("line Tri = %s\n", line);
				obj->faceTri++;
			}
			else if (ft_get_nb_value(&line[2]) == 4)
			{
				// printf("line Quad = %s\n", line);
				obj->faceQuad++;
			}
			else
				exit (-1);
		}
		else if (ft_strncmp(line, "usemtl ", 7) == 0)
		{
			if (ft_strncmp(ft_strtrim(&line[7]), "None\0 ", 5) != 0)
			{
				obj->texture = ft_strtrim(ft_strdup(&line[7]));
				obj->texture = ft_strjoin_free(&obj->path, &obj->texture, 2);
			}
			// check .tga texture
		}
		else if (ft_strncmp(line, "mtllib ", 7) && ft_strncmp(line, "s ", 2) && ft_strncmp(line, "o ", 2) && ft_strncmp(line, "g ", 2) && line[0] != '#' && line[ft_while_space(line, 0)])
		{
			printf("ERROR LINE %d: -|%s|-\n", l, line);
			printf("mtllib : %d\n", ft_strncmp(line, "mtllib ", 7));
			printf("s : %d\n", ft_strncmp(line, "s ", 2));
			printf("# :  %d\n", line[0] != '#');
			printf("vide :  %d\n", !line[ft_while_space(line, 0)]);
			free(line);
			return (-1);
		}

		// if (line && line[0] == 'v')
		// {
		// 	if (line[1] == ' ')
		// 		obj->nbVertex++;
		// 	else if (line[1] == 't')
		// 		obj->nbTexture++;
		// 	else if (line[1] == 'n')
		// 		obj->nbNormal++;
		// }
		// else if (line && line[0] == 'f' && line[1] == ' ')
		// 	obj->face++;
		free(line);
	}
	printf("fin : %s, get = %d\n", line, get);
	free(line);
	close(fd);
	return (get < 0 ? -1 : 1);
}

int		ft_alloc(t_obj *obj)
{
	if (!(obj->v = (float*)ft_memalloc(sizeof(float) * (obj->nbVertex * 3))) ||
		!(obj->vt = (float*)ft_memalloc(sizeof(float) * (obj->nbTexture * 2))) ||
		!(obj->vn = (float*)ft_memalloc(sizeof(float) * (obj->nbNormal * 3))))
		return (-1);
	return (1);
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
		// printf("infini, size = %d\n", size);
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

int		ft_get_value(float **tab, int nb, char *line, int *pos)
{
	int	i;
	float *a;

	a = *tab;
	i = 0;
	if (ft_count_word(line) != nb)
		return (-1);
	while (ft_isspace(line[i]) || ft_isdigit(line[i]) || line[i] == '.' || line[i] == '-' || line[i] == '+')
		i++;
	if (line[i])
		return (-1);
	i = 0;
	while (nb > 0)
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		// printf("line = -|%s|-\n", &line[i]);
		ft_check_value(&line[i], 35);
		a[(*pos)++] = ft_atof(&line[i]);
		// printf("v[%d] = %f\n",*pos - 1, a[*pos - 1]);
		// (*pos)++;
		i = ft_find_space(line, i);
		nb--;
	}
	// printf("\n");
	return (1);
}

int		ft_fill(t_obj *obj, size_t a, int size, int mode, int type)
{
	int i;

	i = 0;
	printf("a = %lu, size = %d, a + size - 1 = %lu, nb = %d, nb * size = %d\n", a, size, a + size - 1, obj->nbVertex, obj->nbVertex * size);
	if (a + size - 1 > obj->nbVertex * size)
		return (-1);
	while (i < size)
	{
		if (mode == 1 && type == 1)
		{
			obj->tabVertexTri[obj->tabVTri++] = obj->v[a * size + i++];
			printf("vertex Tri[%lu] = %f     --------------------------------------\n", obj->tabVTri - 1, obj->tabVertexTri[obj->tabVTri - 1]);
		}
		else if (mode == 1 && type == 2)
		{
			obj->tabVertexQuad[obj->tabVQuad++] = obj->v[a * size + i++];
			printf("vertex Quad[%lu] = %f     --------------------------------------\n", obj->tabVQuad - 1, obj->tabVertexQuad[obj->tabVQuad - 1]);
		}
		// else
		// 	i++;
		else if (mode == 2 && type == 1)
		{
			obj->tabNormalTri[obj->tabNTri++] = obj->vn[a * size + i++];
			// printf("normal[%lu] = %f     --------------------------------------\n", obj->tabN - 1, obj->tabVertexTri[obj->tabN - 1]);
		}
		else if (mode == 2 && type == 2)
			obj->tabNormalQuad[obj->tabNQuad++] = obj->vn[a * size + i++];
		else if (mode == 3 && type == 1)
		{
			obj->tabTextureTri[obj->tabTTri++] = obj->vt[a * size + i++];
			// printf("texture[%lu] = %f     --------------------------------------\n", obj->tabT - 1, obj->tabVertexTri[obj->tabT - 1]);
		}
		else if (mode == 3 && type == 2)
			obj->tabTextureQuad[obj->tabTQuad++] = obj->vt[a * size + i++];
		// printf("indices[%lu] = %f     --------------------------------------\n", obj->posI - 1, obj->indices[obj->posI - 1]);
		// printf("vertex[%lu] = %f     --------------------------------------\n", obj->tabV - 1, obj->tabVertexTri[obj->posI - 1]);
	}
	return (0);
}

int		ft_get_face(t_obj *obj, char *line)
{
	int		i;
	size_t	a;
	int		tour;
	int		type;

	i = 0;
	tour = 0;
	type = ft_get_nb_value(line) == 3 ? 1 : 2;
	// printf("ici\n");
	while (line[i])
	{
		tour++;
		// printf("\n");
		// printf("tour = %d\n", tour);
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (line[i] == '-')
		{
			printf("Error - Inpossible Value In : %s\n", line);
			return (-1);
		}
		ft_check_value(&line[i], 18);
		a = ft_atol(&line[i]) - 1; 				//				//					/// creat atoi - > size_t!!!
		// printf("a = %ld\n", a);
		if (tour % 3 == 1 || (obj->nbNormal == 0 && obj->nbTexture == 0))
		{
			printf("UN\n");
			ft_fill(obj, a, 3, 1, type);
		}
		else if (tour % 3 == 2)
			ft_fill(obj, a, 2, 3, type);
		// 	ft_fill(obj, obj->vn, a, 3);
		else if (tour % 3 == 0)
			ft_fill(obj, a, 3, 2, type);
		// 	ft_fill(obj, obj->vt, a, 2);
		printf("av line = %s\n", &line[i]);
		while (ft_isdigit(line[i]))
			i++;
		while (ft_isspace(line[i]))
			i++;
		printf("ap line = %s\n", &line[i]);
	}
	return (0);
}

int		ft_get_data(t_obj *obj)
{
	int		fd;
	int		get;
	char	*line;
	int		value;
	int		i;

	i = 0;
	fd = open(obj->fileName, O_RDONLY);
	if (ft_alloc(obj) == -1)
		return (-1);
	while ((get = get_next_line(fd, &line)) > 0)
	{
		i++;
		if (ft_strncmp(line, "v ", 2) == 0)
			value = ft_get_value(&obj->v, 3, &line[2], &obj->posV);
		else if (ft_strncmp(line, "vt ", 3) == 0)
			value = ft_get_value(&obj->vt, 2, &line[3], &obj->posT);
		else if (ft_strncmp(line, "vn ", 3) == 0)
			value = ft_get_value(&obj->vn, 3, &line[3], &obj->posN);
		else if (ft_strncmp(line, "f ", 2) == 0)
		{
			printf("ok - %d\n", i);
			ft_get_face(obj, &line[2]);
		}
		printf("ici\n");
		free(line);
		// printf("la\n");
	}
	printf("posV = %zu, posT = %zu, posN = %zu\n", obj->posV, obj->posT, obj->posN);
	// for (int j = 0; j < obj->nbVertex * 3; j++)
	// 	printf("j = %d = %f\n", j, obj->v[j]);
	free(line);
	close(fd);
	return (get < 0 ? -1 : 1);
}

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

void	ft_min_max(t_obj *obj, float tab[6])
{
	int i;

	i = 0;
	while (i < obj->posV)
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

void	ft_center(t_obj *obj)
{
	float midX;
	float midY;
	float midZ;
	float tab[6];
	int i;

	ft_min_max(obj, tab);
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
}

void 	ft_parsing(t_obj *obj, char *name)
{
	if (name == NULL)
		obj->fileName = "lowtri.obj";
	else if (ft_check_extention(name, ".obj"))
	{
		obj->fileName = name;
		obj->path = ft_get_path(name);
		printf("path = %s\n", obj->path);
	}
	else
	{
		printf("Invalid File Name : %s\n", name);
		exit(-2);
	}
	printf("file name = %s\n", obj->fileName);
	if (ft_size_file_pars(obj) < 0)
		exit(0);
	printf("vertex  = %d\n", obj->nbVertex);
	printf("texture = %d\n", obj->nbTexture);
	printf("normal  = %d\n", obj->nbNormal);
	printf("nb face Tri = %d\n", obj->faceTri);
	printf("nb face Quad = %d\n", obj->faceQuad);
	printf("nb face total = %d\n", obj->faceQuad + obj->faceTri);
	printf("texture name = -%s-\n", obj->texture);
	obj->posI = 0;
	obj->tabVTri = 0;
	obj->tabNTri = 0;
	obj->tabTTri = 0;
	obj->tabVQuad = 0;
	obj->tabNQuad = 0;
	obj->tabTQuad = 0;
	obj->size = (3 + 3 + 2) * 3 * (obj->faceTri + obj->faceQuad);
	if (obj->faceTri > 0
		&& (!(obj->tabVertexTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 3)))
		|| !(obj->tabNormalTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 3)))
		|| !(obj->tabTextureTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 2)))))
		exit (0);
	if (obj->faceQuad > 0
			&& (!(obj->tabVertexQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 3)))
			|| !(obj->tabNormalQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 3)))
			|| !(obj->tabTextureQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 2)))))
		exit (0);
	ft_get_data(obj);
	ft_center(obj);
}



		// if (tour % 3 == 1)
		// {
		// 	printf("un\n");
		// 	if (a + 2 > obj->nbVertex * 3)
		// 		return (-1);
		// 	obj->indices[obj->posI++] = obj->v[a * 3];
		// 	printf("indices[%lu] = %f\n", obj->posI - 1, obj->indices[obj->posI - 1]);
		// 	obj->indices[obj->posI++] = obj->v[a * 3 + 1];
		// 	printf("indices[%lu] = %f\n", obj->posI - 1, obj->indices[obj->posI - 1]);
		// 	obj->indices[obj->posI++] = obj->v[a * 3 + 2];
		// 	printf("indices[%lu] = %f\n", obj->posI - 1, obj->indices[obj->posI - 1]);
		// }
		// if (tour % 3 == 2)
		// {
		// 	printf("deux\n");
		// 	if (a + 2 > obj->nbNormal * 3)
		// 		return (-1);
		// 	obj->indices[obj->posI++] = obj->vn[a * 3];
		// 	obj->indices[obj->posI++] = obj->vn[a * 3 + 1];
		// 	obj->indices[obj->posI++] = obj->vn[a * 3 + 2];
		// }
		// if (tour % 3 == 0)
		// {
		// 	printf("trois\n");
		// 	if (a + 1 > obj->nbTexture * 2)
		// 		return (-1);
		// 	obj->indices[obj->posI++] = obj->vt[a * 2];
		// 	obj->indices[obj->posI++] = obj->vt[a * 2 + 1];
		// }


















