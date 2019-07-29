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

int			ft_get_nb_value(char *s)
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
		while ((s[i] >= '0' && s[i] <= '9') || s[i] == '.' || s[i] == '-'
			|| s[i] == '/')
			i++;
	}
	return (nb);
}

void		ft_get_value(float **tab, int nb, char *line, size_t *pos)
{
	int		i;
	float	*a;

	a = *tab;
	i = 0;
	if (ft_count_word(line) != (size_t)nb && nb == 3)
		ft_exit_pars(5, "Need 3 value, have", (int)ft_count_word(line), line);
	else if (ft_count_word(line) != (size_t)nb && nb == 2)
		ft_exit_pars(5, "Need 2 value, have", (int)ft_count_word(line), line);
	while (ft_isspace(line[i]) || ft_isdigit(line[i]) || line[i] == '.'
		|| line[i] == '-' || line[i] == '+')
		i++;
	if (line[i])
		ft_exit_pars(3, "Invalid Line", 0, line);
	i = 0;
	while (nb > 0)
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		ft_check_value(&line[i], 35);
		a[(*pos)++] = ft_atof(&line[i]);
		i = ft_find_space(line, i);
		nb--;
	}
}

size_t		ft_get_point(char *line, int *i, size_t a[3])
{
	size_t step;

	step = 0;
	while (line[*i] && !ft_isspace(line[*i]))
	{
		if (line[*i] == '-')
			ft_exit_pars(4, "Negative Value Imposible", 0, line);
		if (ft_isdigit(line[*i]))
			a[step++] = ft_atost(&line[*i]) - 1;
		else if (line[*i - 1] == '/' && (line[*i] == '/' || !line[*i]
			|| ft_isspace(line[*i])))
			a[step++] = 0;
		else
			ft_exit_pars(4, "Bad Syntaxe", 0, line);
		while (ft_isdigit(line[*i]))
			(*i)++;
		if (line[*i] == '/')
			(*i)++;
	}
	return (step);
}

void		ft_get_face(t_obj *obj, char *line)
{
	int		i;
	int		type;
	size_t	a[3];
	size_t	step;

	i = 0;
	step = 0;
	type = ft_get_nb_value(line) == 3 ? 1 : 10;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (line[i])
		{
			step = ft_get_point(line, &i, a);
			if (step == 0 || step > 3)
				ft_exit_pars(4, "Invalide face", 0, line);
			ft_fill(obj, a[0], 3, 1 * type);
			if (step >= 2)
				ft_fill(obj, a[1], 2, 3 * type);
			if (step == 3)
				ft_fill(obj, a[2], 3, 2 * type);
		}
	}
}

void		ft_get_data(t_obj *obj, size_t *pos_v, size_t pos_t, size_t pos_n)
{
	int		fd;
	int		get;
	char	*line;

	if ((fd = open(obj->file_name, O_RDONLY)) < 0)
		ft_exit_pars(4, "Can't Read File", 0, obj->file_name);
	while ((get = get_next_line(fd, &line)) > 0)
	{
		if (ft_strncmp(line, "v ", 2) == 0)
			ft_get_value(&obj->v, 3, &line[2], pos_v);
		else if (ft_strncmp(line, "vt ", 3) == 0)
			ft_get_value(&obj->vt, 2, &line[3], &pos_t);
		else if (ft_strncmp(line, "vn ", 3) == 0)
			ft_get_value(&obj->vn, 3, &line[3], &pos_n);
		else if (ft_strncmp(line, "f ", 2) == 0)
			ft_get_face(obj, &line[2]);
		free(line);
	}
	free(line);
	close(fd);
	if (get < 0)
		ft_exit_pars(2, "Read File Error", 0, NULL);
}
