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

#include <stdio.h>
#include <unistd.h>
#include "../libft/libft.h"

typedef struct	s_obj
{
	int			nbVertex;
	int			nbNormal;
	int			nbTexture;
	int			face;
	int			posV;
	int			posN;
	int			posT;
	char		*fileName;
	char		*name;
	char		*texture;
	float		*v;
	float		*vt;
	float		*vn;
}				t_obj;

void	ft_exit_pars(int type, char *msg, int line)
{
	if (type == 1)
		printf("ERROR LINE %d : %s\n", line, msg);
	exit(0);
}

static int		ft_size_file(t_obj *obj)
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
		if (ft_strncmp(line, "v ", 2) == 0)
			obj->nbVertex++;
		else if (ft_strncmp(line, "vt ", 3) == 0)
			obj->nbTexture++;
		else if (ft_strncmp(line, "vn ", 3) == 0)
			obj->nbNormal++;
		else if (ft_strncmp(line, "f ", 2) == 0)
			obj->face++;
		else if (ft_strncmp(line, "usemtl ", 7) == 0)
			obj->texture = ft_strtrim(ft_strdup(&line[7]));
		else if (ft_strncmp(line, "mtllib ", 7) && ft_strncmp(line, "s ", 2) && line[0] != '#' && line[ft_while_space(line, 0)])
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
	printf("fin : %s\n", line);
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
		printf("line = -|%s|-\n", &line[i]);
		a[(*pos)++] = ft_atof(&line[i]);
		printf("v[%d] = %f\n",*pos - 1, a[*pos - 1]);
		// (*pos)++;
		i = ft_find_space(line, i);
		nb--;
	}
	printf("\n");
	return (1);
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
		if (ft_strncmp(line, "v ", 2) == 0)
			value = ft_get_value(&obj->v, 3, &line[2], &obj->posV);
		else if (ft_strncmp(line, "vt ", 3) == 0)
			value = ft_get_value(&obj->vt, 2, &line[3], &obj->posT);
		else if (ft_strncmp(line, "vn ", 3) == 0)
			value = ft_get_value(&obj->vn, 3, &line[3], &obj->posN);
		free(line);
	}
	for (int j = 0; j < obj->nbVertex * 3; j++)
		printf("j = %d = %f\n", j, obj->v[j]);
	free(line);
	close(fd);
	return (get < 0 ? -1 : 1);
}

int		main(void)
{
	t_obj obj;

	ft_bzero(&obj, sizeof(t_obj));
	obj.fileName = "test.obj";
	printf("file name = %s\n", obj.fileName);
	if (ft_size_file(&obj) < 0)
		return (-1);
	printf("vertex  = %d\n", obj.nbVertex);
	printf("texture = %d\n", obj.nbTexture);
	printf("normal  = %d\n", obj.nbNormal);
	printf("nb face = %d\n", obj.face);
	printf("texture name = -%s-\n", obj.texture);
	ft_get_data(&obj);

	printf("test : %f\n", ft_atof("-10.1"));
	return (0);
}



















