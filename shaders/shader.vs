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

void main()
{
	gl_Position = pers * base * rotate * vec4(aPos, 1.0f);
	TexCoord = aTexCoord;
	ourColor = aColor;
}