/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_zoom.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 15:35:31 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/18 15:35:32 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static void	ft_min_max(t_obj *obj, float *tab, size_t *posVertex)
{
	size_t i;

	i = 0;
	while (i < *posVertex)
	{
		if(tab[0] > obj->v[i])
			tab[0] = obj->v[i];
		else if(tab[1] < obj->v[i])
			tab[1] = obj->v[i];
		i++;
		if(tab[2] > obj->v[i])
			tab[2] = obj->v[i];
		else if(tab[3] < obj->v[i])
			tab[3] = obj->v[i];
		i++;
		if(tab[4] > obj->v[i])
			tab[4] = obj->v[i];
		else if(tab[5] < obj->v[i])
			tab[5] = obj->v[i];
		i++;
	}
}

static float	ft_fabs(float f)
{
	if (f < 0)
		return (-f);
	return (f);
}

static float	ft_min(float *tab, float midX, float midY, float midZ)
{
	float zoom;
	
	zoom = ft_fabs(tab[0] - midX);
	if (ft_fabs(tab[1] - midX) > zoom)
		zoom = ft_fabs(tab[1] - midX);
	if (ft_fabs(tab[2] - midY) > zoom)
		zoom = ft_fabs(tab[2] - midY);
	if (ft_fabs(tab[3] - midZ) > zoom)
		zoom = ft_fabs(tab[3] - midZ);
	printf("zoom = %f\n", zoom);
	return (zoom);
}

void	ft_center(t_obj *obj, size_t *posVertex)
{
	float	midX;
	float	midY;
	float	midZ;
	float	*tab;
	size_t	i;

	if (!(tab = (float*)ft_memalloc(sizeof(float) * 6)))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	ft_min_max(obj, tab, posVertex);
	midX = (tab[1] + tab[0]) / 2;
	midY = (tab[3] + tab[2]) / 2;
	midZ = (tab[5] + tab[4]) / 2;
	i = 0;
	while (i < obj->tabVTri)
	{
		obj->tabVertexTri[i++] -= midX;
		obj->tabVertexTri[i++] -= midY;
		obj->tabVertexTri[i++] -= midZ;
	}
	i = 0;
	while (i < obj->tabVQuad)
	{
		obj->tabVertexQuad[i++] -= midX;
		obj->tabVertexQuad[i++] -= midY;
		obj->tabVertexQuad[i++] -= midZ;
	}
	obj->zoom = ft_min(tab, midX, midY, midZ);
	free(tab);
}
