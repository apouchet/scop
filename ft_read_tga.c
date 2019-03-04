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
	printf("id length :      %hhd\n", tga->idLength);
	printf("colour map :     %hhd\n", tga->colourMap);
	printf("data type :      %hhd\n", tga->dataType);
	printf("colour origin :  %hd\n", tga->colourOrigin);
	printf("colour length :  %hd\n", tga->colourLength);
	printf("colour depth :   %hhd\n", tga->colourDepth);
	printf("x origin :       %hd\n", tga->xOrigin);
	printf("y origin :       %hd\n", tga->yOrigin);
	printf("width :          %hd\n", tga->width);
	printf("height :         %hd\n", tga->height);
	printf("bpp :            %hhd\n", tga->bpp);
	printf("img descriptor : %hhd\n", tga->imagedescriptor);
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
	int				i;
	int z = 0;
	unsigned char	swap;
	unsigned char	swap1;
	unsigned char	swap2;
	unsigned char	*test;
	unsigned char	*file;
	unsigned char	*file2;
	int k;

	i = 0;
	int j = size - 1;
	if (!(file = (unsigned char*)malloc(size + 1)) ||
		(rd = read(fd, file, size)) < size)
		return (NULL);
	file[size] = '\0';
	// test = (unsigned char*)malloc(size + 1);
	// test[size] = '\0';
	// while (file[i])
	// {
	// 	test[j--] = file[i++];
	// }
	k = size;
	if (rgb == 4)
	{
		if (!(file2 = (unsigned char*)malloc(size + 1)))
			return (NULL);
		while (i < size)
		{
			file2[k] = file[i + 0];
			file2[k - 1] = file[i + 1];
			file2[k - 2] = file[i + 2];
			file2[k - 3] = file[i + 3];
			i += 4;
			k -= 4;
		}
		file = file2;
		// free(file);
	}
	i = 0;
	while (i < size)
	{
		// if (z == 0 && file[i] != 255)
		// {
		// 	z = 1;
		// 	printf("i = %d\n", i);
		// 	printf("file[0] = %d\n", file[i]);
		// 	printf("file[1] = %d\n", file[i + 1]);
		// 	printf("file[2] = %d\n", file[i + 2]);
		// 	printf("file[3] = %d\n", file[i + 3]);
		// }
		if (rgb == 3)
		{
			swap = file[i];
			file[i] = file[i + 2];
			file[i + 2] = swap;
			i += 3;
			// printf("i = %d\n", i);
		}
		else
		{
			// swap = file[i];
			// file[i] = file[i + 3];
			// file[i + 3] = swap;

			// swap = file[i + 1];
			// file[i + 1] = file[i + 2];
			// file[i + 2] = swap;
			// swap = file[i];
			// file[i] = file[i + 2];
			// file[i + 2] = swap;

			// swap = file[i];
			// file[i] = file[i + 3];
			// file[i + 3] = swap;

			// swap = file[i + 1];
			// file[i + 1] = file[i + 2];
			// file[i + 2] = swap;
			i += 4;

		}
		// if (z == 1)
		// {
		// 	z = 0;
		// 	printf("swap :\n");
		// 	printf("file[0] = %d\n", file[i - 4]);
		// 	printf("file[1] = %d\n", file[i - 3]);
		// 	printf("file[2] = %d\n", file[i - 2]);
		// 	printf("file[3] = %d\n", file[i - 1]);
		// 	printf("\n");
		// }
	}
	close(fd);
	printf("size file = %d / size = %zu\n", rd, size);
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
	tga->idLength = headers[0];
	tga->colourMap = headers[1];
	tga->dataType = headers[2];
	tga->colourOrigin = ft_short_little_debian(headers[4], headers[3]);
	tga->colourLength = ft_short_little_debian(headers[6], headers[5]);
	tga->colourDepth = headers[7];
	tga->xOrigin = ft_short_little_debian(headers[9], headers[8]);
	tga->yOrigin = ft_short_little_debian(headers[11], headers[10]);
	tga->width = ft_short_little_debian(headers[13], headers[12]);
	tga->height = ft_short_little_debian(headers[15], headers[14]);
	tga->bpp = (int)headers[16] / 8;
	tga->imagedescriptor = headers[17];
	ft_affich_tga_header(tga);
	return (ft_read_tga(fd, tga->height * tga->width * tga->bpp, tga->bpp));
}
