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
	long	fileLength;
	char	buff[1001];

	fileLength = 0;
	if ((fd = open(name, O_RDONLY)) < 0)
		return (-1);
	while ((rd = read(fd, buff, 1000)) > 0)
		fileLength += rd;
	close(fd);
	return (rd < 0 ? rd : fileLength);
}

char	*ft_get_file(char *name, char *file)
{
	int		fd;
	int		rd;
	int		j;
	char	buff[1001];

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
