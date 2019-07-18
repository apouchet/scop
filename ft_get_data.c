/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_data.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 15:35:23 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/18 15:35:26 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int		ft_get_nb_value(char *s)
{
	int nb;
	int i;

	i = 0;
	nb = 0;
	while (s[i])
	{
		while (ft_isspace(s[i]))
			i++;
		if ((s[i] >= '0' && s[i] <= '9') || s[i] == '.' || s[i] == '-')
			nb++;
		while ((s[i] >= '0' && s[i] <= '9') || s[i] == '.' || s[i] == '-' || s[i] == '/')
			i++;
	}
	return (nb);
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
			a[step++] = ft_atost(&line[*i]) - 1;
		else if (line[*i - 1] == '/' && (line[*i] == '/' || !line[*i] || ft_isspace(line[*i])))
			a[step++] = 0;
		else
			ft_exit_pars(4, "Bad Syntaxe", 0, line);
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


void		ft_get_data(t_obj *obj, size_t *posVertex, size_t posTexture, size_t posNormal)
{
	int		fd;
	int		get;
	char	*line;

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
	free(line);
	close(fd);
	if (get < 0)
		ft_exit_pars(2, "Read File Error", 0, NULL);
}
