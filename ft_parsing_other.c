/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_other.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 15:35:27 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/18 15:35:30 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

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

void		ft_fill(t_obj *obj, size_t a, size_t size, int type)
{
	size_t i;

	i = 0;
	// if (a + size - 1 > obj->nbVertex * size) !!!!!!!!!!!!!!!!!!!!!!!
	// 	return ;
	if ((type < 10 && a + size - 1 > obj->faceTri * 3 * 3)
		|| (type >= 10 && a + size  - 1 > obj->faceQuad * 4 * 3))
	{
		printf("ERROR :Invalid File\n");
		// printf("fail tri\n");
		exit(0);
	}
	// if (type >= 10 && a + size  - 1 > obj->faceQuad * 4 * 3)
	// {
	// 	printf("fail quad\n");
	// 	exit(0);
	// }
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
