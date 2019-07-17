#include "scop.h"

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

void	ft_rotate(t_matrix *mx, double angleX, double angleY, double angleZ)
{
	float cos[3];
	float sin[3];

	cos[X] = cosf(angleX);
	sin[X] = sinf(angleX);
	cos[Y] = cosf(angleY);
	sin[Y] = sinf(angleY);
	cos[Z] = cosf(angleZ);
	sin[Z] = sinf(angleZ);
	mx->rotate[0][0] = cos[Y] * cos[Z];
	mx->rotate[1][0] = sin[X] * sin[Y] * cos[Z] + cos[X] * sin[Z];
	mx->rotate[2][0] = -cos[X] * sin[Y] * cos[Z] + sin[X] * sin[Z];
	mx->rotate[0][1] = - cos[Y] * sin[Z];
	mx->rotate[1][1] = - sin[X] * sin[Y] * sin[Z] + cos[X] * cos[Z];
	mx->rotate[2][1] = cos[X] * sin[Y] * sin[Z] + sin[X] * cos[Z];
	mx->rotate[0][2] = sin[Y];
	mx->rotate[1][2] = - sin[X] * cos[Y];
	mx->rotate[2][2] = cos[X] * cos[Y];
	mx->rotate[3][3] = 1.0f;
}

void	ft_perspective(t_matrix *mx, double near, double far)
{
	mx->pers[0][0] = 2 * mx->near / (mx->right - mx->left);
	mx->pers[1][1] = 2 * mx->near / (mx->top - mx->bottom);
	mx->pers[2][2] = -2 * mx->far * mx->near / (mx->far - mx->near);
	mx->pers[0][3] = -((mx->right + mx->left) / (mx->right - mx->left));
	mx->pers[1][3] = -((mx->top + mx->bottom) / (mx->top - mx->bottom));
	mx->pers[2][3] = -((mx->far + mx->near) / (mx->far - mx->near));
	mx->pers[3][3] = 1;
}

void	ft_matrix(GLuint programID, t_control *ctrl, t_matrix *mx)
{
	unsigned int	matLoc;
	static int		acolor = 0;

	if (ctrl->key == '.')
	{
		acolor = (acolor >= 1? (acolor == 1? 2 : 0) : 1);
		printf("acolor = %f\n", acolor);
	}
	gl_perspective(mx);
	ft_rotate(mx, ctrl->rotX, ctrl->rotY, ctrl->rotZ);
	ft_trans(mx, ctrl->moveX, ctrl->moveY, ctrl->moveZ);
	ft_perspective(mx, 0.01f, 100.0f);
	matLoc = glGetUniformLocation(programID, "color");
	glUniform1f(matLoc, acolor);
	matLoc = glGetUniformLocation(programID, "rotate");
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mx->rotate[0][0]);
	matLoc = glGetUniformLocation(programID, "pers");
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mx->pers[0][0]);
	matLoc = glGetUniformLocation(programID, "move");
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mx->pers[0][0]);
	matLoc = glGetUniformLocation(programID, "base");
	glUniformMatrix4fv(matLoc, 1, GL_FALSE, &mx->base[0][0]);
}