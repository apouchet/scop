/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrix.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apouchet <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/25 11:57:01 by apouchet          #+#    #+#             */
/*   Updated: 2019/07/25 11:57:03 by apouchet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	gl_perspective(t_matrix *mx)
{
	float	scale;

	mx->fov = 90;
	mx->near = 0.1f;
	mx->far = 100.0f;
	mx->ratio = WINDOWX / WINDOWY;
	scale = tan(mx->fov * 0.5 * PI / 180) * mx->near;
	mx->right = mx->ratio * scale;
	mx->left = -mx->right;
	mx->top = scale;
	mx->bottom = -mx->top;
}

void	ft_trans(t_matrix *mx, float x, float y, float z)
{
	mx->base[0][0] = 1;
	mx->base[1][1] = 1;
	mx->base[2][2] = 1;
	mx->base[3][3] = 1;
	mx->base[3][0] = x;
	mx->base[3][1] = y;
	mx->base[3][2] = z;
}

void	ft_rotate(t_matrix *mx, double angle_x, double angle_y, double angle_z)
{
	float cos[3];
	float sin[3];

	cos[X] = cosf(angle_x);
	sin[X] = sinf(angle_x);
	cos[Y] = cosf(angle_y);
	sin[Y] = sinf(angle_y);
	cos[Z] = cosf(angle_z);
	sin[Z] = sinf(angle_z);
	mx->rotate[0][0] = cos[Y] * cos[Z];
	mx->rotate[1][0] = sin[X] * sin[Y] * cos[Z] + cos[X] * sin[Z];
	mx->rotate[2][0] = -cos[X] * sin[Y] * cos[Z] + sin[X] * sin[Z];
	mx->rotate[0][1] = -cos[Y] * sin[Z];
	mx->rotate[1][1] = -sin[X] * sin[Y] * sin[Z] + cos[X] * cos[Z];
	mx->rotate[2][1] = cos[X] * sin[Y] * sin[Z] + sin[X] * cos[Z];
	mx->rotate[0][2] = sin[Y];
	mx->rotate[1][2] = -sin[X] * cos[Y];
	mx->rotate[2][2] = cos[X] * cos[Y];
	mx->rotate[3][3] = 1.0f;
}

void	ft_perspective(t_matrix *mx)
{
	mx->pers[0][0] = 2 * mx->near / (mx->right - mx->left);
	mx->pers[1][1] = 2 * mx->near / (mx->top - mx->bottom);
	mx->pers[2][2] = -2 * mx->far * mx->near / (mx->far - mx->near);
	mx->pers[0][3] = -((mx->right + mx->left) / (mx->right - mx->left));
	mx->pers[1][3] = -((mx->top + mx->bottom) / (mx->top - mx->bottom));
	mx->pers[2][3] = -((mx->far + mx->near) / (mx->far - mx->near));
	mx->pers[3][3] = 1;
}

void	ft_matrix(GLuint program_id, t_control *ctrl, t_matrix *mx)
{
	unsigned int	matloc;
	static int		acolor = 0;
	static float	mix = 0;

	if (ctrl->key == '.')
		acolor = (acolor == 1 ? 0 : 1);
	if (acolor == 1 && mix < 0.98f)
		mix += 0.02;
	else if (acolor == 0 && mix >= 0.02)
		mix -= 0.02;
	gl_perspective(mx);
	ft_rotate(mx, ctrl->rot_x, ctrl->rot_y, ctrl->rot_z);
	ft_trans(mx, ctrl->move_x, ctrl->move_y, ctrl->move_z);
	ft_perspective(mx);
	matloc = glGetUniformLocation(program_id, "color");
	glUniform1f(matloc, mix);
	matloc = glGetUniformLocation(program_id, "rotate");
	glUniformMatrix4fv(matloc, 1, GL_FALSE, &mx->rotate[0][0]);
	matloc = glGetUniformLocation(program_id, "pers");
	glUniformMatrix4fv(matloc, 1, GL_FALSE, &mx->pers[0][0]);
	matloc = glGetUniformLocation(program_id, "move");
	glUniformMatrix4fv(matloc, 1, GL_FALSE, &mx->pers[0][0]);
	matloc = glGetUniformLocation(program_id, "base");
	glUniformMatrix4fv(matloc, 1, GL_FALSE, &mx->base[0][0]);
}
