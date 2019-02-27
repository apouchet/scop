#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
  
// uniform vec16 matrix;
uniform vec4 mat_x;
uniform vec4 mat_y;
uniform vec4 mat_z;
uniform vec4 mat_w;

// = mat_x, mat_y, mat_z, mat_w;

mat4 matrix = mat4(mat_x, mat_y, mat_z, mat_w);

void main()
{
	// mat4 matrix;

	// for (int i = 0; i < 4; i++)
	// {
	// 	matrix[i] = 1;
	// 	// matrix[i + 4] = mat_y[i];
	// 	// matrix[i + 8] = mat_z[i];
	// 	// matrix[i + 12] = mat_w[i];
	// }

	gl_Position = matrix * vec4(aPos, 1.0f);
	if (mat_x[0] == 1 && mat_y[0] == 1 && mat_z[0] == 1 && mat_w[0] == 1 && matrix[0][0] == 1)
	{
		TexCoord = aTexCoord;
	}
} 

// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 2) in vec2 aTexCoord;

// out vec2 TexCoord;

// void main()
// {
//     gl_Position = vec4(aPos, 1.0);
//     TexCoord = aTexCoord;
// }

// #version 330 core
// layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
// layout (location = 2) in vec2 aTexCoord;

// out vec3 ourColor;
// out vec2 TexCoord;

// void main()
// {
//     gl_Position = vec4(aPos, 1.0);
//     ourColor = aColor;
//     TexCoord = aTexCoord;
// }