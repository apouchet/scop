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
	size_t nb_v;
	size_t nb_t;
	size_t nb_n;

	nb_v = 0;
	nb_t = 0;
	nb_n = 0;
	pos_vertex = 0;
	// printf("00000\n");
	// sleep(5);
	if (name == NULL)
		obj->file_name = "lowtri.obj";
	else if (ft_check_extention(name, ".obj"))
	{
		obj->file_name = name;
		obj->path = ft_get_path(name);
		ft_printf("path = %s\n", obj->path);
	}
	else
		ft_exit_pars(2, "Invalid File Name, Need To Be A \".obj\"", 0, NULL);
	// printf("sleep 0\n");
	// sleep(5);
	if (ft_size_file_pars(obj, &nb_v, &nb_t, &nb_n) < 0)
		exit(0);
	// leaks 1
	// printf("sleep 1\n");
	// sleep(5);
	ft_alloc(obj, nb_v, nb_t, nb_n);
	// printf("sleep 2\n");
	// sleep(5);
	ft_get_data(obj, &pos_vertex, 0, 0);
	// leaks 2
	
	// printf("sleep 3\n");
	// sleep(5);
	ft_center(obj, &pos_vertex);
	// printf("sleep 4\n");
	// sleep(5);
}







	// printf("vertex  = %zu\n", nb_v);
	// printf("texture = %zu\n", nb_t);
	// printf("normal  = %zu\n", nb_n);
	// printf("nb face Tri = %d\n", obj->f_tri);
	// printf("nb face Quad = %d\n", obj->f_quad);
	// printf("nb face total = %d\n", obj->f_quad + obj->f_tri);
	// printf("texture name = -%s-\n", obj->texture);
	// obj->posI = 0;
	// obj->tabVTri = 0;
	// obj->tabNTri = 0;
	// obj->tabTTri = 0;
	// obj->tabVQuad = 0;
	// obj->tabNQuad = 0;
	// obj->tabTQuad = 0;
	// obj->size = (3 + 3 + 2) * 3 * (obj->f_tri + obj->f_quad);
	








