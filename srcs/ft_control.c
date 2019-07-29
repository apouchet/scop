/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_control.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 11:32:36 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/29 11:32:38 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

static int		ft_move(t_control *ctrl)
{
	if (ctrl->key == '+' && ctrl->move < 1.5f)
		ctrl->move += 0.2f;
	else if (ctrl->key == '-' && ctrl->move > 0.2f)
		ctrl->move -= 0.2f;
	if (ctrl->key == 'w')
		ctrl->move_y += ctrl->move;
	else if (ctrl->key == 's')
		ctrl->move_y -= ctrl->move;
	else if (ctrl->key == 'd')
		ctrl->move_x += ctrl->move;
	else if (ctrl->key == 'a')
		ctrl->move_x -= ctrl->move;
	else if (ctrl->key == 'z')
		ctrl->move_z += ctrl->move;
	else if (ctrl->key == 'x')
		ctrl->move_z -= ctrl->move;
	else if (ctrl->key == 'o')
	{
		ctrl->rot_x = 0;
		ctrl->rot_y = 0;
		ctrl->rot_z = 0;
	}
	else
		return (0);
	return (1);
}

void			ft_control(t_control *ctrl, SDL_Event e, float zoom)
{
	ctrl->key = 0;
	if (SDL_WaitEventTimeout(&e, 1))
	{
		ctrl->key = e.window.event;
		if (ctrl->key == SDL_WINDOWEVENT_CLOSE || ctrl->key == ' ')
			ctrl->end = 1;
		else if (ctrl->key == 'f')
			ctrl->rot_y -= PI / ctrl->step;
		else if (ctrl->key == 'h')
			ctrl->rot_y += PI / ctrl->step;
		else if (ctrl->key == 'g')
			ctrl->rot_x -= PI / ctrl->step;
		else if (ctrl->key == 't')
			ctrl->rot_x += PI / ctrl->step;
		else if (ctrl->key == 'y')
			ctrl->rot_z -= PI / ctrl->step;
		else if (ctrl->key == 'r')
			ctrl->rot_z += PI / ctrl->step;
		else if (ctrl->key == 'p')
			ctrl->move_z = zoom;
		else
			ft_move(ctrl);
	}
}
