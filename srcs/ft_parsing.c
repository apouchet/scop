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

#include "scop.h"

void	ft_alloc(t_obj *obj, size_t nb_v, size_t nb_t, size_t nb_n)
{
	size_t f;

	f = sizeof(float);
	if (!(obj->v = (float*)ft_memalloc(f * (nb_v * 3)))
		|| !(obj->vt = (float*)ft_memalloc(f * (nb_t * 2)))
		|| !(obj->vn = (float*)ft_memalloc(f * (nb_n * 3))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	if (obj->f_tri > 0
		&& (!(obj->tab_v_tri = (float*)ft_memalloc(f * (obj->f_tri * 3 * 3)))
		|| !(obj->tab_n_tri = (float*)ft_memalloc(f * (obj->f_tri * 3 * 3)))
		|| !(obj->tab_t_tri = (float*)ft_memalloc(f * (obj->f_tri * 3 * 2)))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
	if (obj->f_quad > 0
		&& (!(obj->tab_v_quad = (float*)ft_memalloc(f * (obj->f_quad * 4 * 3)))
		|| !(obj->tab_n_quad = (float*)ft_memalloc(f * (obj->f_quad * 4 * 3)))
		|| !(obj->tab_t_quad = (float*)ft_memalloc(f * (obj->f_quad * 4 * 2)))))
		ft_exit_pars(2, "Fail To Malloc", 0, NULL);
}

void	ft_parsing(t_obj *obj, char *name)
{
	size_t pos_vertex;

	pos_vertex = 0;
	if (name == NULL)
		obj->file_name = "obj_file/lowtri.obj";
	else if (ft_check_extention(name, ".obj"))
	{
		obj->file_name = name;
		obj->path = ft_get_path(name);
	}
	else
		ft_exit_pars(2, "Invalid File Name, Need To Be A \".obj\"", 0, NULL);
	if (ft_size_file_pars(obj) < 0)
		ft_exit_pars(4, "Error while reading file : ", 0, name);
	ft_alloc(obj, obj->nb_v, obj->nb_t, obj->nb_n);
	ft_get_data(obj, &pos_vertex, 0, 0);
	ft_center(obj, &pos_vertex);
}
