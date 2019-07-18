#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float color;

void main()
{
	if (color < 1)
		FragColor = mix(texture(Texture2, TexCoord), vec4(ourColor, 1), color);
	else
		FragColor = vec4(ourColor, 1);
}