/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paring.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/19 15:00:05 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/16 09:01:04 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdio.h>
// #include <unistd.h>
// #include "libft/libft.h"
#include "scop.h"

void		ft_alloc(t_obj *obj, size_t nbVertex, size_t nbTexture, size_t nbNormal)
{
	if (!(obj->v = (float*)ft_memalloc(sizeof(float) * (nbVertex * 3)))
		|| !(obj->vt = (float*)ft_memalloc(sizeof(float) * (nbTexture * 2)))
		|| !(obj->vn = (float*)ft_memalloc(sizeof(float) * (nbNormal * 3))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	if (obj->faceTri > 0
		&& (!(obj->tabVertexTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 3)))
		|| !(obj->tabNormalTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 3)))
		|| !(obj->tabTextureTri = (float*)ft_memalloc(sizeof(float) * (obj->faceTri * 3 * 2)))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	if (obj->faceQuad > 0
		&& (!(obj->tabVertexQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 3)))
		|| !(obj->tabNormalQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 3)))
		|| !(obj->tabTextureQuad = (float*)ft_memalloc(sizeof(float) * (obj->faceQuad * 4 * 2)))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
}

void 	ft_parsing(t_obj *obj, char *name)
{
	size_t posVertex;
	size_t nbVertex;
	size_t nbTexture;
	size_t nbNormal;

	printf("name = %s\n", name);
	nbVertex = 0;
	nbTexture = 0;
	nbNormal = 0;
	posVertex = 0;
	if (name == NULL)
		obj->fileName = "lowtri.obj";
	else if (ft_check_extention(name, ".obj"))
	{
		obj->fileName = name;
		obj->path = ft_get_path(name);
		printf("path = %s\n", obj->path);
	}
	else
		ft_exit_pars(2, "Invalid File Name, Need To Be A \".obj\"", 0, NULL);
	printf("file name = %s\n", obj->fileName);
	if (ft_size_file_pars(obj, &nbVertex, &nbTexture, &nbNormal) < 0)
		exit(0);
	// printf("vertex  = %zu\n", nbVertex);
	// printf("texture = %zu\n", nbTexture);
	// printf("normal  = %zu\n", nbNormal);
	// printf("nb face Tri = %d\n", obj->faceTri);
	// printf("nb face Quad = %d\n", obj->faceQuad);
	// printf("nb face total = %d\n", obj->faceQuad + obj->faceTri);
	// printf("texture name = -%s-\n", obj->texture);
	// obj->posI = 0;
	// obj->tabVTri = 0;
	// obj->tabNTri = 0;
	// obj->tabTTri = 0;
	// obj->tabVQuad = 0;
	// obj->tabNQuad = 0;
	// obj->tabTQuad = 0;
	// obj->size = (3 + 3 + 2) * 3 * (obj->faceTri + obj->faceQuad);
	
	ft_alloc(obj, nbVertex, nbTexture, nbNormal);
	ft_get_data(obj, &posVertex, 0, 0);
	ft_center(obj, &posVertex);
}















