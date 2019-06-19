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
	printf("--------------- face type = %d ---------------\n", nb);
	return (nb);
}

static int		ft_size_file_pars(t_obj *obj)
{
	int		fd;
	int		get;
	int		l;
	char	*line;

	l = 0;
	fd = open(obj->fileName, O_RDONLY);
	while ((get = get_next_line(fd, &line)) > 0)
	{
		l++;
		if (ft_strncmp(line, "v ", 2) == 0 && obj->face == 0)
			obj->nbVertex++;
		else if (ft_strncmp(line, "vt ", 3) == 0 && obj->face == 0)
			obj->nbTexture++;
		else if (ft_strncmp(line, "vn ", 3) == 0 && obj->face == 0)
			obj->nbNormal++;
		else if ((ft_strncmp(line, "v ", 2) == 0 || ft_strncmp(line, "vt ", 3) == 0 || ft_strncmp(line, "vn ", 3) == 0) && obj->face)
		{
			printf("ERROR LINE %d: -|%s|-\n", l, line);
			printf("Please Mouve All \"face\" At The End Of The File\n");
			return (-1);
		}
		else if (ft_strncmp(line, "f ", 2) == 0)
		{
			if (obj->face == 0)
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
			obj->face++;
		}
		else if (ft_strncmp(line, "usemtl ", 7) == 0)
			obj->texture = ft_strtrim(ft_strdup(&line[7]));
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
		printf("infini, size = %d\n", size);
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

int		ft_fill(t_obj *obj, size_t a, int size, int mode)
{
	int i;

	i = 0;
	printf("a = %lu, size = %d, a + size - 1 = %lu, nb = %d, nb * size = %d\n", a, size, a + size - 1, obj->nbVertex, obj->nbVertex * size);
	if (a + size - 1 > obj->nbVertex * size)
		return (-1);
	while (i < size)
	{
		if (mode == 1)
		{
			obj->tabVertex[obj->tabV++] = obj->v[a * size + i++];
			printf("vertex[%lu] = %f     --------------------------------------\n", obj->tabV - 1, obj->tabVertex[obj->tabV - 1]);
		}
		// else
		// 	i++;
		else if (mode == 2)
		{
			obj->tabNormal[obj->tabN++] = obj->vn[a * size + i++];
			printf("normal[%lu] = %f     --------------------------------------\n", obj->tabN - 1, obj->tabVertex[obj->tabN - 1]);
		}
		else if (mode == 3)
		{
			obj->tabTexture[obj->tabT++] = obj->vt[a * size + i++];
			printf("texture[%lu] = %f     --------------------------------------\n", obj->tabT - 1, obj->tabVertex[obj->tabT - 1]);
		}
		// printf("indices[%lu] = %f     --------------------------------------\n", obj->posI - 1, obj->indices[obj->posI - 1]);
		// printf("vertex[%lu] = %f     --------------------------------------\n", obj->tabV - 1, obj->tabVertex[obj->posI - 1]);
	}
	return (0);
}

int		ft_get_face(t_obj *obj, char *line)
{
	int		i;
	size_t	a;
	int		tour;

	i = 0;
	tour = 0;
	
	// printf("ici\n");
	while (line[i])
	{
		tour++;
		printf("\n");
		printf("tour = %d\n", tour);
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (line[i] == '-')
		{
			printf("Error - Inpossible Value In : %s\n", line);
			return (-1);
		}
		ft_check_value(&line[i], 18);
		a = ft_atol(&line[i]) - 1; 				//				//					/// creat atoi - > size_t!!!
		printf("a = %ld\n", a);
		if (tour % 3 == 1 || (obj->nbNormal == 0 && obj->nbTexture == 0))
		{
			printf("UN\n");
			ft_fill(obj, a, 3, 1);
		}
		else if (tour % 3 == 2)
			ft_fill(obj, a, 2, 3);
		// 	ft_fill(obj, obj->vn, a, 3);
		else if (tour % 3 == 0)
			ft_fill(obj, a, 3, 2);
		// 	ft_fill(obj, obj->vt, a, 2);
		printf("av line = %s\n", &line[i]);
		while (ft_isdigit(line[i]))
			i++;
		if (line[i])
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
	// for (int j = 0; j < obj->nbVertex * 3; j++)
	// 	printf("j = %d = %f\n", j, obj->v[j]);
	free(line);
	close(fd);
	return (get < 0 ? -1 : 1);
}

// int		fill_indices(unsigned int **indices, t_obj *obj)
// {

// }

// int		main(void)
float 	*ft_parsing(t_obj *obj, char *name)
{
	// t_obj obj;

	// ft_bzero(&obj, sizeof(t_obj));
	// obj->fileName = "test.obj";
	if (name == NULL)
		obj->fileName = "lowtri.obj";
	else
		obj->fileName = name;
	printf("file name = %s\n", obj->fileName);
	if (ft_size_file_pars(obj) < 0)
		exit(0);
	printf("vertex  = %d\n", obj->nbVertex);
	printf("texture = %d\n", obj->nbTexture);
	printf("normal  = %d\n", obj->nbNormal);
	printf("nb face = %d\n", obj->face);
	printf("texture name = -%s-\n", obj->texture);
	obj->posI = 0;
	obj->tabV = 0;
	obj->tabN = 0;
	obj->tabT = 0;
	obj->size = (3 + 3 + 2) * 3 * obj->face;
	if (!(obj->indices = (float*)malloc(sizeof(float) * obj->size)))
		exit (0);
	if (!(obj->tabVertex = (float*)malloc(sizeof(float) * (obj->face * obj->type_face * 3)))
		|| !(obj->tabNormal = (float*)malloc(sizeof(float) * (obj->face * obj->type_face * 3)))
		|| !(obj->tabTexture = (float*)malloc(sizeof(float) * (obj->face * obj->type_face * 2))))
		exit (0);
	ft_get_data(obj);
	printf("size = %d\n", (3 + 3 + 2) * 3 * obj->face);
	

	// printf("test : %f\n", ft_atof("-10.1"));
	return (obj->indices);
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


















