/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing_other.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/18 15:35:27 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/18 15:35:30 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	ft_exit_pars(int type, char *msg, int l, char *line)
{
	if (type == 1)
	{
		ft_printf("ERROR LINE %d : %s\n", l, msg);
		ft_printf("Line : \"%s\"\n", line);
	}
	else if (type == 2)
		ft_printf("ERROR : %s\n", msg);
	if (type == 3)
	{
		ft_printf("ERROR : %s\n", msg);
		ft_printf("Line : \"%s\"\n", line);
	}
	if (type == 4)
		ft_printf("ERROR : %s - %s\n", msg, line);
	if (type == 5)
		ft_printf("ERROR : %s %d - %s\n", msg, l, line);
	exit(0);
}

void	ft_other_command(t_obj *obj, char *line, size_t l)
{
	if (ft_strncmp(line, "f ", 2) == 0)
	{
		if ((ft_get_nb_value(&line[2])) < 3 || ft_get_nb_value(&line[2]) > 4)
			ft_exit_pars(1, "Invalid Type Can Only Read 3 - 4 Vertex", l, line);
		(ft_get_nb_value(&line[2]) == 3) ? obj->f_tri++ : obj->f_quad++;
	}
	else if (ft_strncmp(line, "usemtl ", 7) == 0)
	{
		obj->texture = ft_strtrim(&line[7]);
		if (ft_strncmp(obj->texture, "None\0 ", 5)
			&& ft_strncmp(obj->texture, "Material\0 ", 9))
		{
			if (ft_check_extention(obj->texture, ".tga") == 1)
				obj->texture = ft_strjoin_free(&obj->path, &obj->texture, 2);
			else
				ft_exit_pars(1, "Invalid Texture, Only Read TGA File", l, line);
		}
		else
			ft_strdel(&obj->texture);
	}
	else if (ft_strncmp(line, "mtllib ", 7) && ft_strncmp(line, "s ", 2)
		&& ft_strncmp(line, "o ", 2) && ft_strncmp(line, "g ", 2)
		&& line[0] != '#' && line[ft_while_space(line, 0)])
		ft_exit_pars(1, "Unknow Command", l, line);
}

void	ft_check_value(char *s, int max)
{
	int i;
	int dot;
	int size;

	i = (s[0] == '-' ? 1 : 0);
	size = 0;
	dot = (max > 20 ? 0 : -1);
	while (s[i] == '0')
		i++;
	while (ft_isdigit(s[i]) || s[i] == '.')
	{
		if (s[i] == '.')
			dot = (max > 20 ? dot + 1 : 10);
		else if (dot <= 0)
			size++;
		i++;
	}
	if (size <= max && dot <= 1)
		return ;
	s[i] = '\0';
	if (size > max)
		ft_exit_pars(5, "Value To Big - More Than", max, s);
	else
		ft_exit_pars(4, "Error - To Many Dot In", 0, s);
}

void	ft_fill(t_obj *obj, size_t a, size_t size, int type)
{
	size_t i;

	i = 0;
	if ((type == 1 || type == 10) && a * size + size > obj->nb_v * size)
		ft_exit_pars(5, "Vertex number :", a + 1, "not exist");
	if ((type == 2 || type == 20) && a * size + size > obj->nb_n * size)
		ft_exit_pars(5, "Vertex normal number :", a + 1, "not exist");
	if ((type == 3 || type == 30) && a * size + size > obj->nb_t * size)
		ft_exit_pars(5, "Vertex texture number :", a + 1, "not exist");
	while (i < size)
	{
		if (type == 1)
			obj->tab_v_tri[obj->nb_v_tri++] = obj->v[a * size + i++];
		else if (type == 10)
			obj->tab_v_quad[obj->nb_v_quad++] = obj->v[a * size + i++];
		else if (type == 2)
			obj->tab_n_tri[obj->nb_n_tri++] = obj->vn[a * size + i++];
		else if (type == 20)
			obj->tab_n_quad[obj->nb_n_quad++] = obj->vn[a * size + i++];
		else if (type == 3)
			obj->tab_t_tri[obj->nb_t_tri++] = obj->vt[a * size + i++];
		else if (type == 30)
			obj->tab_t_quad[obj->nb_t_quad++] = obj->vt[a * size + i++];
	}
}
