#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 ourColor;

uniform mat4 rotate;
uniform mat4 pers;
uniform mat4 move;
uniform mat4 base;
// uniform float color;

void main()
{
	gl_Position = pers * move * rotate  *  base * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
	ourColor = aColor;
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