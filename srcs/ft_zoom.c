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

static void		ft_min_max(t_obj *obj, float *tab, size_t *pos_vertex)
{
	size_t i;

	i = 0;
	while (i < *pos_vertex)
	{
		if (tab[0] > obj->v[i])
			tab[0] = obj->v[i];
		else if (tab[1] < obj->v[i])
			tab[1] = obj->v[i];
		i++;
		if (tab[2] > obj->v[i])
			tab[2] = obj->v[i];
		else if (tab[3] < obj->v[i])
			tab[3] = obj->v[i];
		i++;
		if (tab[4] > obj->v[i])
			tab[4] = obj->v[i];
		else if (tab[5] < obj->v[i])
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

static float	ft_min(float *tab, float mid_x, float mid_y, float mid_z)
{
	float zoom;

	zoom = ft_fabs(tab[0] - mid_x);
	if (ft_fabs(tab[1] - mid_x) > zoom)
		zoom = ft_fabs(tab[1] - mid_x);
	if (ft_fabs(tab[2] - mid_y) > zoom)
		zoom = ft_fabs(tab[2] - mid_y);
	if (ft_fabs(tab[3] - mid_z) > zoom)
		zoom = ft_fabs(tab[3] - mid_z);
	free(tab);
	return (-(zoom + 1));
}

void			ft_center(t_obj *obj, size_t *pos_vertex)
{
	float	mid[3];
	float	*tab;
	size_t	i;

	if (!(tab = (float*)ft_memalloc(sizeof(float) * 6)))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	ft_min_max(obj, tab, pos_vertex);
	mid[X] = (tab[1] + tab[0]) / 2;
	mid[Y] = (tab[3] + tab[2]) / 2;
	mid[Z] = (tab[5] + tab[4]) / 2;
	i = 0;
	while (i < obj->nb_v_tri)
	{
		obj->tab_v_tri[i++] -= mid[X];
		obj->tab_v_tri[i++] -= mid[Y];
		obj->tab_v_tri[i++] -= mid[Z];
	}
	i = 0;
	while (i < obj->nb_v_quad)
	{
		obj->tab_v_quad[i++] -= mid[X];
		obj->tab_v_quad[i++] -= mid[Y];
		obj->tab_v_quad[i++] -= mid[Z];
	}
	obj->zoom = ft_min(tab, mid[X], mid[Y], mid[Z]);
}
