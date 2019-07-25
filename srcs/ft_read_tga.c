/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_tga.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/13 17:16:23 by apouchet          #+#    #+#             */
/*   Updated: 2019/02/13 17:16:24 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void				ft_affich_tga_header(t_tga *tga)
{
	ft_printf("id length :      %hhd\n", tga->id_length);
	ft_printf("colour map :     %hhd\n", tga->colour_map);
	ft_printf("data type :      %hhd\n", tga->data_type);
	ft_printf("colour origin :  %hd\n", tga->colour_origin);
	ft_printf("colour length :  %hd\n", tga->colour_length);
	ft_printf("colour depth :   %hhd\n", tga->colour_depth);
	ft_printf("x origin :       %hd\n", tga->x_origin);
	ft_printf("y origin :       %hd\n", tga->y_origin);
	ft_printf("width :          %hd\n", tga->width);
	ft_printf("height :         %hd\n", tga->height);
	ft_printf("bpp :            %hhd\n", tga->bpp);
	ft_printf("img descriptor : %hhd\n", tga->imagedescriptor);
}

static short			ft_short_little_debian(char a, char b)
{
	short nb;

	nb = a;
	nb <<= 8;
	nb += b;
	return (nb);
}

static unsigned char	*ft_read_tga(int fd, size_t size, int rgb)
{
	int				rd;
	size_t			i;
	unsigned char	swap;
	unsigned char	*file;

	i = 0;
	if (!(file = (unsigned char*)ft_memalloc(size + 1)))
	{
		ft_printf("Error fail to malloc TGA\n");
		return (NULL);
	}
	if ((rd = read(fd, file, size)) < (ssize_t)size)
	{
		ft_printf("Error fail to malloc TGA FILE\n");
		return (NULL);
	}
	while (i < size)
	{
		swap = file[i];
		file[i] = file[i + 2];
		file[i + 2] = swap;
		i += rgb;
	}
	close(fd);
	return (file);
}

unsigned char			*ft_read_tga_headers(char *name, t_tga *tga)
{
	int				fd;
	int				rd;
	unsigned char	headers[100];

	if ((fd = open(name, O_RDONLY)) < 0 ||
		(rd = read(fd, headers, 18)) <= 0)
		return (NULL);
	headers[18] = '\0';
	tga->id_length = headers[0];
	tga->colour_map = headers[1];
	tga->data_type = headers[2];
	tga->colour_origin = ft_short_little_debian(headers[4], headers[3]);
	tga->colour_length = ft_short_little_debian(headers[6], headers[5]);
	tga->colour_depth = headers[7];
	tga->x_origin = ft_short_little_debian(headers[9], headers[8]);
	tga->y_origin = ft_short_little_debian(headers[11], headers[10]);
	tga->width = ft_short_little_debian(headers[13], headers[12]);
	tga->height = ft_short_little_debian(headers[15], headers[14]);
	tga->bpp = (int)headers[16] / 8;
	tga->imagedescriptor = headers[17];
	ft_affich_tga_header(tga);
	return (ft_read_tga(fd, tga->height * tga->width * tga->bpp, tga->bpp));
}
