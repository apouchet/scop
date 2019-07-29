/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_file.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/14 12:58:55 by apouchet          #+#    #+#             */
/*   Updated: 2019/02/14 12:58:55 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

int		ft_size_file(char *name)
{
	int		fd;
	int		rd;
	long	len;
	char	buff[1001];

	len = 0;
	if ((fd = open(name, O_RDONLY)) < 0)
		return (-1);
	while ((rd = read(fd, buff, 1000)) > 0)
		len += rd;
	close(fd);
	return (rd < 0 ? rd : len);
}

char	*ft_get_file(char *name, char *file)
{
	int		fd;
	int		rd;
	int		j;

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

int		ft_size_file_pars(t_obj *obj, size_t *nb_v, size_t *nb_t, size_t *nb_n)
{
	int		fd;
	int		get;
	size_t	l;
	char	*line;

	l = 0;
	if ((fd = open(obj->file_name, O_RDONLY)) < 0)
		ft_exit_pars(2, "Fail To Open File", 0, NULL);
	while ((get = get_next_line(fd, &line)) > 0)
	{
		l++;
		if (ft_strncmp(line, "v ", 2) == 0)
			(*nb_v)++;
		else if (ft_strncmp(line, "vt ", 3) == 0)
			(*nb_t)++;
		else if (ft_strncmp(line, "vn ", 3) == 0)
			(*nb_n)++;
		else
			ft_other_command(obj, line, l);
		free(line);
	}
	free(line);
	close(fd);
	return (get < 0 ? -1 : 1);
}

char	*ft_get_path(char *s)
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
