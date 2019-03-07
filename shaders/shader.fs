#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform float color;

void main()
{
	// vec3 t1 = vec3(texture2D(Texture1));
    // vec3 t2 = vec3(ourColor);
    // FragColor = mix(t0, t2, t2.a);
    // FragColor = texture(Texture1, TexCoord) * vec4(0.5, 0.5, 0.5, 1);
	// FragColor = mix(texture(Texture1, TexCoord), texture(Texture2, TexCoord), 0.2);
	if (color == 0.0f || color == 1.0f)
	{
		FragColor = mix(texture(Texture2, TexCoord), vec4(ourColor, 1), color);
	}
	else
	{
		FragColor = texture(Texture1, TexCoord);
		// FragColor = vec4(0.5, 0.5, 0.0, 1.0);
	}
	// FragColor = texture(Texture1, TexCoord);
	// FragColor = vec4(ourColor, 1.0);
}